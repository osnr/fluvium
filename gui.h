#ifndef gui_h
#define gui_h

#include "graphics.h"

#define GUI_WIDTH 128
#define MENU_HEIGHT 0

typedef struct
{
    graphics *gfx;
    SDL_Surface *surf;
    GLuint texture;
} gui;

void gui_init(gui *ui, graphics *gfx, const int width, const int height);

void gui_add_label(gui *ui, const int y, const char *text);

void gui_build(gui *ui);

void gui_render(gui *ui, const int pointer);

#endif
