#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))//these are not type safe. Only use with int and float maybe
#define MIN(x, y) (((x) < (y)) ? (x) : (y))//see http://stackoverflow.com/questions/3437404/min-and-max-in-c
#define MSIZE 1024
#define TILE_SIZE 4
int main(int argc, const char * argv[]) {

	float (*matrix1)[MSIZE] = malloc(sizeof(float[MSIZE][MSIZE]));
	float (*matrix2)[MSIZE] = malloc(sizeof(float[MSIZE][MSIZE]));
	float (*matrix3)[MSIZE] = malloc(sizeof(float[MSIZE][MSIZE]));
	int i,j,k,i2,j2,k2;
	float* rres,rmul1,rmul2;
	for(i= 0; i<MSIZE; ++i) {
		for (j = 0; j < MSIZE; j++) {
			matrix1[i][j] = j;
			matrix2[j][i] = i;
			matrix3[i][j] = 0;
		}
	}
	//clock_t c1 =clock();
	float tmp = 0;
	
	for(i= 0; i<MSIZE; i = i+TILE_SIZE) {
		//printf("In loop 1\n" );
		for (j = 0; j < MSIZE; j = j+TILE_SIZE) {
			//printf("In loop 2\n" );
			for (k = 0; k < MSIZE; k = k+TILE_SIZE) {
				//printf("In loop 3\n" );
				for (i2=0,rres = &matrix3[i][j],rmul1 = &matrix1[i][k];i2<TILE_SIZE;++i2,rres+=MSIZE,rmul1+=MSIZE) {
					//printf("In loop 4\n" );
					for (k2=0,rmul2=&matrix2[k][j];k2<TILE_SIZE;++k2,rmul2+=MSIZE) {
						//printf("In loop 5\n" );
						for (j2=0;j2<TILE_SIZE;++j2) {
							//printf("In loop 6\n" );

							rres[j2] += rmul1[k2] * rmul2[j2];
				
						}
						
					}
				}
			}
		}
	}
	
	//clock_t c2 = clock();
	free(matrix1);
	free(matrix2);
	free(matrix3);
	//printf("%f",(double)(c1 - c2)/CLOCKS_PER_SEC);
	
	return 0;
}