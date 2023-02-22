#include "Model.h"
#include "Material.h"
#include "Texture.h"
#include "Sampler.h"
#include "GeneralUtilities.h"
#include "Loguru/loguru.hpp"
#include "assimp/DefaultLogger.hpp"
#include "ImGui/imgui.h"

namespace dx = DirectX;

Node::Node(
	std::vector<std::shared_ptr<Mesh>> in_meshes,
	DirectX::XMMATRIX                  in_transform,
	std::string                        name ) :
	meshes( std::move( in_meshes ) ),
	transform( in_transform ),
	name( name )
{
}

void Node::Draw( Graphics& gfx, DirectX::XMMATRIX in_transform ) const
{
	// Draw all meshes and children ( it would be cool to not do this every draw call btw )
	const auto concatenatedTransform =
		dx::XMMatrixMultiply( applied_transform * transform, in_transform );
	for( auto& m : meshes )
	{
		m->BindTransform( concatenatedTransform );
		m->Draw( gfx );
	}
	for( auto& c : children )
	{
		c.Draw( gfx, concatenatedTransform );
	}
}

// Spawns the actual tree structure of model recursively
void Node::SpawnControlWindow(
	int&                nodeIndex,
	std::optional<int>& selectedIndex,
	Node*&               pSelectedNode )
{
	// Store current node index and inc counter for next node
	const int thisNodeIndex = nodeIndex++;

	auto flags = ImGuiTreeNodeFlags_OpenOnArrow
				 | ( selectedIndex.value_or( -1 ) == thisNodeIndex ? ImGuiTreeNodeFlags_Selected :
																	 0u )
				 | ( children.size() == 0 ? ImGuiTreeNodeFlags_Leaf : 0u );

	if( ImGui::TreeNodeEx( static_cast<const void*>( &thisNodeIndex ), flags, "%s", name.c_str() ) )
	{
		if( ImGui::IsItemClicked() )
		{
			selectedIndex = thisNodeIndex;
			pSelectedNode = this;
		}

		for( auto& c : children )
		{
			c.SpawnControlWindow( nodeIndex, selectedIndex, pSelectedNode );
		}
		ImGui::TreePop();
	}
}

void Node::ApplyTransform( DirectX::XMMATRIX newTransform )
{
	applied_transform = newTransform;
}

Node& Node::AddChild( Node&& child )
{
	children.push_back( child );
	return children.back();
}

// pImpl class to control models with imgui
class ModelController
{
public:
	void ShowImGui( Node& node )
	{
		if( ImGui::Begin( "Model Control" ) )
		{
			int ind = 0;
			node.SpawnControlWindow( ind, selectedNodeIndex, pSelectedNode);

			showControlWnd = selectedNodeIndex.has_value();
			if( showControlWnd )
			{
				SpawnControlWindow();
			}
		}
		ImGui::End();
	}

private:
	// Spawn controller that controls selected node
	void SpawnControlWindow()
	{
		DCHECK_NOTNULL_F( pSelectedNode, "Selected node was somehow nullptr, serisouly how?" );

		ImVec2 spawnLoc = ImGui::GetWindowPos();
		spawnLoc.y += ImGui::GetWindowHeight();

		ImGui::SetNextWindowPos( spawnLoc, ImGuiCond_Appearing );
		if( ImGui::Begin( "child", &showControlWnd ) )
		{
			ImGui::DragFloat3( "Pos", &data.pos.x, 0.1f );
			ImGui::Text( "%d", *selectedNodeIndex );
			pSelectedNode->ApplyTransform( genTransform() );
		}
		// handle closure case
		if( !showControlWnd )
			selectedNodeIndex = std::nullopt;

		ImGui::End();
	}
	// Helper for generating transform mat
	dx::XMMATRIX genTransform()
	{
		return 
			dx::XMMatrixScaling( data.scale.x, data.scale.y, data.scale.z ) *
			dx::XMMatrixRotationRollPitchYaw( data.pitch, data.yaw, data.roll ) *
			dx::XMMatrixTranslation( data.pos.x, data.pos.y, data.pos.z );
	}
	void readNodeAppliedTrans()
	{
		DCHECK_NOTNULL_F( pSelectedNode, "Selected node was somehow nullptr, serisouly how?" );

		const auto tran = pSelectedNode->applied_transform;
		
	}
private:
	std::optional<int> selectedNodeIndex = std::nullopt;
	Node*              pSelectedNode     = nullptr;
	bool               showControlWnd    = false;
	struct
	{
		DirectX::XMFLOAT3 pos;
		float pitch;
		float yaw;
		float roll;
		DirectX::XMFLOAT3 scale = { 1,1,1 };
	}data;
};

