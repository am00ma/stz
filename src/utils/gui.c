#include "utils/gui.h"
#include "range.h"

/* ===========================================================================
 * SDL
 * ========================================================================= */
void gui_default_setup(Gui* gui, void* arg) {}

void gui_default_handle(Gui* gui, void* arg)
{
    while (SDL_PollEvent(&gui->ev))
    { // Store in renderer struct
        switch (gui->ev.type)
        {
        case SDL_QUIT: gui->quit = true; break;
        case SDL_KEYDOWN:
            if (gui_key(gui->ev) == SDLK_ESCAPE) gui->quit = true;
            break;
        }
    }
}

void gui_default_render(Gui* gui, void* arg) { gui_clear(gui); }

/* ---------------------------------------------------------------------------
 * Init, Destroy Run
 * ------------------------------------------------------------------------- */
int gui_init(Gui* gui, char* title, i32 width, i32 height, GuiCallbacks callbacks)
{
    gui->bounds   = (SDL_Rect){0, 0, width, height};
    gui->interval = 1000 / FPS;
    gui->setup    = callbacks.setup;
    gui->handle   = callbacks.handle;
    gui->render   = callbacks.render;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) return GUI_FAILED_INIT;
    SDL_ShowCursor(0);

    gui->win = SDL_CreateWindow(title, 0, 0, gui->bounds.w, gui->bounds.h, SDL_WINDOW_SHOWN);
    if (gui->win == NULL) return GUI_FAILED_WINDOW;

    gui->rnd = SDL_CreateRenderer(gui->win, -1, SDL_RENDERER_ACCELERATED);
    if (gui->rnd == NULL) return GUI_FAILED_RENDERER;

    // Load font
    {
        if (IMG_Init(IMG_INIT_PNG) == 0) return GUI_FAILED_IMAGE;

        SDL_Surface* sur = IMG_Load(GUI_FONT_PATH);
        if (sur == NULL) return GUI_FAILED_IMAGE;
        gui->font.w = sur->w;
        gui->font.h = sur->h / GUI_FONT_CHAR_NUM;

        gui->font.tex = SDL_CreateTextureFromSurface(gui->rnd, sur);
        if (gui->font.tex == NULL) return GUI_FAILED_IMAGE;
        SDL_FreeSurface(sur);
    }

    gui_fill(gui, (SDL_Color){0});
    SDL_RenderPresent(gui->rnd);

    return GUI_SUCCESS;
}

int gui_destroy(Gui* gui)
{
    SDL_DestroyWindow(gui->win);
    SDL_DestroyRenderer(gui->rnd);
    SDL_Quit();
    return GUI_SUCCESS;
}

int gui_run(Gui* gui, void* arg)
{
    gui->quit = false;
    while (!gui->quit)
    {
        gui->tstart = SDL_GetTicks();

        gui->handle(gui, arg);
        gui->render(gui, arg);
        SDL_RenderPresent(gui->rnd);

        gui->telapsed = SDL_GetTicks() - gui->tstart;
        if (gui->telapsed < gui->interval) SDL_Delay(gui->interval - gui->telapsed);
    }
    return GUI_SUCCESS;
}

int gui_show(Gui* gui, char* title, i32 width, i32 height, GuiCallbacks callbacks, void* arg)
{
    GuiError err;

    err = gui_init(gui, title, width, height, callbacks);
    if (err) return err;

    gui->setup(gui, arg);

    err = gui_run(gui, arg);
    if (err) return err;

    err = gui_destroy(gui);
    if (err) return err;

    return GUI_SUCCESS;
}

void gui_clear(Gui* gui)
{
    gui_fill(gui, (SDL_Color){0});
    SDL_RenderClear(gui->rnd);
}

void gui_fill(Gui* gui, SDL_Color color)
{
    SDL_SetRenderDrawColor(gui->rnd, color.r, color.g, color.g, color.a);
    SDL_RenderClear(gui->rnd);
}

void gui_rect(Gui* gui, SDL_Rect bounds, SDL_Color color)
{
    SDL_SetRenderDrawColor(gui->rnd, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gui->rnd, &(SDL_Rect){bounds.x, bounds.y, bounds.w, bounds.h});
}

static inline void print_char(Gui* gui, int c, int x, int y)
{
    SDL_RenderCopy(gui->rnd, gui->font.tex,
                   &(SDL_Rect){0, (c - GUI_FONT_CHAR_MIN - 1) * gui->font.h, gui->font.w, gui->font.h},
                   &(SDL_Rect){x, y, gui->font.w, gui->font.h});
}

void gui_text(Gui* gui, char* text, int len, int x, int y)
{
    RANGE(i, len) { print_char(gui, text[i], x + (i * gui->font.w), y); }
}
