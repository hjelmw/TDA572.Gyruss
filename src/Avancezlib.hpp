#pragma once


//////////////////////////////////////////////////////////////
//					AvancezLib
// \brief Game engine. Renders object on the screen
//	
// \params GAME_CENTER_X, GAME_CENTER_Y, ENEMY_ACTION_INTERVAL
// \see Game
/////////////////////////////////////////////////////////////

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "Sprite.hpp"
#include "Common.hpp"
#include <string>     // std::string, std::to_string
#include <iostream>
#include <sstream>


class AvancezLib
{
public:
	// Initialize the engine and creates the main window. Returns true on success.
	bool Init(int width, int height);

	// Process user input by setting flags of KeyStatus
	void ProcessInput();

	// Draw contents of framebuffer
	void Render();

	// Destroys the avancez library instance
	void Destroy();

	// Destroys the avancez library instance and exits
	void quit();

	// Create a sprite given a string.
	Sprite* CreateSprite(const char* name);

	// Draws the given text.
	void DrawText(int x, int y, const char* msg);

	// Draws point on screen
	void DrawPoint(int x, int y, SDL_Rect color);

	// Return the total time spent in the game, in seconds.
	float getElapsedTime();

	// Returns status of engine
	bool isRunning();

	int screenWidth, screenHeight;
	struct KeyStatus
	{
		bool fire; // space
		bool left; // left arrow
		bool right; // right arrow
		bool esc; // escape button
	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void getKeyStatus(KeyStatus& keys);


private:
	SDL_Window * window;
	SDL_Renderer * renderer;

	SDL_Surface * surface;
	SDL_Color color;
	SDL_Texture * texture;

	SDL_Event event;

	SDL_Rect dstrect;

	TTF_Font* font;

	KeyStatus keyStatus;

	bool running;

	// Swap buffers and clear screen. Used by Render()
	void swapBuffers();
	void clearWindow();
};

