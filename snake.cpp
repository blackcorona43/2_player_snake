//
//program: snake.cpp
//author:  Gordon Griesel
//purpose: Simple snake game
//cs335 Spring 2014
//cmps3350 Spring 2018
//
//
//notes:
//This may be used as the starting point for a class project.
//Students should add the following components:
//
//A minor bug was left in the program for demonstration.
//
//These items could be used to generate your requirements phase.
//
// . Introduction page
// . Title page
// . Credits page
// . Main menu
// . Snake texture
// . Rat texture
// . Levels
// . Changes in difficulty/speed
// . Scoring
// . Save high scores
// . Board texture
// . Sound
// . Special effects
// . Mouse dragging
// . Win and lose indication
// . Additional features
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "dflores2.h"
#include "stellez.h"
#include "jsingh.h"
#include "rcorona.h"

#define USE_OPENAL_SOUND
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

//macros
#define rnd() (double)rand()/(double)RAND_MAX

#define DIRECTION_DOWN  0
#define DIRECTION_LEFT  1
#define DIRECTION_UP    2
#define DIRECTION_RIGHT 3
//
#define MAX_GRID 80

void reset_screen();
typedef struct t_grid {
    int status;
    float color[4];
} Grid;
//
typedef struct t_snake {
    int status;
    int length;
    int pos[MAX_GRID*MAX_GRID][2];
    int direction;
    double timer;
    double delay;
} Snake;
//
typedef struct t_rat {
    int status;
    int pos[2];
} Rat;
//
//
//
//
#define MAXBUTTONS 4
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

class Image {
    public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
	    if (fname[0] == '\0')
		return;
	    //printf("fname **%s**\n", fname);
	    int ppmFlag = 0;
	    char name[40];
	    strcpy(name, fname);
	    int slen = strlen(name);
	    char ppmname[80];
	    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
		ppmFlag = 1;
	    if (ppmFlag) {
		strcpy(ppmname, name);
	    } else {
		name[slen-4] = '\0';
		//printf("name **%s**\n", name);
		sprintf(ppmname,"%s.ppm", name);
		//printf("ppmname **%s**\n", ppmname);
		char ts[100];
		//system("convert img.jpg img.ppm");
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
	    }
	    //sprintf(ts, "%s", name);
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
		printf("ERROR opening image: %s\n",ppmname);
		exit(0);
	    }
	    if (!ppmFlag)
		unlink(ppmname);
	}
};
Image img[1] = {"./images/dirt.gif" };
Image img2[1] = {"./images/credits.gif" };
Image img3[1] = {"./images/rat1.gif" };
Image img4[1] = {"./images/snakehead1.png" };
Image img5[1] = {"./images/rat1.gif" };

struct Global {
    int xres, yres;
    Grid grid[MAX_GRID][MAX_GRID];
    Snake snake;
    Snake2 snake2;
    Ai_snake com_snake;
    Rat rat;
    unsigned int flag = 0;
    unsigned int player_flag = 0;
    int gridDim;
    int boardDim;
    int gameover;
    int winner;
    int done = 0;
    unsigned int showcredits;
    unsigned int featJ = 0;
    unsigned int st_feature;
    int size = 16;
    int pixel = 16;
    unsigned int texture_feature = 0;
    int pauseState = 0;
    int p1_points = 0;
    int p2_points = 0;
    int highscore;
    int count = 0;
    int help = 0;
    unsigned int power_up = 0;
    Image *marbleImage;
    Image *mouseImage;
    Image *grassImage;
    Image *creditsImage;
    Image *snakeHead1Image;
    Image *snakeHead2Image;
    GLuint marbleTexture;
    GLuint mouseTexture;
    GLuint grassTexture;
    GLuint snakeHead1Texture;
    GLuint snakeHead2Texture;
    GLuint silTexture;
    Button button[MAXBUTTONS];
    int nbuttons;
    //
    ALuint alBufferDrip, alBufferTick;
    ALuint alSourceDrip, alSourceTick;
    Global() {
	xres = 2400;
	yres = 1800;
	gridDim = 32;
	gameover = 0;
	highscore = 200;
	winner = 0;
	nbuttons = 0;
	showcredits = 0;
	marbleImage=NULL;
	creditsImage=NULL;
	mouseImage=NULL;
	grassImage=NULL;
	snakeHead1Image=NULL;
	snakeHead2Image=NULL;
    }
} g;

class X11_wrapper {
    private:
	Display *dpy;
	Window win;
    public:
	X11_wrapper() {
	    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	    XSetWindowAttributes swa;
	    setupScreenRes(g.xres, g.yres);
	    dpy = XOpenDisplay(NULL);
	    if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	    }
	    Window root = DefaultRootWindow(dpy);
	    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	    if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	    } 
	    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	    swa.colormap = cmap;
	    swa.event_mask = ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask |
		SubstructureNotifyMask;
	    win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,	vi->depth,
		    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	    GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	    glXMakeCurrent(dpy, win, glc);
	    setTitle();
	}
	~X11_wrapper() {
	    XDestroyWindow(dpy, win);
	    XCloseDisplay(dpy);
	}
	void setTitle() {
	    //Set the window title bar.
	    XMapWindow(dpy, win);
	    XStoreName(dpy, win, "Hungry Hungry Snake");
	}
	void setupScreenRes(const int w, const int h) {
	    g.xres = w;
	    g.yres = h;
	}
	void reshapeWindow(int width, int height) {
	    //window has been resized.
	    setupScreenRes(width, height);
	    //
	    glViewport(0, 0, (GLint)width, (GLint)height);
	    glMatrixMode(GL_PROJECTION); glLoadIdentity();
	    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	    glOrtho(0, g.xres, 0, g.yres, -1, 1);
	    setTitle();
	}
	void checkResize(XEvent *e) {
	    //The ConfigureNotify is sent by the server if the window is resized.
	    if (e->type != ConfigureNotify)
		return;
	    XConfigureEvent xce = e->xconfigure;
	    if (xce.width != g.xres || xce.height != g.yres) {
		//Window size did change.
		reshapeWindow(xce.width, xce.height);
	    }
	}
	bool getXPending() {
	    return XPending(dpy);
	}
	XEvent getXNextEvent() {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    return e;
	}
	void swapBuffers() {
	    glXSwapBuffers(dpy, win);
	}
} x11;

