//Samantha Tellez
//my new source file
//no main!

#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <time.h>
#include <GL/glx.h>
//#include <math.h>
//#include <random>
//#include <math.h>
//#include <cmath>
//#include <ctime>
#include "log.h"
#include "fonts.h"

int show_my_name()
{
    printf("samantha\n");
    return 0;
}

//=============================
//     For credits image
//=============================

class MyImage {
    public:
	int width, height;
	unsigned char *data;
	~MyImage(); //destructor
	MyImage(const char *fname);
};

MyImage::~MyImage() { delete [] data; }

MyImage::MyImage(const char *fname) {
    if (strlen(fname) == 0)
	return;
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
	MyImage *backImage;
	GLuint backTexture;
	float xc[2];
	float yc[2];
};

class Globe {
    public:
	int xres, yres;
	int movement;
	int movement2;
	Texture tex;
	Globe() {
	    xres = 2400;
	    yres = 1800;
	    movement = 1;
	    movement2 = 1;
	}
} gg;

//=================================
//     Stuff for Feature Mode
//=================================

//Notes:
//particles/movement = right
//particles2/movement2 = left

const int MAX_PARTICLES = 10000;

class Box {
    public:
	float w;
	float dir;
	float vel[2];
	float pos[2];
	Box() {
	    w = 0.0f; //makes box go straight down
	    dir = 25.0f;
	    pos[0] = gg.xres / 2.0f;
	    pos[1] = gg.yres / 2.0f;
	    vel[0] = vel[1] = 0.0;
	}
	Box(float wid, float d, float p0, float p1) {
	    w = wid;
	    dir = d;
	    pos[0] = p0;
	    pos[1] = p1;
	    vel[0] = vel[1] = 0.0;
	}
} box, particles[MAX_PARTICLES], particles2[MAX_PARTICLES]; 

int n = 0;

void make_particles(int x, int y)
{
    if (n >= MAX_PARTICLES)
	return;

    particles[n].w = 25.0; //size of particles
    particles[n].pos[0] = x;
    particles[n].pos[1] = y;
    particles[n].vel[0] = particles[n].vel[1] = 0.0;
    ++n;
}

void make_particles2(int x, int y)
{
    if (n >= MAX_PARTICLES)
	return;

    particles2[n].w = 25.0; //size of particles
    particles2[n].pos[0] = x;
    particles2[n].pos[1] = y;
    particles2[n].vel[0] = particles2[n].vel[1] = 0.0;
    ++n;
}

void physicsFeat()
{
    //particles1
    if (gg.movement < 500)
	gg.movement += 40; //how far apart the squares are
    if (gg.movement >= 500)
	gg.movement -= 500; //how wide the group of squares are

    make_particles(box.pos[0]+gg.movement, box.pos[1]*2);

    for (int i=0; i<n; i++)
    {
	particles[i].vel[1] -= 0.10; //speed of squares
	particles[i].pos[0] += particles[i].vel[0];
	particles[i].pos[1] += particles[i].vel[1];

	if (particles[i].pos[1] < 0.0)
	    particles[i] = particles[--n];
    }

    //particles2
    /*if (gg.movement2 < -10)
      	gg.movement2 += 40; //how far apart the squares are
      if (gg.movement2 >= -10)
      	gg.movement2 -= 500; //how wide the group of squares are

      make_particles(box.pos[0]+gg.movement2, box.pos[1]*2);
      for (int i=0; i<n; i++)
      {		   
      	particles2[i].vel[1] -= 0.10; //speed of squares
	particles2[i].pos[0] += particles2[i].vel[0];	
      	particles2[i].pos[1] += particles2[i].vel[1];	
     	if (particles2[i].pos[1] < 0.0)	
		particles2[i] = particles2[--n];
      }*/

}

void physicsFeat2()
{
    //particles2
    if (gg.movement2 < -10)
	gg.movement2 += 40; //how far apart the squares are
    if (gg.movement2 >= -10)
	gg.movement2 -= 500; //how wide the group of squares are

    make_particles2(box.pos[0]+gg.movement2, box.pos[1]*2);

    for (int i=0; i<n; i++)
    {
	particles2[i].vel[1] -= 0.10; //speed of squares
	particles2[i].pos[0] += particles2[i].vel[0];
	particles2[i].pos[1] += particles2[i].vel[1];

	if (particles2[i].pos[1] < 0.0)
	    particles2[i] = particles2[--n];
    }
}

