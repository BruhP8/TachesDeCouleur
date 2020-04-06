/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>

/*void gris_uniforme(Image * i) {
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
}*/

Pixel * taches_de_couleur(Pixel * array, int sizeX, int sizeY){
    int y, it, j, k;
    int size = sizeX * sizeY;
    // generate all groups of pixels from the whole image
    for(y = 0; y < size; y++){
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
        explore(array, y, new_pound, &it, array[y], sizeX, sizeY);  

        new_pound->r = getAvgRed(array, new_pound->limits, size);
        new_pound->g = getAvgGreen(array, new_pound->limits, size);
        new_pound->b = getAvgBlue(array, new_pound->limits, size);

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
    }


    // generate only the first group. Fails too if group is too big.

    /*Tache * new_pound;
      new_pound = malloc(sizeof(Tache));
      new_pound->limits = malloc(size * sizeof(int));
     int x= 0;
      while(x < size){
        new_pound->limits[x] = -1;
        x++;
      }
      it = 0;
      explore(array, 0, new_pound, &it, array[0], sizeX, sizeY);   

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
      }*/
    return array;

}

void explore(Pixel * array, int j, Tache * cur_pound, int * it, Pixel previous, int sizeX, int sizeY){
  //printf("%d\n", j);
  // Trop grosse différence de couleur, ce pixel appartiens a une autre tache
  if(abs((int)array[j].r - previous.r) > SEUIL || abs((int)array[j].g - previous.g) > SEUIL || abs((int)array[j].b - previous.b) > SEUIL){
    return;
  }

  array[j].explored = 1;
  cur_pound->limits[* it] = j;
  (* it)++;

  // récursion sur les voisins
  if(j +1 < sizeX * sizeY && array[j+1].explored != 1){
    explore(array, j + 1, cur_pound, it, previous, sizeX, sizeY);
    //explore(array, j + 1, cur_pound, it);//, last);
  }

  if(j > 1 && array[j-1].explored != 1){
    explore(array, j - 1, cur_pound, it, previous, sizeX, sizeY);
    //explore(array, j - 1, cur_pound, it);//, last);

  }

  if(j + sizeX < sizeX * sizeY && array[j + sizeX].explored != 1){
  	explore(array, j + sizeX, cur_pound, it, previous, sizeX, sizeY);
   	//explore(array, j + sizeX, cur_pound, it);//, last);
  }

  if(j - sizeX > 0 && array[j - sizeX].explored != 1){
    explore(array, j - sizeX, cur_pound, it, previous, sizeX, sizeY);
    //explore(array, j - sizeX, cur_pound, it);//, last);

  }
}

GLubyte getAvgRed(Pixel * array, int * indexs, int size){
  int tmp = 0;
  GLubyte avg = 0;
  int i;

  for(i = 0; indexs[i] != -1 && i < size; i++){
    tmp += array[indexs[i]].r;
  }
  avg = tmp / i;

  return avg; 
}

GLubyte getAvgGreen(Pixel * array, int * indexs, int size){
  int tmp = 0;
  GLubyte avg = 0;
  int i;

  for(i = 0; indexs[i] != -1 && i < size; i++){
    tmp += array[indexs[i]].g;
  }
  avg = tmp / i;
  return avg; 
}

GLubyte getAvgBlue(Pixel * array, int * indexs, int size){
  int tmp = 0;
  GLubyte avg = 0;
  int i;

  for(i = 0; indexs[i] != -1 && i < size; i++){
    tmp += array[indexs[i]].b;
  }
  avg = tmp / i;
  return avg; 
}

Pixel * getSection(Pixel * picture, int startPos, int sizeX, int sizeY, Pixel * section){
    Pixel empty = {-1, -1, -1, -1};

    int startX, startY, x, y, i, it;

    it = 0;
    startY = startPos / sizeX;
    startX = startPos - (startY * sizeX);

    for(y = startY; y < startY + 15; y++){
        for(x = startX; x < startX + 15; x++){
            if(x != startX && x >= sizeX){
                section[it] = empty;
                it++;
            }
            else if(y >= sizeY){
                section[it]= empty;
                it++;
            }
            else {
                section[it] = picture[x + sizeX * y];
                it++;
            }
        }
    }
    return section;
}

void oldmain (Image * i){
    int j, k;
    int sizeX, sizeY, size;
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

    int matrix = 15;
    int nbmatX = 0;
    int nbmatY = 0;

    Pixel * section;
    section = malloc(matrix * matrix * sizeof(Pixel));

    nbmatX = sizeX / matrix;
    if(sizeX % matrix != 0){
        nbmatX = (sizeX / matrix) + 1;
    }    
    nbmatY = sizeY / matrix;
    if(sizeY % matrix != 0){
        nbmatY = (sizeY / matrix) + 1;
    }
    for(int y = 0; y < nbmatY; y++){
        printf("%d / %d\n", y, nbmatY - 1);
        for(int x = 0; x < nbmatX; x++){
          int toget = (x * matrix) + sizeX * (y * matrix);
          section = getSection(array, toget, sizeX, sizeY, section);
          // taches de couleur
          section = taches_de_couleur(section, matrix, matrix);
          // fct ecriture
          rewirteImage(im, section, toget, sizeX, sizeY);
        }
    }
    free(array);
    free(section);
}

void rewirteImage(GLubyte * im, Pixel * section, int startPos, int sizeX, int sizeY){
    int it, startY, startX, x, y;
    it = 0;
    startY = startPos / sizeX;
    startX = startPos - (startY * sizeX);

    for(y = startY; y < startY + 15; y++){
      if(y >= sizeY){
        break;
      }
      for(x = startX; x < startX + 15; x++){
        if(x >= sizeX){
          break;
        }
        im[3 * (x + sizeX * y)] = section[(x - startX) + 15 * (y - startY)].r;
        im[3 * (x + sizeX * y) + 1] = section[(x - startX) + 15 * (y - startY)].g;
        im[3 * (x + sizeX * y) + 2] = section[(x - startX) + 15 * (y - startY)].b;
      }
    }
}

//disatnce euclidienne pour les ecarts de couleur ?