#pragma once

//////////////////////////////////////////////////////////////
//					AlienBomb
// \brief Enemy projectiles. Shot by at player when alien in state
//			 STATE_FIRE1, STATE_FIRE2
//	
// \params BOMB_SPEED
// \see Game, Alien, EnemyController
/////////////////////////////////////////////////////////////



#include "AlienBomb.hpp"

void AlienBombBehaviorComponent::Update(float dt)
{
	auto* bomb = (AlienBomb*)go;

	go->position = go->position - (go->direction * bomb->bombSpeed * dt);

	go->width += (bomb->bombSpeed / 25) * dt;
	go->height += (bomb->bombSpeed / 25) * dt;

}

void AlienBomb::Init(Vector2D playerPosition, double speed, double xPos, double yPos)
{
	SDL_Log("Rocket::Init");
	GameObject::Init();

	this->position.x = xPos;
	this->position.y = yPos;
	this->bombSpeed = speed;
	this->width = 15;
	this->height = 15;

	this->angle = atan2(playerPosition.y - this->position.y, playerPosition.x - this->position.x) * (180.0f / M_PI);

	// Aim for player
	double distance = sqrt(pow(playerPosition.x - position.x, 2) + pow(playerPosition.y - position.y, 2));
	this->direction = (position - playerPosition) / distance;
}

void AlienBomb::Receive(Message m)
{
	if (!enabled)
		return;

	if (m == HIT)
	{
		enabled = false;
		SDL_Log("AlienBomb::Hit!");
	}
}