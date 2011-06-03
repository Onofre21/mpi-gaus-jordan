/*
 * jacobi.c
 *
 *  Created on: Jun 2, 2011
 *      Author: lewy
 */

#include"headers/jacobi.h"

//void cleanResult(vector_t *X, int n) {
//	int i;
//	X->b = malloc(n * sizeof(double));
//	for (i = 0; i < n; ++i) {
//		X->b[i] = 0;
//	}
//}

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
	//	   for(i = 0 ; i < size; ++i){
	//		  printf("M %g \n", M->a[i]);
	//		}
	free(LU.a);
	return 0;
}

int calculateN(vector_t *N, matrix_t *D, vector_t *B) {
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

int getDelta(vector_t *XResult, vector_t *XResult_Old, int rowSize) {
	int i;
	double sum = 0;
	printf("Sprawdzam poprawnosc rozwiazania");
	for (i = 0; i < rowSize; i++) {
		sum += abs(XResult_Old->b[i] - XResult->b[i]);
	}
	sum = sum / rowSize;
	printf("Suma: %g \n", sum);
	for (i = 0; i < rowSize; i++) {
		XResult_Old->b[i] = XResult->b[i];
	}
	return sum;
}

void calculateX(int row, int nrows, vector_t *XResult, double *localM, double localN, int rowSize) {
	int i, rank, stop;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double result = 0;
	stop = nrows;
	while (stop > 0) {
		printf("Liczenie rank :%d \n", rank);
		for (i = 0; i < rowSize; i++) {
			//printf("LocalM %g, X %g, localN %g, \n", localM[i], X->b[i], localN);
			result = result + localM[i] * XResult->b[i];
		}
		result = result + localN;
		printf("Dla wiersza %d, rozwiązanie wyszło %g \n", row, result);
		XResult->b[row] = result;
		stop--;
	}
	//MPI_Bcast(&(XResult->b[row]), 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
}

int calculateJacobi(matrix_t A, vector_t B, vector_t* X, int* beginIndexes, int* endIndexes) {
	int rank, procSize, i, j, nrows, dataSize, rowSize, localStart;
	double *localM;
	double localN;
	MPI_Status status;

	matrix_t M, D, L, U;
	vector_t N;
	vector_t XResult, XResultOld;

	if (rank == 0) {
		dataSize = A.n * A.n;
		rowSize = A.n;
	}

	MPI_Bcast(&dataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&rowSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		if (allocMemory(&M, &N, &D, &L, &U, &N, A.n) != 0) {
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
	//		if (rank == 0) {
	//			for (i = 0; i < procSize; i++) {
	//				printf("Begin Indexes %d, End Indexes %d \n", beginIndexes[i],
	//						endIndexes[i]);
	//			}
	//		}

	XResult.b = malloc(rowSize * sizeof(double));
	XResultOld.b = malloc(rowSize * sizeof(double));
	if (XResult.b == NULL) {
		return -4;
	}
	for (i = 0; i < rowSize; i++) {
		XResult.b[i] = 1.0;
		XResultOld.b[i] = 100.0;
	}

	if (rank == 0) {
		for (i = 1; i < procSize; i++) {
			nrows = endIndexes[i] - beginIndexes[i] + 1;
			MPI_Send(&nrows, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&(beginIndexes[i]), 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			for (j = beginIndexes[i]; j <= endIndexes[i]; j++) {
				//	printf("Wysyłam RANK:%d !!!!\n ", i);
				MPI_Send(&(M.a[rowSize * j]), rowSize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
				MPI_Send(&(N.b[j]), 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
		}
		localM = (double*) malloc(rowSize * sizeof(double));
		for (j = 0; j < rowSize; j++) {
			localM[j] = M.a[j];
		}
		localN = N.b[0];
		localStart = beginIndexes[i];
		nrows = endIndexes[0] - beginIndexes[0] + 1;

	} else {
		MPI_Recv(&nrows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&localStart, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		if (nrows > 0) {
			localM = malloc(nrows * rowSize * sizeof(double));
		}

		for (i = 0; i < nrows; i++) {
			//	printf("Odbieram!!!! rank: %d\n ", rank);
			MPI_Recv(localM, rowSize * nrows, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Recv(&localN, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if (nrows >= 1) {
		printf("Rank %d.Nrows %d, dataSize %d, rowSize%d, localN %g, localStart %d  \n", rank, nrows, dataSize, rowSize, localN, localStart);
		printf("X value: %g\n ", X->b[2]);
		for (i = 0; i < rowSize * nrows; i++) {
			printf("Rank %d. Moje lokalne M %g. Moje rozwiązanie %g Stare %g\n", rank, localM[i], XResult.b[i], XResultOld.b[i]);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//LICZENIE
	int accuracy = 1000;
	while (accuracy > 5) {
		for (i = nrows; i >= 1; i--) {
			//printf("Na wejsciu : LocalM %g, X %g, localN %g, \n", localM[1], X->b[1], localN);
			calculateX(localStart, nrows, &XResult, localM, localN, rowSize);
		}
		accuracy = getDelta(&XResult, &XResultOld, rowSize);
		MPI_Barrier(MPI_COMM_WORLD);
		//TODO Wymien sie wynikami
	}

	if (rank == 0) {
		freeMemory(&M, &N, &D, &L, &U);
	}
	return 0;
}

