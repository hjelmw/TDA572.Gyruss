#include "component.h"
#include "game_object.h"
#include "avancezlib.h"

void Component::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(engine, go, game_objects);

	sprite = engine->createSprite(sprite_name);
}

void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;

	if (sprite)
		sprite->draw(int(go->position.x), int(go->position.y), int(go->width), int(go->height), 90);
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
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
			if ((go0->position.x > go->position.x - 10) &&
				(go0->position.x < go->position.x + 10) &&
				(go0->position.y   > go->position.y - 10) &&
				(go0->position.y   < go->position.y + 10))
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

	//Alien* alien = (Alien*)go;
	//if (alien != NULL)
	//{
	//	// Alien should not be removed while traveling to game field
	//	if (alien->currentState != Alien::STATE_INITIAL1 && (xPos <= xMin || xPos >= xMax || yPos <= yMin || yPos >= yMax))
	//	{
	//		go->enabled = false;
	//		SDL_Log("Out of bounds");
	//	}
	//}

	if (xPos <= xMin || xPos >= xMax || yPos <= yMin || yPos >= yMax)
	{
		go->enabled = false;
		SDL_Log("Out of bounds");
	}
}
