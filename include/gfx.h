#ifndef GFX_H
#define GFX_H

typedef struct gfx
{
    char *title;
    void *window;
    void *renderer;
    unsigned int width;
    unsigned int height;
    int closed;
    float delta_time;
    int full_screen;
    int v_sync;
    unsigned long long last_tick;
}gfx_t;

int gfx_init(gfx_t *);
void gfx_update(gfx_t *);
void gfx_clear(gfx_t *);
void gfx_putpixel(gfx_t *, unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
void gfx_destroy(gfx_t *);

#endif