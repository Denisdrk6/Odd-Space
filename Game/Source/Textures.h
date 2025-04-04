#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"

#include "List.h"



class Assets;
class Render;

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:

	// Constructor
	Textures(Assets* assets, Render* render);
	// Destructor
	virtual ~Textures();


	// Called before render is available
	bool Awake(pugi::xml_node&);
	// Called before the first frame
	bool Start();


	// Called before quitting
	bool CleanUp();



	// Load Texture
	SDL_Texture* const Load(const char* path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	bool UnLoad(SDL_Texture* texture);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	List<SDL_Texture*> textures;

private:

	Assets* assets;
	Render* render;
};


#endif // __TEXTURES_H__