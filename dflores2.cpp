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
