
//////////////////////////////////////////////////////////////
//						Game
// \brief Particle system for creating a field of pixels that
//			that simulate a galaxy zooming towards the player.	
//	
// \params AMOUNT_OF_STARS
// \see Game
/////////////////////////////////////////////////////////////



#include "Game.hpp"

// Initialize game objects and components
void Game::Create(AvancezLib* engine)
{
	SDL_Log("Game::Create");

	this->engine = engine;


	// Player
	playerSprite = new Player();
	PlayerBehaviourComponent* player_behaviour = new PlayerBehaviourComponent();
	player_behaviour->Create(engine, playerSprite, &game_objects, &rocketsPool);
	RenderComponent* player_render = new RenderComponent();
	player_render->Create(engine, playerSprite, &game_objects, "data/player.png");
	
	// Player collision
	// Collision with bomb reduces life
	CollideComponent* player_bomb_collision = new CollideComponent();
	player_bomb_collision->Create(engine, playerSprite, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&bombsPool));
	// Collision with alien reduces life
	CollideComponent* player_alien_collision = new CollideComponent();
	player_alien_collision->Create(engine, playerSprite, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&aliensPool));
	// Colision with orb reduces life
	CollideComponent* player_orb_collision = new CollideComponent();
	player_orb_collision->Create(engine, playerSprite, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&orbsPool));
	// Collision with asteroid reduces life
	CollideComponent* player_asteroid_collision = new CollideComponent();
	player_asteroid_collision->Create(engine, playerSprite, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&asteroidsPool));

	playerSprite->Create();
	playerSprite->AddComponent(player_behaviour);
	playerSprite->AddComponent(player_render);
	playerSprite->AddComponent(player_bomb_collision);
	playerSprite->AddComponent(player_orb_collision);
	playerSprite->AddComponent(player_alien_collision);
	playerSprite->AddComponent(player_asteroid_collision);
	playerSprite->AddReceiver(this);
	game_objects.insert(playerSprite);

	// Player rocket pool
	rocketsPool.Create(MAX_NUM_ROCKETS);
	for (auto rocket = rocketsPool.pool.begin(); rocket != rocketsPool.pool.end(); rocket++)
	{
		RocketBehaviourComponent* rocket_behaviour = new RocketBehaviourComponent();
		rocket_behaviour->Create(engine, *rocket, &game_objects);
		RenderComponent* rocket_render = new RenderComponent();
		rocket_render->Create(engine, *rocket, &game_objects, "data/projectile2.png");

		(*rocket)->Create();
		(*rocket)->AddComponent(rocket_behaviour);
		(*rocket)->AddComponent(rocket_render);
	}


	// Creates initial alien pool.
	// In game loop Game::CreateMoreAliens(amount) is insteead used
	// which performs almost the same function. This part is kept for code consistency
	aliensPool.Create(ALIENS_AMOUNT);
	for (auto alien = aliensPool.pool.begin(); alien != aliensPool.pool.end(); alien++)
	{
		// Alien behavior component (State Machine)
		AlienBehaviorComponent* alien_behavior = new AlienBehaviorComponent();
		alien_behavior->Create(engine, *alien, &game_objects, &bombsPool, &orbsPool, playerSprite);

		// Alien renderer
		RenderComponent* alien_render = new RenderComponent();
		alien_render->Create(engine, *alien, &game_objects, "data/enemy_1.png");

		// Alien player collision
		CollideComponent* alien_player_collision = new CollideComponent();
		alien_player_collision->Create(engine, playerSprite, &game_objects, (ObjectPool<GameObject>*) & aliensPool);

		// Alien rocket collision
		CollideComponent* alien_rocket_collision = new CollideComponent();
		alien_rocket_collision->Create(engine, *alien, &game_objects, (ObjectPool<GameObject>*) & rocketsPool);

		// Disables if alien goes out of screen
		BoxCollideComponent* alien_box_collision = new BoxCollideComponent();
		alien_box_collision->Create(engine, *alien, engine->screenHeight, engine->screenWidth);

		(*alien)->rotateInit = rotateInit;
		(*alien)->AddComponent(alien_behavior);
		(*alien)->AddComponent(alien_render);
		(*alien)->AddComponent(alien_player_collision);
		(*alien)->AddComponent(alien_rocket_collision);
		(*alien)->AddComponent(alien_box_collision);
		(*alien)->AddReceiver(this);
	}


	bombsPool.Create(MAX_NUM_BOMBS);
	for (auto bomb = bombsPool.pool.begin(); bomb != bombsPool.pool.end(); bomb++)
	{
		AlienBombBehaviorComponent* bomb_behaviour = new AlienBombBehaviorComponent();
		bomb_behaviour->Create(engine, *bomb, &game_objects);

		RenderComponent* bomb_render = new RenderComponent();
		bomb_render->Create(engine, *bomb, &game_objects, "data/missile2.png");

		BoxCollideComponent* bomb_box_collision = new BoxCollideComponent();
		bomb_box_collision->Create(engine, *bomb, engine->screenHeight, engine->screenWidth);

		(*bomb)->Create();
		(*bomb)->AddComponent(bomb_box_collision);
		(*bomb)->AddComponent(bomb_behaviour);
		(*bomb)->AddComponent(bomb_render);
	}


	orbsPool.Create(MAX_NUM_ORBS);
	for (auto orb = orbsPool.pool.begin(); orb != orbsPool.pool.end(); orb++)
	{
		AlienOrbBehaviorComponent* orb_behaviour = new AlienOrbBehaviorComponent();
		orb_behaviour->Create(engine, *orb, &game_objects);

		RenderComponent* orb_render = new RenderComponent();
		orb_render->Create(engine, *orb, &game_objects, "data/orb_red.png");

		BoxCollideComponent* orb_box_collision = new BoxCollideComponent();
		orb_box_collision->Create(engine, *orb, engine->screenHeight, engine->screenWidth);

		(*orb)->Create();
		(*orb)->AddComponent(orb_box_collision);
		(*orb)->AddComponent(orb_behaviour);
		(*orb)->AddComponent(orb_render);
	}


	asteroidsPool.Create(ASTEROIDS_AMOUNT);
	for (auto asteroid = asteroidsPool.pool.begin(); asteroid != asteroidsPool.pool.end(); asteroid++)
	{
		AsteroidBehaviourComponent* asteroid_behaviour = new AsteroidBehaviourComponent();
		asteroid_behaviour->Create(engine, *asteroid, &game_objects);
		
		RenderComponent* asteroids_render = new RenderComponent();
		asteroids_render->Create(engine, *asteroid, &game_objects, "data/asteroid.png");
		
		// Asteroid player collision
		CollideComponent* asteroid_rocket_collision = new CollideComponent();
		asteroid_rocket_collision->Create(engine, *asteroid, &game_objects, (ObjectPool<GameObject>*) & rocketsPool);

		BoxCollideComponent* asteroid_box_collision = new BoxCollideComponent();
		asteroid_box_collision->Create(engine, *asteroid, engine->screenHeight, engine->screenWidth);

		(*asteroid)->Create();
		(*asteroid)->AddComponent(asteroid_box_collision);
		(*asteroid)->AddComponent(asteroid_rocket_collision);
		(*asteroid)->AddComponent(asteroid_behaviour);
		(*asteroid)->AddComponent(asteroids_render);
	}


	aliens_grid = new EnemyController();
	EnemyControllerBehaviorComponent* aliens_behaviour = new EnemyControllerBehaviorComponent();
	aliens_behaviour->Create(engine, aliens_grid, &game_objects, &aliensPool, &bombsPool, &orbsPool, &asteroidsPool, playerSprite);


	aliens_grid->Create();
	aliens_grid->AddReceiver(this);
	aliens_grid->AddComponent(aliens_behaviour);


	// Aliens shoud spawn immediately
	game_objects.insert(aliens_grid);


	// Load UI assets and other graphics
	starField.Init(engine, AMOUNT_OF_STARS);
	planetSprite = engine->CreateSprite("data/planet.png");
	lifeSprite = engine->CreateSprite("data/life.png");
	shieldSprite = engine->CreateSprite("data/shield_min.png");
}


