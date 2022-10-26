// Dominic Flores
// my new source file
//
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"

void show_pause_screen(int xres, int yres)
{	
    	Rect r;
        int x = xres / 2;
        int y = yres / 2;
        int w = 0;
	r.left = x;
	r.bot = y;
	r.center = 1;
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
        glVertex2f(x-w, y-w);
        glVertex2f(x-w, y+w);
        glVertex2f(x+w, y+w);
        glVertex2f(x+w, y-w);
        glEnd();
	ggprint16(&r, 16, 0x00ffffff, "Paused");
}

void game_Texture(GLuint texture, int center[], int s, int dir) {
        //glColor3f(0.5, 0.5, 0.5);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);

        switch (dir) {
        case 0:
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(center[0] - s, center[1] - s);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(center[0] - s, center[1] + s);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i(center[0] + s, center[1] + s);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i(center[0] + s, center[1] - s);
            break;

        case 1:
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i(center[0] - s, center[1] - s);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(center[0] - s, center[1] + s);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(center[0] + s, center[1] + s);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i(center[0] + s, center[1] - s);
            break;

        case 2:
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i(center[0] - s, center[1] - s);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i(center[0] - s, center[1] + s);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(center[0] + s, center[1] + s);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(center[0] + s, center[1] - s);
            break;

        case 3:
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(center[0] - s, center[1] - s);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i(center[0] - s, center[1] + s);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i(center[0] + s, center[1] + s);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(center[0] + s, center[1] - s);
            break;
    }
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
}

