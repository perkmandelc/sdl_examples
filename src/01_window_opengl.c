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

    SDL_Delay(2000);

    return 0;
}
