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

extern unsigned int gameover_screen(int gameover);
extern unsigned int computer_snake(int check);
extern unsigned int two_player(int check);
extern void ai_direction (int direction, int position [][2]);

extern int check_off_board (int ai_snake_pos [][2], int grid, 
                            int p1_points, int gameover);

extern int ai_cross_itself (int ai_length, int ai_snake_pos[][2], 
                            int p1_points, int gameover);

extern int ai_head_collision (int ai_snake_pos[][2], int snake_pos[][2], int gameover);

extern int ai_crosses_player (int snake_length, int ai_snake_pos[][2],
                              int snake_pos[][2],int gameover);

extern int player_crosses_ai(int ai_length, int ai_pos[][2], 
                             int snake_pos[0][2], int gameover);

extern void ai_long_length (int ai_length, int ai_old_pos[],
        int ai_new_pos[], int ai_snake_pos[][2]);
extern int show_gameover(int xres, int yres);
extern int ruben_mode(int xres, int yres);
