#include "gui.h"

int gui_width, gui_height;
int grid_width, grid_height;

/* Initialize GUI's surface */
void gui_init(gui *ui, graphics *gfx, const int width, const int height)
{
	gui_width = 128;
	gui_height = height;
	
	grid_width = width/8;
	grid_height = height/8;
	
    ui->gfx = gfx;
    ui->surf = SDL_CreateRGBSurface(SDL_HWSURFACE, gui_width, gui_height, 8, 0xff, 0xff, 0xff, 0xff);
    SDL_FillRect(ui->surf, NULL, SDL_MapRGB(gfx->surf->format, 0, 0, 0));
}

/* Add label to GUI at index y */
void gui_add_label(gui *ui, const int y, const char *text)
{
    static SDL_Color fg = {0xff, 0xff, 0xff};
    static SDL_Color bg = {0x00, 0x00, 0x00};
    SDL_Surface* ts = TTF_RenderText_Shaded(ui->gfx->font, text, fg, bg);
    SDL_Rect tr = {10, y * 16, 0, 0};
    SDL_BlitSurface(ts, NULL, ui->surf, &tr);
    SDL_FreeSurface(ts);

}

/* Compile GUI into gl texture */
void gui_build(gui *ui)
{
    glGenTextures(1, &ui->texture);
    glBindTexture(GL_TEXTURE_2D, ui->texture);
    GLubyte tex[ui->surf->w * ui->surf->h * 3];
    int y, x;
    for(y = 0; y < ui->surf->h; ++y)
    {
        for(x = 0; x < ui->surf->w; ++x)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pget(ui->surf, x, y), ui->surf->format, &r, &g, &b);
            tex[(x * 3) + (y * ui->surf->w * 3) + 0] = r;
            tex[(x * 3) + (y * ui->surf->w * 3) + 1] = g;
            tex[(x * 3) + (y * ui->surf->w * 3) + 2] = b;
        }
    }
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ui->surf->w, ui->surf->h, GL_RGB, GL_UNSIGNED_BYTE, tex);
}

/* Render GUI as quad in OpenGL */
void gui_render(gui *ui, const int pointer)
{
	int w = grid_width * 8;
    glEnable(GL_TEXTURE_2D);
    glColor3ub(0xff, 0xff, 0xff);
    glBindTexture(GL_TEXTURE_2D, ui->texture);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(w, 0);
    glTexCoord2i(1, 0); glVertex2i(w+ui->surf->w, 0);
    glTexCoord2i(1, 1); glVertex2i(w+ui->surf->w, ui->surf->h);
    glTexCoord2i(0, 1); glVertex2i(w, ui->surf->h);
    glEnd();
	glDisable(GL_TEXTURE_2D);
}

