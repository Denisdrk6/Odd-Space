#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Audio.h"

#include "Point.h"
#include "SString.h"
#include "Log.h"

#include "SDL/include/SDL.h"

enum class GuiControlType
{
    BUTTON,
    CHECKBOX,
    SLIDER,
};

enum class GuiControlState
{
    NORMAL,
    FOCUSED,
    PRESSED,
    SELECTED
};

class GuiControl
{
public:

    GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

    GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
        type(type),
        state(GuiControlState::NORMAL),
        bounds(bounds),
        text(text) 
    {
        color.r = 255; color.g = 255; color.b = 255;
    }

    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render) const
    {
        return true;
    }

    virtual bool UnLoad() const
    {
        return true;
    }

    void SetObserver(Scene* module)
    {
        observer = module;
    }

    void NotifyObserver()
    {
        observer->OnGuiMouseClickEvent(this);
    }

public:

    uint32 id;
    GuiControlType type;
    GuiControlState state;

    SString text;           // Control text (if required)
    SDL_Rect bounds;        // Position and size
    SDL_Color color;        // Tint color

    //Font font;              // Text font

    bool disabled = false;

    Scene* observer;        // Observer module (it should probably be an array/list)
    Audio* audio;
};

#endif // __GUICONTROL_H__