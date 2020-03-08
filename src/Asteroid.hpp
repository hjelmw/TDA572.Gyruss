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
#include <random>


class AsteroidBehaviourComponent : public Component
{

public:
	virtual void Update(float dt);
};

class Asteroid : public GameObject
{
	Vector2D velocity;

public:
	virtual ~Asteroid();
	virtual void Init();
	virtual void Receive(Message m);
};