#pragma once


class RocketBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		if (RocketShouldMove())
		{
			MoveRocket(dt);
			ShrinkRocket(dt);
		}
		else
			go->enabled = false;
	}

	bool RocketShouldMove()
	{
		return (go->width >= 2 && go->height >= 2);
	}

	void MoveRocket(float dt)
	{
		go->position = go->position - (go->direction * ROCKET_SPEED * dt);
	}

	void ShrinkRocket(float dt)
	{
		go->width -= (ROCKET_SPEED / 19) * dt;
		go->height -= (ROCKET_SPEED / 19) * dt;
	}
};


// rockets are shot by the player towards the aliens
class Rocket : public GameObject
{
public:
	virtual void Init(double xPos, double yPos)
	{
		position.x = xPos;
		position.y = yPos;
		width = 33;
		height = 33;

		// quick maths
		Vector2D end(GAME_CENTER_X, GAME_CENTER_Y);
		float distance = sqrt(pow(end.x - position.x, 2) + pow(end.y - position.y, 2));
		direction = (position - Vector2D(GAME_CENTER_X, GAME_CENTER_Y)) / distance;

		SDL_Log("Rocket::Init");
		GameObject::Init();
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Rocket::Hit");
		}
	}
};

