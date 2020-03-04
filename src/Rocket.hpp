#pragma once

//////////////////////////////////////////////////////////////
//						Rocket
// \brief A rocket moves towards the center of the screen
//			shrinking as it gets closer.	
//	
// \params ROCKET_SPEED
// \see Player.cpp
/////////////////////////////////////////////////////////////


#include "Component.hpp"
#include "GameObject.hpp"

class RocketBehaviourComponent : public Component
{
public:

	void Update(float dt);

	bool RocketShouldMove();

	void MoveRocket(float dt);

	void ShrinkRocket(float dt);
};


// rockets are shot by the player towards the aliens
class Rocket : public GameObject
{
public:
	virtual void Init(double xPos, double yPos);

	virtual void Receive(Message m);
};

