#pragma once

#include <stdlib.h>     /* srand, rand */
#include "alien_bomb.h"

class AlienBehaviorComponent : public Component
{

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienBomb> * bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Init()
	{
		randomInterval = (float) (rand() % 150 + 1) / (float) 10.0f;

		originX = engine->screenWidth / 2 ;
		originY = engine->screenHeight / 2 ;

	}

	virtual void Update(float dt)
	{
		MoveAlien(dt * ALIEN_SPEED);
		ShrinkAlien(dt * ALIEN_SIZE_SPEED);

		if (AlienCanFire())
			ShootBomb();		
	}

private:
	ObjectPool<AlienBomb>* bombs_pool;
	float randomInterval;
	int timeLastInterval;

	double originX;
	double originY;
	double radius = 40;
	double angle;
	bool AlienCanFire()
	{
		if ((engine->getElapsedTime() - timeLastInterval) < randomInterval + BOMB_TIME_INTERVAL)
			return false;

		timeLastInterval = engine->getElapsedTime();
		randomInterval = rand() % 10 + 1;

		SDL_Log("Alien::Fire!");
		return true;
	}

	void MoveAlien(float move)
	{
		if (go->width >= 10 || go->height >= 10)
			go->changeDirection = false;
		else if (go->width <= 2 || go->height <= 2)
			go->changeDirection = true;

		angle += move;

		//int originX = engine->screenWidth / 2;
		//int originY = engine->screenHeight / 2;

		go->position.x = originX + radius * cos((float)((int)angle * (M_PI / 180.0f)));
		go->position.y = originY + radius * sin((float)((int)angle * (M_PI / 180.0f)));

		/* Implement state machine logic here */
	}

	void ShrinkAlien(float move)
	{
		if (go->changeDirection)
		{
			go->width += move; go->height += move;
		}
		else
		{
			go->width -= move; go->height -= move;
		}
	}


	void ShootBomb()
	{
		AlienBomb * bomb = bombs_pool->FirstAvailable();
		if (bomb != NULL)
		{
			//bomb->Init(go->position.x, go->position.y);
			game_objects->insert(bomb);
		}
	}
};

#include <sstream>

class Alien : public GameObject
{
public:

	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init(double xPos, double yPos, double xSize, double ySize)
	{
		std::stringstream ss;
		ss << "Alien:: Init xPos: " << xPos << " | yPos: " << yPos;
		SDL_Log(ss.str().c_str());
		GameObject::Init();

		this->position.x = xPos;
		this->position.y = yPos;
		this->width = xSize;
		this->height = ySize;

		changeDirection = false;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == ALIENS_MOVE_GRID)
		{
			position.y += 40;
			changeDirection = !changeDirection;

			if (changeDirection)
				position.x += 1;
			else
				position.x -= 1;
		}

		if (m == HIT)
		{
			enabled = false;
			Send(ALIEN_HIT);
			SDL_Log("Alien::Hit");
		}

	}

};