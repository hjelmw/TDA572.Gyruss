

//////////////////////////////////////////////////////////////
//					EnemyController
// \brief EnemyController gives the enemies actions to perform during game loop
//	
// \params GAME_CENTER_X, GAME_CENTER_Y, ENEMY_ACTION_INTERVAL
// \see Game
/////////////////////////////////////////////////////////////

//#include "Game.h"


class EnemyControllerBehaviorComponent : public Component
{
	float time_bomb_launched;
	float time_alien_action;
	bool change_direction;

	ObjectPool<Alien>* aliens_pool;
	ObjectPool<AlienBomb>* bombs_pool;
	Player* player;

public:
	virtual ~EnemyControllerBehaviorComponent();

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Alien>* aliens_pool, ObjectPool<AlienBomb>* bombs_pool, Player* player);

	virtual void Init();

	virtual void Update(float dt);

	// return a random action if enough time has passed
	int AlienCanPerformRandomAction();

	void spawnAsteroid();

	// Spawn aliens in grid format
	void SpawnAliens(ObjectPool<Alien>& aliens_pool);

};



class EnemyController : public GameObject
{
public:

	virtual ~EnemyController();

	virtual void Init();

};