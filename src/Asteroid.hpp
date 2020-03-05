#pragma once


//////////////////////////////////////////////////////////////
//						Asteroid
// \brief Asteroids hurl towards the outer edges of the game.
//			They also absors bullets
//
// \params	GAME_CENTER_X, GAME_CENTER_Y, ASTEROIDS_SPEED_BASE
//			, ASTEROIDS_ROTATION_SPEED, 
//
// \see EnemyController, Game
/////////////////////////////////////////////////////////////



#include "Component.hpp"
#include "GameObject.hpp"
#include "Common.hpp"


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