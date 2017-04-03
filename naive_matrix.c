#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))//these are not type safe. Only use with int and float maybe
#define MIN(x, y) (((x) < (y)) ? (x) : (y))//see http://stackoverflow.com/questions/3437404/min-and-max-in-c
#define SIZE 1024

int main(int argc, const char * argv[]) {

	float (*matrix1)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	float (*matrix2)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	float (*matrix3)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	int i,j,k,a,b,c;
	for(i= 0; i<SIZE; ++i) {
		for (j = 0; j < SIZE; j++) {
			matrix1[i][j] = j;
			matrix2[i][j] = i;
			matrix3[i][j] = 0;
		}
	}
	clock_t c1 =clock();
	float tmp = 0;
	for(i= 0; i<SIZE; ++i) {
		for (j = 0; j < SIZE; ++j) {
			for (k = 0; k < SIZE; ++k) {
				matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
				
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