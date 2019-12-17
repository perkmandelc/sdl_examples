cd build/
gcc ../src/01_window_opengl.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 01_window_opengl
gcc ../src/02_input_output.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 02_input_output
gcc ../src/03_sound.c $(sdl2-config --cflags) -lGL -lm $(sdl2-config --libs) -o 03_sound
