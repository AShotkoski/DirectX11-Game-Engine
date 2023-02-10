#pragma once
#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

//MOVE TO CPP ONCE IT EXISTS
namespace dx = DirectX;

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh( std::vector<std::unique_ptr<Bindable>>&& StaticBinds,
		  std::vector<std::unique_ptr<Bindable>>&& binds, Graphics& gfx )
	{
		if ( !isStaticInitialized() )
		{
			for ( auto& b : StaticBinds )
			{
				AddStaticBind( std::move( b ) );
			}
		}
		AddBind( std::make_unique<Binds::TransformationConstBuffer>( gfx, *this ) );
		for ( auto& b : binds )
		{
			AddBind( std::move( b ) );
		}
	}
	virtual void Draw( Graphics& gfx ) const override
	{
		Drawable::Draw( gfx );
	}
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override
	{
		// Return concatenated matrix from all parents
		return DirectX::XMMatrixIdentity();
	}
private:

};

class Node
{
	friend class Model;
public:
	Node( std::vector<std::shared_ptr<Mesh>> in_meshes )
		: meshes(std::move(in_meshes))
	{}
	void Draw( Graphics& gfx ) const
	{
		// Draw all meshes and children
		for ( auto& m : meshes )
		{
			m->Draw( gfx );
		}
		for ( auto& c : children )
		{
			c.Draw( gfx );
		}
	}
private:
	// Returns a ref to added child node
	Node& AddChild(Node&& child)
	{
		children.push_back( child );
		return children.back();
	}
private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<Node> children;
};

class Model
{
public:
	Model(Graphics& gfx, std::string filename )
	{
		Assimp::Importer Importer;

		const auto pAIScene = Importer.ReadFile( filename,
												  aiProcess_JoinIdenticalVertices |
												  aiProcess_Triangulate );
		// Check for scene load success
		if ( pAIScene == nullptr )
		{
			filename += " invalid file/loading failed.";
			assert( false && filename.c_str() );
		}

		// Populate meshes vector with all meshes (in order)
		for ( size_t i = 0; i < pAIScene->mNumMeshes; i++ )
		{
			pMeshes.push_back(makeMesh(gfx, *pAIScene->mMeshes[i] ));
		}

		// Pre process head node
		assert( pAIScene->mRootNode->mNumMeshes == 0 );
		pHead = std::make_unique<Node>( std::vector<std::shared_ptr<Mesh>>{} );

		// Populate node tree from head
		PopulateNodeFromAINode( *pHead, pAIScene->mRootNode );
	}
	void Draw( Graphics& gfx ) const
	{
		pHead->Draw( gfx );
	}
private:
	// Makes mesh from Assimp Mesh and returns shared ptr to it
	std::shared_ptr<Mesh> makeMesh(Graphics& gfx, const aiMesh& mesh )
	{
		std::vector<std::unique_ptr<Bindable>> StaticBinds;
		std::vector<std::unique_ptr<Bindable>> DynamicBinds;

		Vert::VertexBuffer vb( Vert::VertexLayout{}
								   .Append( Vert::VertexLayout::Position_3D )
								   .Append( Vert::VertexLayout::Normal ) );
		// Load vertices into vert::vertex buffer
		for ( size_t i = 0; i < mesh.mNumVertices; i++ )
		{
			vb.Emplace_back( *reinterpret_cast<dx::XMFLOAT3*>( &mesh.mVertices[i] ),
							 *reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] ) );
		}
		// Load indices
		std::vector<unsigned short> Indices;
		Indices.reserve( mesh.mNumFaces * 3 );
		for ( size_t i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			Indices.push_back( face.mIndices[0] );
			Indices.push_back( face.mIndices[1] );
			Indices.push_back( face.mIndices[2] );
		}

		// Create Binds
		StaticBinds.push_back(
			std::make_unique<Binds::Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
		StaticBinds.push_back( std::make_unique<Binds::VertexBuffer>( gfx, vb ) );
		StaticBinds.push_back( std::make_unique<Binds::IndexBuffer>( gfx, Indices ) );
		StaticBinds.push_back( std::make_unique<Binds::PixelShader>( gfx, L"PSPhong.cso" ) );
		auto vs         = std::make_unique<Binds::VertexShader>( gfx, L"VSPhong.cso" );
		auto vsbytecode = vs->pGetBytecode();
		StaticBinds.push_back( std::move( vs ) );
		StaticBinds.push_back(
			std::make_unique<Binds::InputLayout>( gfx, vb.GetD3DInputLayout(), *vsbytecode ) );
		// Material properties
		struct PSCBuf
		{
			DirectX::XMFLOAT3 MatCol;
			float             specularIntensity;
			alignas( 16 ) float specularPower;
		};
		PSCBuf cubeProps = { { 1, 1, 1 }, 1, 50 };
		DynamicBinds.push_back(
			std::make_unique<Binds::PixelConstantBuffer<PSCBuf>>( gfx, cubeProps, 1u ) );

		return std::make_shared<Mesh>( std::move( StaticBinds ), std::move( DynamicBinds ), gfx );
	}

	void PopulateNodeFromAINode(Node& node, const aiNode* pAiNode)
	{
		// Check if ainode has children, if so add child to our node and recurse on child 
		// We will copy the tree offset by one kinda
		
		// If the ainode is a leaf, we have processed all we need to
		if ( pAiNode->mNumChildren == 0 )
			return;

		// Add a child to our tree for each child in the aiscene tree
		for ( size_t i = 0; i < pAiNode->mNumChildren; i++ )
		{
			auto pChild = pAiNode->mChildren[i];

			std::vector<std::shared_ptr<Mesh>> childrenMeshes;
			for ( size_t i = 0; i < pChild->mNumMeshes; i++ )
			{
				childrenMeshes.push_back( pMeshes[pChild->mMeshes[i]] );
			}
			auto& newChild = node.AddChild( Node( std::move( childrenMeshes ) ) );

			// Recurse over each child
			PopulateNodeFromAINode( newChild, pAiNode->mChildren[i] );
		}
	}
private:
	std::unique_ptr<Node> pHead;
	// OWNER 
	std::vector<std::shared_ptr<Mesh>> pMeshes;
};


