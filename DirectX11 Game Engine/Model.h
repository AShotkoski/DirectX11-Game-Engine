#pragma once
#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

// In this class it would be a great idea to upgrade the drawing so that we don't need to 
// re-calculate the concatenated matrices every single call to draw.

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh( std::vector<std::unique_ptr<Bindable>>&& StaticBinds,
		  std::vector<std::unique_ptr<Bindable>>&& binds, Graphics& gfx );
	void BindTransform( DirectX::XMMATRIX Transform );
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	DirectX::XMMATRIX Transform_ = DirectX::XMMatrixIdentity();
};

class Node
{
	friend class Model;
public:
	Node( std::vector<std::shared_ptr<Mesh>> in_meshes, DirectX::XMMATRIX in_transform );
	void Draw( Graphics& gfx, DirectX::XMMATRIX in_transform ) const;
private:
	// Returns a ref to added child node
	Node& AddChild( Node&& child );
private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<Node> children;
	DirectX::XMMATRIX transform;
};

class Model
{
public:
	Model( Graphics& gfx, std::string filename );
	void UpdateTransform( DirectX::XMMATRIX in_transform );
	void Draw( Graphics& gfx ) const;
private:
	// Makes mesh from Assimp Mesh and returns shared ptr to it
	std::shared_ptr<Mesh> makeMesh( Graphics& gfx, const aiMesh& mesh );

	void PopulateNodeFromAINode( Node& node, const aiNode* pAiNode );
private:
	std::unique_ptr<Node> pHead;
	// OWNER 
	std::vector<std::shared_ptr<Mesh>> pMeshes;
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
};


