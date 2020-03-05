#pragma once

//////////////////////////////////////////////////////////////
//						GameObject
// \brief A game object is an entity that is manipulated and drawn on the screen
//			All game entities like player, alien etc derive from this class
//
// \see Game.cpp
/////////////////////////////////////////////////////////////



#include <vector>
#include "Vector2D.hpp"




enum Message { HIT, ALIEN_HIT, GAME_OVER, LEVEL_WIN, NO_MSG, QUIT, ALIENS_MOVE_GRID };
class Component;

// GameObject represents objects which moves are drawn
class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	Vector2D position;
	Vector2D direction;

	double width;
	double height;
	double angle;

	bool changeDirection;
	bool enabled;

	virtual ~GameObject();

	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
};