//function prototypes
void initOpengl(void);
int checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void initSounds(void);
void physics(void);
void render(void);
void getGridCenter(const int i, const int j, int cent[2]);
#ifdef USE_OPENAL_SOUND
void initSound();
void cleanupSound();
void playSound(ALuint source);
unsigned char *buildAlphaData(Image *img);
#endif //USE_OPENAL_SOUND


//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown = 0.0;
double timeSpan = 0.0;
double timeDiff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
	(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
    memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------


int main(int argc, char *argv[])
{
    if (argc) {}
    if (argv[0]) {}
    logOpen();
    initOpengl();
    init();
    initialize_fonts();
    initSound();
    srand((unsigned int)time(NULL));
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    //int done = 0;
    while (g.done == 0) {
	while (x11.getXPending()) {
	    XEvent e = x11.getXNextEvent();
	    x11.checkResize(&e);
	    g.done = checkMouse(&e);
	    if (g.done == 1)
		g.done = g.done;
	    else	    
		g.done = checkKeys(&e);
	}
	//
	//Below is a process to apply physics at a consistent rate.
	//1. Get the time right now.
	clock_gettime(CLOCK_REALTIME, &timeCurrent);
	//2. How long since we were here last?
	timeSpan = timeDiff(&timeStart, &timeCurrent);
	//3. Save the current time as our new starting time.
	timeCopy(&timeStart, &timeCurrent);
	//4. Add time-span to our countdown amount.
	physicsCountdown += timeSpan;
	//5. Has countdown gone beyond our physics rate? 
	//       if yes,
	//           In a loop...
	//              Apply physics
	//              Reducing countdown by physics-rate.
	//              Break when countdown < physics-rate.
	//       if no,
	//           Apply no physics this frame.
	while(physicsCountdown >= physicsRate) {
	    //6. Apply physics
	    physics();
	    //7. Reduce the countdown by our physics-rate
	    physicsCountdown -= physicsRate;
	}
	//Always render every frame.
	render();
	x11.swapBuffers();
    }
    cleanupSound();
    cleanup_fonts();
    logClose();
    return 0;
}

void initSound()
{
#ifdef USE_OPENAL_SOUND
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERROR: alutInit()\n");
	return;
    }
    //Clear error state.
    alGetError();
    //
    //Setup the listener.
    //Forward and up vectors are used.
    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);
    //
    //Buffer holds the sound information.
    g.alBufferDrip = alutCreateBufferFromFile("./sounds/drip.wav");
    g.alBufferTick = alutCreateBufferFromFile("./sounds/tick.wav");
    //
    //Source refers to the sound.
    //Generate a source, and store it in a buffer.
    alGenSources(1, &g.alSourceDrip);
    alSourcei(g.alSourceDrip, AL_BUFFER, g.alBufferDrip);
    //Set volume and pitch to normal, no looping of sound.
    alSourcef(g.alSourceDrip, AL_GAIN, 1.0f);
    alSourcef(g.alSourceDrip, AL_PITCH, 1.0f);
    alSourcei(g.alSourceDrip, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERROR: setting source\n");
	return;
    }
    //Generate a source, and store it in a buffer.
    alGenSources(1, &g.alSourceTick);
    alSourcei(g.alSourceTick, AL_BUFFER, g.alBufferTick);
    //Set volume and pitch to normal, no looping of sound.
    alSourcef(g.alSourceTick, AL_GAIN, 1.0f);
    alSourcef(g.alSourceTick, AL_PITCH, 1.0f);
    alSourcei(g.alSourceTick, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
	printf("ERROR: setting source\n");
	return;
    }
#endif //USE_OPENAL_SOUND
}

void cleanupSound()
{
#ifdef USE_OPENAL_SOUND
    //First delete the source.
    alDeleteSources(1, &g.alSourceDrip);
    alDeleteSources(1, &g.alSourceTick);
    //Delete the buffer.
    alDeleteBuffers(1, &g.alBufferDrip);
    alDeleteBuffers(1, &g.alBufferTick);
    //Close out OpenAL itself.
    //Get active context.
    ALCcontext *Context = alcGetCurrentContext();
    //Get device for active context.
    ALCdevice *Device = alcGetContextsDevice(Context);
    //Disable context.
    alcMakeContextCurrent(NULL);
    //Release context(s).
    alcDestroyContext(Context);
    //Close device.
    alcCloseDevice(Device);
#endif //USE_OPENAL_SOUND
}

void playSound(ALuint source)
{
#ifdef USE_OPENAL_SOUND
    alSourcePlay(source);
#endif //USE_OPENAL_SOUND
}

