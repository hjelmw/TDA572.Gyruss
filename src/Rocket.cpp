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
	return (go->width >= 2 && go->height >= 2);
}

void RocketBehaviourComponent::MoveRocket(float dt)
{
	go->position = go->position - (go->direction * ROCKET_SPEED * dt);
}

void RocketBehaviourComponent::ShrinkRocket(float dt)
{
	go->width -= (ROCKET_SPEED / 19) * dt;
	go->height -= (ROCKET_SPEED / 19) * dt;
}


// rockets are shot by the player towards the aliens
void Rocket::Init(double xPos, double yPos)
{
	position.x = xPos;
	position.y = yPos;
	width = 22;
	height = 22;

	// quick maths
	Vector2D end(GAME_CENTER_X, GAME_CENTER_Y);
	float distance = sqrt(pow(end.x - position.x, 2) + pow(end.y - position.y, 2));
	direction = (position - Vector2D(GAME_CENTER_X, GAME_CENTER_Y)) / distance;

	//SDL_Log("Rocket::Init");
	GameObject::Init();
}

void Rocket::Receive(Message m)
{
	if (!enabled)
		return;

	if (m == HIT)
	{
		enabled = false;
		//SDL_Log("Rocket::Hit");
	}
}

