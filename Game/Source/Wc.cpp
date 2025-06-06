#include "Wc.h"

#include "SceneManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"



// Constructor
Wc::Wc(SceneManager* sceneManager) : Scene()
{
	this->sceneManager = sceneManager;

	map = new Map(sceneManager->tex);

	// L03: DONE: Load map
	// L12b: Create walkability map on map loading
	if (map->Load("world_cantina_wc.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	name.Create("cantina");
}
// Destructor
Wc::~Wc()
{
	Unload();
}

bool Wc::Load() /*EntityManager entityManager)*/
{
	sceneManager->audio->PlayFx(sceneManager->doorClose);

	sceneManager->render->camera.x = 530;
	sceneManager->render->camera.y = 130;
	sceneManager->render->camera.w = sceneManager->win->screenSurface->w;
	sceneManager->render->camera.h = sceneManager->win->screenSurface->h;

	if (sceneManager->wasBattle == true)
	{

	if (!sceneManager->entityManager->CreateEntity(EntityType::HERO)->loadedPos) sceneManager->entityManager->CreateEntity(EntityType::HERO)->position = sceneManager->entityManager->CreateEntity(EntityType::HERO)->prevPos;
	else sceneManager->entityManager->CreateEntity(EntityType::HERO)->loadedPos = false;

	sceneManager->wasBattle = false;
	}
	else
	{
		if (!sceneManager->entityManager->CreateEntity(EntityType::HERO)->loadedPos) sceneManager->entityManager->CreateEntity(EntityType::HERO)->position = iPoint(64, 285);
		else sceneManager->entityManager->CreateEntity(EntityType::HERO)->loadedPos = false;
	}

	wcFx = sceneManager->audio->LoadFx("audio/fx/world_toilet_fx.wav");

	sceneManager->audio->PlayMusic("audio/music/exterior_music.ogg", 2);

	//map = new Map(tex);

	// L03: DONE: Load map
	// L12b: Create walkability map on map loading
	/*if (map->Load("platformer.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}*/

	// Load game entities
	//Player* player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	//player->SetTexture(tex->Load("textures/player.png"));
	//entityManager->CreateEntity(EntityType::ENEMY);
	//entityManager->CreateEntity(EntityType::ENEMY);
	//entityManager->CreateEntity(EntityType::ITEM);
	//entityManager->CreateEntity(EntityType::ITEM);
	//entityManager->CreateEntity(EntityType::ITEM);

	// Initialize player

	//player = new Player();
	//player->position = iPoint(200, 400);

	if (sceneManager->questSystem->currentStepType == QuestType::TRAVEL) sceneManager->questSystem->UpdateMain("WC");

	return false;
}

// Called each loop iteration
inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}

bool Wc::Update(float dt)
{
	sceneManager->collision->CheckCollision(map);

	if (sceneManager->collision->currentInteraction != '/0')
	{
		SDL_Rect playerRect;
		playerRect.x = sceneManager->entityManager->CreateEntity(EntityType::HERO)->position.x;
		playerRect.y = sceneManager->entityManager->CreateEntity(EntityType::HERO)->position.y;
		playerRect.w = playerRect.h = 32;

		if (sceneManager->entityManager->CreateEntity(EntityType::HERO)->interacting == true)
		{
			if (sceneManager->collision->currentInteraction == "flush" && sceneManager->collision->Detect(sceneManager->collision->interactRect, playerRect))
			{
				// Start dialogue
				if (!sceneManager->audio->IsPlaying(wcFx) && !sceneManager->dialogueSystem->inConversation)
				{
					sceneManager->dialogueSystem->SetConversation(7);
					sceneManager->dialogueSystem->inConversation = true;

					sceneManager->toDrawX = false;

					sceneManager->collision->currentInteraction = '/0';
				}
			}
		}

		// If conversation and WCfx ended draw X to interact again
		if (!sceneManager->dialogueSystem->inConversation && !sceneManager->audio->IsPlaying(wcFx)) sceneManager->toDrawX = true;
	}

	// If there's no interaction and X is being drawn, stop drawing it
	else if (sceneManager->toDrawX == true) sceneManager->toDrawX = false;

	// If conversation ended and event is triggered, play WCfx
	if (!sceneManager->dialogueSystem->inConversation && sceneManager->dialogueSystem->triggerEvent)
	{
		sceneManager->audio->PlayFx(wcFx);
		sceneManager->toDrawX = false;
		enemyEncounter += 1500;
		sceneManager->dialogueSystem->triggerEvent = false;
	}

	if (map->doorHit)
	{
		sceneManager->audio->PlayFx(sceneManager->doorOpen);
		//sceneManager->audio->FadeOutFx(2000);
		TransitionToScene(SceneType::CANTINA);
		map->doorHit = false;
	}

	//Enemy Encounter
	if (sceneManager->entityManager->CreateEntity(EntityType::HERO)->position != sceneManager->entityManager->CreateEntity(EntityType::HERO)->temPos)
	{
		enemyEncounter += rand() % 5;
		if (enemyEncounter > rand() % (8500) + 1500)
		{
			enemyEncounter = 0;
			TransitionToScene(SceneType::BATTLE);
		}
	}

	return true;
}

bool Wc::Draw()
{
	// Draw map
	map->Draw(sceneManager->render, sceneManager->drawColliders);

	//player->Draw(render);

	sceneManager->entityManager->Draw();

	return false;
}

bool Wc::Unload()
{
	// TODO: Unload all resources
	*sceneManager->previousScene = SceneType::WC;

	enemyEncounter = 0;

	map->Unload();
	delete map;
	map = nullptr;

	sceneManager->audio->UnloadFx(wcFx - 1);

	return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool Wc::OnGuiMouseClickEvent(GuiControl* control)
{
	if (sceneManager->dialogueSystem->inConversation)
	{
		sceneManager->dialogueSystem->OnGuiMouseClickEvent(control);
	}

	return true;
}