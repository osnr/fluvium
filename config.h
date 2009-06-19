#ifndef config_h
#define config_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "element.h"
#include "block.h"
#include "gui.h"

void config_parse(gui *ui, const char *config_file, int *element_count, int *block_count);

#endif
