#ifndef __STANDARDPIRATE_H__
#define __STANDARDPIRATE_H__

#include "Enemy.h"


class Render;
class Textures;
class Audio;


class StandardPirates : public Enemy
{
public:

    // Get unique instance of the class
    static StandardPirates* GetInstance(Render* render, Textures* tex, Audio* audio);
    // Delete the instance
    void ResetInstance();


    bool Update(float dt);

    bool Draw();

    bool UnLoad();

    void SetTexture(SDL_Texture* tex);

    SDL_Rect GetBounds();

public:

    Animation* standardPirateAnim = new Animation();
    SDL_Texture* standardPirateTexture = nullptr;
    int width, height;

private:

    // ----- SINGLETON METHODS ----- //
    // Singleton instance
    static StandardPirates* instance;
    // Private Constructor
    StandardPirates(Render* render, Textures* tex, Audio* audio);
    // Private Destructor
    virtual ~StandardPirates();
    // Declare the copy constructor and the assignment operator
    // as private (or delete them explicitly) to prevent cloning your object
    StandardPirates(const StandardPirates&);
    StandardPirates& operator=(const StandardPirates&);
    // ----------------------------- //

private:

    Render* render;
    Textures* tex;
    Audio* audio;
};

#endif // __STANDARDPIRATE_H__