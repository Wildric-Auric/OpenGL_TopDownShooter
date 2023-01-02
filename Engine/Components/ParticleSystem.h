#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Transform.h"
#include "Globals.h"
#include <deque>


struct ParticleProperties {
	fVec2 absoluteStartPosition = fVec2(0.0f, 0.0f); //Read only; buffer of position  used to pass beginning absolute position to particle

	float lifetime = 1.0f;
	int lifedistance = 1000; //in pixels
	//s stands for start and e for end; notice that position is relative to particle system position
	fVec2 sPosition = fVec2(0.0f,0.0f); 
	
	fVec2 sDirection = fVec2(0.0f,1.0f);
	fVec2 eDirection = fVec2(0.0f,1.0f);
	float directionVarDuration = 0.0f;

	fVec2 sScale = fVec2(1.0f,1.0f);
	fVec2 eScale = fVec2(1.0f,1.0f);
	float scaleVarDuration = 0.0f;

	float sSpeed = 100.0f; //Pixels per sec
	float eSpeed = 1.0f;
	float speedVarDuration = 0.0f;

};
class Particle {	
private:
public:
	GameObject go;
	Sprite* sprite;
	Transform* transform;
	bool isActive = 0; 
	ParticleProperties prop;
	fVec2 currentPosition;
	fVec2 currentScale;
	float currentSpeed;
	fVec2 currentDirection;

	double clock = 0.0;
	float distance = 0.0;

	void Disable();
	void Enable();
};


class ParticleSystem : public GameComponent {
private:
	double clock = 0.0;
public:
	static std::string GetType() { return "ParticleSystem"; };

	GameObject* attachedObj = nullptr;
	ParticleSystem() {};
	ParticleSystem(GameObject* attachedObj);
    static std::map<GameObject*, ParticleSystem> componentList;

	std::deque<Particle> pool;
	std::deque<int> disabled;
	std::deque<int> enabled;
	int maxParticles = 100;

	double emissionFrequency = 1.0; //every emissionf Emission is called
	uint16 emissionQuantity =  1;
	int initNum = 5;
	ParticleProperties prop;
	bool recycle = 1;
	std::string shader =	SHADER_DEFAULT;
	std::string texture =   TEXTURE_DEFAULT;

	void Update();
	void UpdateParticle(int index);
	void Emit();
	void Init();
	void InitParticle();

	void Gui() override;
};