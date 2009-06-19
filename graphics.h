#ifndef graphics_h
#define graphics_h

#include "particle.h"

#include "SDL_opengl.h"
#include "SDL.h"
#include "SDL_ttf.h"

typedef struct
{
    SDL_Surface *surf;
    TTF_Font* font;
    char key[512], but[256];
	
    int width, height;
    int mouse_x, mouse_y;
    int last_tickcount;
} graphics;

void graphics_init(graphics *gfx, const int width, const int height, const int fullscreen, const char *font);

void graphics_free(graphics *gfx);

char graphics_events(graphics *gfx);

char graphics_onkey(graphics *gfx, const unsigned short code);

char graphics_onbut(graphics *gfx, const unsigned char code);

void graphics_draw_particles(int active);

void graphics_psys_init(const void *rgb, const void *pos);

char graphics_sync_time(graphics *gfx, const unsigned int t);

Uint32 pget(SDL_Surface *surface, int x, int y);

#endif

