//Samantha Tellez
//my new source file
//no main!
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
/*#include <stdlib.h>
#include <string.h>
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

class MyImage {
    public:
	int width, height;
	unsigned char *data;
	~MyImage(); //destructor
	MyImage(const char *fname);
};

//extern class MyImage myimage;

MyImage::~MyImage() { delete [] data; }

MyImage::MyImage(const char *fname) {
    //if (fname[0] == '\0')
    if (strlen(fname) == 0)
	return;
    //apply constants later
    //char name[FILENAME_MAX];
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    name[slen-4] = '\0';
    char ppmname[80];
    sprintf(ppmname,"%s.ppm", name);
    char ts[100];
    sprintf(ts, "convert %s %s", fname, ppmname);
    system(ts);
    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
	char line[200];
	fgets(line, 200, fpi);
	fgets(line, 200, fpi);
	//skip comments and blank lines
	while (line[0] == '#' || strlen(line) < 2)
	    fgets(line, 200, fpi);
	sscanf(line, "%i %i", &width, &height);
	fgets(line, 200, fpi);
	//get pixel data
	int n = width * height * 3;
	data = new unsigned char[n];
	for (int i=0; i<n; i++)
	    data[i] = fgetc(fpi);
	fclose(fpi);
    } else {
	printf("ERROR opening image: %s\n", ppmname);
	exit(0);
    }
    unlink(ppmname);
}
class MyImage myimage = {"images/allsnake.jpg"};
MyImage imgg[1] = {"images/allsnake.jpg"};

class Texture {
    public:
        //change Image to MyImage class
        //Image *backImage;
        MyImage *backImage;
        GLuint backTexture;
        float xc[2];
        float yc[2];
};

class Globe {
    public:
        int xres, yres;
        Texture tex;
        //Texture tex2;
        Globe() {
            xres=640, yres=480;
            //xres=950, yres=690;
        }
} gg;

unsigned int manage_state_st(unsigned int s)
{
    //manages credit to turn off/on
    s = s ^ 1;
    return s;
}

void show_credits(int xres, int yres)
{
    /*Rect r;
    int w = 10;
    //static float w = 400.0f;
    static float pos[2] = {0.0f+w, yres/2.0f};
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor3ub(217,70,40);
    //glColor3ub(150,160,220);
    glTranslatef(pos[0], pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(xres,-yres);
    glVertex2f(xres, yres);
    glVertex2f(xres, yres);
    glVertex2f(xres,-yres);
    glEnd();
    glPopMatrix();

    r.left   = xres/2;
    //r.left   = xres/2;
    //r.bot    = yres-100;
    //r.bot    = yres-300; //centered
    r.bot    = yres-150;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "--Credits--");
    ggprint16(&r, 16, 0x00ffffff, " ");
    ggprint16(&r, 16, 0x00ffffff, "Project Made By:");
    ggprint16(&r, 16, 0x00ffffff, " ");
    ggprint16(&r, 16, 0x00ffffff, "Gabriel Aponte, Dominic Flores");
    ggprint16(&r, 16, 0x00ffffff, " ");
    ggprint16(&r, 16, 0x00ffffff, "Ruben Corona, Samantha Tellez, and Jasdeep Singh");*/

    //
    //credits pic
    int wid;
    int hei;

    //init_opengl
    //
    //this is the main background texture map
    gg.tex.backImage = &imgg[0];
    //create opengl texture elements
    glGenTextures(1, &gg.tex.backTexture);
    wid = gg.tex.backImage->width;
    hei = gg.tex.backImage->height;
    glBindTexture(GL_TEXTURE_2D, gg.tex.backTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, wid, hei, 0,
            GL_RGB, GL_UNSIGNED_BYTE, gg.tex.backImage->data);
    gg.tex.xc[0] = 0.0;
    //gg.tex.xc[1] = 0.25;
    gg.tex.xc[1] = 1.0;
    gg.tex.yc[0] = 0.0;
    gg.tex.yc[1] = 1.0;

    //render section:
    //this is our main background picture
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gg.tex.backTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(gg.tex.xc[0], gg.tex.yc[1]); glVertex2i(0, 0);
    glTexCoord2f(gg.tex.xc[0], gg.tex.yc[0]); glVertex2i(0, yres);
    glTexCoord2f(gg.tex.xc[1], gg.tex.yc[0]); glVertex2i(xres, yres);
    glTexCoord2f(gg.tex.xc[1], gg.tex.yc[1]); glVertex2i(xres, 0);
    //glTexCoord2f(gg.tex.xc[0], gg.tex.yc[0]); glVertex2i(0,      gg.yres);
    //glTexCoord2f(gg.tex.xc[1], gg.tex.yc[0]); glVertex2i(gg.xres, gg.yres);
    //glTexCoord2f(gg.tex.xc[1], gg.tex.yc[1]); glVertex2i(gg.xres, 0);
    glEnd();

}

