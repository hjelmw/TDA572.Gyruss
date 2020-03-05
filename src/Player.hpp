#pragma once

//////////////////////////////////////////////////////////////
//					Player
// \brief Player. Can move in a circle around the screen 
//			and shoot rockets towards the center
//	
// \params PLAYER_SPEED
// \see Game
/////////////////////////////////////////////////////////////

#include "Component.hpp"
#include "AvancezLib.hpp"
#include "ObjectPool.hpp"
#include "GameObject.hpp"
#include "Rocket.hpp"


class PlayerBehaviourComponent : public Component
{
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Rocket>* rocketsPool;

	float radius;
	float angle;
	float angle_percent;

	float originX;
	float originY;
public:
	virtual ~PlayerBehaviourComponent();

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Rocket>* rocketsPool);

	virtual void Init();

	// Player can move left, right or fire rockets towards the middle of the screen
	virtual void Update(float dt);


	// move the player left or right
	void Move(float move);

	// return true if enough time has passed from the previous rocket
	bool CanFire();

	// Changes rotation of player to point towards middle of screen
	void RotatePlayer();
};


// the main player
class Player : public GameObject
{
	float playerInvulnerable;
public:

	int lives;	// it's game over when goes below zero 

	virtual ~Player();

	virtual void Init();

	virtual void Receive(Message m);

	void RemoveLife();
};
