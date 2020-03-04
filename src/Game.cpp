
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
	player = new Player();
	PlayerBehaviourComponent* player_behaviour = new PlayerBehaviourComponent();
	player_behaviour->Create(engine, player, &game_objects, &rockets_pool);
	RenderComponent* player_render = new RenderComponent();
	player_render->Create(engine, player, &game_objects, "data/player.png");
	CollideComponent* player_bomb_collision = new CollideComponent();
	player_bomb_collision->Create(engine, player, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&bombs_pool));
	CollideComponent* player_alien_collision = new CollideComponent();
	player_alien_collision->Create(engine, player, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&aliens_pool));

	player->Create();
	player->AddComponent(player_behaviour);
	player->AddComponent(player_render);
	player->AddComponent(player_bomb_collision);
	player->AddComponent(player_alien_collision);
	player->AddReceiver(this);
	game_objects.insert(player);

	// Player rocket pool
	rockets_pool.Create(30);
	for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++)
	{
		RocketBehaviourComponent* behaviour = new RocketBehaviourComponent();
		behaviour->Create(engine, *rocket, &game_objects);
		RenderComponent* render = new RenderComponent();
		render->Create(engine, *rocket, &game_objects, "data/rocket.bmp");

		(*rocket)->Create();
		(*rocket)->AddComponent(behaviour);
		(*rocket)->AddComponent(render);
	}

	aliens_pool.Create(ALIEN_AMOUNT);
	for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
	{
		AlienBehaviorComponent* alien_behavior = new AlienBehaviorComponent();
		alien_behavior->Create(engine, *alien, &game_objects, &bombs_pool, player);
		RenderComponent* alien_render = new RenderComponent();
		alien_render->Create(engine, *alien, &game_objects, "data/enemy_1.png");

		// Alien player collision
		CollideComponent* alien_player_collision = new CollideComponent();
		alien_player_collision->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & aliens_pool);

		// Alien rocket collision
		CollideComponent* alien_rocket_collision = new CollideComponent();
		alien_rocket_collision->Create(engine, *alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

		BoxCollideComponent* alien_box_collision = new BoxCollideComponent();
		alien_box_collision->Create(engine, *alien, engine->screenHeight, engine->screenWidth);

		(*alien)->AddComponent(alien_behavior);
		(*alien)->AddComponent(alien_render);
		(*alien)->AddComponent(alien_player_collision);
		(*alien)->AddComponent(alien_rocket_collision);
		(*alien)->AddReceiver(this);
	}

	bombs_pool.Create(50);
	for (auto bomb = bombs_pool.pool.begin(); bomb != bombs_pool.pool.end(); bomb++)
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

	aliens_grid = new EnemyController();
	EnemyControllerBehaviorComponent* aliens_behaviour = new EnemyControllerBehaviorComponent();
	aliens_behaviour->Create(engine, aliens_grid, &game_objects, &aliens_pool, &bombs_pool, player);
	aliens_grid->Create();
	aliens_grid->AddComponent(aliens_behaviour);

	// Aliens shoud spawn immediately
	game_objects.insert(aliens_grid);
}

void Game::Init()
{
	player->Init();
	star_field.Init(engine, AMOUNT_OF_STARS);

	spawnDelay = engine->getElapsedTime() + ALIEN_SPAWN_TIME;
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


	// Can we spawn more aliens
	if (canSpawn && spawnLeft > 0 && spawnDelay - engine->getElapsedTime() <= 0)
	{
		// Spawns 4 aliens
		aliens_grid->Init();

		spawnLeft -= 4;
		canSpawn = spawnLeft > 0 ? true : false;
		spawnDelay = engine->getElapsedTime() + ALIEN_SPAWN_TIME;
	}

	// Update game objects
	for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		(*go)->Update(dt);

	// Draw text on screen
	Draw();

	// Update starfield
	star_field.DrawStars(dt);
}

// Draw UI and text elements here
void Game::Draw()
{
	engine->DrawText((engine->screenWidth / 2) - 20, 10, "Gyruss");
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

	life_sprite->destroy();

	game_objects.clear();
	delete player;
}