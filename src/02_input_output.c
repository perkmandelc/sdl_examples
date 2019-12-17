#include <SDL.h>
#include <assert.h>
#include <SDL_opengl.h>
#include <GL/gl.h>

#define SDL_CHECK(funcall) if ((funcall) < 0) { printf("sdl error: %s\n", SDL_GetError()); assert(0); }
#define SDL_CHECK_VAR(var) if (!var) { printf("sdl error: %s\n", SDL_GetError()); assert(0); }

static void print_opengl_info(void) {
    int major_version;
    int minor_version;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version);
    printf("opengl %d.%d\n", major_version, minor_version);
    printf("%s\n", glGetString(GL_VERSION));
}


int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_GLContext gl_context = NULL;

#if 1
    int window_flags = 0;
#else
    int window_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    int window_width = 800;
    int window_height = 600;

    int window_x = SDL_WINDOWPOS_UNDEFINED;
    int window_y = SDL_WINDOWPOS_UNDEFINED;
    
    assert(SDL_Init(SDL_INIT_VIDEO) >= 0);

    {
        window = SDL_CreateWindow("window title",
                                  window_x, window_y,
                                  window_width, window_height, window_flags | SDL_WINDOW_OPENGL);
        SDL_CHECK_VAR(window);
    }

    {
        gl_context = SDL_GL_CreateContext(window);
        SDL_CHECK_VAR(gl_context);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        print_opengl_info();
        
        glViewport(0, 0, window_width, window_height);
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    const char *keyboard_state = SDL_GetKeyboardState(NULL);
    int w_key_was_pressed = 0;
    int mouse_dx, mouse_dy;
    int mouse_x, mouse_y;

    int prev_mouse_x = 0;
    int prev_mouse_y = 0;
    
    while (1) {
        {
            SDL_Event e;
            SDL_PollEvent(&e);
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        SDL_PumpEvents();
        int w_key_held_down = keyboard_state[SDL_SCANCODE_W];

        if (w_key_held_down) {
            if (!w_key_was_pressed) {
                printf("w key pressed\n");
            }
            w_key_was_pressed = 1;
        }
        else {
            w_key_was_pressed = 0;
        }

        SDL_GetMouseState(&mouse_x, &mouse_y);
        SDL_GetRelativeMouseState(&mouse_dx, &mouse_dy);

        if (mouse_x != prev_mouse_x || mouse_y != prev_mouse_y) {
            printf("mouse: %d, %d\n", mouse_x, mouse_y);
            printf("delta mouse: %d, %d\n", mouse_dx, mouse_dy);
        }

        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;
        
        if (keyboard_state[SDL_SCANCODE_Q]) {
            break;
        }

        SDL_Delay(30);
    }

    return 0;
}
