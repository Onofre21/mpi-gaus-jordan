/*
 * jacobiutil.c
 *
 *  Created on: Jun 5, 2011
 *      Author: lewy
 */

#include "headers/jacobiutil.h"

int checkMatrix(matrix_t *A){
	int i, j,pos, diagPos, rowSize, dataSize;
	double sumValue;
	rowSize = A->n;

	for(i = 0; i < rowSize; ++i){
		sumValue = 0;
		diagPos = i * rowSize + i;
		for(j = 0; j < rowSize; ++j){
			pos = i * rowSize + j;
			//printf("A %g \n", A->a[pos]);

			if(pos != diagPos){
				sumValue += abs(A->a[pos]);
			}
		}
		if(sumValue > abs(A->a[diagPos])){
			printf("\n\n Warunek nie sprawdził się dla wiersza %d.\n", i);
			return -1;
		}
	}
	return 0;
}

int allocMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U, vector_t *X, int n) {
	int i;
	M->a = malloc(n * n * sizeof(double));
	if (M->a == NULL) {
		return -4;
	}

	N->b = malloc(n * sizeof(double));
	if (N->b == NULL) {
		return -4;
	}

	D->a = malloc(n * n * sizeof(double));
	if (D->a == NULL) {
		return -4;
	}

	L->a = malloc(n * n * sizeof(double));
	if (L->a == NULL) {
		return -4;
	}

	U->a = malloc(n * n * sizeof(double));
	if (U->a == NULL) {
		return -4;
	}

	X->b = malloc(n * sizeof(double));
	if (X->b == NULL) {
		return -4;
	}

	return 0;
}

void freeMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U) {
	free(M->a);
	free(N->b);
	free(D->a);
	free(L->a);
	free(U->a);
}

int allcMemoryAll(vector_t *XResult, vector_t *XResultOld, int **beginIndexes, int **endIndexes, int rowSize, int rank, int procSize) {
	int i;
	XResult->b = calloc(rowSize, sizeof(double));
	XResultOld->b = malloc(rowSize * sizeof(double));

	for (i = 0; i < rowSize; i++) {
		XResultOld->b[i] = 100.0;
	}

	if (rank != 0) {
		*beginIndexes = malloc(procSize * sizeof(int));
		*endIndexes = malloc(procSize * sizeof(int));
	}
	if (XResult->b == NULL || *beginIndexes == NULL || *endIndexes == NULL || XResultOld->b == NULL) {
		return -4;
	}
	return 0;
}

void freeMemoryAll(int **beginIndexes, int **endIndexes, vector_t *XResult, vector_t *XResultOld){
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	free(XResult->b);
	free(XResultOld->b);
	if (rank != 0) {
		free(*beginIndexes);
		free(*endIndexes);
	}
}

void calculateRDiagonal(matrix_t *A, matrix_t *D) {
	int i, j;
	int n;
	int pos;
	int size;
	n = A->n;
	size = n * n;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			pos = i * n + j;
			if (j == i) {
				D->a[pos] = 1 / (A->a[pos]);
			} else {
				D->a[pos] = 0;
			}
		}
	}
	D->n = A->n;
	//	for(i = 0 ; i < size; ++i){
	//		printf("Diagonala %g \n", D->a[i]);
	//	}
}

void calculateL(matrix_t *A, matrix_t *L) {
	int i, j;
	int n;
	int pos;
	int size;
	n = A->n;
	size = n * n;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			pos = i * n + j;
			if (j < i) {
				L->a[pos] = A->a[pos];
			} else {
				L->a[pos] = 0;
			}
		}
	}
	L->n = A->n;
	//for(i = 0 ; i < size; ++i){
	//	printf("Lower %g \n", L->a[i]);
	//}
}

void calculateU(matrix_t *A, matrix_t *U) {
	int i, j;
	int n;
	int pos;
	int size;
	n = A->n;
	size = n * n;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			pos = i * n + j;
			if (j > i) {
				U->a[pos] = A->a[pos];
			} else {
				U->a[pos] = 0;
			}
		}
	}
	U->n = A->n;
	//  for(i = 0 ; i < size; ++i){
	//	  printf("Lower %g \n", U->a[i]);
	//	}
}

void calculateM(matrix_t *M, matrix_t *D, matrix_t *L, matrix_t *U) {
	int i, j, p;
	int n;
	int pos;
	int size;
	double tmp;
	n = D->n;
	size = n * n;
	matrix_t LU;
	LU.a = malloc(n * n * sizeof(double));

	//Dodanie L + U
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			pos = i * n + j;
			LU.a[pos] = L->a[pos] + U->a[pos];
		}
	}

	//Mnożenie przez Diagonala odwrocona
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			pos = i * n + j;
			if (j == i) {
				M->a[pos] = 0;
			} else {
				tmp = 0;
				for (p = 0; p < n; ++p) {
					tmp = tmp + D->a[i * n + p] * LU.a[p * n + j];
				}
				M->a[pos] = tmp;
			}
		}
	}

	//Zmiana znaku
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			pos = i * n + j;
			M->a[pos] = -(M->a[pos]);
		}
	}

	//	   for(i = 0 ; i < size; ++i){
	//		   printf("L %g \n", L->a[i]);
	//		}
	//	   for(i = 0 ; i < size; ++i){
	//		   printf("U %g \n", U->a[i]);
	//		}
	//	   for(i = 0 ; i < size; ++i){
	//		   printf("LU %g \n", LU.a[i]);
	//		}
	//	   for(i = 0 ; i < size; ++i){
	//		   printf("D %g \n", D->a[i]);
	//		}
//		   for(i = 0 ; i < size; ++i){
//			  printf("M %g \n", M->a[i]);
//			}
	free(LU.a);
}

void calculateN(vector_t *N, matrix_t *D, vector_t *B) {
	int i, j, n;
	double tmp;
	int pos;
	n = D->n;
	N->n = D->n;

	for (i = 0; i < n; ++i) {
		tmp = 0;
		for (j = 0; j < n; ++j) {
			pos = i * n + j;
			tmp = tmp + (D->a[pos] * B->b[j]);
		}
		N->b[i] = tmp;
	}
	//		for(i = 0 ; i < n; ++i){
	//			   printf("B %g \n", B->b[i]);
	//		}
	//		for(i = 0 ; i < n*n; ++i){
	//			printf("D %g \n", D->a[i]);
	//		}
	//		for(i = 0 ; i < n; ++i){
	//			printf("N %g \n", N->b[i]);
	//		}
}
