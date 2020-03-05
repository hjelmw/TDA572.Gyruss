#pragma once
#pragma once

//////////////////////////////////////////////////////////////
//					AlienBomb
// \brief Enemy projectiles. Shot by at playerSprite when alien in state
//			 STATE_FIRE1, STATE_FIRE2
//	
// \params BOMB_SPEED
// \see Game, Alien, EnemyController
/////////////////////////////////////////////////////////////

#include "Component.hpp"
#include "GameObject.hpp"
#include "Common.hpp"
//#include "Game.h"

class AsteroidBehaviourComponent : public Component
{

public:
	virtual void Update(float dt);
};

class Asteroid : public GameObject
{
public:

	virtual void Init(Vector2D playerPosition, double xPos, double yPos);
	virtual void Receive(Message m);
};