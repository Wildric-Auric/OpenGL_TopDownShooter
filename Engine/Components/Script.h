#pragma once
#include <GameObject.h>

class Script : public GameComponent {
public:
	Script() {};
	Script(GameObject* attachedObj);
	GameObject* attachedObj = nullptr;
	Scriptable* script = nullptr;
	static std::map<GameObject*, Script> componentList;
};

