#pragma once

class AlienBombBehaviorComponent : public Component
{

public:
	void Update(float dt)
	{
		go->position = go->position - (go->direction * BOMB_SPEED * dt);

		go->width += (BOMB_SPEED / 25) * dt;
		go->height += (BOMB_SPEED / 25) * dt;
	}
};

class AlienBomb : public GameObject
{
public:
	bool changeDirection;

	virtual void Init(Vector2D playerPosition, double xPos, double yPos)
	{
		SDL_Log("Rocket::Init");
		GameObject::Init();

		position.x = xPos;
		position.y = yPos;
		width = 1;
		height = 1;

		// quick maths
		float distance = sqrt(pow(playerPosition.x - position.x, 2) + pow(playerPosition.y - position.y, 2));
		direction = (position - playerPosition) / distance;

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