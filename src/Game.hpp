#pragma once

//////////////////////////////////////////////////////////////
//						Game
// \brief	
//			
//	
// \params AMOUNT_OF_STARS
// \see Game
/////////////////////////////////////////////////////////////



#include "Component.hpp"
#include "GameObject.hpp"
#include "ObjectPool.hpp"
#include "AvancezLib.hpp"

#include "Player.hpp"
#include "Rocket.hpp"
#include "Sprite.hpp"
#include "Alien.hpp"
#include "AlienBomb.hpp"
#include "EnemyController.hpp"
#include "Starfield.hpp"
#include "Common.hpp"

#include <set>




class Game : public GameObject
{
private:
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/

	AvancezLib* engine;

	Player* player;
	Sprite* life_sprite;
	EnemyController* aliens_grid;
	Starfield star_field;

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets
	ObjectPool<Alien> aliens_pool;		// pool of aliens
	ObjectPool<AlienBomb> bombs_pool;

	bool gameOver = false;
	bool canSpawn = true;
	float spawnDelay;
	int spawnLeft = ALIEN_AMOUNT;

	unsigned int score = 0;
public:

	virtual void Create(AvancezLib* engine);

	virtual void Init();

	// Update game
	virtual void Update(float dt);

	// Draw UI and text elements here
	virtual void Draw();

	virtual void Destroy();

	void Receive(Message m);


};