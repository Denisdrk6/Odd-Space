#include "DrunkCustomer.h"

#include "Log.h"

#include "Render.h"
#include "Textures.h"
#include "Audio.h"



DrunkCustomer* DrunkCustomer::instance = nullptr;
// Instance creator
DrunkCustomer* DrunkCustomer::GetInstance(Render* render, Textures* tex, Audio* audio)
{
    instance = new DrunkCustomer(render, tex, audio);
    LOG("Returning drunk customer instance");

    return instance;
}
// Instance reseter
void DrunkCustomer::ResetInstance()
{
    RELEASE(instance);
}
// Constructor
DrunkCustomer::DrunkCustomer(Render* render, Textures* tex, Audio* audio) : Enemy(EnemyType::DRUNKCUSTOMER)
{
    this->render = render;
    this->tex = tex;
    this->audio = audio;

    drunkCustomerTexture = NULL;
    position = iPoint(0, 0);

    width = 16;
    height = 32;

    //Drunk Customer stats
    infoEntities.info.name = "Drunk Customer";
    infoEntities.info.LVL = (rand() % (3 - 1 + 1)) + 1; // 1-3
    infoEntities.info.HP = (rand() % (20 - 14 + 1)) + 14 + (3 * infoEntities.info.LVL);
    infoEntities.info.maxHP = infoEntities.info.HP;
    infoEntities.stats.ATK = 5 + 2 * infoEntities.info.LVL;
    infoEntities.stats.DEF = 3 + 2 * infoEntities.info.LVL;
    infoEntities.stats.SPD = 2 + infoEntities.info.LVL;
    infoEntities.stats.LCK = 3 + infoEntities.info.LVL;

    // Define Player animations
    drunkCustomerTexture = this->tex->Load("sprites/enemies/nme_aggressivedrunkman_w.png");

    drunkCustomerAnim->loop = true;
    drunkCustomerAnim->speed = 0.15f;
   
    drunkCustomerAnim->PushBack({ 0, 0, 112, 128  });
    drunkCustomerAnim->PushBack({ 112, 0, 112, 128 });
    drunkCustomerAnim->PushBack({ 224, 0, 112, 128 });
    drunkCustomerAnim->PushBack({ 112, 0, 112, 128 });

    // Define Hurt Texture
    hurtTexture = this->tex->Load("sprites/combat/cmb_hurt.png");

    // Define Hurt Sound Fx
    interactFx = this->audio->LoadFx("audio/fx/hurt_drunk.wav");
}
// Destructor
DrunkCustomer::~DrunkCustomer()
{
    tex->UnLoad(drunkCustomerTexture);
    tex->UnLoad(hurtTexture);

    RELEASE(hurtAnim);
    RELEASE(deathAnim);
    
    RELEASE(drunkCustomerAnim);

    render = nullptr;
    tex = nullptr;
}



bool DrunkCustomer::Update(float dt)
{
    drunkCustomerAnim->Update();
    return true;
}


bool DrunkCustomer::Draw()
{
    if (infoEntities.info.HP > 0)
    {
        // TODO: Calculate the corresponding rectangle depending on the
        // animation state and animation frame
        SDL_Rect rec = drunkCustomerAnim->GetCurrentFrame();
        render->DrawTexture(drunkCustomerTexture, position.x, position.y, &rec);

        rec = hurtAnim->GetCurrentFrame();
        render->DrawTexture(hurtTexture, position.x, position.y, &rec);
    }

    return false;
}

void DrunkCustomer::SetTexture(SDL_Texture *tex)
{
    drunkCustomerTexture = tex;
}

SDL_Rect DrunkCustomer::GetBounds()
{
    return { position.x, position.y, width, height };
}

bool DrunkCustomer::UnLoad()
{
    tex->UnLoad(drunkCustomerTexture);
    tex->UnLoad(hurtTexture);

    audio->UnloadFx(interactFx);
    
    RELEASE(drunkCustomerAnim);
    RELEASE(hurtAnim);
    RELEASE(deathAnim);

    render = nullptr;
    tex = nullptr;
    audio = nullptr;
    
    return false;
}
