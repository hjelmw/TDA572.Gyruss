
//////////////////////////////////////////////////////////////
//						Gyruss
// \brief The main file. Contains main game loop for Gyruss
//
//
// \see Game.cpp
/////////////////////////////////////////////////////////////



#include "Avancezlib.hpp"
#include "Game.hpp"
#include "SDL_mixer.h"

int main(int argc, char** argv)
{
	AvancezLib engine;
	Game game;

	engine.Init(950, 950);
	game.Create(&engine);
	game.Init();

	float lastTime = engine.GetElapsedTime();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	/*Mix_Music* bgm = Mix_LoadMUS("data/boss_battle.mp3");
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);*/

	while (engine.IsRunning())
	{
		// Frametime
		float newTime = engine.GetElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * 1.0f;
		lastTime = newTime;

		// handle events, update game state, render to screen
		engine.ProcessInput();
		game.Update(dt);
		engine.Render();
	}

	// clean up
	engine.Destroy();

	return 0;
}