#pragma once

//////////////////////////////////////////////////////////////
//						Orb
// \brief Second type of ammunition for alien
//			Fires a burst of orbs that travel towards points close to player
//
// \params 
//			  GAME_CENTER_X, GAME_CENTER_Y, ALIEN_SPEED_INITIAL
//			, ALIEN_SPEED_BASE, ALIEN_SPEED_CIRCLE,
//			, ALIEN_CONSECUTIVE_SHOTS, ALIEN_FIRE_INTERVAL			
//
// \see EnemyController, Game
/////////////////////////////////////////////////////////////



#include "Component.hpp"
#include "GameObject.hpp"
#include "Vector2D.hpp"


class AlienOrbBehaviorComponent : public Component
{

public:
	virtual void Update(float dt);
};


class AlienOrb : public GameObject
{
public:
	double orbSpeed;

	virtual void Init(Vector2D playerPosition, double speed, double xPos, double yPos);
	virtual void Receive(Message m);
};