void initOpengl(void)
{
    //OpenGL initialization
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    //
    //choose one of these
    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    //
    glEnable(GL_TEXTURE_2D);
    //marble_texture = loadBMP("marble.bmp");
    glBindTexture(GL_TEXTURE_2D, 0);
    //
    //load the image file into a ppm structure.
    //
    //g.marbleImage = ppm6GetImage("./images/marble.ppm");
    g.marbleImage = &img[0];
    //
    //create opengl texture elements
    glGenTextures(1, &g.marbleTexture);
    glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    g.marbleImage->width, g.marbleImage->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, g.marbleImage->data);

    // Grass Image
    g.grassImage = &img2[0];

    //create opengl texture elements
    glGenTextures(1, &g.grassTexture);
    glBindTexture(GL_TEXTURE_2D, g.grassTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    g.grassImage->width, g.grassImage->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, g.grassImage->data);

    // MouseImage
    g.mouseImage = &img3[0];

    //create opengl texture elements
    glGenTextures(1, &g.mouseTexture);
    glBindTexture(GL_TEXTURE_2D, g.mouseTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    g.mouseImage->width, g.mouseImage->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, g.mouseImage->data);

    // snakeHead1Image
    g.snakeHead1Image = &img4[0];

    //create opengl texture elements
    glGenTextures(1, &g.snakeHead1Texture);
    glBindTexture(GL_TEXTURE_2D, g.snakeHead1Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    g.snakeHead1Image->width, g.snakeHead1Image->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, g.snakeHead1Image->data);


    //silhouette (SnakeHead)
    glGenTextures(1, &g.silTexture);
    glBindTexture(GL_TEXTURE_2D, g.silTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *silData = buildAlphaData(img4[0].data, img4[0].height, img4[0].width );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g.snakeHead1Image->width,
	    g.snakeHead1Image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, silData);
    free(silData);


    // snakeHead2Image
    g.snakeHead2Image = &img5[0];

    //create opengl texture elements
    glGenTextures(1, &g.snakeHead2Texture);
    glBindTexture(GL_TEXTURE_2D, g.snakeHead2Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    g.snakeHead2Image->width, g.snakeHead2Image->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, g.snakeHead2Image->data);
}

void initSnake()
{
    int i;
    g.snake.status = 1;
    g.snake.delay = .15;
    g.snake.length = 5;
    for (i=0; i<g.snake.length; i++) {
	g.snake.pos[i][0] = 1;
	g.snake.pos[i][1] = 1;
    }
    g.snake.direction = DIRECTION_RIGHT;
    //snake.timer = glfwGetTime() + 0.5;
    g.snake2.status = 1;
    g.snake2.delay = .15;
    g.snake2.length = 5;
    for (i=0; i<g.snake2.length; i++) {
	g.snake2.pos[i][0] = 30;
	g.snake2.pos[i][1] = 30;
    }
    g.snake2.direction = DIRECTION_LEFT;

    g.com_snake.status = 1;
    g.com_snake.delay = .15;
    g.com_snake.length = 5;
    for (i=0; i < g.com_snake.length; i++) {
	g.com_snake.pos[i][0] = 30;
	g.com_snake.pos[i][1] = 30;
    }
    g.com_snake.direction = DIRECTION_LEFT;

}

void initRat()
{
    g.rat.status = 1;
    g.rat.pos[0] = g.gridDim/2;
    g.rat.pos[1] = g.gridDim/2;
}

void init()
{
    g.boardDim = g.gridDim * 32;
    //
    initSnake();
    initRat();
    //
    //initialize buttons...
    g.nbuttons=0;
    //size and position
    g.button[g.nbuttons].r.width = 140;
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
    g.button[g.nbuttons].r.width = 140;
    g.button[g.nbuttons].r.height = 60;
    g.button[g.nbuttons].r.left = 20;
    g.button[g.nbuttons].r.bot = 160;
    g.button[g.nbuttons].r.right =
	g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
    g.button[g.nbuttons].r.top = g.button[g.nbuttons].r.bot +
	g.button[g.nbuttons].r.height;
    g.button[g.nbuttons].r.centerx = (g.button[g.nbuttons].r.left +
	    g.button[g.nbuttons].r.right) / 2;
    g.button[g.nbuttons].r.centery = (g.button[g.nbuttons].r.bot +
	    g.button[g.nbuttons].r.top) / 2;
    strcpy(g.button[g.nbuttons].text, " Esc to Quit");
    g.button[g.nbuttons].down = 0;
    g.button[g.nbuttons].click = 0;
    g.button[g.nbuttons].color[0] = 0.3f;
    g.button[g.nbuttons].color[1] = 0.3f;
    g.button[g.nbuttons].color[2] = 0.6f;
    g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
    g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
    g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
    g.button[g.nbuttons].text_color = 0x00ffffff;
    g.nbuttons++;
}