void makeFeat()
{
    //Draw all particle
    /*for (int i=0; i<n; i++)
      {
      glPushMatrix();
      glColor3ub(150, 160, 255);
      glTranslatef(particles[i].pos[0], particles[i].pos[1], 0.0f);
      glBegin(GL_QUADS);
      glVertex2f(-particles[i].w, -particles[i].w);
      glVertex2f(-particles[i].w,  particles[i].w);
      glVertex2f( particles[i].w,  particles[i].w);
      glVertex2f( particles[i].w, -particles[i].w);
      glEnd();
      glTranslatef(particles2[i].pos[0], particles2[i].pos[1], 0.0f);
      glBegin(GL_QUADS);
      glVertex2f(-particles2[i].w, -particles2[i].w);
      glVertex2f(-particles2[i].w,  particles2[i].w);
      glVertex2f( particles2[i].w,  particles2[i].w);
      glVertex2f( particles2[i].w, -particles2[i].w);
      glEnd();
      glPopMatrix();
      }*/

    //Draw all particle2
    for (int i=0; i<n; i++)
    {
	glPushMatrix();
	glColor3ub(150, 160, 255);
	glTranslatef(particles[i].pos[0], particles[i].pos[1], 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(-particles[i].w, -particles[i].w);
	glVertex2f(-particles[i].w,  particles[i].w);
	glVertex2f( particles[i].w,  particles[i].w);
	glVertex2f( particles[i].w, -particles[i].w);
	glEnd();
	glPopMatrix();
    }
}

void makeFeat2()
{
    //Draw all particle2
    for (int i=0; i<n; i++)
    {
	glPushMatrix();
	glColor3ub(150, 160, 255);
	glTranslatef(particles2[i].pos[0], particles2[i].pos[1], 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(-particles2[i].w, -particles2[i].w);
	glVertex2f(-particles2[i].w,  particles2[i].w);
	glVertex2f( particles2[i].w,  particles2[i].w);
	glVertex2f( particles2[i].w, -particles2[i].w);
	glEnd();
	glPopMatrix();
    }
}

unsigned int manage_state_st(unsigned int s)
{
    //manages credit to turn off/on
    s = s ^ 1;
    return s;
}

void show_credits(int xres, int yres)
{
    //credits pic
    int wid;
    int hei;

    //init_opengl
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
    gg.tex.xc[1] = 1.0;
    gg.tex.yc[0] = 0.0;
    gg.tex.yc[1] = 1.0;

    //render section:
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gg.tex.backTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(gg.tex.xc[0], gg.tex.yc[1]); glVertex2i(0, 0);
    glTexCoord2f(gg.tex.xc[0], gg.tex.yc[0]); glVertex2i(0, yres);
    glTexCoord2f(gg.tex.xc[1], gg.tex.yc[0]); glVertex2i(xres, yres);
    glTexCoord2f(gg.tex.xc[1], gg.tex.yc[1]); glVertex2i(xres, 0);
    glEnd();

}

//=================================
//      Feature Mode Function
//=================================

void show_feature_st(int xres, int yres)
{
    //text for feature mode
    Rect r;
    r.left   = xres/2;
    r.bot    = yres-150;
    r.center = 1;
    ggprint16(&r, 16, 0x004da6ff, "SAMANTHA'S FEATURE MODE");

    //border
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //transparent
    glEnable(GL_BLEND);
    glColor4f(0.0, 0.0, 1.5, 0.6); //red, green, blue, alpha
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

    //feature mode part
    //
    //
    //

    //background color
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //transparent
    glEnable(GL_BLEND);
    glColor4f(0.1, 0.1, 0.1, 0.3); //red, green, blue, alpha
    glBegin(GL_QUADS);
    glVertex2i(-xres, -yres);
    glVertex2i(-xres, yres);
    glVertex2i(xres, yres);
    glVertex2i(xres, -yres);
    glEnd();
    glDisable(GL_BLEND);


    physicsFeat();
    makeFeat();
    usleep(200);

    physicsFeat2();
    makeFeat2();
    usleep(200);

}
