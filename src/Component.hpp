#pragma once


//////////////////////////////////////////////////////////////
//						Component
// \brief A component iterates over each game object and tells its corresponding subcomponents to update
//			This can be things such as collision detection, movement, rendering
//	
// \see Game
/////////////////////////////////////////////////////////////



#include <set>
#include "ObjectPool.hpp"

class GameObject;
class AvancezLib;
class Sprite;

class Component
{
protected:
	AvancezLib * engine;	// used to access the engine
	GameObject * go;		// the game object this component is part of
	std::set<GameObject*> * game_objects;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects);

	virtual void Init() {}
	virtual void Update(float dt) = 0;
	virtual void Receive(int message) {}
	virtual void Destroy() {}
};


class RenderComponent : public Component
{
	Sprite* sprite;

public:

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name);
	virtual void Update(float dt);
	virtual void Destroy();

	Sprite * GetSprite() { return sprite; }
};


class CollideComponent : public Component
{
	ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects);
	virtual void Update(float dt);
};

class BoxCollideComponent : public Component
{
protected:
	int xMax, yMax, xMin, yMin;
	double radius;

public:
	virtual void Create(AvancezLib* engine, GameObject* go, int xMax, int yMax);
	virtual void Update(float dt);
};