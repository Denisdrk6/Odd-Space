#include "GiantBat.h"

#include "Log.h"

#include "Render.h"
#include "Textures.h"
#include "Audio.h"



GiantBat* GiantBat::instance = nullptr;
// Instance creator
GiantBat* GiantBat::GetInstance(Render* render,Textures* tex, Audio* audio)
{
    instance = new GiantBat(render, tex, audio);
    LOG("Returning giant bat instance");

    return instance;
}
// Instance reseter
void GiantBat::ResetInstance()
{
    RELEASE(instance);
}
// Constructor
GiantBat::GiantBat(Render* render, Textures* tex, Audio* audio) : Enemy(EnemyType::GIANTBAT)
{
    this->render = render;
    this->tex = tex;
    this->audio = audio;

    giantBatTexture = NULL;
    position = iPoint(0, 0);

    width = 16;
    height = 32;

    //Giant Bat stats
    infoEntities.info.name = "Giant Bat";
    infoEntities.info.LVL = (rand() % (4 - 3 + 1)) + 3; // 3-4
    infoEntities.info.HP = (rand() % (12 - 8 + 1)) + 8 + (2 * infoEntities.info.LVL);
    infoEntities.info.maxHP = infoEntities.info.HP;
    infoEntities.stats.ATK = 4 + 2 * infoEntities.info.LVL;
    infoEntities.stats.DEF = 2 + 2 * infoEntities.info.LVL;
    infoEntities.stats.SPD = 8 + infoEntities.info.LVL;
    infoEntities.stats.LCK = 3 + infoEntities.info.LVL;;

    // Define Player animations
    giantBatTexture = this->tex->Load("sprites/enemies/nme_normalbat_w.png");

    giantBatAnim->loop = true;
    giantBatAnim->speed = 0.25f;

    giantBatAnim->PushBack({ 0, 0, 97, 79 });
    giantBatAnim->PushBack({ 97, 0, 97, 79 });
    giantBatAnim->PushBack({ 194, 0, 97, 79 });
    giantBatAnim->PushBack({ 97, 0, 97, 79 });

    //Define Hurt Texture
    hurtTexture = this->tex->Load("sprites/combat/cmb_hurt.png");

    // Define Hurt Sound Fx
    hurtFx = this->audio->LoadFx("audio/fx/hurt_bat.wav");
}
// Destructor
GiantBat::~GiantBat()
{
    UnLoad();
}



bool GiantBat::Update(float dt)
{
    giantBatAnim->Update();
    return true;
}


bool GiantBat::Draw()
{
    if (infoEntities.info.HP > 0)
    {
        // TODO: Calculate the corresponding rectangle depending on the
        // animation state and animation frame
        SDL_Rect rec = giantBatAnim->GetCurrentFrame();
        render->DrawTexture(giantBatTexture, position.x, position.y, &rec);

        rec = hurtAnim->GetCurrentFrame();
        render->DrawTexture(hurtTexture, position.x, position.y, &rec);
    }

    return false;
}



void GiantBat::SetTexture(SDL_Texture *tex)
{
    giantBatTexture = tex;
}

SDL_Rect GiantBat::GetBounds()
{
    return { position.x, position.y, width, height };
}

bool GiantBat::UnLoad()
{
    tex->UnLoad(giantBatTexture);
    tex->UnLoad(hurtTexture);

    audio->UnloadFx(hurtFx);

    RELEASE(hurtAnim);
    RELEASE(deathAnim);

    RELEASE(giantBatAnim);
    
    render = nullptr;
    tex = nullptr;
    audio = nullptr;

    return false;
}
