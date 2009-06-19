#include "element.h"

element elements[256];

/* Initialize all elements */
void element_init()
{
    int i;
    int j;
    
    for(i=0; i<256; ++i)
    {
        element_mass(i, 0.7, -0.7);
        element_size(i, 6.0, 6.0);
        element_dampen(i, 0.987, 0.987);
        element_rgb0(i, 0x00, 0x00, 0xff);
        element_rgb1(i, 0xff, 0x00, 0x00);
        elements[i].flags = 0x00;
        element_bias(i, 0.5);
        
        for(j=0; j<256; ++j) {
            elements[i].reaction[j] = -1;
        }
        elements[i].hot_state = -1;
        elements[i].cold_state = -1;
        element_lifespan(i, 0);
    }
}

void element_reaction(const unsigned char type, const unsigned char type2, const short type3, const short chance)
{
    elements[type].reaction[type2] = type3;
    elements[type].reaction_chance[type2] = chance;
}

void element_hot_state(const unsigned char type, const short type2)
{
    elements[type].hot_state = type2;
}

void element_cold_state(const unsigned char type, const short type2)
{
    elements[type].cold_state = type2;
}

void element_hotkey(const unsigned char type, const unsigned char hotkey)
{
	elements[type].hotkey = hotkey;
}

unsigned char element_get_hotkey(const unsigned char type)
{
	return elements[type].hotkey;
}

void element_lifespan(const unsigned char type, const unsigned short lifespan)
{
    elements[type].lifespan = lifespan;
}

void element_flags(const unsigned char type, const unsigned short flags)
{
    elements[type].flags |= flags;
}

void element_bias(const unsigned char type, const float bias)
{
    elements[type].bias = bias;
}

void element_emit(const unsigned char type, const float emit)
{
    elements[type].emit = emit;
}

void element_mass(const unsigned char type, const float a, const float b)
{
    element *e = &elements[type];
    e->mass[0] = a;
    e->mass[1] = b;
}

void element_dampen(const unsigned char type, const float a, const float b)
{
    element *e = &elements[type];
    e->dampen[0] = a;
    e->dampen[1] = b;
}

void element_size(const unsigned char type, const float a, const float b)
{
    element *e = &elements[type];
    e->size[0] = a;
    e->size[1] = b;
}

void element_rgb0(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b)
{
    element *e = &elements[type];
    e->rgb[0][0] = r;
    e->rgb[0][1] = g;
    e->rgb[0][2] = b;
}

void element_rgb1(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b)
{
    element *e = &elements[type];
    e->rgb[1][0] = r;
    e->rgb[1][1] = g;
    e->rgb[1][2] = b;
}

void element_name(const unsigned char type, const char *name)
{
    strncpy(elements[type].name, name, 31);
}

char *element_get_name(const unsigned char type)
{
    return elements[type].name;
}

/* Apply element properties to particle (each loop) */
void element_apply(particle *p)
{
    if(p->next_type > -1) p->type = p->next_type;

    element *e = &elements[p->type];

    if((e->flags & M_LIFESPAN) && p->birth == -1) p->birth = SDL_GetTicks();

    if(e->flags & M_MORPH)
    {
        const float b = p->energy, a = 1.0-b;
        p->mass =   e->mass[0]*a + e->mass[1]*b;
        p->dampen = e->dampen[0]*a + e->dampen[1]*b;
        p->size =   e->size[0]*a + e->size[1]*b;
        p->rgb[0] = (GLubyte)((float)e->rgb[0][0]*a + (float)e->rgb[1][0]*b);
        p->rgb[1] = (GLubyte)((float)e->rgb[0][1]*a + (float)e->rgb[1][1]*b);
        p->rgb[2] = (GLubyte)((float)e->rgb[0][2]*a + (float)e->rgb[1][2]*b);
    }
    else
    {
        p->mass =   e->mass[0];
        p->dampen = e->dampen[0];
        p->size =   e->size[0];
        p->rgb[0] = e->rgb[0][0];
        p->rgb[1] = e->rgb[0][1];
        p->rgb[2] = e->rgb[0][2];
    }

    if(e->flags & M_BIAS)
    {
        if(p->energy < e->bias) p->energy += 0.0002;
        else if(p->energy > e->bias) p->energy -= 0.0002;
    }
}

element *element_get(const particle *p)
{
    return &elements[p->type];
}
