#pragma once

//////////////////////////////////////////////////////////////
//						Alien
// \brief Main enemy in the game.
//			It has several states which decide its behavior
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
#include "AvancezLib.hpp"
#include "Player.hpp"
#include "AlienBomb.hpp"

#include <sstream>
#include <random>
#include <stdlib.h>     /* srand, rand */

class AlienBomb;
class Alien : public GameObject
{

public:
	/*
	Alien movement patterns
	Initial1: Spawn outside of screen and enter from one of the corners
	Initial2: Spawn in center of screen
	Circle: Circle current position while shooting rockets at player
	Dive: Dive at player then return to circle
	Fire1: Shoot constant speed rocket
	Fire2: Shoot accelerating rocket

	State transitions:
	Fire1,Fire2 -> Circle
	Reposition -> Circle
	Dive -> Reposition -> Circle
	*/
	typedef enum alienStates {
		STATE_INITIAL1,
		STATE_INITIAL2,
		STATE_CIRCLE,
		STATE_CIRCLE_OUTER,
		STATE_REPOSITION,
		STATE_FIRE1, STATE_FIRE2

	} AlienState;
	AlienState currentState;
	AlienState previousState;
	
	int shotsLeft;

	// Movement
	short circularDirectionX;
	short circularDirectionY;
	short circularDirectionZ;

	double originX;
	double originY;
	double angle;
	float radius;

	virtual ~Alien();

	virtual void Init(double xPos, double yPos, double xSize, double ySize, float radius,short circularDirectionX, short circularDirectionY, short circularDirectionZ, AlienState state);

	virtual void Receive(Message m);

};


/*

Alien behavior component
*/
class AlienBehaviorComponent : public Component
{
	Player* player; // Keeps track of where player currently is.

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienBomb>* bombsPool, Player* player);

	virtual void Update(float dt);
		
private:
	Alien* alien;
	ObjectPool<AlienBomb>* bombsPool;

	// Used for by alien to set aim to player etc.
	float distance;
	
	// Used to determine size of alien for 2.5D effect
	float oldDistance;

	// Used to decide how long a STATE_CIRCLE_OUTER should last
	float timeUntilReturn;

	// Firing delay
	float timeSinceLastFire;

	// Used in STATE_INITIAL2 for bernoulli movement
	float scale;

	// Radius of small circle in middle of screen
	float distanceToMiddle;

	void MoveAlien(float dt);

	bool CanFire();

	void Fire(float dt);

	// Alien sprite size changes depending on how close to the middle of the screen it is to simulate 2.5D effect
	void ResizeAlien(double oldDistance, double newDistance, float dt);

	void RotateAlien();

};