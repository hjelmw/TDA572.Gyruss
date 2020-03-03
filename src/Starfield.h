#include "avancezlib.h"
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

	void Destroy();

	// draws stars and updates their position in the field
	void DrawStars(float dt);


};