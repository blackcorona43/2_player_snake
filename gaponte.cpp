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
    ggprint16(&r, 16, 0x00ffffff, "2 player mode: press Q");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Player vs Com: press v");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Player 1 Keys: W, A, S, D");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Player 2 Keys: UP, LEFT, DOWN, RIGHT");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "p to Pause the game     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "c to show the Credits Screen     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "r to reset the game     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "F1 to show the Help Screen    ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    ggprint16(&r, 16, 0x00ffffff, "Esc to quit the game     ");
    ggprint16(&r, 16, 0x00ffffff, "     ");
    glPopMatrix();
    return 0;
}
int show_power_up(int cent[])
{
    //draw power up
        glColor3f(255, 255, 0);
        glBegin(GL_QUADS);
        glVertex2i(cent[0]-4, cent[1]-3);
        glVertex2i(cent[0]-4, cent[1]+4);
        glVertex2i(cent[0]+3, cent[1]+4);
        glVertex2i(cent[0]+3, cent[1]-3);
        glEnd();

    return 0;
}


