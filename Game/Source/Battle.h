#ifndef __SCENEBATTLE_H__
#define __SCENEBATTLE_H__

#include "Scene.h"
#include "Font.h"
#include "Animation.h"
#include "ParticleSystem.h"
#include <thread>
#include <vector>

//#define MAX_ZOOM 3.0f
//#define MIN_ZOOM 0.3f

class Map;

class SceneManager;

class GuiButton;

class Emitter;

class Entity;

class Battle : public Scene
{
public:

    // Constructor
    Battle(SceneManager* sceneManager);
    // Destructor
    virtual ~Battle();


    // Load this scene
    bool Load();

    // Update this scene
    bool Update(float dt);

    // Draw this scene
    bool Draw();

    // Unload this scene
    bool Unload();

    //Menu
    void PlayerMenu(float dt);

    // Damage dealt by Player
    void DamagePlayer(int player);

    //Run
    void BattleEscaped();

    //Player Turns
    void ChangeTurns();

    //Damage dealt by enemy
    void DamageEnemy(int enemy);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

    // Give XP to the characters
    void GiveXP();

    // Little XP notification above players
    void XPnoti(int xp);

    // Add damage info to vectors
    void AddDamage(Entity* entity, int damage, bool critical);

    // Little damage notification behind players and enemies
    void DamageNoti();

    // Draw info screen
    void InfoScreen();


private:

    Map* map;

    SceneManager* sceneManager;

    SDL_Texture* UI;
    SDL_Texture* UI_info;
    SDL_Texture* VorL;

    struct Buttons
    {
        struct ButtonsMenu
        {
            GuiButton* buttonAttack;
            GuiButton* buttonGuard;
            GuiButton* buttonSkills;
            GuiButton* buttonRun;
            GuiButton* buttonItem;
            GuiButton* buttonBack;
        } buttonsMenu;

        struct ButtonsSkills
        {
            GuiButton* buttonSkill[5];
            GuiButton* buttonBack;
        } buttonsSkills;

        struct ButtonsEnemy
        {
            GuiButton* buttonEnemy[5];
            GuiButton* buttonBack;
        } buttonsEnemies;
    } buttons;

    struct Fx
    {
        int loseFx = 0;
        int winFx = 0;
        int strikeFx = 0;
        int hurtFx = 0;
        int deathFx = 0;
        int runFx = 0;
        int guardFx = 0;
        int debuffFx = 0;
        bool playOnce = false;
    }fx;

    int totalEnemies;
    int selectedEnemies[2];
    int selectedCharacters[5];
    int characterTurn;

    float preparetochange = 0;
    int timerequired;
    bool win = false;
    bool lose = false;

    bool playerMenu = true;
    bool animation = false;
    int actualCharacterAnim;
    int actualEnemyAnim;

    //UI Controller
    int chooseMenu;
    int f;
    int c;
    int controllerMenu[2][3] = { {1, 2, 3}, {4, 5, 6} };
    int controllerSkill[2][3] = { {7, 8, 9}, {10, 11, 12} };
    int controllerEnemy[6];
    int hoveredEnemy = -1; // Saves selected enemy for enemy info drawing

    StepedAnimation* stepedAnimation;
    int alpha = 255;

    // XP notifying
    int xp = 0;
    bool notifyXP = false;
    float notifyTime = 0.0f;
    float notifyPos = 80.0f;
    float dt = 0.0f;

    // Damage notifying
    std::vector<Entity*> entities;
    std::vector<int> damages;
    std::vector<float> timers;
    std::vector<float> positions;
    std::vector<float> displacementY;
    std::vector<SDL_Color> colors;
    bool notifyDamage = true;

    // Final info screen
    bool infoScreen = false;
    std::vector<Entity*> drops;
};

#endif // __SCENEBATTLE_H__