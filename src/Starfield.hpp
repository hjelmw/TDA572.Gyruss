

//////////////////////////////////////////////////////////////
//						Starfield
// \brief Particle system for creating a field of pixels that
//			that simulate stars zooming towards the player.	
//	
// \params AMOUNT_OF_STARS, STAR_SPEED_MIN, STAR_SPEED_MAX
// \see Game.cpp
/////////////////////////////////////////////////////////////



#include "avancezlib.hpp"
#include "Vector2D.hpp"
#include <random>
#include <vector>

class Starfield {
private:
	std::default_random_engine generator;
	std::uniform_real_distribution<double> circumferenceDistribution;
	std::uniform_real_distribution<double> radiusDistribution;

	struct Star {
		Vector2D position;
		Vector2D velocity;
		float brightness;
	};
	std::vector<Star> stars;

	// Pointer to engine. Used for rendering starfield
	AvancezLib* engine;



public:
	// Initializes a starfield with stars.
	void Init(AvancezLib* engine, int amount);

	// Draws stars and updates their position in the field
	void DrawStars(float dt);


};