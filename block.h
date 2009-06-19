#ifndef block_h
#define block_h

#include "element.h"

#define A_COLOR 0
#define A_LEFTARROW 1
#define A_RIGHTARROW 2
#define A_UPARROW 3
#define A_DOWNARROW 4

typedef struct
	{
		unsigned short flags;
		unsigned char appearance;
		
		float velocity[2];
		float emit;
		GLubyte rgb[2][3];
		
		char name[32];
		unsigned char hotkey;
	} block;

void block_init();

void block_flags(const unsigned char type, const unsigned short flags);

void block_velocity(const unsigned char type, const float vx, const float vy);

void block_emit(const unsigned char type, const float emit);

void block_rgb0(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b);

void block_name(const unsigned char type, const char *name);

void block_appearance(const unsigned char type, const char appearance);

void block_hotkey(const unsigned char type, const unsigned char hotkey);

unsigned short block_get_flags(const unsigned char type);

unsigned short block_get_appearance(const unsigned char type);

void block_get_velocity(const unsigned char type, float* vx, float* vy);

void block_get_rgb(const unsigned char type, GLubyte** rgb);
					  
float block_get_emit(const unsigned char type);

unsigned char block_get_hotkey(const unsigned char type);

char *block_get_name(const unsigned char type);

#endif
