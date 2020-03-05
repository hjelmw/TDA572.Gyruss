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
#include "Vector2D.hpp"
#include "Common.hpp"

class AlienBombBehaviorComponent : public Component
{

public:
	virtual void Update(float dt);
};

class AlienBomb : public GameObject
{
public:

	float bombSpeed;

	virtual void Init(Vector2D playerPosition, double speed, double xPos, double yPos);
	virtual void Receive(Message m);
};