#ifndef particle_sys_h
#define particle_sys_h

#include <string.h>
#include <stdio.h>

#include "grid.h"
#include "particle.h"
#include "block.h"

void psys_add(particle *p);
void psys_add_element(const unsigned char x, const unsigned char y, const int element_pointer);
void psys_sub(const int x, const int y);
void psys_calc();
void psys_delete(const unsigned char x, const unsigned char y);
void psys_init(const int width, const int height, const int size);
int *psys_size;

#endif
