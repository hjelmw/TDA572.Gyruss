
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
	CollideComponent* player_bomb_collision = new CollideComponent();
	player_bomb_collision->Create(engine, playerSprite, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&bombsPool));
	CollideComponent* player_alien_collision = new CollideComponent();
	player_alien_collision->Create(engine, playerSprite, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&aliensPool));


	playerSprite->Create();
	playerSprite->AddComponent(player_behaviour);
	playerSprite->AddComponent(player_render);
	playerSprite->AddComponent(player_bomb_collision);
	playerSprite->AddComponent(player_alien_collision);
	playerSprite->AddReceiver(this);
	game_objects.insert(playerSprite);


	// Player rocket pool
	rocketsPool.Create(MAX_NUM_ROCKETS);
	for (auto rocket = rocketsPool.pool.begin(); rocket != rocketsPool.pool.end(); rocket++)
	{
		RocketBehaviourComponent* behaviour = new RocketBehaviourComponent();
		behaviour->Create(engine, *rocket, &game_objects);
		RenderComponent* render = new RenderComponent();
		render->Create(engine, *rocket, &game_objects, "data/rocket.bmp");

		(*rocket)->Create();
		(*rocket)->AddComponent(behaviour);
		(*rocket)->AddComponent(render);
	}


	aliensPool.Create(ALIENS_AMOUNT);
	for (auto alien = aliensPool.pool.begin(); alien != aliensPool.pool.end(); alien++)
	{
		AlienBehaviorComponent* alien_behavior = new AlienBehaviorComponent();
		alien_behavior->Create(engine, *alien, &game_objects, &bombsPool, playerSprite);
		RenderComponent* alien_render = new RenderComponent();
		alien_render->Create(engine, *alien, &game_objects, "data/enemy_1.png");

		// Alien player collision
		CollideComponent* alien_player_collision = new CollideComponent();
		alien_player_collision->Create(engine, playerSprite, &game_objects, (ObjectPool<GameObject>*) & aliensPool);

		// Alien rocket collision
		CollideComponent* alien_rocket_collision = new CollideComponent();
		alien_rocket_collision->Create(engine, *alien, &game_objects, (ObjectPool<GameObject>*) & rocketsPool);

		BoxCollideComponent* alien_box_collision = new BoxCollideComponent();
		alien_box_collision->Create(engine, *alien, engine->screenHeight, engine->screenWidth);

		(*alien)->AddComponent(alien_behavior);
		(*alien)->AddComponent(alien_render);
		(*alien)->AddComponent(alien_player_collision);
		(*alien)->AddComponent(alien_rocket_collision);
		(*alien)->AddReceiver(this);
	}


	bombsPool.Create(MAX_NUM_BOMBS);
	for (auto bomb = bombsPool.pool.begin(); bomb != bombsPool.pool.end(); bomb++)
	{
		AlienBombBehaviorComponent* bomb_behaviour = new AlienBombBehaviorComponent();
		bomb_behaviour->Create(engine, *bomb, &game_objects);
		RenderComponent* bomb_render = new RenderComponent();
		bomb_render->Create(engine, *bomb, &game_objects, "data/bomb.bmp");
		BoxCollideComponent* bomb_box_collision = new BoxCollideComponent();
		bomb_box_collision->Create(engine, *bomb, engine->screenHeight, engine->screenWidth);

		(*bomb)->Create();
		(*bomb)->AddComponent(bomb_box_collision);
		(*bomb)->AddComponent(bomb_behaviour);
		(*bomb)->AddComponent(bomb_render);
	}

	asteroidsPool.Create(ASTEROIDS_AMOUNT);
	for (auto asteroid = asteroidsPool.pool.begin(); asteroid != asteroidsPool.pool.end(); asteroid++)
	{
		AsteroidBehaviourComponent* asteroid_behaviour = new AsteroidBehaviourComponent();
		asteroid_behaviour->Create(engine, *asteroid, &game_objects);
		RenderComponent* asteroids_render = new RenderComponent();
		asteroids_render->Create(engine, *asteroid, &game_objects, "data/asteroid.png");
		BoxCollideComponent* asteroid_box_collision = new BoxCollideComponent();
		asteroid_box_collision->Create(engine, *asteroid, engine->screenHeight, engine->screenWidth);

		(*asteroid)->Create();
		(*asteroid)->AddComponent(asteroid_box_collision);
		(*asteroid)->AddComponent(asteroid_behaviour);
		(*asteroid)->AddComponent(asteroids_render);
	}

	aliens_grid = new EnemyController();
	EnemyControllerBehaviorComponent* aliens_behaviour = new EnemyControllerBehaviorComponent();
	aliens_behaviour->Create(engine, aliens_grid, &game_objects, &aliensPool, &bombsPool, &asteroidsPool, playerSprite);
	aliens_grid->Create();
	aliens_grid->AddComponent(aliens_behaviour);

	// Aliens shoud spawn immediately
	game_objects.insert(aliens_grid);


	// Load UI assets
	planetSprite = engine->CreateSprite("data/planet.png");
	lifeSprite = engine->CreateSprite("data/life.png");
}

void Game::Init()
{
	playerSprite->Init();
	starField.Init(engine, AMOUNT_OF_STARS);

	spawnDelay = engine->getElapsedTime() + ALIENS_SPAWN_TIME;
	enabled = true;
}

void Game::Update(float dt)
{
	AvancezLib::KeyStatus keys;
	engine->getKeyStatus(keys);
	if (keys.esc) {
		Destroy();
		engine->quit();
	}
	


	// Draw UI elements
	Draw(dt);

	// Can we spawn more aliens
	if (canSpawn && spawnLeft > 0 && spawnDelay - engine->getElapsedTime() <= 0)
	{
		// Spawns 4 aliens
		aliens_grid->Init();

		spawnLeft -= 8;
		canSpawn = spawnLeft > 0 ? true : false;
		spawnDelay = engine->getElapsedTime() + ALIENS_SPAWN_TIME;
	}

	// Update game objects
	for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		(*go)->Update(dt);

}

// Draw UI and text elements here
void Game::Draw(float dt)
{
	// Update starfield
	starField.DrawStars(dt);
	
	// Draw planet in middle of screen
	planetSprite->draw(GAME_CENTER_X - 50, GAME_CENTER_Y - 40, 100, 100, 0);

	// UI Text
	std::stringstream scoreText;
	scoreText.str("");
	scoreText << "Score: " << score;

	std::stringstream levelText;
	levelText.str("");
	levelText << "Level: " << level;

	// Draw UI
	engine->DrawText((engine->screenWidth / 2) - 50, 15, "Gyruss");
	engine->DrawText((engine->screenWidth) - 170, 15, levelText.str().c_str());
	engine->DrawText(10, 15, scoreText.str().c_str());
	for (int i = 0; i < playerSprite->lives; i++)
	{
		lifeSprite->draw(i * 40 + 20, engine->screenHeight - 60, 35, 35, 0);
	}
}

void Game::Receive(Message m)
{
	if (m == GAME_OVER)
	{
		engine->DrawText(200, 400, "--- Game Over ---");
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

	lifeSprite->destroy();

	game_objects.clear();
	delete playerSprite;
}