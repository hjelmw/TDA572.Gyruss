#pragma once

#include <stdlib.h>     /* srand, rand */
#include "alien_bomb.h"

#include <sstream>
#include <random>

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
	int circularDirectionX;
	int circularDirectionY;

	// Movement
	double originX;
	double originY;
	double angle;
	int radius;

	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init(double xPos, double yPos, double xSize, double ySize, int radius, int circularDirectionX, int circularDirectionY, AlienState state)
	{
		this->position.x = xPos;
		this->position.y = yPos;

		this->width = xSize;
		this->height = ySize;
		this->radius = radius;
		this->shotsLeft = 0;
		this->currentState = state;
		this->circularDirectionX = circularDirectionX;
		this->circularDirectionY = circularDirectionY;

		// Random point inside circle of given radius
		double randN = rand() / (double)(RAND_MAX);
		double a = (randN) * 2 * M_PI;
		double r = this->radius * sqrt(randN);

		// Convert to cartesian coordinates
		double x = r * cos(a);
		double y = r * sin(a);

		// Set aim for center of screen.
		Vector2D end(x, y);
		Vector2D gameCenter(GAME_CENTER_X, GAME_CENTER_Y);

		float distance = sqrt(pow(end.x - this->position.x, 2) + pow(end.y - this->position.y, 2));
		direction = (this->position - gameCenter) / distance;

		this->radius = r;
		this->angle = 180 * this->circularDirectionX;
		this->originX = xPos;
		this->originY = yPos;

		// Print stuff to console
		std::stringstream ss;
		ss << "Alien:: Init xPos: " << this->position.x << " | yPos: " << this->position.y;
		SDL_Log(ss.str().c_str());
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			Send(ALIEN_HIT);
			SDL_Log("Alien::Hit");
		}

	}

};


/*

Alien behavior component
*/
class AlienBehaviorComponent : public Component
{
	Player* player; // Keeps track of where player currently is.

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<AlienBomb>* bombs_pool, Player* player)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
		this->player = player;
	}

	virtual void Update(float dt)
	{
		MoveAlien(dt);
		ResizeAlien(this->oldDistance, this->distance, dt);

		if (CanFire())
		{
			Fire(dt);
		}
	}
		
