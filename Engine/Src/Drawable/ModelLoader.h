#pragma once
#include <log.h>
#include "Model.h"
#include <thread>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class ModelLoader
{
public:
	ModelLoader() = default;
	// Begins loading a model on a separate thread
	void ASyncLoad(std::filesystem::path filename)
	{
		const auto load = [&, filename] 
		{
			pScene = const_cast<aiScene*>(Model::LoadAIScene(Importer, filename.string()));
		};
		loader = std::make_unique<std::jthread>(load);
		Importer.FreeScene();
	}
	// Returns true if the thread is done loading
	bool isReady()
	{
		_Query();
		return ready;
	}
	aiScene* Get()
	{
		return pScene;
	}
private:
	// Checks if the thread is done loading
	void _Query()
	{
		ready = (bool)pScene;
		if (ready)
			loader->detach();
	}
private:
	Assimp::Importer Importer;
	std::unique_ptr<std::jthread> loader;
	aiScene* pScene{ nullptr };
	bool ready{ false };
};