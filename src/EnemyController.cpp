#pragma once

//////////////////////////////////////////////////////////////
//					EnemyController
// \brief EnemyController gives the enemies actions to perform during game loop
//	
// \params GAME_CENTER_X, GAME_CENTER_Y, ENEMY_ACTION_INTERVAL
// \see Game
/////////////////////////////////////////////////////////////




#include "EnemyController.hpp"


EnemyControllerBehaviorComponent::~EnemyControllerBehaviorComponent() {}


void EnemyControllerBehaviorComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Alien>* aliensPool, ObjectPool<AlienBomb>* bombsPool, ObjectPool<AlienOrb>* orbsPool, ObjectPool<Asteroid>* asteroidsPool, Player* player)
{
	Component::Create(engine, go, game_objects);

	this->aliensPool = aliensPool;
	this->bombsPool = bombsPool;
	this->orbsPool = orbsPool;
	this->asteroidsPool = asteroidsPool;
	this->player = player;

	// This will decrement on every new level
	((EnemyController*)go)->asteroidInterval = ASTEROID_TIME_INTERVAL;
	((EnemyController*)go)->alienActionInterval = ENEMY_ACTION_INTERVAL;
}


void EnemyControllerBehaviorComponent::Init()
{
	timeAlienAction = engine->GetElapsedTime() + ((EnemyController*)go)->alienActionInterval;

	SpawnAliens(aliensPool);

	changeDirection = false;
}


void EnemyControllerBehaviorComponent::Update(float dt)
{
	auto* alien = aliensPool->SelectRandom();
	if (alien != NULL)
	{
		double distanceToPlayer = sqrt(pow(player->position.x - alien->position.x, 2) + pow(player->position.y - alien->position.y, 2));

		if (AlienCanSeePlayer(alien) && (alienVisionFireInterval - engine->GetElapsedTime() < 0) && alien->currentState != Alien::STATE_REPOSITION)
		{
			alien->currentState = Alien::STATE_FIRE3;
			alienVisionFireInterval = engine->GetElapsedTime() + ALIEN_NEAR_FIRE_INTERVAL;
			SDL_Log("Alien::See player!");
		}

		int randomAlienAction = AlienCanPerformRandomAction();
		if (randomAlienAction != 0xFFFF)
		{
			if (alien->currentState != Alien::STATE_INITIAL2 && alien->currentState != Alien::STATE_INITIAL1)
			{
				alien->previousState = alien->currentState;
				GiveAlienRandomState(alien, static_cast<Alien::AlienState>(randomAlienAction));
			}
		}

		if (CanSpawnAsteroids())
		{
			SpawnAsteroids();
		}
	}
}


bool EnemyControllerBehaviorComponent::AlienCanSeePlayer(Alien* alien)
{
	double distanceToPlayer = sqrt(pow(player->position.x - alien->position.x, 2) + pow(player->position.y - alien->position.y, 2));
	return distanceToPlayer < ALIEN_NEAR_VISION_DISTANCE && alien->currentState != Alien::STATE_INITIAL2;
}


void EnemyControllerBehaviorComponent::GiveAlienRandomState(Alien* alien, Alien::AlienState alienAction)
{
	switch (alienAction)
	{
	case Alien::STATE_REPOSITION:
	{
		// Generate new random position for the swarm
		int randX = rand() % ((engine->screenWidth / 2) - 50) + 400;
		int randY = rand() % ((engine->screenHeight / 2) - 50) + 400;
		for (int i = 0; i < ALIENS_IN_SWARM; i++)
		{
			Alien* alienToReposition = aliensPool->SelectRandom();
			if ((alienToReposition->currentState == Alien::STATE_CIRCLE || alienToReposition->currentState == Alien::STATE_CIRCLE_OUTER))
			{
				// Slight variation in swarm position for each alien
				double swarmPosX = randX + fmod(rand(), 80);
				double swarmPosY = randX + fmod(rand(), 80);
				Vector2D newPosition = Vector2D(swarmPosX, swarmPosY);

				// Set alien direction to new position
				double distance = sqrt(pow(newPosition.x - alien->position.x, 2) + pow(newPosition.y - alien->position.y, 2));
				alienToReposition->direction = (alien->position - newPosition) / distance;
				alienToReposition->radius = distance / 2;
				alienToReposition->currentState = static_cast<Alien::AlienState>(alienAction);
			}
		}
		SDL_Log("Alien::Reposition");
	}
	break;

	case Alien::STATE_FIRE1:
	{
		alien->currentState = static_cast<Alien::AlienState>(alienAction);
		SDL_Log("Alien::Fire1");
	}
	break;

	case Alien::STATE_FIRE2:
	{
		alien->currentState = static_cast<Alien::AlienState>(alienAction);
	}
	break;

	}
}


// return a random action if enough time has passed
int EnemyControllerBehaviorComponent::AlienCanPerformRandomAction()
{
	// shoot just if enough time passed by
	if (engine->GetElapsedTime() - timeAlienAction < 0)
		return 0xFFFF;

	// Random state between 4-7
	timeAlienAction = engine->GetElapsedTime() + ((EnemyController*)go)->alienActionInterval;
	int randomAction = rand() % 7 + 4;

	return randomAction;
	//return 6;
}


bool EnemyControllerBehaviorComponent::CanSpawnAsteroids()
{
	// Asteroids spawn on an interval
	if ((engine->GetElapsedTime() - timeAsteroidAction) < 0)
		return false;

	//3% chance per frame
	if ((rand() / (float)RAND_MAX) < 0.97f)
		return false;

	timeAsteroidAction = engine->GetElapsedTime() + ((EnemyController*)go)->asteroidInterval;

	return true;
}


void EnemyControllerBehaviorComponent::SpawnAsteroids()
{
	for (int i = 0; i < ASTEROIDS_AMOUNT; i++)
	{
		Asteroid* asteroid = asteroidsPool->FirstAvailable();

		if (asteroid != NULL)
		{
			asteroid->Init();
			game_objects->insert(asteroid);
		}
	}
}


// Spawn aliens in grid format
// Direction controls initial lemniscate of bernoulli movement (true: horizontal, false: vertical)
void EnemyControllerBehaviorComponent::SpawnAliens(ObjectPool<Alien>* aliensPool)
{
	char j = 1;
	char k = 1;

	// Spawn 1 alien in each of corner
	for (int i = 0; i < 4; i++)
	{
		auto* alien = aliensPool->FirstAvailable();
		(*alien).Init(GAME_CENTER_X + 50 * j, GAME_CENTER_Y + 70 * k, 1, 1, 60.0f, j, k, Alien::STATE_INITIAL2);
		game_objects->insert(alien);

		// A little ugly but it works
		j *= -1;
		if (j == -1)
			k *= -1;
	}
}

void EnemyController::Receive(Message m)
{
	if (m == NEW_LEVEL)
	{
		// Alien actions and asteroids get more frequent on new levels
		alienActionInterval = alienActionInterval > 0.2 ? alienActionInterval - ENEMY_DIFFICULTY_INCREMENT : 0.2;
		asteroidInterval = asteroidInterval > 0.5 ? asteroidInterval - ASTEROID_DIFFICULTY_INCREMENT : 0.5;
	}
}

EnemyController:: ~EnemyController() { SDL_Log("AliensGrid::~AliensGrid"); }


void EnemyController::Init()
{
	SDL_Log("AliensGrid::Init");
	GameObject::Init();
}