#include "TextHandler.h"
#include "Transform.h"
#include "Sprite.h"
#include "Utilities.h"

#define MAX_BIT_10 1023.0
TextHandler::TextHandler(GameObject* go) {
	this->attachedObj = go;
};

void TextHandler::Update() {
	float x = this->position.x;
	uint32 temp  = (uint32)(this->colors.g * MAX_BIT_10) | ((uint32)(this->colors.r * MAX_BIT_10) << 0xA);
	uint32 temp0 = (uint32)(this->colors.a * MAX_BIT_10) | ((uint32)(this->colors.b * MAX_BIT_10) << 0xA);

	for (std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Transform* transform       = chr->go.GetComponent<Transform>(); //TODO::add transform variable to glyph
		Sprite*    sprite          = chr->go.GetComponent<Sprite>();


		sprite->vertexAttributes.x = *((float*)&temp );
		sprite->vertexAttributes.y = *((float*)&temp0);

		transform->scale.x = this->scale.x;
		transform->scale.y = this->scale.y;
		transform->position.x = x + (chr->glyph->bearing.x + chr->glyph->size.x / 2) * transform->scale.x;
		transform->position.y = this->position.y - (chr->glyph->size.y - chr->glyph->bearing.y - chr->glyph->size.y / 2) * transform->scale.y;
		sprite->isBatched = this->isBatched;
		sprite->Update(); 
		x += (chr->glyph->advance / 64 ) * transform->scale.x;
	}
}

void TextHandler::UpdateGlyphs() {
	if (characters.size() < 1) 
		characters.push_back(Character());

	std::list<Character>::iterator iter = characters.begin();
	for (char c : text) {
		if (iter == characters.end()) {
			characters.push_back(Character());
			iter = --characters.end();
		}
		iter->go.AddComponent<Transform>();
		Sprite* sprite = iter->go.AddComponent<Sprite>();

		iter->glyph = &this->font.charactersMap.find(c)->second;
		sprite->SetShader(this->shader); //TODO::AddFunction to set shader by its pointer
		sprite->isBatched = this->isBatched;
		sprite->SetTexture(&iter->glyph->texture);
		sprite->container.UpdateSize(iter->glyph->size.x, iter->glyph->size.y);
		++iter;
	}
	while (iter != characters.end()) {
		iter->go.DeleteComponents();
		iter = characters.erase(iter);
	}
}




void* TextHandler::GetGameObject(){
	return (void*)this->attachedObj;
}
void  TextHandler::SetGameObject(void* go) {
	this->attachedObj = (GameObject*)go;
}

void TextHandler::Gui() {
	if (NWGui::FileHolder("Font", this->font.name)) {
		std::string path = GetFile("Font files\0*.ttf");
		if (path != "") {
			this->font.Delete();
			this->font.LoadFont(path);
			UpdateGlyphs();
		}
	}

	if (NWGui::FileHolder("Shader", this->shader)) {
		std::string path = GetFile("Shade\0*.shader");
		if (path != "") {this->shader = path;}
	}
	NWGui::CheckBox("isBatched", &this->isBatched);
	NWGui::DragValue("Position", &this->position, GuiDataType_Float, 2, 1.0);
	NWGui::DragValue("Scale", &this->scale, GuiDataType_Float,2, 0.05f, 0.0f, 100.0f);
	NWGui::DragValue("Color", &this->colors, GuiDataType_Float, 4, 0.05f, 0.0f, 1.0f);
	NWGui::Input("", &this->text);
	if (NWGui::Button("Update characters"))
		this->UpdateGlyphs();
}

int TextHandler::Serialize(std::fstream* data, int offset) {
	return 1;
}

int TextHandler::Deserialize(std::fstream* data, int offset) {
	return 0;
}

TextHandler::~TextHandler() {
	this->font.Delete(); //Temporary
}


