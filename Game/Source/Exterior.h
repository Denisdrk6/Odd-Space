#ifndef __EXTERIOR_H__
#define __EXTERIOR_H__

#define BOTTOM_CAMERA_LIMIT -48
#define TOP_CAMERA_LIMIT 0
#define UPPER_DOOR 500

#include "Scene.h"
#include "Font.h"
#include "Animation.h"
#include "Point.h"

class Map;

class SceneManager;

class Exterior : public Scene
{
public:

    // Constructor
    Exterior(SceneManager* sceneManager);
    // Destructor
    virtual ~Exterior();


    // Load this scene
    bool Load();

    // Update this scene
    bool Update(float dt);

    // Draw this scene
    bool Draw();

    // Unload this scene
    bool Unload();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    // ----- CRAZY MAN VARIABLES ----- //
    SDL_Texture* texCrazyManCantina;
    Animation* currentAnimation;
    bool flagMoving = false;
    iPoint posCrazyMan = { 16 * 32 - 16, 20 * 32 - 52 };
    bool crazyManTalking = false;
    bool crazyManExists = true;
    // ------------------------------- //
    
    // ----- ANIMATION SETS ----- //
    Animation* animCrazyManIdle = new Animation();
    Animation* animCrazyManWalkLeft = new Animation();
    // -------------------------- //

private:

    Map* map;

    SceneManager* sceneManager;
};

#endif // __EXTERIOR_H__