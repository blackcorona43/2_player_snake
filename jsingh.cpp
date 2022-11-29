// Jasdeep Singh
// Additional code for Lab4
// Double Snake

#include <stdio.h>
#include <cstring>
#include <GL/glx.h>
#include "fonts.h"
#define MAXBUTTONS 10

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

struct Global {
    Button button[MAXBUTTONS];
    int xres, yres;
    //int nbuttons;
} j;

Rect r;

void setInit();
void displayIntroText();

void setGlobalJ(int xres, int yres)
{
    j.xres = xres;
    j.yres = yres;
}

void showIntro()
{
    setInit();
}

void setInit() 
{

    // Background
    static float pos[2] = {0.0f+100.0f, j.yres/2.0f};
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor3ub(110,86,68);
    glTranslatef(pos[0], pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-j.xres, -j.yres);
    glVertex2f(-j.xres,  j.yres);
    glVertex2f( j.xres,  j.yres);
    glVertex2f( j.xres, -j.yres);
    glEnd();
    glPopMatrix();
}

void createButtons(Button *btn, int *nbuttons)
{
    btn->r.width = 180;
    btn->r.height = 60;
    btn->r.left = 20;
    btn->r.bot = 100;
    btn->r.right =
	btn->r.left + btn->r.width;
    btn->r.top =
	btn->r.bot + btn->r.height;
    btn->r.centerx =
	(btn->r.left + btn->r.right)/2;
    btn->r.centery =
	(btn->r.bot + btn->r.top)/2;
    strcpy(btn->text, "Return to Menu");
    btn->down = 0;
    btn->click = 0;
    btn->color[0] = 0.4f;
    btn->color[1] = 0.2f;
    btn->color[2] = 0.1f;
    btn->dcolor[0] = btn->color[0] * 0.5f;
    btn->dcolor[1] = btn->color[1] * 0.5f;
    btn->dcolor[2] = btn->color[2] * 0.5f;
    btn->text_color = 0x00ffffff;
    (*nbuttons)++;
    (btn+1)->r.width = 140;
    (btn+1)->r.height = 60;
    (btn+1)->r.left = j.xres/2-((btn+1)->r.width/2);
    (btn+1)->r.bot = j.yres/2-((btn+1)->r.height)+100;
    (btn+1)->r.right =
	(btn+1)->r.left + (btn+1)->r.width;
    (btn+1)->r.top =
	(btn+1)->r.bot + (btn+1)->r.height;
    (btn+1)->r.centerx =
	((btn+1)->r.left + (btn+1)->r.right)/2;
    (btn+1)->r.centery =
	((btn+1)->r.bot + (btn+1)->r.top)/2-8;
    strcpy((btn+1)->text, "Play");
    (btn+1)->down = 0;
    (btn+1)->click = 0;
    (btn+1)->color[0] = 0.4f;
    (btn+1)->color[1] = 0.2f;
    (btn+1)->color[2] = 0.1f;
    (btn+1)->dcolor[0] = (btn+1)->color[0] * 0.5f;
    (btn+1)->dcolor[1] = (btn+1)->color[1] * 0.5f;
    (btn+1)->dcolor[2] = (btn+1)->color[2] * 0.5f;
    (btn+1)->text_color = 0x00ffffff;
    (*nbuttons)++;
    (btn+2)->r.width = 200;
    (btn+2)->r.height = 60;
    (btn+2)->r.left = j.xres/2-((btn+2)->r.width/2);
    (btn+2)->r.bot = j.yres/2-((btn+2)->r.height);
    (btn+2)->r.right =
	(btn+2)->r.left + (btn+2)->r.width;
    (btn+2)->r.top =
	(btn+2)->r.bot + (btn+2)->r.height;
    (btn+2)->r.centerx =
	((btn+2)->r.left + (btn+2)->r.right)/2;
    (btn+2)->r.centery =
	((btn+2)->r.bot + (btn+2)->r.top)/2-8;
    strcpy((btn+2)->text, "Two Players");
    (btn+2)->down = 0;
    (btn+2)->click = 0;
    (btn+2)->color[0] = 0.4f;
    (btn+2)->color[1] = 0.2f;
    (btn+2)->color[2] = 0.1f;
    (btn+2)->dcolor[0] = (btn+2)->color[0] * 0.5f;
    (btn+2)->dcolor[1] = (btn+2)->color[1] * 0.5f;
    (btn+2)->dcolor[2] = (btn+2)->color[2] * 0.5f;
    (btn+2)->text_color = 0x00ffffff;
    (*nbuttons)++;
    (btn+3)->r.width = 180;
    (btn+3)->r.height = 60;
    (btn+3)->r.left = j.xres/2-((btn+3)->r.width/2);
    (btn+3)->r.bot = j.yres/2-((btn+3)->r.height)-100;
    (btn+3)->r.right =
	(btn+3)->r.left + (btn+3)->r.width;
    (btn+3)->r.top =
	(btn+3)->r.bot + (btn+3)->r.height;
    (btn+3)->r.centerx =
	((btn+3)->r.left + (btn+3)->r.right)/2;
    (btn+3)->r.centery =
	((btn+3)->r.bot + (btn+3)->r.top)/2-8;
    strcpy((btn+3)->text, "Versus AI");
    (btn+3)->down = 0;
    (btn+3)->click = 0;
    (btn+3)->color[0] = 0.4f;
    (btn+3)->color[1] = 0.2f;
    (btn+3)->color[2] = 0.1f;
    (btn+3)->dcolor[0] = (btn+3)->color[0] * 0.5f;
    (btn+3)->dcolor[1] = (btn+3)->color[1] * 0.5f;
    (btn+3)->dcolor[2] = (btn+3)->color[2] * 0.5f;
    (btn+3)->text_color = 0x00ffffff;
    (*nbuttons)++;
    (btn+4)->r.width = 170;
    (btn+4)->r.height = 60;
    (btn+4)->r.left = j.xres/2-((btn+4)->r.width/2);
    (btn+4)->r.bot = j.yres/2-((btn+4)->r.height)-200;
    (btn+4)->r.right =
	(btn+4)->r.left + (btn+4)->r.width;
    (btn+4)->r.top =
	(btn+4)->r.bot + (btn+4)->r.height;
    (btn+4)->r.centerx =
	((btn+4)->r.left + (btn+4)->r.right)/2;
    (btn+4)->r.centery =
	((btn+4)->r.bot + (btn+4)->r.top)/2-8;
    strcpy((btn+4)->text, "Settings");
    (btn+4)->down = 0;
    (btn+4)->click = 0;
    (btn+4)->color[0] = 0.4f;
    (btn+4)->color[1] = 0.2f;
    (btn+4)->color[2] = 0.1f;
    (btn+4)->dcolor[0] = (btn+4)->color[0] * 0.5f;
    (btn+4)->dcolor[1] = (btn+4)->color[1] * 0.5f;
    (btn+4)->dcolor[2] = (btn+4)->color[2] * 0.5f;
    (btn+4)->text_color = 0x00ffffff;
    (*nbuttons)++;
    (btn+5)->r.width = 120;
    (btn+5)->r.height = 60;
    (btn+5)->r.left = j.xres/2-((btn+5)->r.width/2);
    (btn+5)->r.bot = j.yres/2-((btn+5)->r.height)-300;
    (btn+5)->r.right =
	(btn+5)->r.left + (btn+5)->r.width;
    (btn+5)->r.top =
	(btn+5)->r.bot + (btn+5)->r.height;
    (btn+5)->r.centerx =
	((btn+5)->r.left + (btn+5)->r.right)/2;
    (btn+5)->r.centery =
	((btn+5)->r.bot + (btn+5)->r.top)/2-8;
    strcpy((btn+5)->text, "Quit");
    (btn+5)->down = 0;
    (btn+5)->click = 0;
    (btn+5)->color[0] = 0.4f;
    (btn+5)->color[1] = 0.2f;
    (btn+5)->color[2] = 0.1f;
    (btn+5)->dcolor[0] = (btn+5)->color[0] * 0.5f;
    (btn+5)->dcolor[1] = (btn+5)->color[1] * 0.5f;
    (btn+5)->dcolor[2] = (btn+5)->color[2] * 0.5f;
    (btn+5)->text_color = 0x00ffffff;
    (*nbuttons)++;
    (btn+6)->r.width = 180;
    (btn+6)->r.height = 60;
    (btn+6)->r.left = j.xres/2-((btn+6)->r.width/2);
    (btn+6)->r.bot = j.yres/2-((btn+6)->r.height);
    (btn+6)->r.right =
	(btn+6)->r.left + (btn+6)->r.width;
    (btn+6)->r.top =
	(btn+6)->r.bot + (btn+6)->r.height;
    (btn+6)->r.centerx =
	((btn+6)->r.left + (btn+6)->r.right)/2;
    (btn+6)->r.centery =
	((btn+6)->r.bot + (btn+6)->r.top)/2-8;
    strcpy((btn+6)->text, "Full Screen");
    (btn+6)->down = 0;
    (btn+6)->click = 0;
    (btn+6)->color[0] = 0.4f;
    (btn+6)->color[1] = 0.2f;
    (btn+6)->color[2] = 0.1f;
    (btn+6)->dcolor[0] = (btn+6)->color[0] * 0.5f;
    (btn+6)->dcolor[1] = (btn+6)->color[1] * 0.5f;
    (btn+6)->dcolor[2] = (btn+6)->color[2] * 0.5f;
    (btn+6)->text_color = 0x00ffffff;
    (*nbuttons)++;
} 