void resetGame()
{
    initSnake();
    initRat();
    g.gameover  = 0;
    g.winner    = 0;
    g.player_flag = 0;
    g.flag = 0;
    g.gameover = 0;
}
extern int show_power_up_mode(int[], int, int);
extern int help_screen(int,int);
extern int my_name();
extern int name3();
extern int show_my_name();
extern int name5();
extern int leaderboard(int,int,int);
int checkKeys(XEvent *e)
{
    static int shift=0;
    if (e->type != KeyRelease && e->type != KeyPress)
	return 0;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    if (e->type == KeyRelease) {
	if (key == XK_Shift_L || key == XK_Shift_R)
	    shift=0;
	return 0;
    }
    if (key == XK_Shift_L || key == XK_Shift_R) {
	shift=1;
	return 0;
    }
    (void)shift;
    switch (key) {
	case XK_r:
	    printf("Reset Game\n");
	    resetGame();
	    break;
	case XK_Escape:// Escape to quit game
	    g.done = 1;
	    printf("Quit Game\n");
	    return 1;
	case XK_c:// open/close credits page
	    printf("Credits Screen\n");
	    g.showcredits = manage_state_st(g.showcredits);
	    break;
	case XK_equal:
	    g.snake.delay *= 0.9;
	    if (g.snake.delay < 0.001)
		g.snake.delay = 0.001;
	    break;
	case XK_minus:
	    g.snake.delay *= (1.0 / 0.9);
	    break;
	case XK_F1: // help game
	    printf("Help Screen\n");
	    g.help ^= 1;
	    break;
	case XK_l: // length power up
	    printf("Length Power Up testing\n");
	    g.power_up ^= 1;
	    break;
	case XK_j: // Jasdeep feature mode
	    printf("Feature mode to test buttons (Jasdeep)\n");
	    g.featJ ^= 1;
	    break;
	case XK_a:
	    g.snake.direction = DIRECTION_LEFT;
	    break;
	case XK_d:
	    g.snake.direction = DIRECTION_RIGHT;
	    break;
	case XK_w:
	    g.snake.direction = DIRECTION_UP;
	    break;
	case XK_s:
	    g.snake.direction = DIRECTION_DOWN;
	    break;
	    // 2ND Snake Buttons
	case XK_Left:
	    g.snake2.direction = DIRECTION_LEFT;
	    break;
	case XK_Right:
	    g.snake2.direction = DIRECTION_RIGHT;
	    break;
	case XK_Up:
	    g.snake2.direction = DIRECTION_UP;
	    break;
	case XK_Down:
	    g.snake2.direction = DIRECTION_DOWN;
	    break;
	case XK_p:
	    g.pauseState ^= 1;
	    break;
	case XK_t:
	    g.texture_feature ^= 1;
	    break;
	case XK_q:
	    g.player_flag = two_player(g.player_flag);
	    break;
	case XK_v:
	    g.flag = computer_snake(g.flag);
	    break;
	case XK_n:
	    g.st_feature = manage_state_st(g.st_feature);
	    int y = g.yres;
	    int x = g.xres/2;
	    make_particles(x,y);
	    break;    
    }
    return 0;
}

int checkMouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    int i,x,y;
    int lbutton=0;
    int rbutton=0;
    //
    if (e->type == ButtonRelease)
	return 0;
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button is down
	    lbutton=1;
	}
	if (e->xbutton.button==3) {
	    //Right button is down
	    rbutton=1;
	    if (rbutton){}
	}
    }
    x = e->xbutton.x;
    y = e->xbutton.y;
    y = g.yres - y;
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
	//Mouse moved
	savex = e->xbutton.x;
	savey = e->xbutton.y;
    }
    for (i=0; i<g.nbuttons; i++) {
	g.button[i].over=0;
	if (x >= g.button[i].r.left &&
		x <= g.button[i].r.right &&
		y >= g.button[i].r.bot &&
		y <= g.button[i].r.top) {
	    g.button[i].over=1;
	    if (g.button[i].over) {
		if (lbutton) {
		    switch (i) {
			case 0:
			    resetGame();
			    break;
			case 1:
			    printf("Quit was clicked!\n");
			    g.done = 1;
			    return 1;
		    }
		}
	    }
	}
    }
    if (g.done == 0)
	return 0;
    else
	return 1;

}

void getGridCenter(const int i, const int j, int cent[2])
{
    //This function can be optimized, and made more generic.
    int b2 = g.boardDim/2;
    int screenCenter[2] = {g.xres/2, g.yres/2};
    int s0 = screenCenter[0];
    int s1 = screenCenter[1];
    int bq;
    //quad upper-left corner
    int quad[2];
    //bq is the width of one grid section
    bq = (g.boardDim / g.gridDim);
    //-------------------------------------
    //because y dimension is bottom-to-top in OpenGL.
    int i1 = g.gridDim - i - 1;
    quad[0] = s0-b2;
    quad[1] = s1-b2;
    cent[0] = quad[0] + bq/2;
    cent[1] = quad[1] + bq/2;
    cent[0] += (bq * j);
    cent[1] += (bq * i1);
}


