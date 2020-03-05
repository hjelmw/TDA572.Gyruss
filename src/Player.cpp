#pragma once

//////////////////////////////////////////////////////////////
//					Player
// \brief Player. Can move in a circle around the screen 
//			and shoot rockets towards the center
//	
// \params PLAYER_SPEED
// \see Game
/////////////////////////////////////////////////////////////



#include "Player.hpp"
#include "Common.hpp"
#include <math.h>


PlayerBehaviourComponent::~PlayerBehaviourComponent() {}

void PlayerBehaviourComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Rocket>* rocketsPool)
{
	Component::Create(engine, go, game_objects);
	this->rocketsPool = rocketsPool;
}

void PlayerBehaviourComponent::Init()
{
	go->width = 50;
	go->height = 50;

	radius  = (float) engine->screenWidth / 2 - 40;
	originX = (float) engine->screenWidth / 2 - 20;
	originY = (float) engine->screenHeight / 2 - 20;
	
	go->position.x = (double) (originX + radius * cos((angle * (M_PI / 180.0f))));
	go->position.y = (double) (originY + radius * sin((angle * (M_PI / 180.0f))));

	time_fire_pressed = -10000.f;
}

// Player can move left, right or fire rockets towards the middle of the screen
void PlayerBehaviourComponent::Update(float dt)
{
	auto* player = (Player*)go;

	if (player->invulnerabilityTime - engine->GetElapsedTime() < 0)
		player->invulnerable = false;

	AvancezLib::KeyStatus keys;
	engine->GetKeyStatus(keys);
	if (keys.right)
	{
		Move(dt * PLAYER_SPEED);
	}
	if (keys.left)
	{
		Move(-dt * PLAYER_SPEED);
	}
	if (keys.fire)
	{
		if (CanFire())
		{
			// fetches a rocket from the pool and use it in game_objects
			Rocket* rocket = rocketsPool->FirstAvailable();
			if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
			{
				rocket->Init(go->position.x, go->position.y);
				game_objects->insert(rocket);
			}
		}
	}

	RotatePlayer();
}


// move the player left or right
void PlayerBehaviourComponent::Move(float move)
{

	angle += move;

	go->position.x = originX + radius * cos((angle * (M_PI / 180.0f)));
	go->position.y = originY + radius * sin((angle * (M_PI / 180.0f)));
}

// return true if enough time has passed from the previous rocket
bool PlayerBehaviourComponent::CanFire()
{
	// shoot just if enough time passed by
	if ((engine->GetElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / 1.0f /*game_speed*/))
		return false;

	time_fire_pressed = engine->GetElapsedTime();

	return true;
}

void PlayerBehaviourComponent::RotatePlayer() {	
	go->angle = atan2(go->position.y - GAME_CENTER_Y, go->position.x - GAME_CENTER_X) * (180.0f / M_PI) - 90;
}

Player::~Player() { SDL_Log("Player::~Player"); }

void Player::Init()
{
	//playerInvulnerable = engine->GetElapsedTime() + PLAYER_INVULNERABLE_TIME;

	SDL_Log("Player::Init");
	GameObject::Init();
	lives = NUM_LIVES;
}

void Player::Receive(Message m)
{
	if (m == HIT)
	{
		if (!invulnerable)
		{
			SDL_Log("Player::Hit!");

			RemoveLife();
			if (lives < 0)
				Send(GAME_OVER);

			invulnerabilityTime = AvancezLib::GetElapsedTime() + PLAYER_INVULNERABLE_TIME;
		}
		invulnerable = true;
	}
}

void Player::RemoveLife()
{
	lives--;
	invulnerable = true;
	SDL_Log("remaining lives %d", lives);
}
