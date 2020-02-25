#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"

#include <set>

const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 3;

const unsigned int	MAX_NUM_ROCKETS = 32;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 100;
const float			FIRE_TIME_INTERVAL = .2f;
const float			BOMB_TIME_INTERVAL = 1.25f;
const float			PLAYER_SPEED = 100.0f;
const float			ROCKET_SPEED = 600.0f;
const float			ALIEN_SPEED = 20.0f;
const float			ALIEN_SIZE_SPEED = 10.0f;
const float			BOMB_SPEED = 120.0f;

const float			GAME_CENTER_X = 950 / 2;
const float			GAME_CENTER_Y = 950 / 2;

float game_speed = 1.0f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier


#include "component.h"
#include "game_object.h"
#include "vector2D.h"

#include "rocket.h"
#include "player.h"
#include "alien.h"
#include "alien_bomb.h"
#include "game.h"


int main(int argc, char** argv)
{
	AvancezLib engine;
	Game game;

	engine.Init(950, 950);
	game.Create(&engine);
	game.Init();

	float lastTime = engine.getElapsedTime();

	while (engine.isRunning())
	{
		// Frametime
		float newTime = engine.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		// handle events, update game state, render to screen
		engine.ProcessInput();
		game.Update(dt);
		engine.Render();
	}

	// clean up
	engine.destroy();

	return 0;
}



