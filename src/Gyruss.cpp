
//////////////////////////////////////////////////////////////
//						Gyruss
// \brief The main file. Comprised of an engine and game logic
//
//
// \see Game.cpp
/////////////////////////////////////////////////////////////



#include "Avancezlib.hpp"
#include "Game.hpp"

int main(int argc, char** argv)
{
	AvancezLib engine;
	Game game;

	engine.Init(950, 950);
	game.Create(&engine);
	game.Init();

	float lastTime = engine.GetElapsedTime();

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