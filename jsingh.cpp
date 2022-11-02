// Jasdeep Singh
// Additional code for Lab4
// Double Snake

#include <stdio.h>
#include <cstring>
#include <GL/glx.h>
#include "fonts.h"
#define MAXBUTTONS 6


typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} Button;

int name5()
{
    printf("Jasdeep \n");
    return 0;
}

void showIntro(int xres, int yres)
{
    Rect r;
    // Background
    //static float w = 100.0f;
    static float pos[2] = {0.0f+100.0f, yres/2.0f};
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

    // Buttons
    int nbuttons = 0;
    Button button[MAXBUTTONS];
    button[nbuttons].r.width = 140;
    button[nbuttons].r.height = 60;
    button[nbuttons].r.left = xres/2;
    button[nbuttons].r.bot = yres/2;
    button[nbuttons].r.right =
	button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top =
	button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx =
	(button[nbuttons].r.left + button[nbuttons].r.right)/2;
    button[nbuttons].r.centery =
	(button[nbuttons].r.bot + button[nbuttons].r.top)/2;
    strcpy(button[nbuttons].text, "Play");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.3f;
    button[nbuttons].color[1] = 0.3f;
    button[nbuttons].color[2] = 0.5f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
    r.left   = xres/2;
    r.bot    = yres-100;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffaaee, "Jasdeep's Feature Mode (button testing)");
    
    if (button[0].over) {
	int w=3;
	glColor3f(0.5f, 0.5f, 0.5f);
	//draw a highlight around button
	glLineWidth(3);
	glBegin(GL_LINE_LOOP);
	glVertex2i(button[0].r.left-w,  button[0].r.bot-w);
	glVertex2i(button[0].r.left-w,  button[0].r.top+w);
	glVertex2i(button[0].r.right+w, button[0].r.top+w);
	glVertex2i(button[0].r.right+w, button[0].r.bot-w);
	glVertex2i(button[0].r.left-w,  button[0].r.bot-w);
	glEnd();
	glLineWidth(1);
    }
    if (button[0].down) {
	glColor3fv(button[0].dcolor);
    } else {
	glColor3fv(button[0].color);
    }
    glBegin(GL_QUADS);
    glVertex2i(button[0].r.left,  button[0].r.bot);
    glVertex2i(button[0].r.left,  button[0].r.top);
    glVertex2i(button[0].r.right, button[0].r.top);
    glVertex2i(button[0].r.right, button[0].r.bot);
    glEnd();
    r.left = button[0].r.centerx;
    r.bot  = button[0].r.centery-8;
    r.center = 1;
    if (button[0].down) {
	ggprint16(&r, 0, button[0].text_color, "Play Clicked");
    } else {
	ggprint16(&r, 0, button[0].text_color, button[0].text);
    }

}

void featureJas(int xres, int yres)
{
    showIntro(xres, yres);
}

