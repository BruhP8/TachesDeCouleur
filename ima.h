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
Pixel * tachesDeCouleur(Pixel * array, int sizeX, int sizeY);
void explore(Pixel * array, int j, Tache * cur_pound, int * it, Pixel previous, int sizeX, int sizeY);

GLubyte getAvgRed(Pixel * array, int * indexs, int size);
GLubyte getAvgGreen(Pixel * array, int * indexs, int size);
GLubyte getAvgBlue(Pixel * array, int * indexs, int size);

Pixel * getSection(Pixel * testArray, int startPos, int sizeX, int sizeY, Pixel * section);
void rewirteImage(GLubyte * im, Pixel * section, int startPos, int sizeX, int sizeY);
void tachesInit (Image * i);
/* 
RANDNAME 
    decoupe l'image raw en main Pixel *
    recupere les sections
    appelle tache_de_couleur avec chaque section
    update le main Pixel * avec le resultat de tache_de_couleur
    parcours le main Pixel * et le remet dans l'image


taches_de_couleur prends chaque Pixel * renvoyé par le sectionnage
    traite pour generer les taches
    renvoie le tableau re coloré

*/