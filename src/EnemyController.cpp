

//////////////////////////////////////////////////////////////
//					EnemyController
// \brief EnemyController gives the enemies actions to perform during game loop
//	
// \params GAME_CENTER_X, GAME_CENTER_Y, ENEMY_ACTION_INTERVAL
// \see Game
/////////////////////////////////////////////////////////////



#include "Component.hpp"
#include "GameObject.hpp"
#include "Avancezlib.hpp"
#include "Alien.hpp"
#include "EnemyController.hpp"
#include "Common.hpp"


EnemyControllerBehaviorComponent::~EnemyControllerBehaviorComponent() {}

void EnemyControllerBehaviorComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Alien>* aliens_pool, ObjectPool<AlienBomb>* bombs_pool, Player* player)
{
	Component::Create(engine, go, game_objects);

	this->aliens_pool = aliens_pool;
	this->bombs_pool = bombs_pool;
	this->player = player;
}

void EnemyControllerBehaviorComponent::Init()
{
	time_alien_action = engine->getElapsedTime();

	short j = 1;
	short k = 1;

	// Spawn 1 alien in each of corner
	// Currently very ugly
	for (int i = 0; i < 4; i++)
	{
		auto *alien = aliens_pool->FirstAvailable();
		(*alien).Init(GAME_CENTER_X + 50 * j, GAME_CENTER_Y + 70 * k, 1, 1, 60, j, k, Alien::STATE_INITIAL2);
		game_objects->insert(alien);
		j *= -1;
		if (j == -1)
			k *= -1;
	}

	change_direction = false;
}

void EnemyControllerBehaviorComponent::Update(float dt)
{
	// is enough time passed from the last bomb, shoot another bomb from a random active alien
	int randomAlienAction = static_cast<Alien::AlienState>(AlienCanPerformRandomAction());
	//int randomAlienAction = 0xFFFF;
	if (randomAlienAction != 0xFFFF)
	{
		auto *alien = aliens_pool->SelectRandom();
		if (alien != NULL && alien->currentState != Alien::STATE_INITIAL2)
		{
			switch (randomAlienAction)
			{
			case Alien::STATE_INITIAL2:
			{
				// This might not be needed							
			}
			break;

			case Alien::STATE_REPOSITION:
			{
				// Generate new random position for the swarm
				int randX = rand() % (engine->screenWidth - 200) + 300;
				int randY = rand() % (engine->screenHeight - 200) + 300;

				for (int i = 0; i < ALIENS_IN_SWARM; i++)
				{
					Alien* alienToReposition = aliens_pool->SelectRandom();
					if (alienToReposition != NULL && alien->currentState == Alien::STATE_CIRCLE)
					{
						// Slight variation in swarm position for each alien
						double swarmPosX = randX + fmod(rand(), 80);
						double swarmPosY = randX + fmod(rand(), 80);
						Vector2D newPosition = Vector2D(swarmPosX, swarmPosY);

						// Set alien direction to new position
						double distance = sqrt(pow(newPosition.x - alien->position.x, 2) + pow(newPosition.y - alien->position.y, 2));
						alienToReposition->direction = (alien->position - newPosition) / distance;
						alienToReposition->radius = distance / 2;
						alienToReposition->currentState = static_cast<Alien::AlienState>(randomAlienAction);
					}
				}
				SDL_Log("Alien::Reposition");
			}
			break;

			case Alien::STATE_FIRE1:
			{
				Alien* alienToFire = aliens_pool->SelectRandom();
				if (alienToFire != NULL)
				{
					alienToFire->currentState = static_cast<Alien::AlienState>(randomAlienAction);
				}
				SDL_Log("Alien::Fire1");
			}
			break;

			case Alien::STATE_FIRE2:
			{
				AlienBomb* bomb = bombs_pool->FirstAvailable();
				if (bomb != NULL)
				{
					Vector2D randomPosNearPlayer(player->position.x + rand() % 100, player->position.y + rand() % 100);
					bomb->Init(randomPosNearPlayer, GAME_CENTER_X, GAME_CENTER_Y);
					game_objects->insert(bomb);
				}
				SDL_Log("Alien::Fire2");
			}
			break;

			}
		}
	}
}

// return a random action if enough time has passed
int EnemyControllerBehaviorComponent::AlienCanPerformRandomAction()
{
	// shoot just if enough time passed by
	if ((engine->getElapsedTime() - time_alien_action) < (ENEMY_ACTION_INTERVAL))
		return 0xFFFF;

	//3% chance per frame
	//if ((rand() / (float)RAND_MAX) < 0.97f)
	//	return 0xFFFF;

	// Random state between 4-7
	time_alien_action = engine->getElapsedTime();
	int randomAction = rand() % 6 + 4;

	return randomAction;
}

void EnemyControllerBehaviorComponent::spawnAsteroid()
{

}

// Spawn aliens in grid format
void EnemyControllerBehaviorComponent::SpawnAliens(ObjectPool<Alien>& aliens_pool)
{
	//int angle = 0;
	//int xPos;
	//int yPos;
	//for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
	//{
	//	angle += 90;
	//	if (angle > 360) angle = 0;

	//	xPos = engine->screenWidth / 2 + 40 * cos((float)((int)angle * (M_PI / 180.0f)));
	//	yPos = engine->screenHeight / 2 + 40 * sin((float)((int)angle * (M_PI / 180.0f)));

	//	(*alien)->Init(xPos, yPos, 2, 2);
	//	(*alien)->enabled = true;
	//}
}


EnemyController:: ~EnemyController() { SDL_Log("AliensGrid::~AliensGrid"); }

void EnemyController::Init()
{
	SDL_Log("AliensGrid::Init");
	GameObject::Init();
}