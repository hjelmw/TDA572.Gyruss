
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
	go->position.x += go->direction.x * dt;
	go->position.y += go->direction.y * dt;

	go->width  += 40 * dt;
	go->height += 40 * dt;

	go->angle += fmod((double)(dt * ASTEROID_ROTATION_SPEED), 360);
}

void Asteroid::Init()
{
	SDL_Log("Asteroid::Init");
	GameObject::Init();


	float random = ((float)rand() / (float)RAND_MAX);
	double randomAngle = (random * (4*M_PI)) - 2*M_PI;

	Vector2D position(GAME_CENTER_X + cos(randomAngle) * 50, GAME_CENTER_Y + sin(randomAngle) * 50);
	Vector2D velocity(ASTEROID_SPEED_BASE * cos(randomAngle), ASTEROID_SPEED_BASE * sin(randomAngle));

	this->position.x = position.x;
	this->position.y = position.y;
	this->direction  = velocity;

	width = 1;
	height = 1;
}


Asteroid::~Asteroid() { SDL_Log("Asteroid::~Asteroid"); }


void Asteroid::Receive(Message m)
{
	if (!enabled)
		return;

	if (m == HIT)
	{
		//SDL_Log("Asteroid::Hit!");
	}
}