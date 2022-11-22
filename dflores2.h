//Dominic Flores
//
#include <stdio.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#define MAX_GRID 80


typedef struct Snake2
{
    int status;
    int length;
    int pos[MAX_GRID*MAX_GRID][2];
    int direction;
    double timer;
    double delay;
}snake2;

extern void snake_direction(int, int [][2]);
extern int snake_hits_edge(int [][2], int);
extern int snake_eats_itself(int, int[][2]);
extern int player_crosses_player(int, int[][2], int[][2]);
extern void show_pause_screen(int, int);
unsigned char * buildAlphaData(unsigned char*, int, int);
extern void game_Texture(GLuint, int[], int, int);

