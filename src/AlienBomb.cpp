#pragma once

//////////////////////////////////////////////////////////////
//					AlienBomb
// \brief Enemy projectiles. Shot by at player when alien in state
//			 STATE_FIRE1, STATE_FIRE2
//	
// \params BOMB_SPEED
// \see Game, Alien, EnemyController
/////////////////////////////////////////////////////////////

#include "Component.hpp"
#include "GameObject.hpp"
#include "Vector2D.hpp"
#include "Common.hpp"

#include "AlienBomb.hpp"



void AlienBombBehaviorComponent::Update(float dt)
{
	go->position = go->position - (go->direction * BOMB_SPEED * dt);

	go->width += (BOMB_SPEED / 25) * dt;
	go->height += (BOMB_SPEED / 25) * dt;
}

void AlienBomb::Init(Vector2D playerPosition, double xPos, double yPos)
{
	SDL_Log("Rocket::Init");
	GameObject::Init();

	position.x = xPos;
	position.y = yPos;
	width = 1;
	height = 1;

	// Aim for player
	double distance = sqrt(pow(playerPosition.x - position.x, 2) + pow(playerPosition.y - position.y, 2));
	direction = (position - playerPosition) / distance;

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