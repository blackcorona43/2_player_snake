#include <iostream>
#include <stdio.h>
#define MAX_GRID 80


typedef struct a_snake
{
    int status;
    int length;
    int pos[MAX_GRID*MAX_GRID][2];
    int direction;
    double timer;
    double delay;
}Ai_snake;


extern unsigned int computer_snake(int check);


extern unsigned int two_player(int check);

