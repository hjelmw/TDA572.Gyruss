#pragma once

#include <stdlib.h>     /* srand, rand */
#include "alien_bomb.h"

#include <sstream>

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
		STATE_INITIAL1, STATE_INITIAL2,
		STATE_CIRCLE,
		STATE_REPOSITION,
		STATE_DIVE,
		STATE_FIRE1, STATE_FIRE2
	} alienState;
	alienState currentState;
	int radius;

	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init(double xPos, double yPos, double xSize, double ySize, int radius, int state)
	{
		this->position.x = xPos;
		this->position.y = yPos;
		this->width = xSize;
		this->height = ySize;
		this->radius = radius;

		//this->currentState = state == 1 ? STATE_INITIAL1 : STATE_INITIAL2;
		this->currentState = STATE_INITIAL1;

		Vector2D end(GAME_CENTER_X, GAME_CENTER_Y);
		float distance = sqrt(pow(end.x - position.x, 2) + pow(end.y - position.y, 2));
		direction = (position - Vector2D(GAME_CENTER_X, GAME_CENTER_Y)) / distance;
		
		std::stringstream ss;
		ss << "Alien:: Init xPos: " << this->position.x << " | yPos: " << this->position.y;
		SDL_Log(ss.str().c_str());
		GameObject::Init();

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


/*

Alien movement behavior

*/
class AlienBehaviorComponent : public Component
{

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienBomb> * bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Update(float dt)
	{
		MoveAlien(dt);
	}

private:
	ObjectPool<AlienBomb>* bombs_pool;
	float randomInterval;
	int timeLastInterval;

	bool changeDirection;
	
	double originX;
	double originY;
	double radius;
	double angle;

	void MoveAlien(float dt)
	{

		if (go->width >= 10 || go->height >= 10)
			go->changeDirection = false;
		else if (go->width <= 2 || go->height <= 2)
			go->changeDirection = true;

		/* Implement state machine logic here */
		Alien* alien = (Alien*)go;
		float distance = sqrt(pow(GAME_CENTER_X - go->position.x, 2) + pow(GAME_CENTER_Y - go->position.y, 2));

		switch (alien->currentState)
		{
		case Alien::STATE_INITIAL1:
			go->position = go->position - (go->direction * ALIEN_SPEED_BASE * dt);

			if (distance <= alien->radius)
			{
				originX = GAME_CENTER_X;
				originY = GAME_CENTER_Y;
				angle = atan2(go->position.y - GAME_CENTER_Y, go->position.x - GAME_CENTER_X) * (180.0f / M_PI);

				alien->currentState = Alien::STATE_CIRCLE;
				SDL_Log("Alien::In circle");
			}
			break;	
		case Alien::STATE_INITIAL2:
 			break;
		case Alien::STATE_DIVE:
			go->position = go->position - (go->direction * ALIEN_SPEED_BASE * dt);
			break;
		case Alien::STATE_CIRCLE:


			angle += dt * ALIEN_SPEED_CIRCLE;
			go->position.x = originX + alien->radius * cos((float)((int)angle * (M_PI / 180.0f)));
			go->position.y = originY + alien->radius * sin((float)((int)angle * (M_PI / 180.0f)));
			ShrinkAlien(dt * ALIEN_SIZE_SPEED);
			break;
		}
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
};

