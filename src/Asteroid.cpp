
//////////////////////////////////////////////////////////////
//						Asteroid
// \brief Asteroids hurl towards the outer edges of the game.
//			They also absors bullets
//
// \params	GAME_CENTER_X, GAME_CENTER_Y, ASTEROIDS_SPEED_BASE
//			, ASTEROIDS_ROTATION_SPEED, 
//
// \see EnemyController, Game
/////////////////////////////////////////////////////////////




#include "Asteroid.hpp"

void AsteroidBehaviourComponent::Update(float dt)
{
	go->position.x = (go->position.x - GAME_CENTER_X) * 1.005f + GAME_CENTER_X;
	go->position.y = (go->position.y - GAME_CENTER_Y) * 1.005f + GAME_CENTER_Y;

	go->width += (ASTEROID_SPEED_BASE * 1.8f) * dt;
	go->height += (ASTEROID_SPEED_BASE * 1.8f) * dt;

	go->angle += fmod((double)(dt * ASTEROID_ROTATION_SPEED), 360);
}

void Asteroid::Init(Vector2D playerPosition, double xPos, double yPos)
{
	SDL_Log("Asteroid::Init");
	GameObject::Init();

	position.x = xPos;
	position.y = yPos;
	width = 1;
	height = 1;

	// Aim for player
	double distance = sqrt(pow(playerPosition.x - position.x, 2) + pow(playerPosition.y - position.y, 2));
	direction = (position - playerPosition) / distance;
}

void Asteroid::Receive(Message m)
{
	if (!enabled)
		return;

	if (m == HIT)
	{
		SDL_Log("Asteroid::Hit!");
	}
}