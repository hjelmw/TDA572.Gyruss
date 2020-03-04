#pragma once

//////////////////////////////////////////////////////////////
//					AlienBomb
// \brief Enemy projectiles. Shot by at player when alien in state
//			 STATE_FIRE1, STATE_FIRE2
//	
// \params BOMB_SPEED
// \see Game, Alien, EnemyController
/////////////////////////////////////////////////////////////

//#include "Component.h"
//#include "GameObject.h"
//#include "Game.h"

class AlienBombBehaviorComponent : public Component
{

public:
	void Update(float dt);
};

class AlienBomb : public GameObject
{
public:
	bool changeDirection;

	virtual void Init(Vector2D playerPosition, double xPos, double yPos);
	virtual void Receive(Message m);
};