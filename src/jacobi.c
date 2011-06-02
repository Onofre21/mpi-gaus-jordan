/*
 * jacobi.c
 *
 *  Created on: Jun 2, 2011
 *      Author: lewy
 */

#include"headers/jacobi.h"

void cleanResult(vector_t *X) {
	int i;
	for (i = 0; i < X->n; ++i) {
		X->b[i] = 0;
	}
}

int allocMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L,
		matrix_t *U, int n) {
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
	return 0;
}

void freeMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U) {
	free(M->a);
	free(N->b);
	free(D->a);
	free(L->a);
	free(U->a);
}

int calculateRDiagonal(matrix_t *A, matrix_t *D) {
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
	return 0;
}

int calculateL(matrix_t *A, matrix_t *L) {
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
	return 0;
}

int calculateU(matrix_t *A, matrix_t *U) {
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
	return 0;
}

int calculateM(matrix_t *M, matrix_t *D, matrix_t *L, matrix_t *U) {
	int i, j, p;
	int n;
	int pos;
	int size;
	double tmp;
	n = D->n;
	size = n * n;
	matrix_t LU;
	LU.a = malloc( n * n *sizeof(double));

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
			if(j == i){
				M->a[pos] = 0;
			}else{
				tmp = 0;
				for(p = 0 ; p < n; ++p){
					tmp = tmp + D->a[i*n + p] * LU.a[p*n + j];
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

//   for(i = 0 ; i < size; ++i){
//	   printf("L %g \n", L->a[i]);
//	}
//   for(i = 0 ; i < size; ++i){
//	   printf("U %g \n", U->a[i]);
//	}
//   for(i = 0 ; i < size; ++i){
//	   printf("LU %g \n", LU.a[i]);
//	}
//   for(i = 0 ; i < size; ++i){
//	   printf("D %g \n", D->a[i]);
//	}
//   for(i = 0 ; i < size; ++i){
//	  printf("M %g \n", M->a[i]);
//	}
	free(LU.a);
	return 0;
}

int calculateN(vector_t *N, matrix_t *D, vector_t *B){
	int i, j, n;
	double tmp;
	int pos;
	n = D->n;
	N->n = D->n;

	for(i = 0 ; i < n ; ++i){
		tmp = 0;
		for(j = 0 ; j < n; ++j){
			pos = i * n + j;
			tmp = tmp + (D->a[pos] * B->b[j]);
		}
		N->b[i] = tmp;
	}
//	for(i = 0 ; i < n; ++i){
//		   printf("B %g \n", B->b[i]);
//	}
//	for(i = 0 ; i < n*n; ++i){
//		printf("D %g \n", D->a[i]);
//	}
//	for(i = 0 ; i < n; ++i){
//		printf("N %g \n", N->b[i]);
//	}
}

int calculateJacobi(matrix_t A, vector_t B, vector_t* X, int* beginIndexes,
		int* endIndexes) {
	int rank, procSize;
	int i, j, n;
	double *localM;
	double  localN;

	matrix_t M, N, D, L, U;

	n = A.n;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		cleanResult(X);
	}

	if (rank == 0) {
		if (allocMemory(&M, &N, &D, &L, &U, M.n) != 0) {
			freeMemory(&M, &N, &D, &L, &U);
			printError(-4);
			return -1;
		}
		calculateRDiagonal(&A, &D);
		calculateL(&A, &L);
		calculateU(&A, &U);
		calculateM(&M, &D, &L, &U);
		calculateN(&N, &D, &B);

	}

	if(rank == 0){
		for(i = 0 ;i< procSize; i++){
			printf("Begin Indexes %d, End Indexes %d \n",beginIndexes[i], endIndexes[i]);
		}
	}

	for(i = 0 ;i< procSize; i++){
		if((endIndexes[i] - beginIndexes[i] + 1)>1){
		}else{
			//Nic nie rób. Za mało równań, Za mało procesów
		}
	}


	for(i = 0 ; i < procSize; i++){

	}

	if(rank == 0){
		freeMemory(&M, &N, &D, &L, &U);
	}
	return 0;
}

