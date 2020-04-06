/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>
/** Simplifies an array by making pounds out of similar pixels
 * array is the original chunk of image that needs to be simplified
 * sizeX and sizeY are the dimensions of the chunk
 * returns the modified chunk of pixels
 **/
Pixel * tachesDeCouleur(Pixel * array, int sizeX, int sizeY){
    int y, it, j, k;
    int size = sizeX * sizeY;
    // for all pixels in array
    for(y = 0; y < size; y++){
      // skip if it is explored
      if(array[y].explored == 1){
        continue;
      }else{
        // create a new pound
        Tache * new_pound;
        new_pound = malloc(sizeof(Tache));
        new_pound->limits = malloc(size * sizeof(int));

        int x= 0;
        while(x < size){
          new_pound->limits[x] = -1;
          x++;
        }
        it = 0;
        // expand the pound
        explore(array, y, new_pound, &it, array[y], sizeX, sizeY);  

        // get its average color
        new_pound->r = getAvgRed(array, new_pound->limits, size);
        new_pound->g = getAvgGreen(array, new_pound->limits, size);
        new_pound->b = getAvgBlue(array, new_pound->limits, size);

        k = 0;
        // replace the array data with generated pound data for applicable pixels
        for(j = 0; j < size; j++){
          for(k = 0; new_pound->limits[k] != -1; k++){
            //when the curent pixel is in the pound
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
    // uncomment the following code bloc and comment the above for loop
    // to only create a single pound per chunk

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
      }*/
    return array;

}

/** recusrive fonction that expands around a pixel to create a pound
 * array is the chunk of pixels we are working on (the matrix from getSection)
 * j is the 1D index of the current Pixel inside array
 * cur_pound is the current pound we are expanding
 * it is the index of cur_pound->limits where the next pixel will be added
 * previous is the reference pixel to which we compare the color
 * sizeX and sizeY are the size of array
 */
void explore(Pixel * array, int j, Tache * cur_pound, int * it, Pixel previous, int sizeX, int sizeY){
  // Too big color delta, this pixel does not fit in the current pound
  if(abs((int)array[j].r - previous.r) > SEUIL || abs((int)array[j].g - previous.g) > SEUIL || abs((int)array[j].b - previous.b) > SEUIL){
    return;
  }

  // mark the pixel as explored to prevent duplicates and add it to the limits list
  array[j].explored = 1;
  cur_pound->limits[* it] = j;
  (* it)++;

  // recurse on neighboring pixels that :
  //    - are not outside the image
  //    - are not explored yet
  if(j +1 < sizeX * sizeY && array[j+1].explored != 1){
    explore(array, j + 1, cur_pound, it, previous, sizeX, sizeY);
  }
  if(j > 1 && array[j-1].explored != 1){
    explore(array, j - 1, cur_pound, it, previous, sizeX, sizeY);
  }
  if(j + sizeX < sizeX * sizeY && array[j + sizeX].explored != 1){
  	explore(array, j + sizeX, cur_pound, it, previous, sizeX, sizeY);
  }
  if(j - sizeX > 0 && array[j - sizeX].explored != 1){
    explore(array, j - sizeX, cur_pound, it, previous, sizeX, sizeY);
  }
}

/** get the average red color of all indexes
 * array is the Pixel array representing the image
 * indexs is an array of index representing the pound to extract the average from
 * size is the size of the chunk we are working on
 * returns the average red color of the pound
 */
GLubyte getAvgRed(Pixel * array, int * indexs, int size){
  int tmp = 0;
  GLubyte avg = 0;
  int i;
  // get the sum of the r fields from array that are included in indexs
  for(i = 0; indexs[i] != -1 && i < size; i++){
    tmp += array[indexs[i]].r;
  }
  // get average
  avg = tmp / i;

  return avg; 
}

/** get the average green color of all indexes
 * array is the Pixel array representing the image
 * indexs is an array of index representing the pound to extract the average from
 * size is the size of the chunk we are working on
 * returns the average green color of the pound
 */
GLubyte getAvgGreen(Pixel * array, int * indexs, int size){
  int tmp = 0;
  GLubyte avg = 0;
  int i;
  // get the sum of the g fields from array that are included in indexs
  for(i = 0; indexs[i] != -1 && i < size; i++){
    tmp += array[indexs[i]].g;
  }
  //get average
  avg = tmp / i;
  return avg; 
}

/** get the average blue color of all indexes
 * array is the Pixel array representing the image
 * indexs is an array of index representing the pound to extract the average from
 * size is the size of the chunk we are working on
 * returns the average blue color of the pound
 */
GLubyte getAvgBlue(Pixel * array, int * indexs, int size){
  int tmp = 0;
  GLubyte avg = 0;
  int i;
  // get the sum of the b fields from array that are included in indexs
  for(i = 0; indexs[i] != -1 && i < size; i++){
    tmp += array[indexs[i]].b;
  }
  //get the average
  avg = tmp / i;
  return avg; 
}

/** fill up current section starting at startpos
 * picture is the array of Pixels representing the full image
 * startpos is the 1D index of the starting position of the section relative to picture
 * sizeX and sizeY are the dimensions of the image
 * section is the section that needs to be filled up
 * returns this section
 */
Pixel * getSection(Pixel * picture, int startPos, int sizeX, int sizeY, Pixel * section){
    Pixel empty = {-1, -1, -1, -1};
    int startX, startY, x, y, it;

    it = 0;
    // get 2D coordinates of the 1D coordinates startPos 
    startY = startPos / sizeX;
    startX = startPos - (startY * sizeX);

    // fill up the section
    for(y = startY; y < startY + 15; y++){
        for(x = startX; x < startX + 15; x++){
            // out of bounds in X axis : case the section only partially covers the image
            if(x != startX && x >= sizeX){
                // fill it up with empty data
                section[it] = empty;
                it++;
            }
            // out of bounds in Y axis : case the section only partially covers the image
            else if(y >= sizeY){
                // fill it up with empty data
                section[it]= empty;
                it++;
            }
            else {
                // fill it up with Pixel array data corresponding to these coordinates
                section[it] = picture[x + sizeX * y];
                it++;
            }
        }
    }
    return section;
}

/** start the conversion process
 * i is the raw displayed image
 */
void tachesInit (Image * i){
    int j, k;
    int sizeX, sizeY, size, matrix, nbmatX, nbmatY;
    GLubyte * im;
    Pixel * array;
    Pixel * section;


    sizeX = i->sizeX;
    sizeY = i->sizeY;
    size = sizeX * sizeY;
    matrix = 15;
    nbmatX = 0;
    nbmatY = 0;
    k = 0;
    array = malloc(size * sizeof(Pixel));
    section = malloc(matrix * matrix * sizeof(Pixel));
    im = i->data;

    /* convert image data into Pixel structure */
    for(j = 0; j < 3 * size; j+= 3){
       array[k].explored = 0;
       array[k].r = i->data[j];
       array[k].g = i->data[j + 1];
       array[k].b = i->data[j + 2];
       //printf("%d : %d %d %d\n", k, array[k].r, array[k].g, array[k].b);
       k++;
    }

    // see how many matrix are needed to get all the image 
    nbmatX = sizeX / matrix;
    if(sizeX % matrix != 0){
        nbmatX = (sizeX / matrix) + 1;
    }    
    nbmatY = sizeY / matrix;
    if(sizeY % matrix != 0){
        nbmatY = (sizeY / matrix) + 1;
    }
    // for each required matrix
    for(int y = 0; y < nbmatY; y++){
        printf("%d / %d\n", y, nbmatY - 1);
        for(int x = 0; x < nbmatX; x++){
          // get 1D index of starting pos of the matrix
          int toget = (x * matrix) + sizeX * (y * matrix);
          // fill up the matrix
          section = getSection(array, toget, sizeX, sizeY, section);
          // get corresponding color pounds for this matrix
          section = tachesDeCouleur(section, matrix, matrix);
          // write the new colors from this area of the image into the displayed one
          rewirteImage(im, section, toget, sizeX, sizeY);
        }
    }
    free(array);
    free(section);
}

/** rewrites the color data from a matrix.
  * im is the raw displayed image
  * section is a Pixel array representing the current matrix we are working on
  * startPos is the 1D index in the iamge of the starting position of section
  * sizeX and sizeY are the image dimensions
  */
void rewirteImage(GLubyte * im, Pixel * section, int startPos, int sizeX, int sizeY){
    int startY, startX, x, y;
    // get 2D coordinates from starting 1D position
    startY = startPos / sizeX;
    startX = startPos - (startY * sizeX);

    // iterate over the raw image using the smaller section of image
    for(y = startY; y < startY + 15; y++){
      // out of bounds in section on Y axis
      if(y >= sizeY){
        break;
      }
      for(x = startX; x < startX + 15; x++){
        // out of bounds in section on X axis
        if(x >= sizeX){
          break;
        }
        // update image using section data with offset for r g b
        im[3 * (x + sizeX * y)] = section[(x - startX) + 15 * (y - startY)].r;
        im[3 * (x + sizeX * y) + 1] = section[(x - startX) + 15 * (y - startY)].g;
        im[3 * (x + sizeX * y) + 2] = section[(x - startX) + 15 * (y - startY)].b;
      }
    }
}

//disatnce euclidienne pour les ecarts de couleur ?