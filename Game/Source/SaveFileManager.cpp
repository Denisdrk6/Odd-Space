#include "SaveFileManager.h"

#include "SceneManager.h"
#include "EntityManager.h"

#include "SDL/include/SDL.h"


SaveFileManager::SaveFileManager(SceneManager* sceneManager)
{
	this->sceneManager = sceneManager;
	encrypt = new Encrypt();
}

SaveFileManager::~SaveFileManager() 
{
	delete encrypt;
}

// Load / Save
void SaveFileManager::LoadGameRequest(const char* fileName)
{
	loadedgame.Create(fileName);
}

// ---------------------------------------
void SaveFileManager::SaveGameRequest(const char* fileName) const
{
	savedgame.Create(fileName);
}

/*bool SaveFileManager::StartFile(pugi::xml_node& data) const
{
	pugi::xml_node camerasave = data.append_child("Camera");
	pugi::xml_node playersave = data.append_child("Player");
	pugi::xml_node scenesave = data.append_child("Scene");

	camerasave.append_attribute("y") = -1000;

	return true;
}

bool SaveFileManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node camerasave = data.append_child("Camera");
	pugi::xml_node playersave = data.append_child("Player");
	pugi::xml_node scenesave = data.append_child("Scene");

	camerasave.append_attribute("y") = sceneManager->render->camera.y;

	return true;
}*/

/*bool SaveFileManager::LoadState(pugi::xml_node& data)
{
	sceneManager->render->camera.x = data.child("Camera").attribute("x").as_int();
	sceneManager->render->camera.y = data.child("Camera").attribute("y").as_int();

	return true;
}*/

// ---------------------------------------
// Create a method to actually load an xml file
// then call all the modules to load themselves
void SaveFileManager::LoadSaveState()
{
	savedgame.Clear();
	savedgame.Create(encrypt->Decrypt("save_game_encrypted.xml", "save_game.xml").c_str());

	if (savedgame.Length() != 0)
	{
		sceneManager->savedataexist = true;
		savedgame.Create(encrypt->EncryptFile("save_game.xml", "save_game_encrypted.xml").c_str());
	}
}

bool SaveFileManager::LoadGame()
{
	savedgame.Clear();
	savedgame.Create(encrypt->Decrypt("save_game_encrypted.xml", "save_game.xml").c_str());

	bool ret = false;

	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file("save_game.xml");

	if (result != NULL)
	{
		LOG("Loading new Game State from save_game.xml...");
		ret = true;

		pugi::xml_node node = file.child("save_state");

		// Load sceneManager things
		sceneManager->render->camera.y = node.child("scenemanager").child("Camera").attribute("y").as_int();

		sceneManager->initialTextTextSaid = node.child("scenemanager").child("Dialogues").attribute("initialTextTextSaid").as_bool();

		int aux;
		aux = node.child("scenemanager").child("Scene").attribute("scene").as_int();
		if (sceneManager->currentscenetype != (SceneType)aux)
		{
			sceneManager->current->TransitionToScene((SceneType)aux);
		}

		// Load entity things
		sceneManager->entityManager->CreateEntity(EntityType::HERO)->position.x = node.child("entitymanager").child("Player").attribute("x").as_int();
		sceneManager->entityManager->CreateEntity(EntityType::HERO)->position.y = node.child("entitymanager").child("Player").attribute("y").as_int();
		if (sceneManager->currentscenetype != (SceneType)aux) sceneManager->entityManager->CreateEntity(EntityType::HERO)->loadedPos = true;
		sceneManager->entityManager->CreateEntity(EntityType::HERO)->infoEntities.info.HP = node.child("entitymanager").child("Player").attribute("hp").as_int();

		sceneManager->entityManager->CreateEntity(EntityType::CAPTAIN)->infoEntities.info.HP = node.child("entitymanager").child("Captain").attribute("hp").as_int();

		if (ret == true)
		{
			LOG("...finished loading");
		}

		LoadDialogueFile();
	}
	else LOG("Could not load game state xml file savegame.xml. pugi error: %s", result.description());

	savedgame.Create(encrypt->EncryptFile("save_game.xml", "save_game_encrypted.xml").c_str());

	return ret;
}

bool SaveFileManager::LoadDialogueFile()
{
	bool ret = false;

	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file("save_completed_dialogues.xml");

	if (result != NULL)
	{
		LOG("Loading completed dialogues from save_completed_dialogues.xml...");
		ret = true;

		pugi::xml_node node = file.child("Saved_dialogues");

		pugi::xml_node dialogue;
		for (dialogue = node.child("Dialogue"); dialogue && ret; dialogue = dialogue.next_sibling("Dialogue"))
		{
			sceneManager->dialogueSystem->completedDialoguesId.Add(dialogue.attribute("Id").as_int());
		}

		// Initial exterior dialogue
		//if (sceneManager->dialogueSystem->completedDialoguesId.Find(0) != -1) sceneManager->initialExtTextSaid = true;

		if (ret == true)
		{
			LOG("...finished loading");
		}
	}

	else LOG("Could not load completed dialogues xml file Saved_dialogues.xml. pugi error: %s", result.description());

	return ret;
}

