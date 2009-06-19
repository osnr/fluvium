#include "config.h"

typedef struct
{
    char *data;
    unsigned int i;
    unsigned int len;
} buffer;

/* Load entire file into buffer */
void buffer_init(buffer *buf, const char *filename)
{
    FILE *fin = fopen(filename, "rb");
	
	fseek(fin, 0, SEEK_END);
	const int size = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	buf->data = malloc(size+1);
	fread(buf->data, 1, size, fin);
	fclose(fin);
	buf->data[size] = '\0';
	buf->len = size;
	buf->i = 0;
}

/* Release buffer from memory */
void buffer_free(buffer *buf)
{
    free(buf->data);
}

/* Move buffer cursour back */
void buffer_prev(buffer *buf)
{
    buf->i -= 1;
}

/* Move buffer cursour forward */
void buffer_next(buffer *buf)
{
    buf->i += 1;
}

/* Get current character */
char buffer_at(buffer *buf)
{
    return buf->data[buf->i];
}

/* Advance past whitespace */
void buffer_trim(buffer *buf)
{
    if(isgraph(buffer_at(buf))) return;
    while(!isgraph(buffer_at(buf)))
    {
        buffer_next(buf);
    }
}

/* Return whether cursor is at the end of the buffer */
int buffer_eof(buffer *buf)
{
    return buf->i >= buf->len;
}

/* Read command label from buffer */
int buffer_get_command(buffer *buf, char command[32])
{
    int i = 0;
    buffer_trim(buf);
    while(!buffer_eof(buf) && buffer_at(buf) != '(' && i < 32)
    {
        if(isgraph(buffer_at(buf)))
        {
            command[i] = buffer_at(buf);
            ++i;
        }
        buffer_next(buf);
    }
    buffer_next(buf);
    if(i < 32) command[i] = '\0';
    else return 1;
    return 0;
}

/* Read quoted string from buffer */
int buffer_get_string(buffer *buf, char str[32])
{
    int i = 0;
    buffer_trim(buf);
    if(buffer_at(buf) != '"') return 1;
    buffer_next(buf);
    while(!buffer_eof(buf) && buffer_at(buf) != '"' && i < 32)
    {
        str[i] = buffer_at(buf);
        buffer_next(buf);
        ++i;
    }
    buffer_next(buf);
    if(buffer_at(buf) == ',') buffer_next(buf);

    if(i < 32) str[i] = '\0';
    else return 1;
    return 0;
}

/* Read integer from buffer */
int buffer_get_int(buffer *buf, int *ret)
{
    char str[32];
    int i = 0;
    buffer_trim(buf);
    while(!buffer_eof(buf) && buffer_at(buf) >= '0'
        && buffer_at(buf) <= '9' && i < 32)
    {
        str[i] = buffer_at(buf);
        ++i;
        buffer_next(buf);
    }
    if(i < 32) str[i] = '\0';
    else return 1;

    buffer_next(buf);
    if(buffer_at(buf) == ',') buffer_next(buf);

    *ret = atoi(str);
    return 0;
}

/* Read float from buffer */
int buffer_get_float(buffer *buf, float *ret)
{
    char str[32];
    int i = 0;
    buffer_trim(buf);
    while(!buffer_eof(buf) && ((buffer_at(buf) >= '0' && buffer_at(buf) <= '9')
        || buffer_at(buf) == '.' || buffer_at(buf) == '-')  && i < 32)
    {
        str[i] = buffer_at(buf);
        ++i;
        buffer_next(buf);
    }
    if(i < 32) str[i] = '\0';
    else return 1;

    buffer_next(buf);
    if(buffer_at(buf) == ',') buffer_next(buf);

    *ret = atof(str);
    return 0;
}

