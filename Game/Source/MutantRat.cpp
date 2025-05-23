#include "MutantRat.h"

#include "Log.h"

#include "Render.h"
#include "Textures.h"
#include "Audio.h"


MutantRat* MutantRat::instance = nullptr;
// Instance creator
MutantRat* MutantRat::GetInstance(Render* render, Textures* tex, Audio* audio)
{
    instance = new MutantRat(render, tex, audio);
    LOG("Returning mutant rat instance");

    return instance;
}
// Instance reseter
void MutantRat::ResetInstance()
{
    RELEASE(instance);
}
// Constructor
MutantRat::MutantRat(Render* render, Textures* tex, Audio* audio) : Enemy(EnemyType::MUTANTRAT)
{
    this->render = render;
    this->tex = tex;
    this->audio = audio;

    mutantRatTexture = NULL;
    position = iPoint(0, 0);

    width = 16;
    height = 32;

    //Mutant Rat stats
    infoEntities.info.name = "Mutant Rat";
    infoEntities.info.LVL = (rand() % (3 - 2 + 1)) + 2; // 2-3
    infoEntities.info.HP = (rand() % (14 - 10 + 1)) + 10 + (2* infoEntities.info.LVL);
    infoEntities.info.maxHP = infoEntities.info.HP;
    infoEntities.stats.ATK = 6 + 2 * infoEntities.info.LVL;
    infoEntities.stats.DEF = 2 + infoEntities.info.LVL;
    infoEntities.stats.SPD = 5 + infoEntities.info.LVL;
    infoEntities.stats.LCK = 2 + 2 * infoEntities.info.LVL;

    // Define Player animations
    mutantRatTexture = this->tex->Load("sprites/enemies/nme_normalrat_w.png");


    mutantRatAnim->loop = true;
    mutantRatAnim->speed = 0.15f;

    mutantRatAnim->PushBack({ 0, 0, 64, 64 });
    mutantRatAnim->PushBack({ 64, 0, 64, 64 });
    mutantRatAnim->PushBack({ 128, 0, 64, 64 });
    mutantRatAnim->PushBack({ 64, 0, 64, 64 });

    //Define Hurt Texture
    hurtTexture = this->tex->Load("sprites/combat/cmb_hurt.png");

    // Define Hurt Sound Fx
    interactFx = this->audio->LoadFx("audio/fx/hurt_rat.wav");
}
// Destructor
MutantRat::~MutantRat()
{
    UnLoad();
}



bool MutantRat::Update(float dt)
{
    mutantRatAnim->Update();
    return true;
}


bool MutantRat::Draw()
{
    if (infoEntities.info.HP > 0)
    {
        // TODO: Calculate the corresponding rectangle depending on the
        // animation state and animation frame
        SDL_Rect rec = mutantRatAnim->GetCurrentFrame();
        render->DrawTexture(mutantRatTexture, position.x, position.y, &rec);

        rec = hurtAnim->GetCurrentFrame();
        render->DrawTexture(hurtTexture, position.x, position.y, &rec);
    }

    return false;
}



void MutantRat::SetTexture(SDL_Texture *tex)
{
    mutantRatTexture = tex;
}

SDL_Rect MutantRat::GetBounds()
{
    return { position.x, position.y, width, height };
}

bool MutantRat::UnLoad()
{
    tex->UnLoad(mutantRatTexture);
    tex->UnLoad(hurtTexture);

    audio->UnloadFx(interactFx);

    RELEASE(mutantRatAnim);

    RELEASE(hurtAnim);
    RELEASE(deathAnim);

    render = nullptr;
    tex = nullptr;
    audio = nullptr;

    return false;
}
