//////////////////////////////////////////////////////////////
//						Orb
// \brief Second type of ammunition for alien
//			Fires a burst of orbs that travel towards points close to player
//
// \params 
//			  GAME_CENTER_X, GAME_CENTER_Y, ALIEN_SPEED_INITIAL
//			, ALIEN_SPEED_BASE, ALIEN_SPEED_CIRCLE,
//			, ALIEN_CONSECUTIVE_SHOTS, ALIEN_FIRE_INTERVAL			
//
// \see EnemyController, Game
/////////////////////////////////////////////////////////////

#include "AlienOrb.hpp"


void AlienOrbBehaviorComponent::Update(float dt)
{
	auto* orb = (AlienOrb*)go;

	go->position = go->position - (go->direction * orb->orbSpeed * dt);

	go->width += (orb->orbSpeed / 25) * dt;
	go->height += (orb->orbSpeed / 25) * dt;
}

void AlienOrb::Init(Vector2D aimPosition, double speed, double xPos, double yPos)
{
	SDL_Log("Orb::Init");
	GameObject::Init();

	this->position.x = xPos;
	this->position.y = yPos;
	this->orbSpeed = speed;
	this->width = 15;
	this->height = 15;

	this->angle = atan2(aimPosition.y - this->position.y, aimPosition.x - this->position.x) * (180.0f / M_PI);

	double distance = sqrt(pow(aimPosition.x - position.x, 2) + pow(aimPosition.y - position.y, 2));
	this->direction = (position - aimPosition) / distance;
}

void AlienOrb::Receive(Message m)
{
	if (!enabled)
		return;

	if (m == HIT)
	{
		enabled = false;
		SDL_Log("AlienOrb::Hit!");
	}
}

