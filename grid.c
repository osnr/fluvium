#include "grid.h"

void ****grid_grid; // [y][x][32 particles]
unsigned char **grid_size, ***grid_flags; // [y][x], [y][x][2 - block type and flags]
int grid_height, grid_width;

/* Add pointer to grid at (x,y) */
void grid_add(const unsigned char x, const unsigned char y, void *i)
{
    if(grid_size[y][x]<32)
    {
        grid_grid[y][x][grid_size[y][x]] = i;
        grid_size[y][x]++;
    }
}

/* Return pointer from (x,y,i) */
void *grid_get(const unsigned char x, const unsigned char y, const unsigned char i)
{
    return grid_grid[y][x][i];
}

/* Get size of block (number of pointers) at (x, y) */
unsigned char grid_get_size(const unsigned char x, const unsigned char y)
{
    return grid_size[y][x];
}

/* Clear entire grid by setting sizes to 0 */
void grid_clear()
{
	int y;
	
    for(y=0; y<grid_height; ++y)
    {
		memset(grid_size[y], 0, sizeof(unsigned char)*grid_width);
    }
	
}

/* Initialize all grids */
void grid_init(const int width, const int height)
{
	int x, y;
	grid_height = height;
	grid_width = width;
	
	grid_grid = malloc(sizeof(void ***)*height);
	memset(grid_grid, 0, sizeof(void ***)*height);
	
	grid_size = malloc(sizeof(unsigned char *)*height);
	memset(grid_size, 0, sizeof(unsigned char *)*height);
	
	grid_flags = malloc(sizeof(unsigned char **)*height);
	memset(grid_flags, 0, sizeof(unsigned char **)*height);
    
    for(y=0; y<height; ++y)
    {
		grid_grid[y] = malloc(sizeof(void **)*width);
		memset(grid_grid[y], 0, sizeof(void **)*width);
		
		grid_size[y] = malloc(sizeof(unsigned char)*width);
		memset(grid_size[y], 0, sizeof(unsigned char)*width);
		
		grid_flags[y] = malloc(sizeof(unsigned char *)*width);
		memset(grid_flags[y], 0, sizeof(unsigned char *)*width);
		
        for(x=0; x<width; ++x)
        {
			grid_grid[y][x] = malloc(sizeof(particle*)*32);
			memset(grid_grid[y][x], 0, sizeof(particle*)*32);
			
			grid_flags[y][x] = malloc(sizeof(unsigned char)*2);
			memset(grid_flags[y][x], 0, sizeof(unsigned char)*2);
			
            if(!x || !y || x==width-1 || y==height-1) grid_type(x, y, 1);
        }
    }
	grid_clear();
}

/* Set block type at (x, y) */
void grid_type(const unsigned char x, const unsigned char y, const unsigned char type)
{
    grid_flags[y][x][0] = type;
}

/* Set block data at (x, y) */
void grid_data(const unsigned char x, const unsigned char y, const unsigned char data)
{
    grid_flags[y][x][1] = data;
}

/* Get block type at (x, y) */
unsigned char grid_get_type(const unsigned char x, const unsigned char y)
{	
    return grid_flags[y][x][0];
}

/* Get block data at (x, y) */
unsigned char grid_get_data(const unsigned char x, const unsigned char y)
{
    return grid_flags[y][x][1];
}
