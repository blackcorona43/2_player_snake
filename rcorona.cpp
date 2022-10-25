//Ruben Corona
// AI player snake code
// 
#include <stdio.h>
#include <string>
#define MAX_GRID 80
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#define DIRECTION_DOWN   0
#define DIRECTION_LEFT   1
#define DIRECTION_UP     2
#define DIRECTION_RIGHT  3


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
        gameover = 1;
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
            gameover = 1;
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
        gameover = 1;
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
    int p2_points = 0;
    for(int i=0; i < snake_length; i++)
    {
        if (snake_pos[i][0] == ai_snake_pos[0][0] &&
                snake_pos[i][1] == ai_snake_pos[0][1]) {
            gameover = 1;
            printf("\n");
            printf("-----------------------------\n");
            printf("player 1 killed AI snake!\n");
            printf("Player1 wins!!!!\n");
            printf("Player 1 has %d points\n", ++p2_points);
            printf("-----------------------------\n");
            return gameover;
        }
    }
    return gameover;
}

//Does the logic for checking
//if player crosses AI
int player_crosses_ai(int ai_length, int ai_snake_pos[][2], 
                      int snake_pos[][2], int gameover)
{
    int p1_points = 0;
    for(int i=0; i < ai_length; i++)
    {
        if (ai_snake_pos[i][0] == snake_pos[0][0] &&
                ai_snake_pos[i][1] == snake_pos[0][1]) {
            gameover = 1;
            printf("\n");
            printf("Gameover = %d", gameover);
            printf("-----------------------------\n");
            printf("Computer killed PLayer1!\n");
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

int show_gameover(int xres, int yres)
{
    Rect r;
    r.left = xres/2;
    r.bot  = yres/2-25;
    r.center = 1;
    int w = 0;
    static float pos[2] = {0.0f+w, yres/2.0f};
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor3ub(217,70,40);
    glTranslatef(pos[0], pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-xres,-yres);
    glVertex2f(-xres, yres);
    glVertex2f(xres,yres);
    glVertex2f(xres,-yres);
    glEnd();
    ggprint16(&r, 16, 0x00ffffff, "GAME OVER");
    ggprint16(&r, 16, 0x00ffffff, "Press R to Restart");
    ggprint16(&r, 16, 0x00ffffff, "Press Esc to Quit");
    return 0;
}

int ruben_mode(int xres, int yres)
{  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor3f(1.0,1.0,0.0);
    glColor4f(0.0,1.0,0.0,0.5);
    int w = 20;
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(0,0);
    glVertex2f(0 + w, w);

    glVertex2f(0, yres);
    glVertex2f(0 + w, yres - w);

    glVertex2f(xres, yres);
    glVertex2f(xres - w, yres - w);
    
    glVertex2f(xres, 0);
    glVertex2f(xres - w, w);

    glVertex2f(0, 0);
    glVertex2f(0 + w, w);
    glEnd();
    glDisable(GL_BLEND);
    return 0;

}






