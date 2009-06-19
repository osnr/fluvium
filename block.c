#include "block.h"

block blocks[256];

/* Initialize all blocks */
void block_init()
{
	block_name(0, "Clear");
	block_hotkey(0, 'x');
	
	block_name(1, "Wall");
	block_hotkey(1, ' ');
	block_flags(1, M_SOLID);
	block_rgb0(1, 128, 128, 128);
	
    int i;    
    for(i=2; i<256; ++i)
    {
        block_rgb0(i, 0x00, 0x00, 0xff);
        
		blocks[i].flags = 0x00;
		blocks[i].velocity[0] = 0.0;
		blocks[i].velocity[1] = 0.0;
    }
}

void block_flags(const unsigned char type, const unsigned short flags)
{
    blocks[type].flags |= flags;
}

void block_hotkey(const unsigned char type, const unsigned char hotkey)
{
	blocks[type].hotkey = hotkey;
}

unsigned short block_get_flags(const unsigned char type)
{
	return blocks[type].flags;
}

float block_get_emit(const unsigned char type)
{
	return blocks[type].emit;
}

unsigned short block_get_appearance(const unsigned char type)
{
	return blocks[type].appearance;
}

unsigned char block_get_hotkey(const unsigned char type)
{
	return blocks[type].hotkey;
}

void block_get_velocity(const unsigned char type, float* vx, float* vy)
{
	*vx = blocks[type].velocity[0];
	*vy = blocks[type].velocity[1];
}

void block_emit(const unsigned char type, const float emit)
{
    blocks[type].emit = emit;
}

void block_velocity(const unsigned char type, const float vx, const float vy)
{
	block *e = &blocks[type];
	e->velocity[0] = vx;
	e->velocity[1] = vy;
}

void block_get_rgb(const unsigned char type, GLubyte** rgb) {
	*rgb = blocks[type].rgb[0];
}

void block_rgb0(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b)
{
    block *e = &blocks[type];
    e->rgb[0][0] = r;
    e->rgb[0][1] = g;
    e->rgb[0][2] = b;
}

void block_appearance(const unsigned char type, const char appearance) {
	block *e = &blocks[type];
	
	e->appearance = appearance;
}

void block_name(const unsigned char type, const char *name)
{
    strncpy(blocks[type].name, name, 31);
}

char *block_get_name(const unsigned char type)
{
    return blocks[type].name;
}