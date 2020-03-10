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

	playerFire = engine->LoadAudio("data/fire.wav", false);
	laserFire = engine->LoadAudio("data/laser.wav", false);
	playerHit = engine->LoadAudio("data/damage.wav", false);

	// Player stands still at start of game
	velocity = 30;
	acceleration = -velocity;
}

// Player can move left, right or fire rockets towards the middle of the screen
void PlayerBehaviourComponent::Update(float dt)
{
	auto* player = (Player*)go;

	if (player->lives < playerHP)
		playerHit->Play(5);

	if (player->invulnerabilityTime - engine->GetElapsedTime() < 0)
	{
		player->invulnerable = false;
		player->shotsPiercing = false;
	}

	velocity = PLAYER_VELOCITY * dt + acceleration * dt;

	AvancezLib::KeyStatus keys;
	engine->GetKeyStatus(keys);
	if (keys.right)
	{
		acceleration += PLAYER_ACCELERATION * dt;
		if (acceleration > PLAYER_MAXIMUM_VELOCITY)
			acceleration = PLAYER_MAXIMUM_VELOCITY;
	}
	else if (keys.left)
	{
		acceleration -= PLAYER_ACCELERATION * dt;
		if (acceleration < -PLAYER_MAXIMUM_VELOCITY)
			acceleration = -PLAYER_MAXIMUM_VELOCITY;
	}

	// If no key is pressed player speed should slowly decelerate to 0
	else
	{
		if (velocity > 0)
		{
			acceleration -= PLAYER_IDLE_DECELERATION * dt;
			if (velocity <= 0)
				velocity = 0;
		}
		else if (velocity < 0)
		{
			acceleration += PLAYER_IDLE_DECELERATION * dt;
			if (velocity >= 0)
				velocity = 0;
		}
	}
	Move(velocity);


	if (keys.fire)
	{
		if (CanFire())
		{
			// fetches a rocket from the pool and use it in game_objects
			Rocket* rocket = rocketsPool->FirstAvailable();
			if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
			{
				rocket->Init(go->position.x, go->position.y, player->shotsPiercing);
				game_objects->insert(rocket);

				if (player->shotsPiercing)
					laserFire->Play(0);
				else
					playerFire->Play(0);

			}
		}
	}
	playerHP = player->lives;
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
		}
	}

	if (m == POWERUP_PICKUP)
	{
		PickeUpPowerup();
	}
}

void Player::RemoveLife()
{
	lives--;
	invulnerable = true;
	invulnerabilityTime = AvancezLib::GetElapsedTime() + PLAYER_INVULNERABLE_TIME;
	//SDL_Log("remaining lives %d", lives);
}

void Player::PickeUpPowerup()
{
	lives+=2;
	invulnerable = true;
	shotsPiercing = true;
	invulnerabilityTime = AvancezLib::GetElapsedTime() + PLAYER_INVULNERABLE_TIME;
}