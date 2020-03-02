class EnemyObserverBehaviorComponent : public Component
{
	float time_bomb_launched;
	float time_alien_action;
	bool change_direction;

	ObjectPool<Alien>* aliens_pool;
	ObjectPool<AlienBomb>* bombs_pool;
	Player* player;

public:
	virtual ~EnemyObserverBehaviorComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Alien>* aliens_pool, ObjectPool<AlienBomb>* bombs_pool, Player *player)
	{
		Component::Create(engine, go, game_objects);

		this->aliens_pool = aliens_pool;
		this->bombs_pool = bombs_pool;
		this->player = player;
	}

	virtual void Init()
	{
		time_alien_action = engine->getElapsedTime();
		int randomInitialState = rand() % 2; // Aliens can start in one of two places

		short j = 1;
		for (int i = 0; i < aliens_pool->pool.size(); i++ )
		{
			
			auto alien = aliens_pool->FirstAvailable();
			(*alien).Init(j * (engine->screenWidth - 200) + (j*i*40), engine->screenHeight + 100 + (i*40), 35, 35, 60, randomInitialState);
			game_objects->insert(alien);

			j *= -1;
		}

		change_direction = false;
	}

	virtual void Update(float dt)
	{
		// is enough time passed from the last bomb, shoot another bomb from a random active alien
		int randomAlienAction = static_cast<Alien::alienState>(AlienCanPerformRandomAction());
		if (randomAlienAction != 0xFFFF)
		{
			auto alien = aliens_pool->SelectRandom();
			if (randomAlienAction != Alien::STATE_INITIAL1)
			{
				switch (randomAlienAction)
				{
				//case Alien::STATE_DIVE:
				//{

				//	// Aim for player
				//	float distance = sqrt(pow(player->position.x - alien->position.x, 2) + pow(player->position.y - alien->position.y, 2));
				//	alien->direction = (alien->position - player->position) / distance;
				//	alien->radius = distance;
				//		
				//	alien->originX = GAME_CENTER_X;
				//	alien->originY = GAME_CENTER_Y;

				//	alien->currentState = static_cast<Alien::alienState>(randomAlienAction);
				//	//alien->currentState = Alien::STATE_DIVE;
				//	SDL_Log("Alien::Dive");
				//}
				break;
				case Alien::STATE_REPOSITION:
				{ 
					// Generate new random position for alien
					int randX = rand() % (engine->screenWidth - 400);
					int randY = rand() % (engine->screenHeight - 400);

					for (int i = 0; i < ALIENS_IN_SWARM; i++)
					{
						Alien* alienToReposition = aliens_pool->SelectRandom();
						if (alienToReposition != NULL && alien->currentState == Alien::STATE_CIRCLE)
						{
							// New position for alien in swarm
							int swarmPosX = randX + fmod(rand(), 80);
							int swarmPosY = randX + fmod(rand(), 80);
							Vector2D newPosition = Vector2D(swarmPosX, swarmPosY);

							// Set alien direction to new position
							float distance = sqrt(pow(newPosition.x - alien->position.x, 2) + pow(newPosition.y - alien->position.y, 2));
							alienToReposition->direction = (alien->position - newPosition) / distance;
							alienToReposition->radius = distance / 2;
							alienToReposition->currentState = static_cast<Alien::alienState>(randomAlienAction);
						}
					}
					//alien->direction = (alien->position - newPosition) / distance;
					//alien->radius = distance/2;
					//alien->currentState = static_cast<Alien::alienState>(randomAlienAction);
 					SDL_Log("Alien::Reposition");
				}
				break;
				case Alien::STATE_FIRE1:
				{
					Alien* alienToFire = aliens_pool->SelectRandom();
					if (alienToFire != NULL)
					{
						alienToFire->currentState = static_cast<Alien::alienState>(randomAlienAction);
					}
					SDL_Log("Alien::Fire1");
				}				
				break;

				case Alien::STATE_FIRE2:
				{
					AlienBomb* bomb = bombs_pool->FirstAvailable();
					if (bomb != NULL)
					{
						bomb->Init(player->position, GAME_CENTER_X, GAME_CENTER_Y);
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
	int AlienCanPerformRandomAction()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_alien_action) < (ALIEN_ACTION_INTERVAL / game_speed))
			return 0xFFFF;

		//3% chance per frame
		//if ((rand() / (float)RAND_MAX) < 0.97f)
		//	return 0xFFFF;

		// Random state between 4-7
		time_alien_action = engine->getElapsedTime();
		int randomAction = rand() % 3 + 2;

		return randomAction;
	}

	void spawnAsteroid()
	{

	}

	// Spawn aliens in grid format
	void SpawnAliens(ObjectPool<Alien>& aliens_pool)
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

};



class EnemyObserver : public GameObject
{
public:
	
	virtual ~EnemyObserver() { SDL_Log("AliensGrid::~AliensGrid"); }



	virtual void Init()
	{
		SDL_Log("AliensGrid::Init");
		GameObject::Init();
	}


};