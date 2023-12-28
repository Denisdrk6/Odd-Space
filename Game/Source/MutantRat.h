#ifndef __MUTANTRAT_H__
#define __MUTANTRAT_H__

#include "Enemy.h"


class Render;
class Textures;
class Audio;


class MutantRat : public Enemy
{
public:

    // Get unique instance of the class
    static MutantRat* GetInstance(Render* render, Textures* tex, Audio* audio);
    // Delete the instance
    void ResetInstance();


    bool Update(float dt);

    bool Draw();

    bool UnLoad();

    void SetTexture(SDL_Texture *tex);

    SDL_Rect GetBounds();

public:

    Animation* mutantRatAnim = new Animation();
    SDL_Texture* mutantRatTexture;
    int width, height;

private:

    // ----- SINGLETON METHODS ----- //
    // Singleton instance
    static MutantRat* instance;
    // Private Constructor
    MutantRat(Render* render, Textures* tex, Audio* audio);
    // Private Destructor
    virtual ~MutantRat();
    // Declare the copy constructor and the assignment operator
    // as private (or delete them explicitly) to prevent cloning your object
    MutantRat(const MutantRat&);
    MutantRat& operator=(const MutantRat&);
    // ----------------------------- //

private:

    Render* render;
    Textures* tex;
    Audio* audio;
};

#endif // __MUTANTRAT_H__
