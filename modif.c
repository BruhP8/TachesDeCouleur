/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>

int sizeX, sizeY;

void gris_uniforme(Image * i) {
  int j, size;
  GLubyte * im, val;
  val = 128;
  sizeX = i->sizeX;
  sizeY = i->sizeY;
  size = 3 * sizeY * sizeX;
  im = i->data;
  for (j = 0; j < size; j ++) {
	  *im++ = val;
  }
  printf("gris\n");
}

void taches_de_couleur(Image *i){
    int j, k, y, size, it;
    GLubyte * im;
    Pixel * array;

    sizeX = i->sizeX;
    sizeY = i->sizeY;  
    k = 0;
    size = sizeX * sizeY;
    array = malloc(size * sizeof(Pixel));
    im = i->data;

    /* parcours la data et remplis une structure de pixels r g b */
    for(j = 0; j < 3 * size; j+= 3){
       array[k].explored = 0;
       array[k].r = i->data[j];
       array[k].g = i->data[j + 1];
       array[k].b = i->data[j + 2];
       //printf("%d : %d %d %d\n", k, array[k].r, array[k].g, array[k].b);
       k++;
    }

    // generate all groups of pixels from the whole image

    /*for(y = 0; y < size; y++){
      if(array[y].explored == 1){
        continue;
      }else{
        Tache * new_pound;
        new_pound = malloc(sizeof(Tache));
        new_pound->limits = malloc(size * sizeof(int));

        int x= 0;
        while(x < size){
          new_pound->limits[x] = -1;
          x++;
        }
        it = 0;

        explore(array, y, new_pound, &it, array[y]);  
        //previous = array[y];
        //explore(array, y, new_pound, &it);

        new_pound->r = getAvgRed(array, new_pound->limits);
        new_pound->g = getAvgGreen(array, new_pound->limits);
        new_pound->b = getAvgBlue(array, new_pound->limits);

        k = 0;
        for(j = 0; j < size; j++){
          for(k = 0; new_pound->limits[k] != -1; k++){
            if(j == new_pound->limits[k]){
              array[j].r = new_pound->r;
              array[j].g = new_pound->g;
              array[j].b = new_pound->b;
            }
          }
        }
        free(new_pound->limits);
        free(new_pound);
      }
    }*/

    // generate only the first group. Fails too if group is too big.

    Tache * new_pound;
    new_pound = malloc(sizeof(Tache));
    new_pound->limits = malloc(size * sizeof(int));
    int x= 0;
    while(x < size){
      new_pound->limits[x] = -1;
      x++;
    }
    it = 0;
    explore(array, 0, new_pound, &it, array[0]);   

    new_pound->r = getAvgRed(array, new_pound->limits);
    new_pound->g = getAvgGreen(array, new_pound->limits);
    new_pound->b = getAvgBlue(array, new_pound->limits);

    k = 0;
    for(j = 0; j < size; j++){
      for(k = 0; new_pound->limits[k] != -1; k++){
        if(j == new_pound->limits[k]){
          array[j].r = new_pound->r;
          array[j].g = new_pound->g;
          array[j].b = new_pound->b;
        }
      }
    }

    // display updated image
    k = 0;
    for(j = 0; j < 3 * size; j+=3){
      im[j] = array[k].r ;
      im[j+1] = array[k].g ;
      im[j + 2] = array[k].b;
      k++;
    }
}

void explore(Pixel * array, int j, Tache * cur_pound, int * it, Pixel previous){

  printf("%d\n", j);
  // depassement de l'image
  /*if(j >= sizeX * sizeY){
    printf("size\n");
    return;
  }*/

  // pixel deja traité, pas besoin de l'explorer
  /*if(array[j].explored == 1){
    printf("explored\n");
    return;
  }*/

  // Trop grosse différence de couleur, ce pixel appartiens a une autre tache
  if(abs((int)array[j].r - previous.r) > SEUIL || abs((int)array[j].g - previous.g) > SEUIL || abs((int)array[j].b - previous.b) > SEUIL){
    return;
  }

  array[j].explored = 1;
  cur_pound->limits[* it] = j;
  (* it)++;
  //Pixel last = array[j];
  //previous = array[j];

  // récursion sur les voisins
  if(j +1 < sizeX * sizeY && array[j+1].explored != 1){
    //printf("lel\n");
    explore(array, j + 1, cur_pound, it, previous);
    //explore(array, j + 1, cur_pound, it);//, last);
  }

  if(j > 1 && array[j-1].explored != 1){
    //printf("left\n");
    explore(array, j - 1, cur_pound, it, previous);
    //explore(array, j - 1, cur_pound, it);//, last);

  }

  if(j + sizeX < sizeX * sizeY && array[j + sizeX].explored != 1){
    //printf("down\n");
  	explore(array, j + sizeX, cur_pound, it, previous);
   	//explore(array, j + sizeX, cur_pound, it);//, last);
  }

  if(j - sizeX > 0 && array[j - sizeX].explored != 1){
    //printf("up\n");
    explore(array, j - sizeX, cur_pound, it, previous);
    //explore(array, j - sizeX, cur_pound, it);//, last);

  }
}

GLubyte getAvgRed(Pixel * array, int * indexs){
  int tmp = 0;
  GLubyte avg = 0;
  int i;

  for(i = 0; indexs[i] != -1; i++){
    tmp += array[indexs[i]].r;
  }
  avg = tmp / i;

  return avg; 
}

GLubyte getAvgGreen(Pixel * array, int * indexs){
  int tmp = 0;
  GLubyte avg = 0;
  int i;

  for(i = 0; indexs[i] != -1; i++){
    tmp += array[indexs[i]].g;
  }
  avg = tmp / i;
  return avg; 
}

GLubyte getAvgBlue(Pixel * array, int * indexs){
  int tmp = 0;
  GLubyte avg = 0;
  int i;

  for(i = 0; indexs[i] != -1; i++){
    tmp += array[indexs[i]].b;
  }
  avg = tmp / i;
  return avg; 
}


//disatnce euclidienne pour les ecarts de couleur ?