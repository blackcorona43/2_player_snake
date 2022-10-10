// Jasdeep Singh
// Additional code for Lab4
// Double Snake
//
#include <stdio.h>
#include <GL/glx.h>
int name5()
{
    printf("Jasdeep \n");
    return 0;
}

void showIntro(int xres, int yres)
{
    static float w = 100.0f;
    static float pos[2] = {0.0f+w, yres/2.0f};
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor3ub(150,150,150);
    glTranslatef(pos[0], pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-xres, -yres);
    glVertex2f(-xres,  yres);
    glVertex2f( xres,  yres);
    glVertex2f( xres, -yres);
    glEnd();
    glPopMatrix();
}
