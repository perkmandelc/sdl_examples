#include <SDL.h>
#include <assert.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <stdint.h>
#include <math.h>

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

#define NUM_SOUND_CHANNELS 1
#define SOUND_BUFFER_NUM_SAMPLES 512
#define SOUND_BUFFER_LENGTH NUM_SOUND_CHANNELS*48000
static int sound_samples_since_fill = 0;
static int16_t sound_buffer[SOUND_BUFFER_LENGTH];
static float *wav_sound_buffer;
static int wav_sound_length;

static void sdl_audio_callback(void *not_used, uint8_t *audio_data, int size_to_write) {
    int length_to_write = size_to_write/sizeof(int16_t);

    static int cur_sample_to_read = 0;
    uint16_t *write_to = (uint16_t *)audio_data;

    for (int i = 0; i < length_to_write; i++) {
        write_to[i] = (int16_t)(1000.0f*wav_sound_buffer[2*cur_sample_to_read % wav_sound_length]);
        cur_sample_to_read++;
    }
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

    {
        uint8_t *wav_data;
        SDL_AudioSpec wav_spec;

        SDL_LoadWAV("../src/sound.wav", &wav_spec, &wav_data, &wav_sound_length);
        wav_sound_length /= sizeof(float);
        assert(wav_data);
        assert(wav_spec.freq == 48000);
        assert(wav_spec.format == AUDIO_F32);
        wav_sound_buffer = (float *)wav_data;
    }

    {
        SDL_AudioSpec audio_settings = {0};
                
        audio_settings.freq = 48000;
        audio_settings.format = AUDIO_S16LSB;
        audio_settings.channels = NUM_SOUND_CHANNELS;
        audio_settings.samples = SOUND_BUFFER_NUM_SAMPLES;
        audio_settings.callback = &sdl_audio_callback;
                
        SDL_OpenAudio(&audio_settings, 0);
        SDL_PauseAudio(0);
    }

    const char *keyboard_state = SDL_GetKeyboardState(NULL);
    int w_key_was_pressed = 0;
    int mouse_dx, mouse_dy;
    int mouse_x, mouse_y;

    int prev_mouse_x = 0;
    int prev_mouse_y = 0;

    int cur_sample = SOUND_BUFFER_LENGTH;
    
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
        
        SDL_Delay(100);
    }

    return 0;
}
