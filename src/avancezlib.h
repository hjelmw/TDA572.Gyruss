#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

class Sprite
{
private:
	SDL_Renderer * renderer;
	SDL_Texture * texture;

public:
	SDL_Rect* spriteDim;

	Sprite(SDL_Renderer * renderer, SDL_Texture * texture);

	// Destroys the sprite instance
	void destroy();

	// Draw the sprite at the given position.
	// Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
	void draw(int x, int y, int w, int h, int angle);
};


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
	void destroy();

	// Destroys the avancez library instance and exits
	void quit();

	// Create a sprite given a string.
	Sprite* createSprite(const char* name);

	// Draws the given text.
	void drawText(int x, int y, const char* msg);

	// Draws point on screen
	void drawPoint(int x, int y, SDL_Rect color);

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

	KeyStatus key;

	bool running;

	// Swap buffers and clear screen. Used by Render()
	void swapBuffers();
	void clearWindow();
};

