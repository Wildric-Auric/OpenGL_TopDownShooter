#pragma once
#include "Texture.h"
#include "Maths.h"
#include "Primitives.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "map"

//Virtual
class Drawable {
public:
	virtual void Draw(uint8_t slot = 0) {};
};

class Updatable {
public:
	virtual void Update() {};
};
//-----------------

class GameObject : public Drawable {
private:
	static int numberOfGameObjects;
public:
	//int id;  //ReadOnly
	std::string name  = "new GameObject"; //Read only see Rename function;
	//identifier
	uint32 id = 0;
	void Draw(int8 textureSlot = 0);
	void BasicDraw(int8 textureSlot = 0);
	void Rename(std::string newName);
	GameObject();

	template<typename T>
	T* GetComponent() {
		T* component = nullptr;
		for (auto it = T::componentList.begin(); it != T::componentList.end(); it++) {
			if (it->first == this) {
				component = &(it->second);
				break;
			}
		}
		return component;
	};

	template<typename T>
	T* AddComponent() {
		T::componentList[this] = T(this);
		return &(T::componentList[this]);
	};

	template<typename T>
	void DeleteComponent() {
		T::componentList.erase(this);
	}
};






//WARNING::Collider is deprecated see Collider2

class Collider {
private: 
	Vector2<int> manualSize;
public:
	Collider() {};
	Collider(GameObject* attachedObj, Vector2<int> offset = Vector2<int>(0, 0), Vector2<int>* newSize = nullptr);
	GameObject* attachedObj;  //TODO:: Make this GameObjectClone it's a mistake
	iVec2* position; //ReadOnly   //TODO:: Make it impossible to overwrite readonly variables
	iVec2* size; //ReadOnly
	iVec2 offset;
	iVec2 GetPosition();
	iVec2 GetSize();
	void Resize(Vector2<int> newSize);

	static std::map<GameObject*, Collider> componentList;
};





class Scriptable {
public:
	Scriptable(GameObject* goc = nullptr) {}
	virtual void Start() {};
	virtual void Update() {};
	virtual std::string name() { return "None"; };
	GameObject* goc;
};



//------------------------------------
