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

int allocMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U , int n){
	M->a = malloc(n*n*sizeof(double));
	if(M->a == NULL){
		return -4;
	}

	N->b = malloc(n*sizeof(double));
	if(N->b == NULL){
		return -4;
	}

	D->a = malloc(n*n*sizeof(double));
	if(D->a == NULL){
		return -4;
	}

	U->a = malloc(n*n*sizeof(double));
	if(U->a == NULL){
		return -4;
	}

	U->a = malloc(n*n*sizeof(double));
	if(U->a == NULL){
		return -4;
	}
}

void freeMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U){
	free(M->a);
	free(M);
	free(N->b);
	free(N);
	free(D->a);
	free(D);
	free(L->a);
	free(L);
	free(U->a);
	free(U);
}

int calculateRDiagonal(matrix_t *A, matrix_t *D){
	int i, j;
	int n;
	int pos;
	n = A->n;
	for(i = 0; i < n ; ++i){
		for(j = 0; j <n; ++j){
			pos = i * n + j;
			if(j == i){
				D->a[pos] = 1/(A->a[pos]);
			}else{
				D->a[pos] = 0;
			}
		}
	}
	return 0;
}



int calculateJacobi(matrix_t A, vector_t B, vector_t* X, int* beginIndexes, int* endIndexes) {
	int rank, procSize;
	int i, j;
	matrix_t M,N,D,L,U;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		cleanResult(X);
	}

	if(rank == 0){
		if(allocMemory(&M,&N,&D,&L,&U, M.n) != 0){
			freeMemory(&M,&N,&D,&L,&U);
			return -1;
		}
		//calculateRDiagonal(&A, &D);
		freeMemory(&M,&N,&D,&L,&U);
	}


	return 0;
}

