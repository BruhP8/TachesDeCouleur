#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

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

void gris_uniforme (Image *);
void taches_de_couleur(Image *);
void explore(Pixel * array, int j, Tache * cur_pound, int * it, Pixel previous);
//void explore(Pixel * array, int j, Tache * cur_pound, int * it);

GLubyte getAvgRed(Pixel * array, int * indexs);
GLubyte getAvgGreen(Pixel * array, int * indexs);
GLubyte getAvgBlue(Pixel * array, int * indexs);