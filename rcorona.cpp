//Ruben Corona
//2 player snake code
//
#include <stdio.h>
#define MAX_GRID 80
#include <X11/Xlib.h>
#include <GL/glx.h>


typedef struct a_snake
{
    int status;
    int length;
    int pos[MAX_GRID*MAX_GRID][2];
    int direction;
    double timer;
    double delay;
}Ai_snake;

unsigned int computer_snake (int check)
{
    check ^= 1;
    return check;
}
unsigned int two_player(int player)
{
    player = player ^ 1;
    return player;
}
