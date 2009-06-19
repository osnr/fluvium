#include "graphics.h"

/* Release graphics */
void graphics_free(graphics *gfx)
{
    SDL_FreeSurface(gfx->surf);
    TTF_Quit();
    SDL_Quit();
}

/* Grab SDL events, update internal representation */
char graphics_events(graphics *gfx)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                gfx->key[event.key.keysym.sym] = 1;
            break;

            case SDL_KEYUP:
                gfx->key[event.key.keysym.sym] = 0;
            break;

            case SDL_MOUSEBUTTONDOWN:
                gfx->but[event.button.button] = 1;
            break;

            case SDL_MOUSEBUTTONUP:
                gfx->but[event.button.button] = 0;
            break;

            case SDL_MOUSEMOTION:
                gfx->mouse_x = event.motion.x;
                gfx->mouse_y = event.motion.y;
            break;

            case SDL_QUIT:
                gfx->key[SDLK_ESCAPE] = 1;
            break;
        }
    }
    return !gfx->key[SDLK_ESCAPE];
}

/* Catch key press, then unset it */
char graphics_onkey(graphics *gfx, const unsigned short code)
{
    const char c = gfx->key[code];
    gfx->key[code] = 0;
    return c;
}

/* Catch button press, then unset it */
char graphics_onbut(graphics *gfx, const unsigned char code)
{
    const char c = gfx->but[code];
    gfx->but[code] = 0;
    return c;
}

/* Stall when needed (to stablize time) */
char graphics_sync_time(graphics *gfx, const unsigned int t)
{
    const unsigned int tickcount = SDL_GetTicks();
    if(tickcount - gfx->last_tickcount < t) return 1;
    gfx->last_tickcount = tickcount;
    return 0;
}

/* Grab a pixel from an SDL_Surface */
Uint32 pget(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp)
    {
        case 1: return *p;
        case 2: return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
            else return p[0] | p[1] << 8 | p[2] << 16;
        case 4: return *(Uint32 *)p;
        default: return 0;
    }
}

void graphics_draw_particles(int active)
{
	 glDrawArrays(GL_POINTS, 0, active);
}

void graphics_psys_init(const void *rgb, const void *pos)
{
	glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(particle), rgb);
    glVertexPointer(2, GL_FLOAT, sizeof(particle), pos);
}
/* Initialize keys / SDL / OpenGL / Font */
void graphics_init(graphics *gfx, const int width, const int height, const char *font)
{
    gfx->width = width;
    gfx->height = height;
    
    int i;
    for(i=0; i<512; ++i) gfx->key[i] = 0;
    for(i=0; i<256; ++i) gfx->but[i] = 0;
    
    gfx->last_tickcount = 0;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    const SDL_VideoInfo* n = SDL_GetVideoInfo();
    const int f = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
    gfx->surf = SDL_SetVideoMode(gfx->width, gfx->height, n->vfmt->BitsPerPixel, f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, gfx->width, gfx->height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glAlphaFunc(GL_GREATER, 0.2);
    glLineWidth(1);
    glPointSize(5);
    gfx->font = TTF_OpenFont(font, 12);
}

