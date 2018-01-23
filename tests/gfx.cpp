#include<SDL.h>

struct gfx
{
    void *window;
    void *renderer;

    gfx();
    ~gfx();
};

gfx::gfx()
{
    window = NULL;
    renderer = NULL;
}

gfx::~gfx()
{
    if(this->window)
        SDL_DestroyWindow(this->window);
    if(this->renderer)
        SDL_DestroyRenderer(this->renderer);
    SDL_Quit();
}

struct gfx_safe
{
    gfx *gfx_ptr;

    gfx_safe();
    ~gfx_safe();
};

gfx_safe::gfx_safe()
{
    gfx_ptr = new gfx();

    if (SDL_Init(SDL_INIT_VIDEO))
        delete gfx_ptr;

    SDL_Window *window = SDL_CreateWindow("title",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        600,
        600,
        0);

    if (!window)
        delete gfx_ptr;

    gfx_ptr->window = window;

    SDL_Renderer *renderer = SDL_CreateRenderer(gfx_ptr->window, -1, 0);

    if (!renderer)
        delete gfx_ptr;

    gfx_ptr->renderer = renderer;
}

gfx_safe::~gfx_safe()
{
    delete gfx_ptr;
}






