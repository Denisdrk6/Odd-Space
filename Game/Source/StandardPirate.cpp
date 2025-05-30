#include "StandardPirate.h"

#include "Log.h"

#include "Render.h"
#include "Textures.h"
#include "Audio.h"



StandardPirates* StandardPirates::instance = nullptr;
// Instance creator
StandardPirates* StandardPirates::GetInstance(Render* render, Textures* tex, Audio* audio)
{
    instance = new StandardPirates(render, tex, audio);
    LOG("Returning drunk customer instance");

    return instance;
}
// Instance reseter
void StandardPirates::ResetInstance()
{
    RELEASE(instance);
}
// Constructor
StandardPirates::StandardPirates(Render* render, Textures* tex, Audio* audio) : Enemy(EnemyType::STANDARTPIRATE)
{
    this->render = render;
    this->tex = tex;
    this->audio = audio;

    standardPirateTexture = NULL;
    position = iPoint(0, 0);

    width = 16;
    height = 32;

    //Drunk Customer stats
    infoEntities.info.name = "Standard Pirate";
    infoEntities.info.HP = (rand() % (17 - 13 + 1)) + 13;
    infoEntities.info.maxHP = infoEntities.info.HP;
    infoEntities.info.LVL = 1;
    infoEntities.stats.ATK = 0;
    infoEntities.stats.DEF = 0;
    infoEntities.stats.SPD = 0;
    infoEntities.stats.LCK = 0;

    // Define Player animations

    //Define Hurt Texture
    hurtTexture = this->tex->Load("sprites/combat/cmb_hurt.png");

    // Define Hurt Sound Fx
    interactFx = this->audio->LoadFx("audio/fx/hurt_drunk.wav");
}
// Destructor
StandardPirates::~StandardPirates()
{
    UnLoad();
}



bool StandardPirates::Update(float dt)
{
    standardPirateAnim->Update();
    return true;
}


bool StandardPirates::Draw()
{
    if (infoEntities.info.HP > 0)
    {
        // TODO: Calculate the corresponding rectangle depending on the
        // animation state and animation frame
        SDL_Rect rec;

        render->DrawRectangle(GetBounds(), 255, 0, 0, 255);

        rec = hurtAnim->GetCurrentFrame();
        render->DrawTexture(hurtTexture, position.x, position.y, &rec);
    }

    return false;
}

void StandardPirates::SetTexture(SDL_Texture* tex)
{
    standardPirateTexture = tex;
}

SDL_Rect StandardPirates::GetBounds()
{
    return { position.x, position.y, width, height };
}

bool StandardPirates::UnLoad()
{
    tex->UnLoad(hurtTexture);

    audio->UnloadFx(interactFx);

    RELEASE(standardPirateAnim);

    RELEASE(hurtAnim);
    RELEASE(deathAnim);

    render = nullptr;
    tex = nullptr;
    audio = nullptr;

    return false;
}