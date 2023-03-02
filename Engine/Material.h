#pragma once
#include "DynamicCB.h"
#include "Vertex.h"
#include <assimp/material.h>

class Technique;

class Material
{
public:
	Material(Graphics& gfx, const aiMaterial& aiMat, std::filesystem::path modelPath );
	//void parseAIMat( const aiMaterial& aiMat );
private:
	Vert::VertexLayout vertlayout_;
	std::vector<Technique> techniques_;
};
