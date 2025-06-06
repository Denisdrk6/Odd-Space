#include "Render.h"

#include "Window.h"

#include "Defs.h"
#include "Log.h"
#include "Font.h"


Render::Render(Window* win) : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;

	this->win = win;
}
// Destructor
Render::~Render()
{}


// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(win->window, -1, flags);
	scale = win->GetScale();

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = win->screenSurface->w;
		camera.h = win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}
// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	debug = false;
	return true;
}


// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}


// Set background color
void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	
	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw rectangle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw line to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw circle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint)
{
	bool ret = true;

	int length = strlen(text);
	int posX = x;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);

	for (int i = 0; i < length; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, y, (scale * recGlyph.w), size };

		SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);

		posX += ((float)recGlyph.w * scale + spacing);
	}

	return ret;
}

int Render::GetTextWidth(Font* font, const char* text, int size, int spacing)
{
	int width = 0;
	float scale = (float)size / font->GetCharRec(32).h;
	int length = strlen(text);

	for (int i = 0; i < length; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		width += ((float)recGlyph.w * scale + spacing);
	}

	return width;
}

bool Render::DrawDegradedRectHorizontal(const SDL_Rect& rect, SDL_Color color1, SDL_Color color2, bool useCamera)
{
	bool ret = true;
	uint scale = win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int y = rect.y;
	int x2 = (rect.x + rect.w);
	float rModifier = (float)(color1.r - color2.r) / (float)rect.h;
	float gModifier = (float)(color1.g - color2.g) / (float)rect.h;
	float bModifier = (float)(color1.b - color2.b) / (float)rect.h;

	int result = -1;

	for (int i = 0; i < rect.h; i++)
	{
		SDL_SetRenderDrawColor(renderer, color1.r + rModifier * i, color1.g + gModifier * i, color1.b + bModifier * i, 255);

		if (useCamera)
			result = SDL_RenderDrawLine(renderer, camera.x + rect.x * scale, camera.y + y * scale, camera.x + x2 * scale, camera.y + y * scale);
		else
			result = SDL_RenderDrawLine(renderer, rect.x * scale, y * scale, x2 * scale, y * scale);

		if (result != 0)
		{
			LOG("Cannot draw line to screen. SDL_RenderFillRect error: %s", SDL_GetError());
			ret = false;
		}

		y++;
	}

	return ret;
}

bool Render::DrawDegradedRectVertical(const SDL_Rect& rect, SDL_Color color1, SDL_Color color2, bool useCamera)
{
	bool ret = true;
	uint scale = win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int x = rect.x;
	int y2 = (rect.h + rect.y);
	float rModifier = (float)(color1.r - color2.r) / (float)rect.w;
	if (color2.r > color1.r && rModifier < 0) rModifier *= -1;
	else if (color2.r < color1.r && rModifier > 0) rModifier *= -1;
	float gModifier = (float)(color1.g - color2.g) / (float)rect.w;
	if (color2.g > color1.g && gModifier < 0) gModifier *= -1;
	else if (color2.g < color1.g && gModifier > 0) gModifier *= -1;
	float bModifier = (float)(color1.b - color2.b) / (float)rect.w;
	if (color2.b > color1.b && bModifier < 0) bModifier *= -1;
	else if (color2.b < color1.b && bModifier > 0) bModifier *= -1;

	int result = -1;

	for (int i = 0; i < rect.w; i++)
	{
		SDL_SetRenderDrawColor(renderer, color1.r + rModifier * i, color1.g + gModifier * i, color1.b + bModifier * i, 255);

		if (useCamera)
			result = SDL_RenderDrawLine(renderer, camera.x + x * scale, camera.y + rect.y * scale, camera.x + x * scale, camera.y + y2 * scale);
		else
			result = SDL_RenderDrawLine(renderer, x * scale, rect.y * scale, x * scale, y2 * scale);

		if (result != 0)
		{
			LOG("Cannot draw line to screen. SDL_RenderFillRect error: %s", SDL_GetError());
			ret = false;
		}

		x++;
	}

	return ret;
}

bool Render::DrawParticle(SDL_Texture* texture, int x, int y, const SDL_Rect* section, const SDL_Rect* rectSize, SDL_Color color, SDL_BlendMode blendMode, float speed, double angle, int pivotX, int pivotY) const
{
	uint scale = win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if (rectSize != NULL)
	{
		rect.w = rectSize->w;
		rect.h = rectSize->h;
	}
	else if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	if (SDL_SetTextureColorMod(texture, color.r, color.g, color.b) != 0)
	{
		LOG("Cannot set texture color mode. SDL_SetTextureColorMod error: %s", SDL_GetError());
	}
	if (SDL_SetTextureAlphaMod(texture, color.a) != 0)
	{
		LOG("Cannot set texture alpha mode. SDL_SetTextureAlphaMod error: %s", SDL_GetError());
	}
	if (SDL_SetTextureBlendMode(texture, blendMode) != 0)
	{
		LOG("Cannot set texture blend mode. SDL_SetTextureBlendMode error: %s", SDL_GetError());
	}


	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, NULL, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		return false;
	}

	return true;
}