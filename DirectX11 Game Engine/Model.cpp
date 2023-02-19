#include "Model.h"
#include "Material.h"
#include "Texture.h"
#include "Sampler.h"
#include "GeneralUtilities.h"

namespace dx = DirectX;

Mesh::Mesh( std::vector<std::shared_ptr<Bindable>>&& binds, Graphics& gfx )
{
	AddBind( Binds::TransformationConstBuffer::Resolve( gfx, *this ) );
	for( auto& b : binds )
	{
		AddBind( std::move( b ) );
	}
 }

void Mesh::BindTransform( DirectX::XMMATRIX Transform )
{
	Transform_ = Transform;
}

DirectX::XMMATRIX Mesh::GetTransformationMatrix() const noexcept
 {
	 // Return concatenated matrix from all parents
	 return Transform_;
 }

Node::Node( std::vector<std::shared_ptr<Mesh>> in_meshes, DirectX::XMMATRIX in_transform )
	 : meshes( std::move( in_meshes ) )
	 , transform( in_transform )
 {}

void Node::Draw( Graphics & gfx, DirectX::XMMATRIX in_transform ) const
 {
	 // Draw all meshes and children ( it would be cool to not do this every draw call btw )
	 const auto concatenatedTransform = dx::XMMatrixMultiply( transform, in_transform );
	 for ( auto& m : meshes )
	 {
		 m->BindTransform( concatenatedTransform );
		 m->Draw( gfx );
	 }
	 for ( auto& c : children )
	 {
		 c.Draw( gfx, concatenatedTransform );
	 }
 }

Node& Node::AddChild( Node&& child )
 {
	 children.push_back( child );
	 return children.back();
 }

Model::Model( Graphics& gfx, std::filesystem::path filename )
	: tag(filename.string())
 {
	 Assimp::Importer Importer;
	 // TODO assimp logging here for error on file load reason
	 const auto pAIScene = Importer.ReadFile(
		 filename.string(),
		 aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_ConvertToLeftHanded);

	 // Check for scene load success
	 if ( pAIScene == nullptr )
	 {
		 assert( false && "Error loading file");
	 }

	 // Populate meshes vector with all meshes (in order)
	 for( size_t i = 0; i < pAIScene->mNumMeshes; i++ )
	 {
		 const auto& pMesh = pAIScene->mMeshes[i];
		 if ( pMesh->mMaterialIndex < 0 )
		 {
			 pMeshes.push_back( makeMesh( gfx, *pMesh, nullptr ) );
		 }
		 else
		 {
			 pMeshes.push_back( makeMesh( gfx, *pMesh, pAIScene->mMaterials[pMesh->mMaterialIndex] ) );
		 }
	 }

	 // Pre process head node
	 assert( pAIScene->mRootNode->mNumMeshes == 0 );
	 const auto row_maj_trans = *reinterpret_cast<dx::XMFLOAT4X4*>(
		 &pAIScene->mRootNode->mTransformation );
	 pHead = std::make_unique<Node>(
		 std::vector<std::shared_ptr<Mesh>>{},
		 dx::XMMatrixTranspose( dx::XMLoadFloat4x4( &row_maj_trans ) ) );

	 // Populate node tree from head
	 PopulateNodeFromAINode( *pHead, pAIScene->mRootNode );
 }

void Model::UpdateTransform( DirectX::XMMATRIX in_transform )
 {
	 transform = in_transform;
 }

void Model::Draw( Graphics& gfx ) const
 {
	 pHead->Draw( gfx, transform );
 }

std::shared_ptr<Mesh> Model::makeMesh( Graphics& gfx, const aiMesh& mesh, const aiMaterial* pAiMat)
{
	std::filesystem::path rootpath = tag;
	rootpath = rootpath.parent_path();
	// set tag for bindable instancing to be the name of the mesh, for VB and IB
	tag += mesh.mName.C_Str();

	std::vector<std::shared_ptr<Bindable>> Binds;
	assert(pAiMat->GetTextureCount( aiTextureType_DIFFUSE ) && "Mesh must have texture");

	Vert::VertexLayout vl;
	vl.Append( Vert::VertexLayout::Position_3D )
		.Append( Vert::VertexLayout::Normal )
		.Append( Vert::VertexLayout::TexCoordUV );
	Vert::VertexBuffer vb( std::move(vl) );

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
	for ( size_t i = 0; i < mesh.mNumFaces; i++ )
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
	auto vs = static_cast<Binds::VertexShader*>( Binds.back().get() );
	auto vsbytecode = vs->pGetBytecode();
	Binds.push_back( Binds::InputLayout::Resolve( gfx, vb.GetLayout(), *vsbytecode ) );

	// Material properties
	Material mat;
	// Set defaults
	mat.specular_intensity( 1.0f ).specular_power( 1.1f );
	// load material for this mesh from ai, if it has one
	if ( mesh.mMaterialIndex >= 0 )
	{
		mat.parseAIMat( *pAiMat );	
	}
	Binds.push_back( Binds::PixelConstantBuffer<Material>::Resolve( gfx, mat, tag, 1u ) );

	// Load Texture
	aiString filename;
	pAiMat->GetTexture( aiTextureType_DIFFUSE, 0, &filename );
	using namespace std::string_literals;
	std::string texturePath = rootpath.string() + "\\"s + filename.C_Str();
	Binds.push_back( Binds::Texture::Resolve( gfx, texturePath));
	Binds.push_back( Binds::Sampler::Resolve( gfx ) );	

	return std::make_shared<Mesh>( std::move( Binds ), gfx );
}

void Model::PopulateNodeFromAINode( Node& node, const aiNode* pAiNode )
 {
	 // Check if ainode has children, if so add child to our node and recurse on child 

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

		 const auto row_maj_trans = *reinterpret_cast<dx::XMFLOAT4X4*>( &pChild->mTransformation );

		 auto& newChild = node.AddChild( Node(
			 std::move( childrenMeshes ),
			 dx::XMMatrixTranspose( dx::XMLoadFloat4x4( &row_maj_trans ) ) ) );

		 // Recurse over each child
		 PopulateNodeFromAINode( newChild, pAiNode->mChildren[i] );
	 }
 }
