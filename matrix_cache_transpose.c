#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 1024
int main(int argc, const char * argv[]) {

	float (*matrix1)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	float (*matrix2)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	float (*matrix3)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	int i,j,k;
	for(i= 0; i<SIZE; ++i) {
		for (j = 0; j < SIZE; j++) {
			matrix1[i][j] = j;
			matrix2[j][i] = i;
			matrix3[i][j] = 0;
		}
	}
	clock_t t1 = clock();
	float tmp = 0;
	for(i= 0; i<SIZE; ++i) {
		for (j = 0; j < SIZE; ++j) {
			for (k = 0; k < SIZE; ++k) {

				tmp += matrix1[i][k] * matrix2[j][k];
				
			}
			matrix3[i][j] = tmp;
		}
	}

	clock_t t2 = clock();
	free(matrix1);
	free(matrix2);
	free(matrix3);

	printf("\n%f",(double)(t2 - t1)/CLOCKS_PER_SEC);
	return 0;
}