void Game::Init()
{
	playerSprite->Init();
	spawnDelay = engine->GetElapsedTime() + ALIENS_SPAWN_TIME;
	
	// Skip instant level increment and just spawn aliens
	canSpawn = true;
	spawning = true;
	spawnLeft = ALIENS_AMOUNT;

	enabled = true;
}


void Game::Update(float dt)
{
	if (gameOver)
	{
		dt = 0;
	}

	AvancezLib::KeyStatus keys;
	engine->GetKeyStatus(keys);
	if (keys.esc) {
		Destroy();
		engine->Quit();
	}
	
	// DrawSprite UI elements
	Draw(dt);
	
	// Update game objects
	for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		(*go)->Update(dt);


	// All aliens are dead means we can advance to next level
	bool areAliensStillThere = false;
	for (auto alien = aliensPool.pool.begin(); alien != aliensPool.pool.end(); alien++)
		areAliensStillThere |= (*alien)->enabled;
	if (!areAliensStillThere && score > 0)
	{
		spawning = true; // Avoid infinite loop of spawning aliens
		canSpawn = true;

		/* Advance to next level */ 
		// Increasing alien count by ALIENS_AMOUNT
		level++;
		spawnLeft = ALIENS_AMOUNT + ALIENS_AMOUNT * level;
		rotateInit = !rotateInit;

		// Received in EnemyController
		aliens_grid->Receive(NEW_LEVEL);

		createMoreAliens(ALIENS_AMOUNT, rotateInit);
	}

	// Next level means we spawn more aliens
	if (canSpawn && spawning && spawnDelay - engine->GetElapsedTime() < 0)
	{
		spawnLeft -= 4;
		spawning = spawnLeft > 0 ? true : false;

		// Spawns 4 aliens
		aliens_grid->Init();

		spawnDelay = engine->GetElapsedTime() + ALIENS_SPAWN_TIME;
	}
}


