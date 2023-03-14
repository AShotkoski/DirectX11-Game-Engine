#pragma once
#include "Drawable/Mesh.h"
#include <memory>
#include <filesystem>

namespace RDG
{
	class RenderGraph;
}


class Node
{
	friend class Model;
	friend class ModelController;
public:
	Node( std::vector<std::shared_ptr<Mesh>> in_meshes, DirectX::XMMATRIX in_transform, std::string name );
	void Submit(DirectX::XMMATRIX in_transform ) const;
	void SpawnControlWindow( int& nodeIndex, std::optional<int>& selectedIndex, Node*& pSelectedNode );
	void ApplyTransform( DirectX::XMMATRIX newTransform );
private:
	// Returns a ref to added child node
	Node& AddChild( Node&& child );
private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<Node> children;
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX applied_transform = DirectX::XMMatrixIdentity();
	std::string name;
};

class Model
{
public:
	Model( Graphics& gfx, std::filesystem::path filename, RDG::RenderGraph* pGraph);
	void UpdateTransform( DirectX::XMMATRIX in_transform );
	void Submit() const;
	void SpawnControlWindow();
	~Model();
private:
	// Makes mesh from Assimp Mesh and returns shared ptr to it
	void PopulateNodeTreeFromAINode( Node* pNode, const struct aiNode* pAiNode, bool isHead = false );
	Node MakeNode( const aiNode& ai_Node ) const;
	// TODO add a (slow, optional) function for obj files and other non-hierarchical model formats
	// that uses miniball to calculate centers of each mesh and use that for ModelController
private:
	std::string tag;
	std::unique_ptr<Node> pHead;
	// OWNER 
	std::vector<std::shared_ptr<Mesh>> pMeshes;
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	//std::unique_ptr<class ModelController> pController;
};


