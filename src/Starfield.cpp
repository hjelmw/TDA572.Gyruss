

//////////////////////////////////////////////////////////////
//						Starfield
// \brief Particle system for creating a field of pixels that
//			that simulate stars zooming towards the player.	
//	
// \params AMOUNT_OF_STARS, STAR_SPEED_MIN, STAR_SPEED_MAX
// \see Game.cpp
/////////////////////////////////////////////////////////////



#include "Starfield.hpp"


void Starfield::Init(AvancezLib* engine, int amount) {
	this->engine = engine;
	this->circumferenceDistribution = std::uniform_real_distribution<double>(-M_PI, M_PI);
	this->radiusDistribution = std::uniform_real_distribution<double>(50, 800);


	// reserve vector size for speed
	stars.reserve(amount);
	for (int i = 0; i < amount; i++)
	{
		double randomAngle = circumferenceDistribution(generator);
		double randomRadius = radiusDistribution(generator);
		double randomSpeed = 1.0f * (rand() % STAR_SPEED_MAX) + STAR_SPEED_MIN;

		Vector2D position(GAME_CENTER_X + cos(randomAngle) * randomRadius, GAME_CENTER_Y + sin(randomAngle) * randomRadius);
		Vector2D velocity(randomSpeed * cos(randomAngle), randomSpeed * sin(randomAngle));

		this->stars.push_back(Star({position,velocity, 0.0f}));
	}
}


void Starfield::DrawStars(float dt)
{
	for (auto& star : stars) 
	{
		// Animate stars
		star.position.x += star.velocity.x * dt;
		star.position.y += star.velocity.y * dt;
		star.brightness += star.brightness >= 0xFF ? 1 * dt : 0xFF;

		// Regenerate star if out of screen bounds
		if (star.position.x < 0 || star.position.x > engine->screenWidth || star.position.y < 0 || star.position.y > engine->screenHeight)
		{
			double randomAngle = circumferenceDistribution(generator);
			double randomRadius = radiusDistribution(generator);
			double randomSpeed = 1.0f * (rand() % STAR_SPEED_MAX) + STAR_SPEED_MIN;

			Vector2D position(GAME_CENTER_X + cos(randomAngle) * randomRadius, GAME_CENTER_Y + sin(randomAngle) * randomRadius);
			Vector2D velocity(randomSpeed * cos(randomAngle), randomSpeed * sin(randomAngle));

			star = Star({ position,velocity, 0.0f });
		}

		SDL_Rect starColor = { star.brightness + 100, star.brightness + 100, star.brightness + 100 };

		// Ask engine nicely to Draw point. Hello engine please draw this star :)
		engine->DrawPoint(star.position.x, star.position.y, starColor);
	}
}
