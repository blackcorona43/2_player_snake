// Dominic Flores
// my new source file

#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <cstdlib>

#define MAX_GRID 80
#define DIRECTION_DOWN   0
#define DIRECTION_LEFT   1
#define DIRECTION_UP     2
#define DIRECTION_RIGHT  3

typedef struct Snake2
{
    int status;
    int length;
    int pos[MAX_GRID*MAX_GRID][2];
    int direction;
    double timer;
    double delay;
}snake2;

void snake_direction(int dir, int pos[][2])
{
    switch (dir) {
	case DIRECTION_DOWN:  pos[0][1] += 1; break;
	case DIRECTION_LEFT:  pos[0][0] -= 1; break;
	case DIRECTION_UP:    pos[0][1] -= 1; break;
	case DIRECTION_RIGHT: pos[0][0] += 1; break;
    }
}

int snake_hits_edge(int pos[][2], int gridDim)
{
    //check for snake2 off board...
    if (pos[0][0] < 0 ||
	    pos[0][0] > gridDim-1 ||
	    pos[0][1] < 0 ||
	    pos[0][1] > gridDim-1) {
	return 1;
    }
    return 0;
}

int snake_eats_itself(int length, int pos[][2])
{
    for (int i=1; i<length; i++) {
	if (pos[i][0] == pos[0][0] &&
		pos[i][1] == pos[0][1]) {
	    return 1;
	}
    }
    return 0;
}

int player_crosses_player(int length, int pos1[][2], int pos2[][2])
{
    for (int i=0; i<length; i++) {
	if (pos1[i][0] == pos2[0][0] &&
		pos1[i][1] == pos2[0][1]) {
	    return 1;
	}
    }
    return 0;

}


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
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
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

//Creates Sprite Image
unsigned char *buildAlphaData(unsigned char* pdata, int height, int width)
{
    int i;
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = pdata;
    newdata = (unsigned char *)malloc(width * height * 4);
    ptr = newdata;
    for (i=0; i<width * height * 3; i+=3) {
	a = *(data+0);
	b = *(data+1);
	c = *(data+2);
	*(ptr+0) = a;
	*(ptr+1) = b;
	*(ptr+2) = c;
	//-----------------------------------------------
	//get largest color component...
	//*(ptr+3) = (unsigned char)((
	//      (int)*(ptr+0) +
	//      (int)*(ptr+1) +
	//      (int)*(ptr+2)) / 3);
	//d = a;
	//if (b >= a && b >= c) d = b;
	//if (c >= a && c >= b) d = c;
	//*(ptr+3) = d;
	//-----------------------------------------------
	//this code optimizes the commented code above.
	//code contributed by student: Chris Smith
	//
	*(ptr+3) = (a|b|c);
	//-----------------------------------------------
	ptr += 4;
	data += 3;
    }
    return newdata;
}


