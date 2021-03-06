#pragma once

//////////////////////////////////////////////////////////////
//						Rocket
// \brief A rocket moves towards the center of the screen
//			shrinking as it gets closer.	
//	
// \params ROCKET_SPEED
// \see Player.cpp
/////////////////////////////////////////////////////////////



#include "Rocket.hpp"
#include "Common.hpp"


void RocketBehaviourComponent::Update(float dt)
{
	if (RocketShouldMove())
	{
		MoveRocket(dt);
		ShrinkRocket(dt);
	}
	else
		go->enabled = false;
}


bool RocketBehaviourComponent::RocketShouldMove()
{
	return (go->width >= 5 && go->height >= 5);
}


void RocketBehaviourComponent::MoveRocket(float dt)
{
	// Rocket go swosh towards middle of the screen
	go->position = go->position - (go->direction * ROCKET_SPEED * dt);
}


void RocketBehaviourComponent::ShrinkRocket(float dt)
{
	go->width -= (ROCKET_SPEED / 16) * dt;
	go->height -= (ROCKET_SPEED / 16) * dt;
}


// rockets are shot by the player towards the aliens
void Rocket::Init(double xPos, double yPos, bool piercing)
{
	this->position.x = xPos;
	this->position.y = yPos;
	this->width  = 32;
	this->height = 32;
	this->piercing = piercing;

	// quick maths
	Vector2D end(GAME_CENTER_X, GAME_CENTER_Y);
	float distance = sqrt(pow(end.x - position.x, 2) + pow(end.y - position.y, 2));
	direction = (position - Vector2D(GAME_CENTER_X, GAME_CENTER_Y)) / distance;

	this->angle = atan2(GAME_CENTER_Y - this->position.y, GAME_CENTER_X - this->position.x) * (180.0f / M_PI) + 90;


	//SDL_Log("Rocket::Init");
	GameObject::Init();
}


void Rocket::Receive(Message m)
{
	if (!enabled)
		return;

	if (m == HIT)
	{
		// Piercing shots go through aliens
  		enabled = piercing ? true : false;
		SDL_Log("Rocket::Hit");
	}
}

