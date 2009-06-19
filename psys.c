#include "psys.h"
#include "element.h"

particle *m_p;

int m_active, m_size, m_width, m_height;

void psys_add(particle *p)
{
    if(grid_get_type((int)p->pos[0]>>3, (int)p->pos[1]>>3) == G_SOLID) return;
    if(m_active<m_size)
    {
        memcpy(&m_p[m_active], p, sizeof(particle));
        m_active++;
    }
}

void psys_add_element(const unsigned char x, const unsigned char y, const int element_pointer)
{
    particle p;
    p.vel[0] = 0.0;
    p.vel[1] = 0.0;
    p.pos[0] = (x<<3) + (rand()%8);
    p.pos[1] = (y<<3) + (rand()%8);
    particle_factory(&p, element_pointer);
    psys_add(&p);
}

void psys_calc()
{
    particle *e = m_p + m_active;
    particle *i = m_p;
    int y, x;
    unsigned char c;
    
    for(i=m_p; i<e; ++i)
    {
        const float recip = 1.0 / 8;
        int dx1 = (int)((i->pos[0] - i->size * 2) * recip);
        int dy1 = (int)((i->pos[1] - i->size * 2) * recip);
        int dx2 = (int)((i->pos[0] + i->size * 2) * recip);
        int dy2 = (int)((i->pos[1] + i->size * 2) * recip);
        if(dx1 < 0) dx1 = 0;
        else if(dx2 > 63) dx2 = 63;
        if(dy1 < 0) dy1 = 0;
        else if(dy2 > 63) dy2 = 63;
        for(y=dy1; y<=dy2; ++y)
        {
            for(x=dx1; x<=dx2; ++x)
            {
                if(!(block_get_flags(grid_get_type(x, y)) & M_SOLID))
                {
                    const unsigned int size = grid_get_size(x, y);
                    for(c=0; c < size; ++c)
                    {
                        particle *n = grid_get(x, y, c);
                        if(n != i) particle_react(i, n);
                    }
                }
            }
        }
    }
    grid_clear();

    /* Kill bad particles */
    for(i=m_p; i<m_p+m_active; ++i)
    {
        const unsigned char gtype = grid_get_type((int)i->pos[0]>>3, (int)i->pos[1]>>3);
		const unsigned int gflags = block_get_flags(gtype);
        if(gflags & M_SOLID || gflags & M_DESTROY || i->pos[0]<0 || i->pos[1]<0 || i->pos[0]>511 || i->pos[1]>511)
        {
            m_active--;
            *i = m_p[m_active];
            continue;
        }
        if(i->birth > -1 && element_get(i)->flags & M_LIFESPAN)
        {
            if(i->birth + element_get(i)->lifespan < SDL_GetTicks())
            {
                m_active--;
                *i = m_p[m_active];
                continue;
            }
        }
    }

    e = m_p + m_active;
    for(i=m_p; i<e; ++i)
    {
        const element *et = element_get(i);
        if(i->net_energy_count)
        {
            i->net_energy /= i->net_energy_count;
            i->energy = i->energy * 0.8 + i->net_energy * 0.2;
            i->net_energy = 0.0;
            i->net_energy_count = 0;
        }

        if(i->energy > 1.0) i->energy = 1.0;
        else if(i->energy < 0.0) i->energy = 0.0;

        element_apply(i);

        if(i->energy > 0.9)
        {
            if(et->hot_state > -1) i->next_type = et->hot_state;
        }
        else if(i->energy < 0.1)
        {
            if(et->cold_state > -1) i->next_type = et->cold_state;
        }

        const unsigned char gtype = grid_get_type((int)i->pos[0]>>3, (int)i->pos[1]>>3);
		float vx, vy;
		block_get_velocity(gtype, &vx, &vy);
		
		i->vel[0] += vx;
		i->vel[1] += vy;
		
        if(et->flags & M_SHARE)
        {
            i->energy += block_get_emit(gtype);
        }

        i->vel[1] += i->mass * 0.055;
        i->vel[0] *= i->dampen;
        i->vel[1] *= i->dampen;

        /* Take note that the condition is on the SQUARED value */
        float vlen = i->vel[0]*i->vel[0] + i->vel[1]*i->vel[1];
        if(vlen > 16.0)
        {
            vlen = (1.0 / sqrtf(vlen)) * 4.0;
            i->vel[0] *= vlen;
            i->vel[1] *= vlen;
        }

        const float test[2] = {i->pos[0] + i->vel[0], i->pos[1] + i->vel[1]};
        const int t[2][2] = {{(int)test[0]>>3, (int)test[1]>>3}, {(int)i->pos[0]>>3, (int)i->pos[1]>>3}};
        const char hitx = block_get_flags(grid_get_type(t[0][0], t[1][1])) & M_SOLID;
        const char hity = block_get_flags(grid_get_type(t[1][0], t[0][1])) & M_SOLID;
        if(!(block_get_flags(grid_get_type(t[0][0], t[0][1])) & M_SOLID))
        {
            i->pos[0] += i->vel[0];
            i->pos[1] += i->vel[1];
        }
        else if(!hity && hitx)
        {
            i->pos[1] += i->vel[1];
            i->vel[0] *= -1.0;
        }
        else if(!hitx && hity)
        {
            i->pos[0] += i->vel[0];
            i->vel[1] *= -1.0;
        }
        else
        {
            i->vel[0] *= -1.0;
            i->vel[1] *= -1.0;
        }

        if(!(i->pos[0]<0 || i->pos[1]<0 || i->pos[0]>511 || i->pos[1]>511))
        {
            grid_add(((int)i->pos[0])>>3, ((int)i->pos[1])>>3, i);
        }
    }
	graphics_draw_particles(m_active);
}

void psys_delete(const unsigned char x, const unsigned char y)
{
	particle *i;
	for(i=m_p; i<m_p+m_active; ++i)
    {
		if((int)i->pos[0]>>3 == x && (int)i->pos[1]>>3 == y)
		{
			i->pos[0] = -1;
			i->pos[1] = -1;
			continue;
		}
	}
}
				 
void psys_init(const int width, const int height, const int size)
{
    m_p = malloc(sizeof(particle) * size);
    m_size = size;
    m_width = width;
    m_height = height;
    m_active = 0;
    psys_size = &m_active;
    grid_init();
    graphics_psys_init(m_p[0].rgb, m_p[0].pos);
}

