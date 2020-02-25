#pragma once

bool change_direction = false;

class Game : public GameObject
{
private:
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/

	AvancezLib* engine;

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets

	ObjectPool<Alien> aliens_pool;		// pool of aliens

	ObjectPool<AlienBomb> bombs_pool;

	Player* player;

	Sprite* life_sprite;
	bool game_over = false;

	bool canSpawn = true;

	unsigned int score = 0;

	// Spawn aliens in grid format
	void SpawnAliens(ObjectPool<Alien>& aliens_pool)
	{
		int angle = 0;
		int xPos;
		int yPos;
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
		{
			angle += 90;
			if (angle > 360) angle = 0;

			xPos = engine->screenWidth / 2 + 40 * cos((float)((int)angle * (M_PI / 180.0f)));
			yPos = engine->screenHeight / 2 + 40 * sin((float)((int)angle * (M_PI / 180.0f)));

			(*alien)->Init(xPos, yPos, 2, 2);
			(*alien)->enabled = true;
		}
	}

public:

	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");

		this->engine = engine;

		// Player
		player = new Player();
		PlayerBehaviourComponent* player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects, &rockets_pool);
		RenderComponent* player_render = new RenderComponent();
		player_render->Create(engine, player, &game_objects, "data/player.bmp");

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
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


		aliens_pool.Create(4);
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
		{
			AlienBehaviorComponent* alien_behavior = new AlienBehaviorComponent();
			alien_behavior->Create(engine, *alien, &game_objects, &bombs_pool);
			RenderComponent* alien_render = new RenderComponent();
			alien_render->Create(engine, *alien, &game_objects, "data/enemy_0.bmp");

			// Alien player collision
			CollideComponent* alien_player_collision = new CollideComponent();
			alien_player_collision->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) & aliens_pool);

			// Alien rocket collision
			CollideComponent* alien_rocket_collision = new CollideComponent();
			alien_rocket_collision->Create(engine, *alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

			(*alien)->AddComponent(alien_behavior);
			(*alien)->AddComponent(alien_render);
			(*alien)->AddComponent(alien_player_collision);
			(*alien)->AddComponent(alien_rocket_collision);
			(*alien)->AddReceiver(this);

			game_objects.insert(*alien); // Aliens should spawn at start of game
		}
	}

	virtual void Init()
	{
		player->Init();
		SpawnAliens(aliens_pool); // Spawn aliens in grid
		enabled = true;
	}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		engine->getKeyStatus(keys);
		if (keys.esc) {
			Destroy();
			engine->quit();
		}

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt);

		// Transfer text and sprites to framebuffer
		Draw();
	}

	// Draw UI and text elements here
	virtual void Draw()
	{
		engine->drawText(0, 0, "Test");
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
		{
			engine->drawText(200, 400, "--- Game Over ---");
			game_over = true;
		}

		if (m == ALIEN_HIT)
		{
			canSpawn = true;
			score += 100;
		}

	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		life_sprite->destroy();

		game_objects.clear();
		delete player;
	}


};