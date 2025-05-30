#ifndef __GIANTBAT_H__
#define __GIANTBAT_H__

#include "Enemy.h"


class Render;
class Textures;
class Audio;


class GiantBat : public Enemy
{
public:

    // Get unique instance of the class
    static GiantBat* GetInstance(Render* render, Textures* tex, Audio* audio);
    // Delete the instance
    void ResetInstance();


    bool Update(float dt);

    bool Draw();

    bool UnLoad(); 

    void SetTexture(SDL_Texture *tex);

    SDL_Rect GetBounds();

public:

    Animation* giantBatAnim = new Animation();
    SDL_Texture* giantBatTexture;
    int width, height;

private:

    // ----- SINGLETON METHODS ----- //
    // Singleton instance
    static GiantBat* instance;
    // Private Constructor
    GiantBat(Render* render, Textures* tex, Audio* audio);
    // Private Destructor
    virtual ~GiantBat();
    // Declare the copy constructor and the assignment operator
    // as private (or delete them explicitly) to prevent cloning your object
    GiantBat(const GiantBat&);
    GiantBat& operator=(const GiantBat&);
    // ----------------------------- //

private:

    Render* render;
    Textures* tex;
    Audio* audio;
};

#endif // __GIANTBAT_H__
