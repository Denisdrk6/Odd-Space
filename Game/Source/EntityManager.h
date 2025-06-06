#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Collision.h"

#include "List.h"

class Input;
class Render;
class Textures;
class Player;
class Audio;

enum class SceneType;

class EntityManager : public Module
{
public:

	// Constructor
	EntityManager(Input* input, Render* render, Textures* tex, Audio* audio);
	// Destructor
	virtual ~EntityManager();


	// Called before render is available
	bool Awake(pugi::xml_node&);


	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	Entity* GenerateListItems();

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

	bool Draw();

	bool DestroyEntity(int i);
	bool DestroyPlayer();

public:

	Player* player;

	List<Entity*> entities[3];

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	Collision collision;

	SceneType* previousScene;
	int enemyKills = 0;

	int quantity[7] = {0, 0, 0, 0, 0, 0};

private:

	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
};

#endif // __ENTITYMANAGER_H__