Model::Model( Graphics& gfx, std::filesystem::path filename ) :
	tag( filename.string() ),
	pController( std::make_unique<ModelController>( ) )
{
	DLOG_F( INFO, "Model ctor begins for %s", filename.string().c_str() );
	// Create assimp logger to log assimp messages during file loading
	Assimp::DefaultLogger::create( "logs\\asslog.log", Assimp::Logger::VERBOSE );

	Assimp::Importer Importer;
	const auto       pAIScene = Importer.ReadFile(
        filename.string(),
        aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenNormals
            | aiProcess_ConvertToLeftHanded );

	DLOG_F( INFO, "AIScene Loaded" );
	// Check for scene load success
	if( pAIScene == nullptr )
	{
		assert( false && "Error loading file" );
	}

	// Populate meshes vector with all meshes (in order)
	for( size_t i = 0; i < pAIScene->mNumMeshes; i++ )
	{
		const auto& pMesh = pAIScene->mMeshes[i];
		if( pMesh->mMaterialIndex < 0 )
		{
			pMeshes.push_back( makeMesh( gfx, *pMesh, nullptr ) );
		}
		else
		{
			pMeshes.push_back(
				makeMesh( gfx, *pMesh, pAIScene->mMaterials[pMesh->mMaterialIndex] ) );
		}
	}
	// Pre process head node
	LOG_IF_F(
		WARNING,
		pAIScene->mRootNode->mNumMeshes > 0,
		"root node has nonzero meshes in model %s",
		pAIScene->mName.C_Str() );

	const auto row_maj_trans = *reinterpret_cast<dx::XMFLOAT4X4*>(
		&pAIScene->mRootNode->mTransformation );
	pHead = std::make_unique<Node>(
		std::vector<std::shared_ptr<Mesh>>{},
		dx::XMMatrixTranspose( dx::XMLoadFloat4x4( &row_maj_trans ) ),
		pAIScene->mRootNode->mName.C_Str() );

	// Populate node tree from head
	PopulateNodeFromAINode( *pHead, pAIScene->mRootNode );
	// Kill logger
	Assimp::DefaultLogger::kill();
}

void Model::UpdateTransform( DirectX::XMMATRIX in_transform )
{
	transform = in_transform;
}

void Model::Draw( Graphics& gfx ) const
{
	pHead->Draw( gfx, transform );
}

void Model::SpawnControlWindow()
{
	pController->ShowImGui( *pHead );
}

Model::~Model() {}

