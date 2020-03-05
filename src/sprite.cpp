#include "Sprite.hpp"

Sprite::Sprite(SDL_Renderer* renderer, SDL_Texture* texture)
{
	Sprite::spriteDim = new SDL_Rect{ 0, 0, 0, 0 };
	Sprite::renderer = renderer;
	Sprite::texture = texture;

	int w = 0;
	int h = 0;
	int angle;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h); // get the width and height of the texture
}

void Sprite::Destroy()
{
	SDL_DestroyTexture(texture);
	delete spriteDim;
}

void Sprite::DrawSprite(int x, int y, int w, int h, int angle)
{
	spriteDim->x = x;
	spriteDim->y = y;

	// Default sprite size is 32x32
	spriteDim->w = w != 0 ? w : 32;
	spriteDim->h = h != 0 ? h : 32;

	SDL_RenderCopyEx(renderer, texture, NULL, spriteDim, angle, NULL, SDL_FLIP_NONE);
}