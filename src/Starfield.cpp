#include "Starfield.hpp"

void Starfield::Init(AvancezLib* engine, int amount) {
	this->engine = engine;

	// reserve vector size for speed
	stars.reserve(amount);
	for (int i = 0; i < amount; i++)
		this->stars.push_back(Star{ 1.0f * (rand() % engine->screenWidth), 1.0f * (rand() % engine->screenHeight), 0.0f });
}


void Starfield::DrawStars(float dt)
{
	for (auto& star : stars) 
	{
		// Animate stars
		star.x = (star.x - engine->screenWidth / 2)  * 1.004 + engine->screenWidth / 2;
		star.y = (star.y - engine->screenHeight / 2) * 1.004 + engine->screenHeight / 2;
		star.z = star.z < 0xFF ? star.z + 1 : star.z + 0;

		// Regenerate star if out of screen bounds
		if (star.x < 0 || star.x > engine->screenWidth || star.y < 0 || star.y > engine->screenHeight)
			star = Star(Star{ 1.0f * (rand() % engine->screenWidth), 1.0f * (rand() % engine->screenHeight), 0.0f });

		// Ask engine to DrawSprite star
		SDL_Rect starColor = { star.z + 200, star.z + 200, star.z + 200, 0xFF };
		engine->DrawPoint(star.x, star.y, starColor);

	}
}