void physics(void)
{
    int i;
    if (g.gameover)
	return;
    if (g.pauseState) {

    }
    else {
	//
	//
	//Is it time to move the snake?
	static struct timespec snakeTime;
	static int firsttime=1;
	if (firsttime) {
	    firsttime=0;
	    clock_gettime(CLOCK_REALTIME, &snakeTime);
	}
	struct timespec tt;
	clock_gettime(CLOCK_REALTIME, &tt);
	double timeSpan = timeDiff(&snakeTime, &tt);
	if (timeSpan < g.snake.delay)
	    return;
	if (timeSpan < g.snake2.delay)
	    return;
	timeCopy(&snakeTime, &tt);
	//
	playSound(g.alSourceDrip);
	//move the snake segments...
	int headpos[2], headpos2[2], aihead[2];
	int newpos[2], newpos2[2], ainewpos[2];
	int oldpos[2], oldpos2[2], aioldpos[2];
	//save the head position.
	headpos[0] = g.snake.pos[0][0];
	headpos[1] = g.snake.pos[0][1];
	headpos2[0] = g.snake2.pos[0][0];
	headpos2[1] = g.snake2.pos[0][1];
	aihead[0] = g.com_snake.pos[0][0];
	aihead[1] = g.com_snake.pos[0][1];

	if (g.flag == 1){
	    //does the logic of what direction to move
	    if (g.rat.pos[0] < aihead[0])
		g.com_snake.direction = DIRECTION_LEFT;
	    else if (g.rat.pos[0] > aihead[0])
		g.com_snake.direction = DIRECTION_RIGHT;
	    else if (g.rat.pos[1] > aihead[1])
		g.com_snake.direction = DIRECTION_DOWN;
	    else if (g.rat.pos[1] < aihead[1])
		g.com_snake.direction = DIRECTION_UP;
	    //does the self move
	    ai_direction(g.com_snake.direction, g.com_snake.pos);
	    //checks if AI went out of bounds
	    g.gameover = check_off_board(g.com_snake.pos, g.gridDim, 
		    g.p1_points, g.gameover);
	    //AI crosses itself
	    g.gameover = ai_cross_itself(g.com_snake.length,
		    g.com_snake.pos, g.p1_points, g.gameover);
	    //Check for AI Colliding heads
	    g.gameover = ai_crosses_player (g.snake.length, g.com_snake.pos,
		    g.snake.pos, g.gameover);
	    //check if player crosses AI
	    g.gameover = player_crosses_ai(g.com_snake.length, g.com_snake.pos, 
		    g.snake.pos, g.gameover);

	    ainewpos[0] = aihead[0];
	    ainewpos[1] = aihead[1];
	    //makes the snake linger
	    ai_long_length(g.com_snake.length, aioldpos, ainewpos, g.com_snake.pos);

	    //check if computer ate the rat
	    if (aihead[0] == g.rat.pos[0] && aihead[1] == g.rat.pos[1])
	    {
		playSound(g.alSourceTick);
		Log("COMPUTER ate the Rat. com_snake.length: %i dir: %i\n",
			g.com_snake.length, g.com_snake.direction);
		int addlength = rand() % 4 + 4;
		for (i=0; i < addlength; i++) {
		    g.com_snake.pos[g.com_snake.length][0] =
			g.com_snake.pos[g.com_snake.length-1][0];
		    g.com_snake.pos[g.com_snake.length][1] =
			g.com_snake.pos[g.com_snake.length-1][1];
		    g.com_snake.length++; 
		}
		int collision = 0;
		int ntries = 0;
		while(1) {
		    g.rat.pos[0] = rand() % g.gridDim;
		    g.rat.pos[1] = rand() % g.gridDim;
		    collision = 0;
		    for (i=0; i < g.com_snake.length; i++){
			if (g.rat.pos[0] == g.com_snake.pos[i][0] &&
				g.rat.pos[0] == g.com_snake.pos[i][0] &&
				g.rat.pos[1] == g.com_snake.pos[i][1]){ 
			    collision=1;
			    break;
			}
		    }
		    if (!collision) break;
		    if (++ntries > 1000000) break;
		}
		Log("new rat: %i %i\n",g.rat.pos[0],g.rat.pos[1]);
		return;
	    }
	}


	//snake.direction:
	//0=down
	//1=left
	//2=up
	//3=right
	switch (g.snake.direction) {
	    case DIRECTION_DOWN:  g.snake.pos[0][1] += 1; break;
	    case DIRECTION_LEFT:  g.snake.pos[0][0] -= 1; break;
	    case DIRECTION_UP:    g.snake.pos[0][1] -= 1; break;
	    case DIRECTION_RIGHT: g.snake.pos[0][0] += 1; break;
	}

	if (g.player_flag == 1) {
	    // 2ND Snake Direction
	    snake_direction(g.snake2.direction, g.snake2.pos);
	}
	//check for snake off board...
	if (g.snake.pos[0][0] < 0 ||
		g.snake.pos[0][0] > g.gridDim-1 ||
		g.snake.pos[0][1] < 0 ||
		g.snake.pos[0][1] > g.gridDim-1) {
	    g.p2_points += 20;
	    g.gameover=1;
	    printf("\n");
	    printf("-----------------------------\n");
	    printf("Snake 1 went off the board!\n");
	    printf("Snake 2 Wins!\n");
	    printf("Player 2 has %d points\n", g.p2_points); 
	    printf("-----------------------------\n");
	    return;
	}
	if (g.player_flag == 1) {
	    //check for snake2 off board...
	    g.gameover = snake_hits_edge(g.snake2.pos, g.gridDim);
	    if (g.gameover == 1) {
		printf("\n");
		printf("-----------------------------\n");
		printf("Snake 2 went off the board!\n");
		printf("Snake 1 Wins!\n");
		printf("Player 1 has %d points\n", g.p1_points); 
		printf("-----------------------------\n");
		return;
	    }
	}
	//check for snake crossing itself...
	for (i=1; i<g.snake.length; i++) {
	    if (g.snake.pos[i][0] == g.snake.pos[0][0] &&
		    g.snake.pos[i][1] == g.snake.pos[0][1]) {

		g.p2_points += 20;
		g.gameover=1;
		printf("\n");
		printf("-----------------------------\n");
		printf("Snake 1 ate itself!\n");
		printf("Snake 2 Wins!\n");
		printf("Player 2 has %d points\n", g.p2_points); 
		printf("-----------------------------\n");
		return;
	    }
	}
	if (g.player_flag == 1){
	    //check for snake2 crossing itself
	    g.gameover = snake_eats_itself(g.snake2.length, g.snake2.pos);
	    if (g.gameover == 1) {
		printf("\n");
		printf("-----------------------------\n");
		printf("Snake 2 ate itself!\n");
		printf("Snake 1 Wins!\n");
		printf("Player 1 has %d points\n", g.p1_points); 
		printf("-----------------------------\n");
		return;
	    }
	}
	//
	// Check for Colliding Heads
	if (g.snake2.pos[0][0] == g.snake.pos[0][0] &&
		g.snake2.pos[0][1] == g.snake.pos[0][1]) {
	    g.p1_points += 20;
	    g.p2_points += 20;
	    g.gameover=1;
	    printf("\n");
	    printf("-----------------------------\n");
	    printf("Snake 1 and Snake 2 killed each other!\n");
	    printf("It is a draw!!\n");
	    printf("Player 1 has %d points\n", g.p1_points); 
	    printf("Player 2 has %d points\n", g.p2_points); 
	    printf("-----------------------------\n");
	    return;
	}
	//check for snake crossing snake2...
	for (i=0; i<g.snake2.length; i++) {
	    if (g.snake2.pos[i][0] == g.snake.pos[0][0] &&
		    g.snake2.pos[i][1] == g.snake.pos[0][1]) {
		g.p2_points += 20;
		g.gameover=1;
		printf("\n");
		printf("-----------------------------\n");
		printf("Snake 2 killed Snake 1!\n");
		printf("Snake 2 Wins!\n");
		printf("Player 2 has %d points\n", g.p2_points); 
		printf("-----------------------------\n");
		return;
	    }
	}
	//check for snake2 crossing snake...
	if (g.player_flag == 1){
	    g.gameover = player_crosses_player(g.snake.length, g.snake.pos, g.snake2.pos);
	    if (g.gameover == 1) {
		printf("\n");
		printf("-----------------------------\n");
		printf("Snake 1 killed Snake 2!\n");
		printf("Snake 1 Wins!\n");
		printf("Player 1 has %d points\n", g.p1_points); 
		printf("-----------------------------\n");
		return;
	    }
	}

	//
	newpos[0] = headpos[0];
	newpos[1] = headpos[1];
	//2nd Snake Head Position
	newpos2[0] = headpos2[0];
	newpos2[1] = headpos2[1];
	//
	for (i=1; i<g.snake.length; i++) {
	    oldpos[0] = g.snake.pos[i][0];
	    oldpos[1] = g.snake.pos[i][1];
	    if (g.snake.pos[i][0] == newpos[0] &&
		    g.snake.pos[i][1] == newpos[1])
		break;
	    g.snake.pos[i][0] = newpos[0];
	    g.snake.pos[i][1] = newpos[1];
	    newpos[0] = oldpos[0];
	    newpos[1] = oldpos[1];
	}
	if (g.player_flag == 1) {
	    // Snake2
	    for (i=1; i<g.snake2.length; i++) {
		oldpos2[0] = g.snake2.pos[i][0];
		oldpos2[1] = g.snake2.pos[i][1];
		if (g.snake2.pos[i][0] == newpos2[0] &&
			g.snake2.pos[i][1] == newpos2[1])
		    break;
		g.snake2.pos[i][0] = newpos2[0];
		g.snake2.pos[i][1] = newpos2[1];
		newpos2[0] = oldpos2[0];
		newpos2[1] = oldpos2[1];
	    }
	}
	//did the snake eat the rat???
	if (headpos[0] == g.rat.pos[0] && headpos[1] == g.rat.pos[1]) {
	    if (g.power_up)
	    {
		int addlength = rand() % 4 + 25;
		for (i=0; i<addlength; i++) {
		    g.snake.pos[g.snake.length][0] = 
			g.snake.pos[g.snake.length-1][0];
		    g.snake.pos[g.snake.length][1] = 
			g.snake.pos[g.snake.length-1][1];
		    g.snake.length++;
		}

	    }
	    //yes, increase length of snake.
	    playSound(g.alSourceTick);
	    //put new segment at end of snake.
	    Log("snake ate rat. snake.length: %i   dir: %i\n",
		    g.snake.length,g.snake.direction);
	    int addlength = rand() % 4 + 4;
	    for (i=0; i<addlength; i++) {
		g.snake.pos[g.snake.length][0] = 
		    g.snake.pos[g.snake.length-1][0];
		g.snake.pos[g.snake.length][1] = 
		    g.snake.pos[g.snake.length-1][1];
		g.snake.length++;
	    }
	    //new position for rat...
	    int collision=0;
	    int ntries=0;
	    g.p1_points += 5;
	    while (1) {
		g.rat.pos[0] = rand() % g.gridDim;
		g.rat.pos[1] = rand() % g.gridDim;
		collision=0;
		for (i=0; i<g.snake.length; i++) {
		    if (g.rat.pos[0] == g.snake.pos[i][0] &&
			    g.rat.pos[1] == g.snake.pos[i][1]) {
			collision=1;
			break;
		    }
		}
		if (!collision) break;
		if (++ntries > 1000000) break;
	    }
	    Log("new rat: %i %i\n",g.rat.pos[0],g.rat.pos[1]);
	    return;
	}
	if (g.player_flag == 1) {
	    //did snake2 eat the rat???
	    if (headpos2[0] == g.rat.pos[0] && headpos2[1] == g.rat.pos[1]) {
		if (g.power_up) {
		    int addlength = rand() % 4 + 25;
		    for (i=0; i<addlength; i++) {
			g.snake2.pos[g.snake2.length][0] = 
			    g.snake2.pos[g.snake2.length-1][0];
			g.snake2.pos[g.snake2.length][1] = 
			    g.snake2.pos[g.snake2.length-1][1];
			g.snake2.length++;
		    }

		}
		//yes, increase length of snake.
		playSound(g.alSourceTick);
		//put new segment at end of snake.
		Log("snake2 ate rat. snake2.length: %i   dir: %i\n",
			g.snake2.length,g.snake2.direction);
		int addlength = rand() % 4 + 4;
		for (i=0; i<addlength; i++) {
		    g.snake2.pos[g.snake2.length][0] = 
			g.snake2.pos[g.snake2.length-1][0];
		    g.snake2.pos[g.snake2.length][1] = 
			g.snake2.pos[g.snake2.length-1][1];
		    g.snake2.length++;
		}
		//new position for rat...
		int collision=0;
		int ntries=0;
		g.p2_points += 5;
		while (1) {
		    g.rat.pos[0] = rand() % g.gridDim;
		    g.rat.pos[1] = rand() % g.gridDim;
		    collision=0;
		    for (i=0; i<g.snake2.length; i++) {
			if (g.rat.pos[0] == g.snake2.pos[i][0] &&
				g.rat.pos[1] == g.snake2.pos[i][1]) {
			    collision=1;
			    break;
			}
		    }
		    if (!collision) break;
		    if (++ntries > 1000000) break;
		}
		Log("new rat: %i %i\n",g.rat.pos[0],g.rat.pos[1]);
		return;
	    }
	}
    }
}