/* Parse parameters using format string / destination references */
void parsef(buffer *buf, const char *fmt, ...)
{
    if(!strlen(fmt))
    {
        while(buffer_at(buf) != ')') buffer_next(buf);
        buffer_next(buf);
        return;
    }

    va_list args;
    va_start(args, fmt);
    
    int i;
    for(i=0; i<strlen(fmt); ++i)
    {
        switch(fmt[i])
        {
            case 's':
            {
                char str[32];
                buffer_get_string(buf, str);
                strncpy(va_arg(args, char*), str, 31);
            }
            break;
            case 'i': buffer_get_int(buf, va_arg(args, int*)); break;
            case 'f': buffer_get_float(buf, va_arg(args, float*)); break;
        }
    }
    va_end(args);

    buffer_trim(buf);
    if(buffer_at(buf) == ')') buffer_next(buf);
}

void config_parse(gui *ui, const char *config_file, int *element_count, int *block_count)
{
    buffer buf;
	
    buffer_init(&buf, config_file);

	unsigned char element_pointer = -1;
	unsigned char block_pointer = 1;	
	int element = -1;
	
    char command[32];

    int reaction_count[256];
    short reaction_chance[256][256];
    char reaction[256][256][2][32];

    char hot_state[256][32];
    char cold_state[256][32];
    
    int i;
    for(i=0; i<256; ++i)
    {
        hot_state[i][0] = '\0';
        cold_state[i][0] = '\0';
        reaction_count[i] = 0;
    }
	
    while(!buffer_get_command(&buf, command) && !buffer_eof(&buf))
    {
        if(!strcmp(command, "newElement"))
        {
            char str[32];
            parsef(&buf, "s", str);
            ++element_pointer;
			element_name(element_pointer, str);
			
			element = 1;
        }
		else if(!strcmp(command, "newBlock"))
        {
            char str[32];
            parsef(&buf, "s", str);
            ++block_pointer;
			block_name(block_pointer, str);
			
			element = 0;
        }
		else if(!strcmp(command, "setAppearance"))
        {
            char str[32];
            parsef(&buf, "s", str);
			
			char appearance = A_COLOR;
			
			if(!strcmp(str, "LEFTARROW")) appearance = A_LEFTARROW;
			else if(!strcmp(str, "RIGHTARROW")) appearance = A_RIGHTARROW;
			else if(!strcmp(str, "UPARROW")) appearance = A_UPARROW;
			else if(!strcmp(str, "DOWNARROW")) appearance = A_DOWNARROW;
			
			block_appearance(block_pointer, appearance);
        }
		else if(!strcmp(command, "setHotkey"))
        {
            char str[1];
            parsef(&buf, "s", str);
			
			if(element) element_hotkey(element_pointer, str[0]);
			else block_hotkey(block_pointer, str[0]);
        }
        else if(!strcmp(command, "setRGB0"))
        {
            int r, g, b;
            parsef(&buf, "iii", &r, &g, &b);
			
			if(element) element_rgb0(element_pointer, r, g, b);
			else block_rgb0(block_pointer, r, g, b);
        }
        else if(!strcmp(command, "setRGB1"))
        {
            int r, g, b;
            parsef(&buf, "iii", &r, &g, &b);
			
            element_rgb1(element_pointer, r, g, b);
        }
        else if(!strcmp(command, "setGravity"))
        {
            float g1, g2;
            parsef(&buf, "ff", &g1, &g2);
            element_mass(element_pointer, g1, g2);
        }
        else if(!strcmp(command, "setSize"))
        {
            float s1, s2;
            parsef(&buf, "ff", &s1, &s2);
            element_size(element_pointer, s1, s2);
        }
        else if(!strcmp(command, "setBias"))
        {
            float b;
            parsef(&buf, "f", &b);
            element_bias(element_pointer, b);
        }
        else if(!strcmp(command, "setEmit"))
        {
            float e;
            parsef(&buf, "f", &e);
            
			if (element) element_emit(element_pointer, e);
			else block_emit(block_pointer, e);
        }
		else if(!strcmp(command, "setVelocity"))
        {
            float vx, vy;
            parsef(&buf, "ff", &vx, &vy);
            
			block_velocity(block_pointer, vx, vy);
        }
        else if(!strcmp(command, "setFriction"))
        {
            float f1, f2;
            parsef(&buf, "ff", &f1, &f2);
            element_dampen(element_pointer, f1, f2);
        }
        else if(!strcmp(command, "enableBias"))
        {
            parsef(&buf, "");
            element_flags(element_pointer, M_BIAS);
        }
		else if(!strcmp(command, "enableVelocity"))
        {
            parsef(&buf, "");
            block_flags(block_pointer, M_VELOCITY);
        }
        else if(!strcmp(command, "enableMorphing"))
        {
            parsef(&buf, "");
            element_flags(element_pointer, M_MORPH);
        }
        else if(!strcmp(command, "enableSharing"))
        {
            parsef(&buf, "");
            element_flags(element_pointer, M_SHARE);
        }
        else if(!strcmp(command, "enableEmit"))
        {
            parsef(&buf, "");
			
			if (element) element_flags(element_pointer, M_EMIT);
			else block_flags(block_pointer, M_EMIT);
        }
        else if(!strcmp(command, "enableLifespan"))
        {
            parsef(&buf, "");
            element_flags(element_pointer, M_LIFESPAN);
        }
        else if(!strcmp(command, "enableClumping"))
        {
            parsef(&buf, "");
            element_flags(element_pointer, M_CLUMP);
        }
		else if(!strcmp(command, "enableSolid"))
        {
            parsef(&buf, "");
            block_flags(block_pointer, M_SOLID);
        }
		else if(!strcmp(command, "enableDestroy"))
        {
            parsef(&buf, "");
            block_flags(block_pointer, M_DESTROY);
        }
        else if(!strcmp(command, "setLifespan"))
        {
            int lifespan;
            parsef(&buf, "i", &lifespan);
            element_lifespan(element_pointer, lifespan);
        }
        else if(!strcmp(command, "addReaction"))
        {
            const int i = element_pointer;
            parsef(&buf, "ssi", reaction[i][reaction_count[i]][0],
                reaction[i][reaction_count[i]][1],
                &reaction_chance[i][reaction_count[i]]);
            ++reaction_count[i];
        }
        else if(!strcmp(command, "setHotState"))
        {
            parsef(&buf, "s", hot_state[element_pointer]);
        }
        else if(!strcmp(command, "setColdState"))
        {
            parsef(&buf, "s", cold_state[element_pointer]);
        }
        else
        {
            printf("Unknown command: %s\n", command);
        }
    }

    int j;
    for(i=0; i<=element_pointer; ++i)
    {
		gui_add_label(ui, i, element_get_name(i));
        if(hot_state[i][0])
        {
            for(j=0; j<=element_pointer; ++j)
            {
                if(!strncmp(hot_state[i], element_get_name(j), 32))
                {
                    element_hot_state(i, j);
                }
            }
        }
        if(cold_state[i][0])
        {
            for(j=0; j<=element_pointer; ++j)
            {
                if(!strncmp(cold_state[i], element_get_name(j), 32))
                {
                    element_cold_state(i, j);
                }
            }
        }
    }

    int k;
    /* This is terribly inefficient (lots of strcmps) */
    for(i=0; i<=element_pointer; ++i)
    {
        for(j=0; j<reaction_count[i]; ++j)
        {
            short a = -1;
            short b = -1;
            for(k=0; k<=element_pointer; ++k)
            {
                if(!strncmp(reaction[i][j][0], element_get_name(k), 32)) a = k;
                if(!strncmp(reaction[i][j][1], element_get_name(k), 32)) b = k;
                if(a > -1 && b > -1) break; 
            }
            element_reaction(i, a, b, reaction_chance[i][j]);
        }
    }

	for(i=0; i<=block_pointer; ++i)
    {
		gui_add_label(ui, i+element_pointer+1, block_get_name(i));
	}
	
    buffer_free(&buf);
    
	*element_count = element_pointer + 1;
	*block_count = block_pointer + 1;
}

