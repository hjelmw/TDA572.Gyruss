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
#include "Sprite.hpp"
#include "Audio.hpp"

#include "Player.hpp"
#include "Rocket.hpp"

#include "Alien.hpp"
#include "AlienBomb.hpp"
#include "AlienOrb.hpp"
#include "Asteroid.hpp"
#include "Powerup.hpp"
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
	Sprite* shieldSprite;

	EnemyController* aliens_grid;
	Starfield starField;

	Audio* backgroundAudio;
	Audio* alienHitAudio;
	Audio* pickupAudio;
	Audio* levelUpAudio;

	ObjectPool<Rocket> rocketsPool;	
	ObjectPool<Alien> aliensPool;
	ObjectPool<AlienBomb> bombsPool;
	ObjectPool<Asteroid> asteroidsPool;
	ObjectPool<AlienOrb> orbsPool;
	ObjectPool<Powerup> powerupsPool;

	bool gameOver = false;
	bool canSpawn = true;
	bool spawning = false;
	bool rotateInit;
	float spawnDelay;
	float powerupSpawnDelay;

	int spawnLeft = 0;
	unsigned int score = 0;
	unsigned int level = 0;
public:

	virtual void Create(AvancezLib* engine);

	virtual void Init();

	// Updates the game state and repositions sprites for rendering
	virtual void Update(float dt);

	// DrawSprite UI and text elements here
	virtual void Draw(float dt);

	virtual void Destroy();

	virtual void Receive(Message m);

	// Initializes more aliens to the aliensPool. Used when incrementing level counter
	void createMoreAliens(int numAliens, bool initRotate);
};