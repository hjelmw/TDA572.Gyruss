#define _CRT_SECURE_NO_WARNINGS

/* Misc game related parameters */
const unsigned int	MAX_NUM_GAME_OBJECTS       = 10000;
const unsigned int	NUM_LIVES                  = 3;
const unsigned int  POINTS_PER_ALIEN_CLUSTER   = 1000;
const unsigned int	POINTS_PER_ALIEN           = 100;


/* Player parameters */
const float			FIRE_TIME_INTERVAL = 1.0f;
const float			PLAYER_SPEED       = 70.0f;
const float			ROCKET_SPEED       = 600.0f;
const unsigned int	MAX_NUM_ROCKETS    = 32;


/* Alien ship parameters */
const float			ALIEN_ACTION_INTERVAL = 5.0f;
const float			ALIEN_SPEED_BASE      = 300.0f;
const float			ALIEN_SPEED_CIRCLE    = 140.0f;
const float			ALIEN_SIZE_SPEED      = 5.0f;
const float			BOMB_SPEED            = 400.0f;
const float			BOMB_TIME_INTERVAL    = 3.0f;
const unsigned int	MAX_NUM_BOMBS         = 32;


/* Other enemy parameters */
const float			ASTEROID_TIME_INTERVAL = 10.0f;
const float			ASTEROID_SPEED_BASE    = 10.0f;
const float			ASTEROID_ACCELERATION  = 10.0f;


/* Engine parameters */
const float			GAME_CENTER_X = 950 / 2;
const float			GAME_CENTER_Y = 950 / 2;
const float			game_speed    = 1.0f; // speed of the game; it is increased each time all the aliens are hit
										  // it is also the score multiplier


/* All game components */
#include "avancezlib.h"
#include "component.h"
#include "game_object.h"
#include "object_pool.h"
#include "vector2D.h"

#include "stdio.h"
#include <set>

#include "rocket.h"
#include "player.h"
#include "alien.h"
#include "alien_bomb.h"
#include "aliens_grid.h"
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