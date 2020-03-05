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
#include "Asteroid.hpp"
#include "Common.hpp"

class EnemyControllerBehaviorComponent : public Component
{
	float time_bomb_launched;
	float time_alien_action;
	float alienVisionFireInterval;
	bool change_direction;

	ObjectPool<Alien>* aliensPool;
	ObjectPool<AlienBomb>* bombsPool;
	ObjectPool<Asteroid>* asteroidsPool;
	Player* player;

public:
	virtual ~EnemyControllerBehaviorComponent();

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Alien>* aliensPool, ObjectPool<AlienBomb>* bombsPool, ObjectPool<Asteroid>* asteroidsPool, Player* player);

	virtual void Init();

	virtual void Update(float dt);

	bool AlienCanSeePlayer(Alien* alien);

	// return a random action if enough time has passed
	int AlienCanPerformRandomAction();

	void GiveAlienRandomState(Alien* alien, Alien::AlienState state);

	void spawnAsteroids();

	// Spawn aliens in grid format
	void SpawnAliens(ObjectPool<Alien>& aliensPool);

};



class EnemyController : public GameObject
{
public:

	virtual ~EnemyController();

	virtual void Init();

};