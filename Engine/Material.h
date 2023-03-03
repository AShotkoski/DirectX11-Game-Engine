#pragma once
#include "DynamicCB.h"
#include "Vertex.h"
#include <assimp/material.h>
#include <filesystem>

class Technique;

class Material
{
public:
	Material(Graphics& gfx, const aiMaterial& aiMat, std::filesystem::path modelPath );
	const std::vector<Technique>& GetTechniques() const;
	const Vert::VertexLayout& GetVertexLayout() const;
private:
	Vert::VertexLayout vertlayout_;
	std::vector<Technique> techniques_;
};
