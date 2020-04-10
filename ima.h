#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define SEUIL 50
#include <GL/gl.h>	

struct Pixel {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    char explored;
};
typedef struct Pixel Pixel;

struct Tache {
    int * limits;
    GLubyte r;
    GLubyte g;
    GLubyte b; 
};
typedef struct Tache Tache;

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    GLubyte *data;
};
typedef struct Image Image;
typedef unsigned short utab [3][3][3];

int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);

Pixel * tachesDeCouleur(Pixel * array, int sizeX, int sizeY);
void explore(Pixel * array, int j, Tache * cur_pound, int * it, Pixel previous, int sizeX, int sizeY);
GLubyte getAvgRed(Pixel * array, int * indexs, int size);
GLubyte getAvgGreen(Pixel * array, int * indexs, int size);
GLubyte getAvgBlue(Pixel * array, int * indexs, int size);
Pixel * getSection(Pixel * testArray, int startPos, int sizeX, int sizeY, Pixel * section);
void rewirteImage(GLubyte * im, Pixel * section, int startPos, int sizeX, int sizeY);
void tachesInit (Image * i);
int toFile(Tache * pound, int size);
void fromFile(char * filename, Image * i);
void Display(void);