#pragma once
#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "Vertex.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include <filesystem>

class Node
{
	friend class Model;
public:
	Node( std::vector<std::shared_ptr<Mesh>> in_meshes, DirectX::XMMATRIX in_transform, std::string name );
	void Draw( Graphics& gfx, DirectX::XMMATRIX in_transform ) const;
	void SpawnControlWindow();
private:
	// Returns a ref to added child node
	Node& AddChild( Node&& child );
private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<Node> children;
	DirectX::XMMATRIX transform;
	std::string name;
};

class Model
{
public:
	Model( Graphics& gfx, std::filesystem::path filename );
	void UpdateTransform( DirectX::XMMATRIX in_transform );
	void Draw( Graphics& gfx ) const;
	void SpawnControlWindow();
	~Model();
private:
	// Makes mesh from Assimp Mesh and returns shared ptr to it
	std::shared_ptr<Mesh> makeMesh( Graphics& gfx, const aiMesh& mesh, const aiMaterial* pAiMat );
	void PopulateNodeFromAINode( Node& node, const aiNode* pAiNode );
private:
	std::string tag;
	std::unique_ptr<Node> pHead;
	// OWNER 
	std::vector<std::shared_ptr<Mesh>> pMeshes;
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
};


