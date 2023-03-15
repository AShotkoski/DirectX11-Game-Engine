#pragma once
#include "DynamicCB.h"
#include <Geometry/Vertex.h>
#include <assimp/material.h>
#include <filesystem>

class Technique;

namespace RDG
{
	class RenderGraph;
}

class Material
{
public:
	Material(Graphics& gfx, const aiMaterial& aiMat, std::filesystem::path modelPath, RDG::RenderGraph* pGraph );
	const std::vector<Technique>& GetTechniques() const;
	const Vert::VertexLayout& GetVertexLayout() const;
	const std::string& GetTag() const;
private:
	Vert::VertexLayout vertlayout_;
	std::vector<Technique> techniques_;
	std::string tag_;
};
