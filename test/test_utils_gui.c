#include "macros.h"
#include "utils/gui.h"

/* ---------------------------------------------------------------------------
 * Some state we want to track
 * ------------------------------------------------------------------------- */
typedef struct {
    i32 pos;
    i32 len;
} State;

/* ---------------------------------------------------------------------------
 * Callbacks
 * ------------------------------------------------------------------------- */
void setup(Gui* gui, void* arg)
{
    State* state = arg;

    state->len = 100; // Initialize
}

void handle(Gui* gui, void* arg)
{
    while (gui_poll(&gui->ev))
    { // Store in renderer struct
        switch (gui->ev.type)
        {
        case SDL_QUIT: gui->quit = true; break;
        case SDL_KEYDOWN:
            if (gui_key(gui->ev) == SDLK_ESCAPE) gui->quit = true;
            break;
            // default: break;
        }
    }
}

void render(Gui* gui, void* arg)
{
    State*   state = arg;         // Reference
    SDL_Rect b     = gui->bounds; // Copy

    // Prettify
    b.x = 100;
    b.y = 200;
    b.w = b.w - 200;
    b.h = b.h - 400;

    // Update based on state
    b.w = (f32)((f32)state->pos / (f32)state->len) * (f32)b.w;

    // Render
    gui_clear(gui);
    gui_rect(gui, b, (SDL_Color){80, 20, 20, 255});
    gui_text(gui, "hello", 5, 200, 100);

    // Update state
    state->pos++;
    if (state->pos >= state->len) { state->pos = 0; }
}

/* ---------------------------------------------------------------------------
 * Boilerplate
 * ------------------------------------------------------------------------- */
int main()
{
    // Variables we can have access to in callbacks
    Gui   gui   = {0};
    State state = {0};

    // Declare callbacks
    GuiCallbacks callbacks = {
        .setup  = setup,
        .handle = handle,
        .render = render,
    };

    // Init, run main loop and exit
    // Pass state as arg to all gui callbacks
    GuiError err = gui_show(&gui, " GUI ", 640, 480, callbacks, &state);
    die(err, "gui_show: %d", err);

    return 0;
}
