#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>
#define SIZE 513

int main() {

	float diff,tmp;
	
	float (*w)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	float (*x)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	float (*y)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
	float scratchpad[8];
	for (int i=0; i< SIZE; i++) {
		for (int j=0; j< SIZE; j++) {
			w[i][j]=(float)(rand()%1000)/800.0f;
			x[i][j]=(float)(rand()%1000)/800.0f;
			y[i][j]=(float)(rand()%1000)/800.0f;
		}
	}
	

	clock_t t1, t2;

	/*t1 = clock();
	for(int i= 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			for (int k = 0; k < SIZE; ++k) {
				tmp += w[i][k] * x[k][j];
				
			}
			y[i][j] = tmp;
		}	
	}
	t2 = clock();

	printf("%f  : \n",(double)(t2 - t1)/CLOCKS_PER_SEC);*/

	/*
	 *Only works for square matrices.To handle
	 *rectangular matrices,either simply pad them to the closest
	 *power of 32,or add conditions to handle it.
	 *Note: The second approach may lead to some very ugly intrinsic calc.
	 */ 
	
	__m256 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7,
	ymm8, ymm9, ymm10, ymm11, ymm12, ymm13, ymm14, ymm15;

	t1 = clock();
	const int SIZE_reduced = SIZE - SIZE%64;
	const int SIZE_reduced_32 = SIZE - SIZE%32;
	for (int i=0; i < SIZE; i++) {
		for(int l=0; l < SIZE;l++){
			float res = 0;
			for (int j=0; j<SIZE_reduced; j+=64) {
				ymm8 = __builtin_ia32_loadups256(&x[j][l]);
				ymm9 = __builtin_ia32_loadups256(&x[j+8][l]);
				ymm10 = __builtin_ia32_loadups256(&x[j+16][l]);
				ymm11 = __builtin_ia32_loadups256(&x[j+24][l]);
				ymm12 = __builtin_ia32_loadups256(&x[j+32][l]);
				ymm13 = __builtin_ia32_loadups256(&x[j+40][l]);
				ymm14 = __builtin_ia32_loadups256(&x[j+48][l]);
				ymm15 = __builtin_ia32_loadups256(&x[j+56][l]);

				ymm0 = __builtin_ia32_loadups256(&w[i][j]);
				ymm1 = __builtin_ia32_loadups256(&w[i][j+8]);
				ymm2 = __builtin_ia32_loadups256(&w[i][j+16]);
				ymm3 = __builtin_ia32_loadups256(&w[i][j+24]);
				ymm4 = __builtin_ia32_loadups256(&w[i][j+32]);
				ymm5 = __builtin_ia32_loadups256(&w[i][j+40]);
				ymm6 = __builtin_ia32_loadups256(&w[i][j+48]);
				ymm7 = __builtin_ia32_loadups256(&w[i][j+56]);

				ymm0 = __builtin_ia32_mulps256(ymm0, ymm8 );
				ymm1 = __builtin_ia32_mulps256(ymm1, ymm9 );
				ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
				ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);
				ymm4 = __builtin_ia32_mulps256(ymm4, ymm12);
				ymm5 = __builtin_ia32_mulps256(ymm5, ymm13);
				ymm6 = __builtin_ia32_mulps256(ymm6, ymm14);
				ymm7 = __builtin_ia32_mulps256(ymm7, ymm15);

				ymm0 = __builtin_ia32_addps256(ymm0, ymm1);
				ymm2 = __builtin_ia32_addps256(ymm2, ymm3);
				ymm4 = __builtin_ia32_addps256(ymm4, ymm5);
				ymm6 = __builtin_ia32_addps256(ymm6, ymm7);
				ymm0 = __builtin_ia32_addps256(ymm0, ymm2);
				ymm4 = __builtin_ia32_addps256(ymm4, ymm6);
				ymm0 = __builtin_ia32_addps256(ymm0, ymm4);

				__builtin_ia32_storeups256(scratchpad, ymm0);

				for (int k=0; k<8; k++)
					res += scratchpad[k];
			}
			for (int j=SIZE_reduced; j<SIZE_reduced_32; j+=32) {
				ymm8 = __builtin_ia32_loadups256(&x[j][i]);
				ymm9 = __builtin_ia32_loadups256(&x[j+8][i]);
				ymm10 = __builtin_ia32_loadups256(&x[j+16][i]);
				ymm11 = __builtin_ia32_loadups256(&x[j+24][i]);

				ymm0 = __builtin_ia32_loadups256(&w[i][j]);
				ymm1 = __builtin_ia32_loadups256(&w[i][j+8]);
				ymm2 = __builtin_ia32_loadups256(&w[i][j+16]);
				ymm3 = __builtin_ia32_loadups256(&w[i][j+24]);

				ymm0 = __builtin_ia32_mulps256(ymm0, ymm8 );
				ymm1 = __builtin_ia32_mulps256(ymm1, ymm9 );
				ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
				ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);

				ymm0 = __builtin_ia32_addps256(ymm0, ymm1);
				ymm2 = __builtin_ia32_addps256(ymm2, ymm3);
				ymm0 = __builtin_ia32_addps256(ymm0, ymm2);

				__builtin_ia32_storeups256(scratchpad, ymm0);
				for (int k=0; k<8; k++)
					res += scratchpad[k];
			}
			for (int j=SIZE_reduced_32; j < SIZE; j++) {
				res += w[i][j] * x[j][l];
			}
			y[i][l] = res;
		}
	}
	t2 = clock();
	free(w);
	free(x);
	free(y);
	printf("%f  : \n",(double)(t2 - t1)/CLOCKS_PER_SEC);
	return 0;
}