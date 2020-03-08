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
#include "SDL_mixer.h"
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

	// DrawSprite contents of framebuffer to the screen
	void Render();

	// Destroys the avancez library instance
	void Destroy();

	// Destroys the avancez library instance and exits
	void Quit();

	// Create a sprite given a string.
	Sprite* CreateSprite(const char* name);

	// Draws the given text.
	void DrawText(int x, int y, const char* msg);

	// Draws point on screen
	void DrawPoint(int x, int y, SDL_Rect color);
	
	// Return the total time spent in the game, in seconds.
	static float GetElapsedTime();

	// Returns status of engine
	bool IsRunning();

	int screenWidth, screenHeight;
	struct KeyStatus
	{
		bool fire; // space
		bool left; // left arrow
		bool right; // right arrow
		bool esc; // escape button
	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void GetKeyStatus(KeyStatus& keys);

	struct Audio {
		std::string fileName;
		bool playing;

		void Play();
		void Stop();
		void Resume();
		void DeleteAudio();
	};

	Audio* LoadAudio(const char *filename);



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
	void SwapBuffers();
	void ClearWindow();


	//struct Audio {
	//	std::string fileName;
	//	bool playing;
	//};
};

