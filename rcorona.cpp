//Ruben Corona
//2 player snake code
//
#include <stdio.h>
#define MAX_GRID 80


/*typedef struct a_snake
{
    int status;
    int length;
    int pos[MAX_GRID*MAX_GRID][2];
    int direction;
    double timer;
    double delay;
}Ai_snake;*/


unsigned int computer_snake(unsigned int state)
{
    state = state ^ 1;
    return state;
}
