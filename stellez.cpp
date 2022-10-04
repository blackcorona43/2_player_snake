//Samantha Tellez
//my new source file
//no main!
#include <stdio.h>
/*#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>*/
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"

int show_my_name()
{
    printf("samantha\n");
    return 0;
}

unsigned int manage_state_st(unsigned int s)
{
    //manages credit to turn off/on
    s = s ^ 1;
    return s;
}

void show_credits(int xres, int yres)
{
    Rect r;
    static float w = 400.0f;
    static float pos[2] = {0.0f+w, yres/2.0f};
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor3ub(150,160,220);
    glTranslatef(pos[0], pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(xres,-yres);
    glVertex2f(xres, yres);
    glVertex2f(xres, yres);
    glVertex2f(xres,-yres);
    glEnd();
    glPopMatrix();
    r.left   = xres/2;
    r.bot    = yres-100;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "--Credits--");


}