void reset_screen()
{

    glViewport(0, 0, g.xres, g.yres);
    //clear color buffer
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //init matrices
    glMatrixMode (GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //this sets to 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void render(void)
{ 
    if(g.showcredits == 1)
    {
	g.pauseState = 1;
	show_credits(g.xres, g.yres);
    }
    else if (g.help)
    {
	g.pauseState = 1;
	help_screen(g.xres, g.yres);            
    }
    else if (g.gameover)
    {
	reset_screen();
	show_gameover(g.xres, g.yres);
	leaderboard(g.p1_points,g.highscore,g.xres);
	g.gameover = 1;

    }
    else {

	int i,j;
	Rect r;
	//--------------------------------------------------------
	//This code is repeated several times in this program, so
	//it can be made more generic and cleaner with some work.
	int b2 = g.boardDim/2;
	int s0 = g.xres>>1;
	int s1 = g.yres>>1;
	//center of a grid
	int cent[2];
	//bq is the width of one grid section
	//--------------------------------------------------------
	//start the opengl stuff
	//set the viewing area on screen
	glViewport(0, 0, g.xres, g.yres);
	//clear color buffer
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//init matrices
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	//screen background
	glColor3f(0.5f, 0.5f, 0.5f);
	glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,      0);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,      g.yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, g.yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//draw all buttons
	for (i=0; i<g.nbuttons; i++) {
	    if (g.button[i].over) {
		int w=2;
		glColor3f(1.0f, 1.0f, 0.0f);
		//draw a highlight around button
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
		glVertex2i(g.button[i].r.left-w,  g.button[i].r.top+w);
		glVertex2i(g.button[i].r.right+w, g.button[i].r.top+w);
		glVertex2i(g.button[i].r.right+w, g.button[i].r.bot-w);
		glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
		glEnd();
		glLineWidth(1);
	    }
	    if (g.button[i].down) {
		glColor3fv(g.button[i].dcolor);
	    } else {
		glColor3fv(g.button[i].color);
	    }
	    glBegin(GL_QUADS);
	    glVertex2i(g.button[i].r.left,  g.button[i].r.bot);
	    glVertex2i(g.button[i].r.left,  g.button[i].r.top);
	    glVertex2i(g.button[i].r.right, g.button[i].r.top);
	    glVertex2i(g.button[i].r.right, g.button[i].r.bot);
	    glEnd();
	    r.left = g.button[i].r.centerx;
	    r.bot  = g.button[i].r.centery-8;
	    r.center = 1;
	    if (g.button[i].down) {
		ggprint16(&r, 0, g.button[i].text_color, "Pressed!");
	    } else {
		ggprint16(&r, 0, g.button[i].text_color, g.button[i].text);
	    }
	}
	//draw the main game board in middle of screen
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_QUADS);
	glVertex2i(s0-b2, s1-b2);
	glVertex2i(s0-b2, s1+b2);
	glVertex2i(s0+b2, s1+b2);
	glVertex2i(s0+b2, s1-b2);
	glEnd();
	//
	//grid lines...
	int x0 = s0-b2;
	int x1 = s0+b2;
	int y0 = s1-b2;
	int y1 = s1+b2;
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_LINES);
	for (i=1; i<g.gridDim; i++) {
	    y0 += 32;
	    glVertex2i(x0,y0);
	    glVertex2i(x1,y0);
	}
	x0 = s0-b2;
	y0 = s1-b2;
	y1 = s1+b2;
	for (j=1; j<g.gridDim; j++) {
	    x0 += 32;
	    glVertex2i(x0,y0);
	    glVertex2i(x0,y1);
	}
	glEnd();
	//
#define COLORFUL_SNAKE
	//
	//draw snake...
#ifdef COLORFUL_SNAKE
	float c[3]={1.0f,0.0,0.0};
	float c2[3]={0.1f,1.0,0.1};
	float rgb[3];
	float rgb2[3];
	rgb[0] = -0.8 / (float)g.snake.length;
	rgb[2] = -0.40 / (float)g.snake.length;
	rgb2[0] = -0.1 / (float)g.snake2.length;
	rgb2[2] = -0.2 / (float)g.snake2.length;
	glColor3fv(c);
	//
	glBegin(GL_QUADS);
	for (i=1; i<g.snake.length; i++) {
	    getGridCenter(g.snake.pos[i][1],g.snake.pos[i][0],cent);
	    if (g.texture_feature == 1) {
		if (i == 0) {
		    glEnd();
		    glColor3f(1.0f, 0.0, 0.0);
		    game_Texture(g.silTexture, cent, g.pixel * 5, 
			    g.snake.direction);
		    i = 1;
		}        
		else {
		    glEnd();
		    glBegin(GL_QUADS);
		    glVertex2i(cent[0]-g.size, cent[1]-g.size);
		    glVertex2i(cent[0]-g.size, cent[1]+g.size);
		    glVertex2i(cent[0]+g.size, cent[1]+g.size);
		    glVertex2i(cent[0]+g.size, cent[1]-g.size);
		    c[0] +=	rgb[0];
		    c[2] +=	rgb[2];
		    glColor3fv(c);
		    if (i == 1) {
			i = -1;
		    }
		}
	    }
	    else {
		glBegin(GL_QUADS);
		glVertex2i(cent[0]-g.size, cent[1]-g.size);
		glVertex2i(cent[0]-g.size, cent[1]+g.size);
		glVertex2i(cent[0]+g.size, cent[1]+g.size);
		glVertex2i(cent[0]+g.size, cent[1]-g.size);
		c[0] +=	rgb[0];
		c[2] +=	rgb[2];
		glColor3fv(c);
	    }
	}
	if (g.player_flag == 1) {
	    //2ND Snake
	    for (i=1; i<g.snake2.length; i++) {
		getGridCenter(g.snake2.pos[i][1],g.snake2.pos[i][0],cent);
		if (g.texture_feature == 1) {
		    if (i == 0) {
			glEnd();
			glColor3f(0.0f, 1.0, 0.0);
			game_Texture(g.silTexture, cent, g.pixel * 5, 
				g.snake2.direction);
			i = 1;
		    }        
		    else {
			glBegin(GL_QUADS);
			glVertex2i(cent[0]-g.size, cent[1]-g.size);
			glVertex2i(cent[0]-g.size, cent[1]+g.size);
			glVertex2i(cent[0]+g.size, cent[1]+g.size);
			glVertex2i(cent[0]+g.size, cent[1]-g.size);
			c2[0] +=	rgb2[0];
			c2[2] +=	rgb2[2];
			glColor3fv(c2);
			if (i == 1) {
			    i = -1;
			}
		    }
		}
		else {
		    glVertex2i(cent[0]-g.size, cent[1]-g.size);
		    glVertex2i(cent[0]-g.size, cent[1]+g.size);
		    glVertex2i(cent[0]+g.size, cent[1]+g.size);
		    glVertex2i(cent[0]+g.size, cent[1]-g.size);
		    c2[0] +=	rgb2[0];
		    c2[2] +=	rgb2[2];
		    glColor3fv(c2);
		}
	    }
	}
	if (g.flag == 1)
	{
	    for(i=0; i<g.com_snake.length; i++) {
		getGridCenter(g.com_snake.pos[i][1],g.com_snake.pos[i][0],cent);
		glVertex2i(cent[0]-4, cent[1]-3);
		glVertex2i(cent[0]-4, cent[1]+4);
		glVertex2i(cent[0]+3, cent[1]+4);
		glVertex2i(cent[0]+3, cent[1]-3);
		glColor3f(0.0f, 0.6, 0.0f);
	    }
	}
	glEnd();
#else //COLORFUL_SNAKE
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	for (i=0; i<g.snake.length; i++) {
	    getGridCenter(g.snake.pos[i][1],g.snake.pos[i][0],cent);
	    glVertex2i(cent[0]-4, cent[1]-3);
	    glVertex2i(cent[0]-4, cent[1]+4);
	    glVertex2i(cent[0]+3, cent[1]+4);
	    glVertex2i(cent[0]+3, cent[1]-3);
	    glColor3f(0.0f, 0.6f, 0.0f);
	}

	for (i=0; i<g.snake2.length; i++) {
	    getGridCenter(g.snake2.pos[i][1],g.snake2.pos[i][0],cent);
	    glVertex2i(cent[0]-4, cent[1]-3);
	    glVertex2i(cent[0]-4, cent[1]+4);
	    glVertex2i(cent[0]+3, cent[1]+4);
	    glVertex2i(cent[0]+3, cent[1]-3);
	    glColor3f(0.0f, 0.6f, 0.0f);
	}
	glEnd();
#endif //COLORFUL_SNAKE
       //
       //
       //draw rat...
	getGridCenter(g.rat.pos[1],g.rat.pos[0],cent);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2i(cent[0]-4, cent[1]-3);
	glVertex2i(cent[0]-4, cent[1]+4);
	glVertex2i(cent[0]+3, cent[1]+4);
	glVertex2i(cent[0]+3, cent[1]-3);
	glEnd();
	//
	//
	r.left   = g.xres/2;
	r.bot    = g.yres-100;
	r.center = 1;
	ggprint16(&r, 16, 0x00ffffff, "Hungry Hungry Snake");
	Rect h;
	h.left   = 50;
	h.bot    = 10;
	h.center = 1;
	ggprint16(&h, 16, 0x00ffffff, "F1 for help");
	h.left   = g.xres/2 - 100;
	h.bot    = 100;
	h.center = 1;
	ggprint16(&h, 16, 0xffff0000, "P1 Points: %i", g.p1_points);
	h.left   = g.xres/2 + 100;
	h.bot    = 100;
	h.center = 1;
	ggprint16(&h, 16, 0xf000f0, "P2 Points: %i", g.p2_points);

	if (g.power_up) {
	    show_power_up_mode(cent, g.xres, g.yres);
	}

	//Texture Feature created by Dominic
	if (g.texture_feature == 1) {
	    //draw a border using a triangle strip
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glEnable(GL_BLEND);
	    glColor3f(1.0,1.0,0.0);
	    glColor4f(0.0,1.0,0.0,0.5);
	    int w = 20;
	    glBegin(GL_TRIANGLE_STRIP);
	    glVertex2f(0,0);
	    glVertex2f(0 + w, w);

	    glVertex2f(0,g.yres);
	    glVertex2f(0 + w, g.yres - w);

	    glVertex2f(g.xres, g.yres);
	    glVertex2f(g.xres - w, g.yres - w);

	    glVertex2f(g.xres, 0);
	    glVertex2f(g.xres - w, w);

	    glVertex2f(0,0);
	    glVertex2f(0 + w, w);
	    glEnd();
	    glDisable(GL_BLEND);

	    glColor3f(0.5, 0.5, 0.5);
	    game_Texture(g.mouseTexture, cent, g.pixel, g.snake.direction);
	}

	if (g.flag == 1)
	    ruben_mode(g.xres, g.yres);
	if (g.featJ == 1) {
	    show_pause_screen(g.xres, g.yres);
	    featureJas(g.xres, g.yres);
	}

	if (g.pauseState) {
	    show_pause_screen(g.xres, g.yres);
	}
	if (g.st_feature != 0) {
	    show_feature_st(g.xres, g.yres);
	}
    }
}

