//Ruben Corona
//2 player snake code
//
#include <stdio.h>
#include <string>
#define MAX_GRID 80
#include <X11/Xlib.h>
#include <GL/glx.h>
#define DIRECTION_DOWN   0
#define DIRECTION_LEFT   1
#define DIRECTION_UP     2
#define DIRECTION_RIGHT  3

unsigned int gameover_screen(int);

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

void ai_direction(int direction, int position [][2])
{
    switch(direction)
    {
        case DIRECTION_DOWN:  position[0][1] += 1; break;
        case DIRECTION_LEFT:  position[0][0] -= 1; break;
        case DIRECTION_UP:    position[0][1] -= 1; break;
        case DIRECTION_RIGHT: position[0][0] += 1; break;
    }

}

int check_off_board (int ai_snake_pos[][2], int grid, int p1_points, int gameover)
{
    if (ai_snake_pos[0][0] < 0 ||
            ai_snake_pos [0][0] > grid-1 ||
            ai_snake_pos [0][1] < 0 ||
            ai_snake_pos [0][1] > grid-1) {
        gameover = gameover + 1;
        printf("\n");
        printf("-----------------------------\n");
        printf("COMPUTER WENT OFF BOARD!\n");
        printf("Player 1 wins!\n");
        printf("player 1 has %d points\n", ++p1_points);
        printf("-----------------------------\n");
        return gameover;
    }
    return gameover;
}

int ai_cross_itself(int ai_length, int ai_snake_pos [][2], int p1_points, int gameover)
{
    for (int i=1; i < ai_length; i++) {
        if (ai_snake_pos[i][0] == ai_snake_pos[0][0] &&
                ai_snake_pos[i][1] == ai_snake_pos[0][1]) {
            gameover = gameover + 1;
            printf("\n");
            printf("-----------------------------\n");
            printf("COMPUTER ATE ITSELF!\n");
            printf("Player 1 wins!\n");
            printf("player 1 has %d points\n", ++p1_points);
            printf("-----------------------------\n");
            return gameover;
        }
    }
    return gameover;
}

int ai_head_collision (int ai_snake_pos[][2], int snake_pos[][2], int gameover)
{
    if(ai_snake_pos[0][0] == snake_pos[0][0] && 
            ai_snake_pos[0][1] == snake_pos[0][0]) {
        gameover = gameover + 1;
        printf("\n");
        printf("-----------------------------\n");
        printf("COM and Player1 killied each other!\n");
        printf("Its A Draw!!!!\n");
        return gameover;
    }
    return gameover;
}

int ai_crosses_player (int snake_length, int ai_snake_pos[][2],
                  int snake_pos[][2],int gameover)
{
    int p1_points = 0;
    for(int i=0; i < snake_length; i++)
    {
        if (snake_pos[i][0] == ai_snake_pos[0][0] &&
                snake_pos[i][1] == ai_snake_pos[0][1]) {
            gameover = gameover + 1;
            printf("\n");
            printf("-----------------------------\n");
            printf("player 1 killed AI snake!\n");
            printf("Player1 wins!!!!\n");
            printf("Player 1 has %d points\n", ++p1_points);
            printf("-----------------------------\n");
            return gameover;
        }
    }
    return gameover;
}

int player_crosses_ai(int ai_length, int ai_snake_pos[][2], 
                      int snake_pos[][2], int gameover)
{
    int p1_points = 0;
    for(int i=0; i < ai_length; i++)
    {
        if (ai_snake_pos[i][0] == snake_pos[0][0] &&
                ai_snake_pos[i][1] == snake_pos[0][1]) {
            gameover = gameover + 1;
            printf("\n");
            printf("-----------------------------\n");
            printf("COMPUTER killed PLAYER 1!\n");
            printf("COMPUTER WINS!!!!\n");
            printf("COMPUTER has %d points\n", ++p1_points);
            printf("-----------------------------\n");
            return gameover;
        }
    }
    return gameover;
}

void ai_long_length (int ai_length, int ai_old_pos[], 
                     int ai_new_pos[], int ai_snake_pos[][2])
{
    for (int i=1; i < ai_length; i++)
        {
            ai_old_pos[0] = ai_snake_pos[i][0];
            ai_old_pos[1] = ai_snake_pos[i][1];
            if (ai_snake_pos[i][0] == ai_new_pos[0] &&
                    ai_snake_pos[i][1] == ai_new_pos[1])
                break;
            ai_snake_pos[i][0] = ai_new_pos[0];
            ai_snake_pos[i][1] = ai_new_pos[1];
            ai_new_pos[0] = ai_old_pos[0];
            ai_new_pos[1] = ai_old_pos[1];
        }
    return;
}