// Implement the xml save method for current state
bool SaveFileManager::SaveGame() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", savedgame.GetString());

	pugi::xml_document file;
	pugi::xml_node base = file.append_child("save_state");

	// Scene save
	pugi::xml_node node = base.append_child(sceneManager->name.GetString());
	
	// Things to save
	pugi::xml_node saveFile = node.append_child("SaveFile");
	pugi::xml_node camerasave = node.append_child("Camera");
	pugi::xml_node scenesave = node.append_child("Scene");
	pugi::xml_node dialoguessave = node.append_child("Dialogues");

	camerasave.append_attribute("y") = sceneManager->render->camera.y;

	scenesave.append_attribute("scene") = (int)sceneManager->currentscenetype;

	dialoguessave.append_attribute("initialTextTextSaid") = sceneManager->initialTextTextSaid;

	// SceneManager variables
	pugi::xml_node sceneManagerSave = node.append_child("SceneManager");
	pugi::xml_node exterior = node.append_child("Exterior");
	pugi::xml_node floor1 = node.append_child("Floor1");
	pugi::xml_node floor2 = node.append_child("Floor2");
		// EXTERIOR
	exterior.append_attribute("crazyManActive") = sceneManager->crazyManActive;
	exterior.append_attribute("crazyManDrawable") = sceneManager->crazyManDrawable;
		// F1
	floor1.append_attribute("leverTri") = sceneManager->leverTri;
	floor1.append_attribute("leverCir") = sceneManager->leverCir;
	floor1.append_attribute("leverCro") = sceneManager->leverCro;
	floor1.append_attribute("doorOpening") = sceneManager->doorOpening;
	floor1.append_attribute("door2Opening") = sceneManager->door2Opening;
	floor1.append_attribute("leversSize") = sceneManager->levers.size();
	for (int i = 0; i < sceneManager->levers.size(); i++)
	{
		std::string name = "lever" + std::to_string(i);
		floor1.append_attribute("name") = sceneManager->levers.at(i);
	}
	floor1.append_attribute("door1Open") = sceneManager->door1Open;
	floor1.append_attribute("door2Open") = sceneManager->door2Open;
		// F2
	floor2.append_attribute("p1") = sceneManager->p1;
	floor2.append_attribute("p2") = sceneManager->p2;
	floor2.append_attribute("p3") = sceneManager->p3;
	floor2.append_attribute("p4") = sceneManager->p4;
	floor2.append_attribute("p5") = sceneManager->p5;
	floor2.append_attribute("p6") = sceneManager->p6;
	floor2.append_attribute("p7") = sceneManager->p7;
	floor2.append_attribute("p8") = sceneManager->p8;
	floor2.append_attribute("chestopen") = sceneManager->chestopen;
	floor2.append_attribute("seq1") = sceneManager->seq1;
	floor2.append_attribute("seq2") = sceneManager->seq2;
	floor2.append_attribute("seq3") = sceneManager->seq3;
	floor2.append_attribute("seqESP") = sceneManager->seqESP;

	//Entities save
	node = base.append_child(sceneManager->entityManager->name.GetString());

	pugi::xml_node playersave = node.append_child("Player");
	pugi::xml_node captainsave = node.append_child("Captain");

	playersave.append_attribute("x") = sceneManager->entityManager->CreateEntity(EntityType::HERO)->position.x;
	playersave.append_attribute("y") = sceneManager->entityManager->CreateEntity(EntityType::HERO)->position.y;
	playersave.append_attribute("hp") = sceneManager->entityManager->CreateEntity(EntityType::HERO)->infoEntities.info.HP;

	captainsave.append_attribute("hp") = sceneManager->entityManager->CreateEntity(EntityType::CAPTAIN)->infoEntities.info.HP;

	bool succ = file.save_file(fileName.GetString());
	if (succ != true)
	{
		LOG("File save error. pugi error: %d", pugi::status_internal_error);
	}
	else LOG("... finished saving");

	SaveDialogueFile();

	savedgame.Clear();
	savedgame.Create(encrypt->EncryptFile("save_game.xml", "save_game_encrypted.xml").c_str());

	return ret;
}

bool SaveFileManager::SaveDialogueFile() const
{
	bool ret = true;

	LOG("Saving Game State to save_completed_dialogues.xml");

	pugi::xml_document file;
	pugi::xml_node base = file.append_child("Saved_dialogues");

	// Save dialogue IDs
	ListItem<int>* item;
	item = sceneManager->dialogueSystem->completedDialoguesId.start;

	int i = 0;

	while (item != NULL)
	{
		pugi::xml_node dialogue = base.append_child("Dialogue");
		dialogue.append_attribute("Id") = sceneManager->dialogueSystem->completedDialoguesId.At(i)->data;
		item = item->next;
		i++;
	}

	bool succ = file.save_file("save_completed_dialogues.xml");
	if (succ != true)
	{
		LOG("File save error. pugi error: %d", pugi::status_internal_error);
	}
	else LOG("... finished saving");

	return ret;
}