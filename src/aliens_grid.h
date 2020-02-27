class AliensGridBehaviourComponent : public Component
{
	float time_bomb_launched;
	float time_alien_action;
	bool change_direction;

	Player* player;

	ObjectPool<Alien>* aliens_pool;
	ObjectPool<AlienBomb>* bombs_pool;

public:
	virtual ~AliensGridBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Alien>* aliens_pool, ObjectPool<AlienBomb>* bombs_pool, Player* player)
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

		for (int i = 0; i < aliens_pool->pool.size(); i++ )
		{
			auto alien = aliens_pool->FirstAvailable();
			(*alien).Init(engine->screenWidth + (i*32), engine->screenHeight + 100 + (i*32), 15, 15, 40, randomInitialState);
			game_objects->insert(alien);
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
			if (alien->currentState == Alien::STATE_CIRCLE && randomAlienAction != Alien::STATE_CIRCLE)
			{
				switch (randomAlienAction)
				{
				case Alien::STATE_DIVE:
					SDL_Log("Alien::Dive!");
					break;
				case Alien::STATE_FIRE1:
					SDL_Log("Alien::Fire1!");
					break;
				case Alien::STATE_FIRE2:
					SDL_Log("Alien::Fire2!");
					break;
				}
			}
		}
	}

	// return true if enough time has passed from the previous bomb
	int AlienCanPerformRandomAction()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_alien_action) < (ALIEN_ACTION_INTERVAL / game_speed))
			return 0xFFFF;

		//1% chance per frame
		if ((rand() / (float)RAND_MAX) < 0.99f)
			return 0xFFFF;

		time_alien_action = engine->getElapsedTime();
		int randomAction = rand() % 3 + 4;

		return randomAction;
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



class AliensGrid : public GameObject
{
public:
	
	virtual ~AliensGrid() { SDL_Log("AliensGrid::~AliensGrid"); }



	virtual void Init()
	{
		SDL_Log("AliensGrid::Init");
		GameObject::Init();
	}


};