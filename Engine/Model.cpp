#include "Model.h"
#include "Material.h"
#include "Texture.h"
#include "Sampler.h"
#include "GeneralUtilities.h"
#include "log.h"
#include "assimp/DefaultLogger.hpp"
#include "ImGui/imgui.h"
#include "DynamicCB.h"

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

void Node::Submit( FrameCommander& frame, DirectX::XMMATRIX in_transform ) const
{
	// Draw all meshes and children ( it would be cool to not do this every draw call btw )
	const auto concatenatedTransform = applied_transform * transform * in_transform;
	for( auto& m : meshes )
	{
		m->BindTransform( concatenatedTransform );
		m->Submit( frame );
	}
	for( auto& c : children )
	{
		c.Submit( frame, concatenatedTransform );
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
private:
	struct Properties
	{
		DirectX::XMFLOAT3 pos = { 0,0,0 };
		float pitch = 0;
		float yaw = 0;
		float roll = 0;
		DirectX::XMFLOAT3 scale = { 1,1,1 };
	};
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
		spawnLoc.x += ImGui::GetWindowWidth() / 8;
		ImGui::SetNextWindowPos( spawnLoc, ImGuiCond_Appearing );
		ImVec2 spawnSize = {};
		spawnSize.x = ImGui::GetWindowWidth() - spawnLoc.x;
		spawnSize.y = 100;
		ImGui::SetNextWindowSize( spawnSize, ImGuiCond_Appearing );

		if( ImGui::Begin( pSelectedNode->name.c_str(), &showControlWnd) )
		{
			// QOL ref
			auto& currProp = propMap[*selectedNodeIndex];
			bool willReapply = false;

			if ( ImGui::DragFloat3( "Position", &currProp.pos.x, 0.1f ) )
				willReapply = true;
			if ( ImGui::DragFloat3( "Rotation", &currProp.pitch, 0.005f ) )
				willReapply = true;
			if ( ImGui::DragFloat3( "Scale", &currProp.scale.x, 0.01f ) )
				willReapply = true;

			// Apply the generated transform.
			if(willReapply )
				pSelectedNode->ApplyTransform( genTransform(currProp) );
		}
		// handle closure case
		if( !showControlWnd )
			selectedNodeIndex = std::nullopt;

		ImGui::End();
	}
	// Helper for generating transform mat
	dx::XMMATRIX genTransform(Properties& data)
	{
		return 
			dx::XMMatrixRotationRollPitchYaw( data.pitch, data.yaw, data.roll ) *
			dx::XMMatrixScaling( data.scale.x, data.scale.y, data.scale.z ) *
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
	// map of index to data, each new node that is modified adds to it
	std::unordered_map<int, Properties> propMap;
};

Model::Model( Graphics& gfx, std::filesystem::path filename ) :
	tag( filename.string() ),
	pController( std::make_unique<ModelController>( ) )
{
	LOG_SCOPE_FUNCTION( INFO );
	DLOG_F( INFO, "Model ctor begins for %s", filename.string().c_str() );
	// Create assimp logger to log assimp messages during file loading
	Assimp::DefaultLogger::create( "logs\\asslog.log", Assimp::Logger::DEBUGGING );

	Assimp::Importer Importer;
	const auto       pAIScene = Importer.ReadFile(
        filename.string(),
        aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenNormals
            | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace );
	
	// Check for scene load success
	CHECK_NOTNULL_F( pAIScene, "Error loading file.\t%s", Importer.GetErrorString() );

	DLOG_F( INFO, "AIScene Loaded" );

	// Populate meshes vector with all meshes (in order)
	for( size_t i = 0; i < pAIScene->mNumMeshes; i++ )
	{
		const auto& mesh = *pAIScene->mMeshes[i];
		DCHECK_F( mesh.mMaterialIndex >= 0, "mesh material index not right" );
		const auto& material = *pAIScene->mMaterials[mesh.mMaterialIndex];
		Material mat( gfx, material, filename );
		pMeshes.push_back( std::make_shared<Mesh>( gfx, mat, mesh ) );
		
	}

	// Populate node tree from head
	PopulateNodeTreeFromAINode(nullptr, pAIScene->mRootNode, true );

	// Kill assimp logger
	Assimp::DefaultLogger::kill();
}

void Model::UpdateTransform( DirectX::XMMATRIX in_transform )
{
	transform = in_transform;
}

void Model::Submit( FrameCommander& frame ) const
{
	pHead->Submit( frame, transform );
}

void Model::SpawnControlWindow()
{
	pController->ShowImGui( *pHead );
}

Model::~Model() {}


// Check if ainode has children, if so add child to our node and recurse on child
void Model::PopulateNodeTreeFromAINode( Node* pNode, const aiNode* pAiNode, bool isHead )
{
	DCHECK_F( ( pHead == nullptr ? isHead : !isHead ), "Attempting to create two head nodes in %s", pAiNode->mName.C_Str() );

	// isHead is only true for the head, in which case we simply populate pHead
	if ( isHead )
	{
		pHead = std::make_unique<Node>( MakeNode( *pAiNode ) );
		// Re-call so we have good ref
		PopulateNodeTreeFromAINode( pHead.get(), pAiNode);
		return;
	}

	// If the ainode is a leaf, we have processed all we need to
	if( pAiNode->mNumChildren == 0 )
		return;

	// Add a child to our tree for each child in the aiscene tree
	for( size_t i = 0; i < pAiNode->mNumChildren; i++ )
	{
		auto& newChild = pNode->AddChild( MakeNode(*pAiNode->mChildren[i]));

		// Recurse over each child
		PopulateNodeTreeFromAINode( &newChild, pAiNode->mChildren[i] );
	}
}

Node Model::MakeNode( const aiNode& ai_Node ) const
{
	std::vector<std::shared_ptr<Mesh>> pNodeMeshes;
	for ( size_t i = 0; i < ai_Node.mNumMeshes; i++ )
	{
		pNodeMeshes.push_back( pMeshes[ai_Node.mMeshes[i]] );
	}
	const auto row_maj_trans = *reinterpret_cast<const dx::XMFLOAT4X4*>( &ai_Node.mTransformation );
	return Node(
		std::move( pNodeMeshes ),
		dx::XMMatrixTranspose( dx::XMLoadFloat4x4( &row_maj_trans ) ),
		ai_Node.mName.C_Str() );
}
