#pragma once


//////////////////////////////////////////////////////////////
//						Sprite
// \brief A sprite is an image representation of an object 
//			that moves on the screen	
//	
// \see AvancezLib
/////////////////////////////////////////////////////////////


#include "SDL.h"

class Sprite
{
private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	bool animated;
	int frames;
	int frameDelay;

public:
	SDL_Rect* spriteDim;

	Sprite(SDL_Renderer* renderer, SDL_Texture* texture);

	// Destroys the sprite instance
	void Destroy();

	// DrawSprite the sprite at the given position.
	// Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
	void DrawSprite(int x, int y, int w, int h, int angle);
};