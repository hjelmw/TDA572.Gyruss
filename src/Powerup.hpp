#pragma once

//////////////////////////////////////////////////////////////
//						Powerup
// \brief Spawns along the circumference on the circle that the player moves around
//			Colliding with it gives player invlunerability and shots pierce aliens
//
//	
// \see AvancezLib
/////////////////////////////////////////////////////////////



#include "Component.hpp"
#include "GameObject.hpp"
#include "Audio.hpp"
#include "Player.hpp"


class PowerupBehaviorComponent: public Component
{
Player* player;
public:
	virtual void Init();
	virtual void Update(float dt);
};


class Powerup : public GameObject
{
public:
	virtual void Init();
	virtual void Receive(Message m);
};

//class AlienOrbBehaviorComponent : public Component
//{
//
//public:
//	virtual void Update(float dt);
//};
//
//
//class AlienOrb : public GameObject
//{
//public:
//	double orbSpeed;
//
//	virtual void Init(Vector2D playerPosition, double speed, double xPos, double yPos);
//	virtual void Receive(Message m);
//};