std::shared_ptr<Mesh> Model::makeMesh( Graphics& gfx, const aiMesh& mesh, const aiMaterial* pAiMat )
{
	std::filesystem::path rootpath = tag;
	rootpath                       = rootpath.parent_path();
	// set tag for bindable instancing to be the name of the mesh, for VB and IB
	tag += mesh.mName.C_Str();

	std::vector<std::shared_ptr<Bindable>> Binds;
	if( pAiMat->GetTextureCount( aiTextureType_DIFFUSE ) == 0 )
		LOG_F( WARNING, "Mesh %s doesn't have a diffuse texture.", mesh.mName.C_Str() );

	Vert::VertexLayout vl;
	vl.Append( Vert::VertexLayout::Position_3D )
		.Append( Vert::VertexLayout::Normal )
		.Append( Vert::VertexLayout::TexCoordUV );
	Vert::VertexBuffer vb( std::move( vl ) );

	// Load vertices into vert::vertex buffer
	for( size_t i = 0; i < mesh.mNumVertices; i++ )
	{
		vb.Emplace_back(
			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mVertices[i] ),
			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] ),
			*reinterpret_cast<dx::XMFLOAT2*>( &mesh.mTextureCoords[0][i] ) );
	}
	// Load indices
	std::vector<unsigned short> Indices;
	Indices.reserve( ( size_t( mesh.mNumFaces ) * 3 ) );
	for( size_t i = 0; i < mesh.mNumFaces; i++ )
	{
		const auto& face = mesh.mFaces[i];
		assert( face.mNumIndices == 3 );
		Indices.push_back( face.mIndices[0] );
		Indices.push_back( face.mIndices[1] );
		Indices.push_back( face.mIndices[2] );
	}

	// Create Binds
	Binds.push_back( Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	Binds.push_back( Binds::VertexBuffer::Resolve( gfx, vb, tag ) );
	Binds.push_back( Binds::IndexBuffer::Resolve( gfx, Indices, tag ) );
	Binds.push_back( Binds::PixelShader::Resolve( gfx, L"PSPhong.cso" ) );
	Binds.push_back( Binds::VertexShader::Resolve( gfx, L"VSPhong.cso" ) );
	auto vs         = static_cast<Binds::VertexShader*>( Binds.back().get() );
	auto vsbytecode = vs->pGetBytecode();
	Binds.push_back( Binds::InputLayout::Resolve( gfx, vb.GetLayout(), *vsbytecode ) );

	// Material properties
	Material mat;
	// Set defaults
	mat.specular_intensity( 1.0f ).specular_power( 1.1f );
	// load material for this mesh from ai, if it has one
	if( mesh.mMaterialIndex >= 0 )
	{
		mat.parseAIMat( *pAiMat );
	}
	Binds.push_back( Binds::PixelConstantBuffer<Material>::Resolve( gfx, mat, tag, 1u ) );

	// Load Texture
	aiString filename;
	if( pAiMat->GetTextureCount( aiTextureType_DIFFUSE ) != 0 )
	{
		pAiMat->GetTexture( aiTextureType_DIFFUSE, 0, &filename );
		using namespace std::string_literals;
		std::string texturePath = rootpath.string() + "\\"s + filename.C_Str();
		Binds.push_back( Binds::Texture::Resolve( gfx, texturePath ) );
	}
	Binds.push_back( Binds::Sampler::Resolve( gfx ) );

	// Log mesh creation

	DLOG_F(
		INFO,
		"Created %s mesh, which uses %s diffuse tex",
		mesh.mName.C_Str(),
		filename.C_Str() );

	return std::make_shared<Mesh>( std::move( Binds ), gfx );
}

void Model::PopulateNodeFromAINode( Node& node, const aiNode* pAiNode )
{
	// Check if ainode has children, if so add child to our node and recurse on child

	// If the ainode is a leaf, we have processed all we need to
	if( pAiNode->mNumChildren == 0 )
		return;

	// Add a child to our tree for each child in the aiscene tree
	for( size_t i = 0; i < pAiNode->mNumChildren; i++ )
	{
		auto pChild = pAiNode->mChildren[i];

		std::vector<std::shared_ptr<Mesh>> childrenMeshes;
		for( size_t i = 0; i < pChild->mNumMeshes; i++ )
		{
			childrenMeshes.push_back( pMeshes[pChild->mMeshes[i]] );
		}

		const auto row_maj_trans = *reinterpret_cast<dx::XMFLOAT4X4*>( &pChild->mTransformation );

		auto& newChild = node.AddChild( Node(
			std::move( childrenMeshes ),
			dx::XMMatrixTranspose( dx::XMLoadFloat4x4( &row_maj_trans ) ),
			pChild->mName.C_Str() ) );

		// Recurse over each child
		PopulateNodeFromAINode( newChild, pAiNode->mChildren[i] );
	}
}
