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

    IntroButtons[nbuttons].r.width = 140;
    g.button[g.nbuttons].r.height = 60;
    g.button[g.nbuttons].r.left = 20;
    g.button[g.nbuttons].r.bot = 320;
    g.button[g.nbuttons].r.right =
        g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
    g.button[g.nbuttons].r.top =
        g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.height;
    g.button[g.nbuttons].r.centerx =
        (g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.right) / 2;
    g.button[g.nbuttons].r.centery =
        (g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.top) / 2;
    strcpy(g.button[g.nbuttons].text, "r to Reset");
    g.button[g.nbuttons].down = 0;
    g.button[g.nbuttons].click = 0;
    g.button[g.nbuttons].color[0] = 0.4f;
    g.button[g.nbuttons].color[1] = 0.4f;
    g.button[g.nbuttons].color[2] = 0.7f;
    g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
    g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
    g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
    g.button[g.nbuttons].text_color = 0x00ffffff;
    g.nbuttons++;

}