void drawButtons(Button *btn)
{
    for (int i = 3; i<=7; i++) { 
        if (btn[i].over) {
            int w=3;
            glColor3f(1.0f, 1.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
            glVertex2i(btn[i].r.left-w,  btn[i].r.bot-w);
            glVertex2i(btn[i].r.left-w,  btn[i].r.top+w);
            glVertex2i(btn[i].r.right+w, btn[i].r.top+w);
            glVertex2i(btn[i].r.right+w, btn[i].r.bot-w);
            glVertex2i(btn[i].r.left-w,  btn[i].r.bot-w);
            glEnd();
            glLineWidth(1);
        }
        if (btn[i].down) {
            glColor3fv(btn[i].dcolor);
        } else {
            glColor3fv(btn[i].color);
        }
        glBegin(GL_QUADS);
        glVertex2i(btn[i].r.left,  btn[i].r.bot);
        glVertex2i(btn[i].r.left,  btn[i].r.top);
        glVertex2i(btn[i].r.right, btn[i].r.top);
        glVertex2i(btn[i].r.right, btn[i].r.bot);
        glEnd();
        r.left = btn[i].r.centerx;
        r.bot  = btn[i].r.centery;
        r.center = 1;
        if (btn[i].down) {
            ggprint16(&r, 0, btn[i].text_color, "Button Clicked");
        } else {
            ggprint16(&r, 0, btn[i].text_color, btn[i].text);
        }
    }
}

void displayIntroText()
{
    Rect h;
    h.left = j.xres/2;
    h.bot = j.yres/2+300;
    h.center = 1;
    ggprint16(&h, 16, 0x00ffffff, "HUNGRY HUNGRY SNAKE");
    h.left = 50;
    h.bot = 10;
    h.center = 1;
    ggprint16(&h, 16, 0x00ffffff, "F1 for help");
}

void gameSettings(Button *btn)
{
    Rect h;
    h.left = j.xres/2;
    h.bot = j.yres/2+300;
    h.center = 1;
    ggprint16(&h, 16, 0x00ffffff, "Settings");

    if (btn[8].over) {
        int w=3;
        glColor3f(1.0f, 0.1f, 0.1f);
        //draw a highlight around button
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        glVertex2i(btn[8].r.left-w,  btn[8].r.bot-w);
        glVertex2i(btn[8].r.left-w,  btn[8].r.top+w);
        glVertex2i(btn[8].r.right+w, btn[8].r.top+w);
        glVertex2i(btn[8].r.right+w, btn[8].r.bot-w);
        glVertex2i(btn[8].r.left-w,  btn[8].r.bot-w);
        glEnd();
        glLineWidth(1);
    }
    if (btn[8].down) {
        glColor3fv(btn[8].dcolor);
    } else {
        glColor3fv(btn[8].color);
    }
    glBegin(GL_QUADS);
    glVertex2i(btn[8].r.left,  btn[8].r.bot);
    glVertex2i(btn[8].r.left,  btn[8].r.top);
    glVertex2i(btn[8].r.right, btn[8].r.top);
    glVertex2i(btn[8].r.right, btn[8].r.bot);
    glEnd();
    r.left = btn[8].r.centerx;
    r.bot  = btn[8].r.centery;
    r.center = 1;
    if (btn[8].down) {
        ggprint16(&r, 0, btn[8].text_color, "Button Clicked");
    } else {
        ggprint16(&r, 0, btn[8].text_color, btn[8].text);
    }
}

