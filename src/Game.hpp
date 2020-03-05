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
#include "Asteroid.hpp"
#include "EnemyController.hpp"
#include "Starfield.hpp"
#include "Common.hpp"

#include <set>


class Game : public GameObject
{
private:
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/

	AvancezLib* engine;

	Player* playerSprite;
	Sprite* lifeSprite;
	Sprite* planetSprite;
	EnemyController* aliens_grid;
	Starfield starField;


	ObjectPool<Rocket> rocketsPool;	
	ObjectPool<Alien> aliensPool;
	ObjectPool<AlienBomb> bombsPool;
	ObjectPool<Asteroid> asteroidsPool;


	bool gameOver = false;
	bool canSpawn = true;
	float spawnDelay;
	int spawnLeft = ALIENS_AMOUNT;

	unsigned int score = 0;
	unsigned int level = 1;
public:

	virtual void Create(AvancezLib* engine);

	virtual void Init();

	// Update game
	virtual void Update(float dt);

	// Draw UI and text elements here
	virtual void Draw(float dt);

	virtual void Destroy();

	virtual void Receive(Message m);


};