private:
	Alien* alien;
	ObjectPool<AlienBomb>* bombs_pool;
	int timeLastInterval;
	float distance;
	float oldDistance;

	float timeUntilReturn;
	float timeSinceLastFire;

	float scale;

	float distanceToMiddle = sqrt(pow(GAME_CENTER_X - GAME_CENTER_X + 40, 2) + pow(GAME_CENTER_Y - GAME_CENTER_Y + 40, 2));

	void MoveAlien(float dt)
	{

		alien = (Alien*)go;
		oldDistance = distance;
		distance = sqrt(pow(GAME_CENTER_X - go->position.x, 2) + pow(GAME_CENTER_Y - go->position.y, 2));

		alien->previousState = alien->currentState;

		// ---------- ALIEN LOGIC ---------- //
		/*
		Alien behavior operates as a Finite State Machine (FSM).
			- Actions happen on ALIEN_ACTION_INTERVAL.
			- After an action the alien always returns to STATE_CIRCLE.
			- The Component AlienGridBehavior selects an alien from a pool.
				and gives it a random action to perform
		*/
		switch (alien->currentState)
		{
		case Alien::STATE_INITIAL2:
		{
			if (distance <= alien->radius)
			{
				alien->originX = GAME_CENTER_X;
				alien->originY = GAME_CENTER_Y;
				alien->angle = atan2(go->position.y - GAME_CENTER_Y, go->position.x - GAME_CENTER_X) * (180.0f / M_PI);

				alien->currentState = Alien::STATE_CIRCLE;
			}
			double circleSpeedModifier = distanceToMiddle != 0 ? alien->radius / distanceToMiddle : 1;
			// Parameterized lemniscate of Gerono. Notice the axes have been flipped
			alien->angle += fmod(dt * ALIEN_SPEED_INITIAL, 360);

			go->position.y = alien->originY + 200  * alien->circularDirectionY *  cos(alien->angle * (M_PI / 180.0f)) ;
			go->position.x = alien->originX + 150  * alien->circularDirectionX * sin(2 * alien->angle * (M_PI / 180.0f)) / 2 ;

		}
		break;
		case Alien::STATE_INITIAL1:
		{
			if (distance <= alien->radius)
			{
				alien->originX = GAME_CENTER_X;
				alien->originY = GAME_CENTER_Y;
				alien->angle = atan2(go->position.y - GAME_CENTER_Y, go->position.x - GAME_CENTER_X) * (180.0f / M_PI);

				alien->currentState = Alien::STATE_CIRCLE;
			}
			go->position = go->position - (go->direction * ALIEN_SPEED_BASE * dt);
		}
		break;

		case Alien::STATE_REPOSITION:
		{
			float distance = sqrt(pow(alien->position.x - GAME_CENTER_X, 2) + pow(alien->position.y - GAME_CENTER_Y, 2));
	 		if (distance >= alien->radius)
			{
				alien->originX = GAME_CENTER_X;
				alien->originY = GAME_CENTER_Y;
				alien->angle = atan2(go->position.y - GAME_CENTER_Y, go->position.x - GAME_CENTER_X) * (180.0f / M_PI);

				// Random time until return to STATE_CIRCLE
				timeUntilReturn = engine->getElapsedTime() + (rand() % 15 + 5);

				alien->currentState = Alien::STATE_CIRCLE_OUTER;
			}
			go->position = go->position - (go->direction * ALIEN_SPEED_BASE * dt / 2);
		}
		break;

		case Alien::STATE_CIRCLE_OUTER:
		{
			if (timeUntilReturn - engine->getElapsedTime() <= 0)
			{
				Vector2D returnPos = Vector2D(GAME_CENTER_X, GAME_CENTER_Y);
				float distance = sqrt(pow(returnPos.x - alien->position.x, 2) + pow(returnPos.y - alien->position.y, 2));
				alien->direction = (alien->position - returnPos) / distance;

				alien->currentState = Alien::STATE_INITIAL1;
				break;
			}
		} // STATE_CIRCLE_OUTER falls through to STATE_CIRCLE
		case Alien::STATE_CIRCLE:
		{
			double circleSpeedModifier = distanceToMiddle != 0 ?  alien->radius / distanceToMiddle : 1;
			alien->angle += fmod(dt * ALIEN_SPEED_CIRCLE * circleSpeedModifier, 360);
			go->position.x = alien->originX + alien->radius * cos(alien->angle * (M_PI / 180.0f));
			go->position.y = alien->originY + alien->radius * sin(alien->angle * (M_PI / 180.0f));
		}
		break;

		case Alien::STATE_FIRE1:
		{
			alien->shotsLeft = ALIEN_CONSECUTIVE_SHOTS;
			timeSinceLastFire = engine->getElapsedTime() + ALIEN_FIRE_INTERVAL;
			alien->currentState = alien->Alien::STATE_CIRCLE_OUTER;
		}
		break;
		}
	}

	bool CanFire()
	{
		return alien->shotsLeft > 0;
	}

	void Fire(float dt)
	{
		if(timeSinceLastFire - engine->getElapsedTime() <= 0 && alien->shotsLeft >= 0)
		{
			AlienBomb* bomb = bombs_pool->FirstAvailable();
			if (bomb != NULL)
			{
				bomb->Init(player->position, go->position.x, go->position.y);
				game_objects->insert(bomb);
				alien->shotsLeft--;
			}
			timeSinceLastFire = engine->getElapsedTime() + ALIEN_FIRE_INTERVAL;
		}
	}

	void ResizeAlien(double oldDistance, double newDistance, float dt)
	{
		double alienSizeModifier = 50.0f * dt;

		// Only need to resize if position has changed
		if (newDistance == oldDistance)
		{
			return;
		}
		else if (newDistance > oldDistance && (go->width < 32 && go->height < 32))
		{
			go->width += alienSizeModifier;
			go->height += alienSizeModifier;
		}
		else if (newDistance < oldDistance && (go->width > 15 && go->height > 15))
		{
			go->width -= alienSizeModifier;
			go->height -= alienSizeModifier;
		}
	}

};