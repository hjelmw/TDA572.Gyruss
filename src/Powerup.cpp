//////////////////////////////////////////////////////////////
//						Powerup
// \brief Spawns along the circumference on the circle that the player moves around
//			Colliding with it gives player invlunerability
//			, causes shots to pierce aliens and one hp back
//
//	
// \see AvancezLib
/////////////////////////////////////////////////////////////


#include "Powerup.hpp"


void PowerupBehaviorComponent::Update(float dt)
{
	// Currently doesn't move
	go->angle += fmod((double)(dt * 40), 360);
}


void PowerupBehaviorComponent::Init()
{

	float radius = (float)engine->screenWidth / 2 - 40;
	float originX = (float)engine->screenWidth / 2 - 20;
	float originY = (float)engine->screenHeight / 2 - 20;

	float random = ((float)rand() / (float)RAND_MAX);
	double randomAngle = (random * (4 * M_PI)) - 2 * M_PI;

	go->position.x = (double)(originX + radius * cos((randomAngle)));
	go->position.y = (double)(originY + radius * sin((randomAngle)));

	go->width = 50;
	go->height = 50;

}

void Powerup::Init()
{
	SDL_Log("Powerup::Init");
	GameObject::Init();
}

void Powerup::Receive(Message m)
{

	if (m == HIT)
	{
		enabled = false;
		Send(POWERUP_PICKUP);
		SDL_Log("Powerup::Pickup");
	}

}