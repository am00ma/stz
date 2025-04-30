#pragma once

#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>

/* ---------------------------------------------------------------------------
 * 'Opaque' container
 * ------------------------------------------------------------------------- */
typedef struct _Gui Gui; // Container for each platform

constexpr i32      FPS    = 30;
constexpr SDL_Rect BOUNDS = {0, 0, 640, 480};

/* ---------------------------------------------------------------------------
 * Callbacks
 * ------------------------------------------------------------------------- */
typedef void (*gui_fn_setup)(Gui* gui, void* arg);
typedef void (*gui_fn_render)(Gui* gui, void* arg);
typedef void (*gui_fn_handle)(Gui* gui, void* arg);

typedef struct {

    gui_fn_setup  setup;
    gui_fn_handle handle;
    gui_fn_render render;

} GuiCallbacks;

void gui_default_setup(Gui* gui, void* arg);
void gui_default_handle(Gui* gui, void* arg);
void gui_default_render(Gui* gui, void* arg);

/* ---------------------------------------------------------------------------
 * Lifetime
 * ------------------------------------------------------------------------- */
typedef enum error_t {

    GUI_SUCCESS = 0,
    GUI_FAILED_INIT,
    GUI_FAILED_WINDOW,
    GUI_FAILED_INPUTS,
    GUI_FAILED_RENDERER,
    GUI_FAILED_IMAGE,

} GuiError;

// Constructor, main loop, destructor
int gui_init(Gui* r, char* title, i32 width, i32 height, GuiCallbacks callbacks);
int gui_run(Gui* gui, void* arg);
int gui_destroy(Gui* r);

// Oneshot, share arg with callbacks
int gui_show(Gui* gui, char* title, i32 width, i32 height, GuiCallbacks callbacks, void* arg);

/* ---------------------------------------------------------------------------
 * Methods
 * ------------------------------------------------------------------------- */
void gui_clear(Gui* gui);
void gui_fill(Gui* gui, SDL_Color color);
void gui_rect(Gui* gui, SDL_Rect bounds, SDL_Color color);
void gui_text(Gui* gui, char* text, int len, int x, int y);

/* ===========================================================================
 * SDL - only implementation so far
 * ========================================================================= */
#define gui_poll   SDL_PollEvent
#define gui_key(e) (e).key.keysym.sym

// NOTE: Currently fails if font.png is not found
constexpr int  GUI_FONT_CHAR_MAX    = 126;
constexpr int  GUI_FONT_CHAR_MIN    = 31;
constexpr int  GUI_FONT_CHAR_NUM    = 95;
constexpr int  GUI_FONT_CHAR_WIDTH  = 17;
constexpr int  GUI_FONT_CHAR_HEIGHT = 25;
constexpr char GUI_FONT_PATH[]      = "/usr/local/share/stz/font.png";

typedef struct {

    const char*  path;
    SDL_Texture* tex;
    int          h, w;

} GuiFont;

typedef struct _Gui {

    // SDL references
    SDL_Window*   win;
    SDL_Renderer* rnd;
    SDL_Event     ev;

    // State
    SDL_Rect bounds;
    bool     quit;

    // Callbacks
    gui_fn_setup  setup;
    gui_fn_render render;
    gui_fn_handle handle;

    // Timing
    Uint32 interval;
    Uint32 tstart;
    Uint32 telapsed;

    // Font
    GuiFont font;

} Gui;
