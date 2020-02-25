#pragma once

class AlienBombBehaviorComponent : public Component
{

public:
	void Update(float dt)
	{
		go->position.y += BOMB_SPEED * dt;

		if (go->position.y > 640)
		{
			go->enabled = false;
		}
	}
};

class AlienBomb : public GameObject
{
public:
	bool changeDirection;

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Rocket::Init");
		GameObject::Init();

		position.x = xPos;
		position.y = yPos;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("AlienBomb::Hit!");
		}
	}

};