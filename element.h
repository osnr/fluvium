#ifndef element_h
#define element_h

#include "particle.h"

#define M_MORPH    (1<<0)
#define M_SHARE    (1<<1)
#define M_EMIT     (1<<2)
#define M_BIAS     (1<<3)
#define M_LIFESPAN (1<<4)
#define M_CLUMP    (1<<5)
#define M_SOLID    (1<<6)
#define M_DESTROY  (1<<7)
#define M_VELOCITY (1<<8)

typedef struct
{
    unsigned short flags;
    float bias;
    float emit;
    float mass[2];
    float dampen[2];
    float size[2];
    GLubyte rgb[2][3];
    char name[32];
	unsigned char hotkey;
    short reaction[256];
    short reaction_chance[256];
    short hot_state;
    short cold_state;
    unsigned short lifespan;
} element;

void element_init();

void element_reaction(const unsigned char type, const unsigned char type2, const short type3, const short chance);

void element_hot_state(const unsigned char type, const short type2);

void element_cold_state(const unsigned char type, const short type2);

void element_lifespan(const unsigned char type, const unsigned short lifespan);

void element_flags(const unsigned char type, const unsigned short flags);

void element_bias(const unsigned char type, const float bias);

void element_emit(const unsigned char type, const float emit);

void element_mass(const unsigned char type, const float a, const float b);

void element_dampen(const unsigned char type, const float a, const float b);

void element_size(const unsigned char type, const float a, const float b);

void element_rgb0(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b);

void element_rgb1(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b);

void element_name(const unsigned char type, const char *name);

void element_hotkey(const unsigned char type, const unsigned char hotkey);

char *element_get_name(const unsigned char type);

unsigned char element_get_hotkey(const unsigned char type);

void element_apply(particle *p);

element *element_get(const particle *p);

#endif