//Feature Mode
void show_feature_st(int xres, int yres)
{
    //text for feature mode
    Rect r;
    r.left   = xres/2;
    r.bot    = yres-150;
    r.center = 1;
    ggprint16(&r, 16, 0x00df9fdf, "SAMANTHA'S FEATURE MODE");
    
    //border
    //
    //
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //transparent
    glEnable(GL_BLEND);
    //glColor3f(1.0, 1.0, 0.0);
    glColor4f(0.6, 0.5, 1.5, 0.6); //red, green, blue, alpha
    int w = 25;
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2i(0, 0);
    glVertex2i(w, w);

    glVertex2i(0, yres);
    glVertex2i(w, yres-w);

    glVertex2i(xres, yres);
    glVertex2i(xres-w, yres-w);

    glVertex2i(xres, 0);
    glVertex2i(xres-w, w);

    glVertex2i(0, 0);
    glVertex2i(w, w);

    glEnd();
    glDisable(GL_BLEND);

    //feature part
    //
    //
    //Box 1 (middle)
    //		From struct Global
    int boardDim; 
    int gridDim=15; //size of square
    //		From init()
    boardDim = gridDim*32;
    //		From render
    int s0 = xres>>1;
    int s1 = yres>>1;
    int b2 = boardDim/2;
    //		From asteroids game
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //transparent
    glEnable(GL_BLEND);
    glColor4f(1.6, 0.5, 1.5, 1.0); //red, green, blue, alpha
    glBegin(GL_QUADS);
    //		From render
    glVertex2i(s0-b2, s1-b2);
    glVertex2i(s0-b2, s1+b2);
    glVertex2i(s0+b2, s1+b2);
    glVertex2i(s0+b2, s1-b2);
    glEnd();
    //
    //Box 2 (lower left)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_BLEND);
    glColor4f(2.6, 0.5, 0.5, 1.0); 
    int s0_2 = xres>>2;
    int s1_2 = yres>>2;
    glBegin(GL_QUADS);
    glVertex2i(s0_2-b2, s1_2-b2);
    glVertex2i(s0_2-b2, s1_2+b2);
    glVertex2i(s0_2+b2, s1_2+b2);
    glVertex2i(s0_2+b2, s1_2-b2);
    glEnd();
    //
    //Box 3 (mid bottom)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_BLEND);
    glColor4f(1.0, 2.5, 0.5, 1.0); 
    int s0_3 = xres>>1;
    int s1_3 = yres>>3;
    glBegin(GL_QUADS);
    glVertex2i(s0_3-b2, s1_3-b2);
    glVertex2i(s0_3-b2, s1_3+b2);
    glVertex2i(s0_3+b2, s1_3+b2);
    glVertex2i(s0_3+b2, s1_3-b2);
    glEnd();
    //
    //Box 4 (blue square)
    int boardDim_2;
    int gridDim_2=25; 
    boardDim_2 = gridDim_2*32;
    int s0_4 = xres>>2;
    int s1_4 = yres>>1;
    int b2_2 = boardDim_2/2;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_BLEND);
    glColor4f(0.5, 0.5, 3.5, 1.0); 
    glBegin(GL_QUADS);
    glVertex2i(s0_4-b2_2, s1_4-b2_2);
    glVertex2i(s0_4-b2_2, s1_4+b2_2);
    glVertex2i(s0_4+b2_2, s1_4+b2_2);
    glVertex2i(s0_4+b2_2, s1_4-b2_2);
    glEnd();
    //No: 3x3, 3x2, 3x1, 1x4, 4x1, 4x2, 5x1, 0x2
    //Maybe: 2x1

}

