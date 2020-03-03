#include "avancezlib.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <SDL_image.h>
#include <string>     // std::string, std::to_string
#include <iostream>
#include <sstream>

AvancezLib::KeyStatus keyStatus;

// ------------------ ENGINE ---------------------------
bool AvancezLib::Init(int width, int height)
{
	screenWidth = width; screenHeight = height;

	SDL_Log("Initializing the system...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow("aVANCEZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	TTF_Init();
	font = TTF_OpenFont("data/space_invaders.ttf", 14); //this opens a font style and sets a size
	if (font == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// initialize the keys
	key.fire = false;	key.left = false;	key.right = false;

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Clear screen
	SDL_RenderClear(renderer);

	SDL_Log("System up and running...\n");
	return true;
}

void AvancezLib::swapBuffers()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_GL_SwapWindow(window);
	SDL_RenderPresent(renderer);
}

void AvancezLib::clearWindow()
{
	SDL_RenderClear(renderer);
}

void AvancezLib::destroy()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
}

void AvancezLib::quit()
{
	destroy();
	SDL_Quit();
	running = false;
}

bool AvancezLib::isRunning() 
{
	return running;
}

void AvancezLib::ProcessInput()
{

	SDL_Event event;


	while (SDL_PollEvent(&event))
	{

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				keyStatus.esc = true;
				running = false;
				break;
			case SDLK_SPACE:
				keyStatus.fire = true;
				break;
			case SDLK_LEFT:
  				keyStatus.left = true;
				break;
			case SDLK_RIGHT:
				keyStatus.right = true;
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				keyStatus.fire = false;
				break;
			case SDLK_LEFT:
				keyStatus.left = false;
				break;
			case SDLK_RIGHT:
				keyStatus.right = false;
				break;
			}
		}

	}
}

void AvancezLib::Render()
{
	swapBuffers();
	clearWindow();
}

void AvancezLib::getKeyStatus(KeyStatus& keys)
{
	keys = keyStatus;
}

void AvancezLib::drawText(int x, int y, const char *msg)
{
	surface = TTF_RenderText_Solid(font, msg, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { x, y, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	// Need to deallocate memory between iterations
	SDL_DestroyTexture(texture); // Free up texture
	SDL_FreeSurface(surface);    // Free up surface
}

void AvancezLib::drawPoint(int x, int y, SDL_Rect color)
{
	SDL_SetRenderDrawColor(renderer, color.x, color.y, color.w, color.h);
	
	// Draw a 13 pixel thick point
	SDL_RenderDrawPoint(renderer, x,     y);
	
	SDL_RenderDrawPoint(renderer, x + 1, y);
	SDL_RenderDrawPoint(renderer, x - 1, y);
	SDL_RenderDrawPoint(renderer, x + 2, y);
	SDL_RenderDrawPoint(renderer, x - 2, y);

	SDL_RenderDrawPoint(renderer, x,     y + 1);
	SDL_RenderDrawPoint(renderer, x,     y - 1);
	SDL_RenderDrawPoint(renderer, x,     y + 2);
	SDL_RenderDrawPoint(renderer, x,     y - 2);

	SDL_RenderDrawPoint(renderer, x + 1, y + 1);
	SDL_RenderDrawPoint(renderer, x - 1, y - 1);
	SDL_RenderDrawPoint(renderer, x + 2, y + 2);
	SDL_RenderDrawPoint(renderer, x - 2, y - 2);



}

Sprite* AvancezLib::createSprite(const char *name)
{
	SDL_Texture *spriteTexture = IMG_LoadTexture(renderer, name);
	return new Sprite(renderer, spriteTexture);
}

float AvancezLib::getElapsedTime()
{
	return SDL_GetTicks() / 1000.0f;
}


