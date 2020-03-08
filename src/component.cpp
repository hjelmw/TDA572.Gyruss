
#include "ObjectPool.hpp"
#include "GameObject.hpp"
#include "Avancezlib.hpp"
#include "Sprite.hpp"

#include "Component.hpp"

void Component::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;
}


void RenderComponent::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(engine, go, game_objects);

	sprite = engine->CreateSprite(sprite_name);
}


void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;

	if (sprite)
		sprite->DrawSprite(int(go->position.x), int(go->position.y), int(go->width), int(go->height), (go->angle));
}


void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->Destroy();
	sprite = NULL;
}


void CollideComponent::Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if ((go0->position.x > go->position.x - go->width/2) &&
				(go0->position.x < go->position.x + go->width/2) &&
				(go0->position.y   > go->position.y  - go->height/2) &&
				(go0->position.y   < go->position.y  + go->height/2))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}


void BoxCollideComponent::Create(AvancezLib* engine, GameObject* go, int xMax, int yMax)
{
	Component::Create(engine, go, game_objects);

	this->xMin = 0;
	this->xMax = xMax;
	this->yMin = 0;
	this->yMax = yMax;
}


void BoxCollideComponent::Update(float dt)
{
	double xPos = go->position.x;
	double yPos = go->position.y;

	if (xPos <= xMin - 350.0f || xPos >= xMax + 350.0f || yPos <= yMin - 350.0f || yPos >= yMax + 350.0f)
	{
		go->enabled = false;
		//SDL_Log("Out of bounds");
	}
}

void SATCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<GameObject>* coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;

}


void SATCollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject* go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if (CheckSATCollision(go, go0))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}


bool SATCollideComponent::CheckSATCollision(GameObject *a, GameObject *b)
{
	return true;
}