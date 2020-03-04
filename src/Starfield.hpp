

//////////////////////////////////////////////////////////////
//						Starfield
// \brief Particle system for creating a field of pixels that
//			that simulate a galaxy zooming towards the player.	
//	
// \params AMOUNT_OF_STARS
// \see Game.cpp
/////////////////////////////////////////////////////////////

// Particle system for generating a starfield that zooms on the Z-axis
// Control amount of stars using AMOUNT_OF_STARS in Gyruss.cpp
// Stars are initialzied in Game.Init 
// Stars are continously drawn in Game.Update


#include "avancezlib.hpp"
#include <vector>

class Starfield {
private:
	struct Star {
		float x;
		float y;
		float z;
	};
	std::vector<Star> stars;

	// Pointer to engine. Used for rendering starfield
	AvancezLib* engine;

public:
	// Initializes a starfield with stars.
	void Init(AvancezLib* engine, int amount);

	// Destroy the starfield
	void Destroy();

	// Draws stars and updates their position in the field
	void DrawStars(float dt);


};