// DrawSprite UI and text elements here
void Game::Draw(float dt)
{
	if (gameOver)
		engine->DrawText(GAME_CENTER_X - 100, engine->screenHeight - 200, "*** G A M E  O V E R ***");		
	

	// Update starfield
	starField.DrawStars(dt);
	
	// DrawSprite planet in middle of screen
	planetSprite->DrawSprite(GAME_CENTER_X - 50, GAME_CENTER_Y - 40, 100, 100, 0);

	// UI Text
	std::stringstream scoreText;
	scoreText.str("");
	scoreText << "Score: " << score;

	std::stringstream levelText;
	levelText.str("");
	levelText << "Level: " << level;

	// Draw shield around player if recently hit
	if (playerSprite->invulnerable)
	{
		shieldSprite->DrawSprite(playerSprite->position.x - 10, playerSprite->position.y - 10, 70, 70, 0);
	}

	// Draw UI
	engine->DrawText((engine->screenWidth / 2) - 50, 15, "Gyruss");
	engine->DrawText((engine->screenWidth) - 170, 15, levelText.str().c_str());
	engine->DrawText(10, 15, scoreText.str().c_str());
	for (int i = 0; i < playerSprite->lives; i++)
	{
		lifeSprite->DrawSprite(i * 40 + 20, engine->screenHeight - 60, 35, 35, 0);
	}
}


void Game::Receive(Message m)
{
	if (m == GAME_OVER)
	{
		gameOver = true;
	}

	if (m == ALIEN_HIT)
	{
		canSpawn = true;
		score += 100;
	}

}


void Game::Destroy()
{
	SDL_Log("Game::Destroy");

	for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		(*go)->Destroy();

	//lifeSprite->Destroy();

	aliensPool.Destroy();
	bombsPool.Destroy();
	rocketsPool.Destroy();

	game_objects.clear();
}


void Game::createMoreAliens(int numAliens, bool initRotate)
{
	// Need to reset all aliens before continuing
	//aliensPool.Destroy();

	// Adds numAliens to back of aliensPool
	aliensPool.Create(numAliens);
 	for (auto alien = aliensPool.pool.begin(); alien != aliensPool.pool.end(); alien++)
	{
		// Hack. Every level the orientation of STATE_INITIAL2 should rotate so we set it directly for all aliens
		(*alien)->rotateInit = rotateInit;

		// Don't add new components to already existing aliens
		if ((*alien)->GetComponents().size() != 0)
			continue;

		/* !! For new aliens only !! */

		// Alien behavior component (State Machine)
		AlienBehaviorComponent* alien_behavior = new AlienBehaviorComponent();
		alien_behavior->Create(engine, *alien, &game_objects, &bombsPool, &orbsPool, playerSprite);

		// Alien renderer
		RenderComponent* alien_render = new RenderComponent();
		alien_render->Create(engine, *alien, &game_objects, "data/enemy_1.png");

		// Alien player collision
		CollideComponent* alien_player_collision = new CollideComponent();
		alien_player_collision->Create(engine, playerSprite, &game_objects, (ObjectPool<GameObject>*) & aliensPool);

		// Alien rocket collision
		CollideComponent* alien_rocket_collision = new CollideComponent();
		alien_rocket_collision->Create(engine, *alien, &game_objects, (ObjectPool<GameObject>*) & rocketsPool);

		// Disables if alien goes out of screen
		BoxCollideComponent* alien_box_collision = new BoxCollideComponent();
		alien_box_collision->Create(engine, *alien, engine->screenHeight, engine->screenWidth);

		// Re-add components
		(*alien)->AddComponent(alien_behavior);
		(*alien)->AddComponent(alien_render);
		(*alien)->AddComponent(alien_player_collision);
		(*alien)->AddComponent(alien_rocket_collision);
		(*alien)->AddComponent(alien_box_collision);
		(*alien)->AddReceiver(this);
	}
}
