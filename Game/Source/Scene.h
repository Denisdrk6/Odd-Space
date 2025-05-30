#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"

class GuiControl;

enum class SceneType
{
    LOGO,
    TITLE,
    NAME_SELECTOR,
    CANTINA,
    WC,
    EXTERIOR,
    BATTLE,
    DUNGEON_EXT,
    DUNGEON_F1,
    DUNGEON_F2,
    ENDDEMO
};

class Scene
{
public:

    Scene() : active(true), loaded(false), transitionRequired(false) {}

    virtual bool Load()
    {
        return true;
    }

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool Draw()
    {
        return true;
    }

    virtual bool Unload()
    {
        return true;
    }

    void TransitionToScene(SceneType scene)
    {
        transitionRequired = true;
        nextScene = scene;
    }

    // Define multiple Gui Event methods
    virtual bool OnGuiMouseClickEvent(GuiControl* control)
    {
        return true;
    }

public:

    bool active = true;
    SString name;         // Scene name identifier?

    // Possible properties
    bool loaded = false;
    // TODO: Transition animation properties

    bool transitionRequired;
    SceneType nextScene;

    int enemyEncounter = 0;
};

#endif // __SCENE_H__