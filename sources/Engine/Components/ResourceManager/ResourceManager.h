#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Graphics\RenderSystem\Texture.h>
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\Graphics\RenderSystem\Sprite.h>
#include <Engine\Components\Graphics\RenderSystem\Model.h>

#include "MeshLoader.h"
#include "MaterialLoader.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Sprite* loadSprite(const std::string&);
	Texture* loadTexture(const std::string&);
	GpuProgram* loadShader(const std::string&);
	Mesh* loadMesh(const std::string&);

	void loadMaterialsPackage(const std::string& path);
	Material* createEmptyMaterial(const std::string& name);
	Material* getMaterial(const std::string& name);

private:
	std::map<std::string, Texture*> m_texturesMap;
	std::map<std::string, GpuProgram*> m_shadersMap;
	std::map<std::string, Sprite*> m_spritesMap;
	std::map<std::string, Mesh*> m_meshesMap;

	std::unordered_map<std::string, Material*> m_materialsMap;
};