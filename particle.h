#ifndef particle_h
#define particle_h

#include "SDL_opengl.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "graphics.h"

typedef struct
{
    unsigned char type;
    short next_type;
    unsigned char net_energy_count;
    float pos[2];
    float vel[2];
    float energy, net_energy;
    float mass, dampen, size;
    GLubyte rgb[3];
    int birth;
} particle;

void particle_factory(particle *p, const unsigned char type);

void particle_react(particle *a, const particle *b);


#endif
