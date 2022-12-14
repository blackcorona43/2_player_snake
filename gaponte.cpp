//Gabriel Aponte
//new source code
//
#include <stdio.h>
#include <GL/glx.h>
#include "fonts.h"

int my_name()
{
    printf("Gabriel \n");
    return 0;
}

int help_screen(int xres, int yres)
{
    Rect r;
    r.left   = xres/2;
    r.bot    = yres/2 - 25;
    r.center = 1;
    int w = 0 ;
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
    ggprint16(&r, 16, 0x00ffffff, "--Help Screen--");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Player 1 Keys: W, A, S, D");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Player 2 Keys: UP, LEFT, DOWN, RIGHT");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Vs COmputer Press V");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "p to Pause the game     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "c to show the Credits Screen     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "l to see length power up     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "r to reset the game     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "n for Samantha's feature mode     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "F1 to show the Help Screen    ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Esc to quit the game     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "t for Dominic's feature mode     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    glPopMatrix();
    return 0;
}
int show_power_up_mode(int cent[], int xres,int yres)
{
    //draw power up
    glColor3f(255, 255, 0);
    glBegin(GL_QUADS);
    glVertex2i(cent[0]-4, cent[1]-3);
    glVertex2i(cent[0]-4, cent[1]+4);
    glVertex2i(cent[0]+3, cent[1]+4);
    glVertex2i(cent[0]+3, cent[1]-3);
    glEnd();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor3f(1.0,1.0,0.0);
    glColor4f(0.0,1.0,0.0,0.5);
    int w = 20;
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(0,0);
    glVertex2f(0 + w, w);

    glVertex2f(0,yres);
    glVertex2f(0 + w, yres - w);

    glVertex2f(xres, yres);
    glVertex2f(xres - w, yres - w);

    glVertex2f(xres, 0);
    glVertex2f(xres - w, w);

    glVertex2f(0,0);
    glVertex2f(0 + w, w);
    glEnd();
    glDisable(GL_BLEND);


    return 0;
}
int show_scores(int p1, int p2, int xres, int yres)
{   
    Rect r;
    r.left   = xres/2;
    r.bot    = yres-100;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "Hungry Hungry Snake");
    Rect h;
    h.left   = 50;
    h.bot    = 10;
    h.center = 1;
    ggprint16(&h, 16, 0x00ffffff, "F1 for help");
    h.left   = xres/2 - 100;
    h.bot    = 100;
    h.center = 1;
    ggprint16(&h, 16, 0xffff0000, "P1 Points: %i", p1);
    h.left   = xres/2 + 100;
    h.bot    = 100;
    h.center = 1;
    ggprint16(&h, 16, 0xf000f0, "P2 Points: %i", p2);
    return 0;

}
int leaderboard(int your_score, int high_score,int xres)
{
    //output leader board
    Rect h;
    h.left   = xres/2;
    h.bot    = 100;
    h.center = 1;
    ggprint16(&h, 16, 0x00ffffff, "----------LeaderBoard----------");
    ggprint16(&h, 16, 0x00ffffff, "     ");
    ggprint16(&h, 16, 0x00ffffff, "     ");
    if (your_score > high_score) {
        //printf("New High Score!!\n");
        ggprint16(&h, 16, 0x00ffffff, "New High Score!!");
        ggprint16(&h, 16, 0x00ffffff, "     ");
        ggprint16(&h, 16, 0x00ffffff, " Points: %i", your_score);
        ggprint16(&h, 16, 0x00ffffff, "     ");
        ggprint16(&h, 16, 0x00ffffff, " High Score was: %i", high_score);
    }
    else if (your_score == high_score) {
        //printf("High Score Tied!\n");
        ggprint16(&h, 16, 0x00ffffff, "High Score Tied!");
        ggprint16(&h, 16, 0x00ffffff, "     ");
        ggprint16(&h, 16, 0x00ffffff, "Points: %i", your_score);
        ggprint16(&h, 16, 0x00ffffff, "     ");
        ggprint16(&h, 16, 0x00ffffff, " High Score is: %i", high_score);
    }
    else if (your_score < high_score) {
        //printf("Better Luck Next Time...\n");
        ggprint16(&h, 16, 0x00ffffff, "Better Luck Next Time...");
        ggprint16(&h, 16, 0x00ffffff, "     ");
        ggprint16(&h, 16, 0x00ffffff , "Points: %i", your_score);
        ggprint16(&h, 16, 0x00ffffff, "     ");
        ggprint16(&h, 16, 0x00ffffff, " High Score is: %i", high_score);
    }

    return 0;
}


