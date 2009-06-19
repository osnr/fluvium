#ifndef grid_h
#define grid_h

#include <stdlib.h>
#include <string.h>
#include "block.h"

#define G_S 8

#define G_CLEAR (0)
#define G_SOLID (1)

#define G_EMIT  (0xFF)

/*
    The flag grid (for walls)
    The grid grid (stores particles, used to optimize collision)
    And the size grid (how many particles are in the grid grid at x, y)
*/

void grid_add(const unsigned char x, const unsigned char y, void *i);

void *grid_get(const unsigned char x, const unsigned char y, const unsigned char i);

void grid_clear();

void grid_init();

void grid_type(const unsigned char x, const unsigned char y, const unsigned char type);

void grid_data(const unsigned char x, const unsigned char y, const unsigned char data);

unsigned char grid_get_type(const unsigned char x, const unsigned char y);

unsigned char grid_get_data(const unsigned char x, const unsigned char y);

unsigned char grid_get_size(const unsigned char x, const unsigned char y);

#endif
