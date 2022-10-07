// Jasdeep Singh - header file
// include header guard

typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned text color;
} IntroButtons;

extern void showIntro(int, int);
