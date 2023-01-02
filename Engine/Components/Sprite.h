#pragma once
#include "GameObject.h"
#include "RessourcesLoader.h"

#include "Primitives.h"

class Sprite : public GameComponent {
private:
	uint32 lastSortingLayer = 0;
public:
	static std::string GetType() { return "Sprite"; };
	Texture* texture = &Texture::resList[TEXTURE_DEFAULT];
	Sprite() {};
	Sprite(GameObject* go);
	Shader* shader = &Shader::resList[SHADER_DEFAULT];
	Quad container = Quad(iVec2(0), Texture::resList[TEXTURE_DEFAULT].size.x, Texture::resList[TEXTURE_DEFAULT].size.y); //ReadOnly
	GameObject* go;
	uint32 sortingLayer = 0; //ReadOnly
	double zbuffer = 1.0; //ReadOnly

	void SetTexture(std::string path, bool alpha = 1, bool repeat = 0);
	void SetShader(std::string path);
	void SetSortingLayer(uint32 order);
	void Gui() override;
	static std::map<GameObject*, Sprite> componentList;
};