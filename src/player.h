#pragma once

#include <math.h>

class PlayerBehaviourComponent : public Component
{
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Rocket> * rockets_pool;

	int radius;
	float angle;
	float angle_percent;


	double originX;
	double originY;
public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Rocket> * rockets_pool)
	{
		Component::Create(engine, go, game_objects);
		this->rockets_pool = rockets_pool;
	}

	virtual void Init()
	{

		radius = engine-> screenWidth  / 2 - 40;
		originX = engine->screenWidth / 2 - 20;
		originY = engine->screenHeight / 2 - 20;

		go->position.x = originX + radius * cos((float)((int)angle * (M_PI / 180.0f)));
		go->position.y = originY + radius * sin((float)((int)angle * (M_PI / 180.0f)));

		time_fire_pressed = -10000.f;
	}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		engine->getKeyStatus(keys);
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
				Rocket * rocket = rockets_pool->FirstAvailable();
				if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
				{
					rocket->Init(go->position.x, go->position.y);
					game_objects->insert(rocket);
				}
			}
		}
	}


	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{

		angle += move;

		//int originX = engine->screenWidth / 2;
		//int originY = engine->screenHeight / 2;

		go->position.x = originX + radius * cos( (float) ( (int) angle * (M_PI /180.0f) ));
		go->position.y   = originY  + radius * sin( (float) ( (int) angle * (M_PI / 180.0f) ));

		/*go->position.x += move;

		if (go->position.x > (640 - 32))
			go->position.x = 640 - 32;
			
		if (go->position.x < 0)
			go->position.x = 0;*/
	}

	// return true if enough time has passed from the previous rocket
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed))
			return false;

		time_fire_pressed = engine->getElapsedTime();

		return true;
	}
};


// the main player
class Player : public GameObject
{
	float playerInvulnerable;
public:

	int lives;	// it's game over when goes below zero 

	virtual ~Player()	{		SDL_Log("Player::~Player");	}

	virtual void Init()
	{
		//playerInvulnerable = engine->getElapsedTime() + PLAYER_INVULNERABLE_TIME;

		SDL_Log("Player::Init");
		GameObject::Init();
		lives = NUM_LIVES;
	}

	virtual void Receive(Message m) 
	{
		if (m == HIT)
		{ 
			SDL_Log("Player::Hit!");
			RemoveLife();

			if (lives < 0)
				Send(GAME_OVER);
		}
	}

	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
	}
};
