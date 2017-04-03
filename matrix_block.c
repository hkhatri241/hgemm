#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))//these are not type safe. Only use with int and float maybe
#define MIN(x, y) (((x) < (y)) ? (x) : (y))//see http://stackoverflow.com/questions/3437404/min-and-max-in-c
#define MSIZE 1024
#define TILE_SIZE 6 //size 4 should give optimal as 4*4*4 = 64 bytes which is the cache line size but doesnt. Has to do with presence 
int main(int argc, const char * argv[]) { //of l2 cache too.

	float (*matrix1)[MSIZE] = malloc(sizeof(float[MSIZE][MSIZE]));
	float (*matrix2)[MSIZE] = malloc(sizeof(float[MSIZE][MSIZE]));
	float (*matrix3)[MSIZE] = malloc(sizeof(float[MSIZE][MSIZE]));
	int i,j,k,a,b,c;
	for (int i=0; i< MSIZE; i++) {
		for (int j=0; j< MSIZE; j++) {
			matrix1[i][j]=(float)(rand()%1000)/800.0f;
			matrix2[i][j]=(float)(rand()%1000)/800.0f;
			matrix3[i][j]=(float)(rand()%1000)/800.0f;
		}
	}
	clock_t c1 =clock();
	float tmp = 0;
	for(j= 0; j<MSIZE; j = j+TILE_SIZE) { //changing the order of access results in better multi level cache behavious here. See
		//printf("In loop 1\n" );
		for (i = 0; i < MSIZE; i = i+TILE_SIZE) { //software.intel.com/en-us/articles/efficient-use-of-tiling
			//printf("In loop 2\n" );
			for (k = 0; k < MSIZE; k = k+TILE_SIZE) {
				//printf("In loop 3\n" );
				for (a = i; a < MIN(MSIZE, i+TILE_SIZE); a++) {
					//printf("In loop 4\n" );
					for (b = j; b < MIN(MSIZE, j+TILE_SIZE); b++) {
						//printf("In loop 5\n" );
						for (c = k; c < MIN(MSIZE, k+TILE_SIZE); c++) {
							//printf("In loop 6\n" );

							tmp += matrix1[a][c] * matrix2[c][b];
				
						}
						matrix3[a][b] = tmp;
					}
				}
			}
		}
	}
	
	clock_t c2 = clock();
	free(matrix1);
	free(matrix2);
	free(matrix3);
	printf("%f",(double)(c1 - c2)/CLOCKS_PER_SEC);
	
	return 0;
}