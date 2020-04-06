#include <stdio.h>
#include <stdlib.h>
struct Test{
    int x;
};
typedef struct Test Test;

Test * getSection(Test * testArray, int startPos, int sizeX, int sizeY){
    Test * section;
    int startX, startY, x, y, i, it;

    section = malloc(100 * 100 * sizeof(Test));
    it = 0;
    startY = startPos / sizeX;
    startX = startPos - (startY * sizeX);

    for(y = startY; y < startY + 100; y++){
        for(x = startX; x < startX + 100; x++){
            if(x != startX && x >= sizeX){
                section[it].x = -1;
                it++;
            }
            else if(y >= sizeY){
                section[it].x = -1;
                it++;
            }
            else {
                section[it].x = testArray[x + sizeX * y].x;
                it++;
            }
        }
    }
    return section;
}


void rewirteImage(int startPos, int sizeX, int sizeY){
    int it, startY, startX, x, y;
    /*int k = 0;
    for(int j = 0; j < 3 * size; j+=3){
      im[j] = section[k].r ;
      im[j+1] = section[k].g ;
      im[j + 2] = section[k].b;
      k++;
    }*/
    it = 0;
    startY = startPos / sizeX;
    startX = startPos - (startY * sizeX);
    printf("pre process\n");
    for(y = startY; y < startY + 100; y++){
      if(y >= sizeY){
        break;
      }
      for(x = startX; x < startX + 100; x++){
        if(x >= sizeX){
          break;
        }
        //printf("count : %d ", count);
        printf("%d %d %d\n", x, y, 3 * (x + sizeX * y));
        //im[count +(x + startX * y)] = section[x + startX * y].r;
        //im[count +(x + startX * y) + 1] = section[x + startX * y].g;
        //im[count +(x + startX * y) + 2] = section[x + startX * y].b;
        //count += 2;
      }
      /*if(startX + 100 < sizeX){
        printf("depassement : %d add %d \n", sizeX - (startX + 100), ((sizeX - (startX + 100)) - 2)* 3);
        count += ((sizeX - (startX + 100)) - 2)* 3;
        //count += 3 * 
      }*/
    }
}




int main(int argc, char const *argv[])
{
    int dimX = 1024;
    int dimY = 768;
    int matrix = 100;
    int nbmatX = 0;
    int nbmatY = 0;

    /*Test * testArray;
    Test * section;
    testArray = malloc(dimX * dimY * sizeof(Test));
    section = malloc(matrix * matrix * sizeof(Test));
    for(int i =0; i < dimX * dimY; i++){
        testArray[i].x = i;
    }

    nbmatX = dimX / matrix;
    if(dimX % matrix != 0){
        nbmatX = (dimX / matrix) + 1;
    }    
    nbmatY = dimY / matrix;
    if(dimY % matrix != 0){
        nbmatY = (dimY / matrix) + 1;
    }

    for(int y = 0; y < nbmatY; y++){
        for(int x = 0; x < nbmatX; x++){
            int toget = (x * matrix) + dimX * (y * matrix);
            section = getSection(testArray, toget, dimX, dimY);
        }
    }*/
    rewirteImage(0, 110, 110);
    return 0;
}
// x + sizeX * Y

