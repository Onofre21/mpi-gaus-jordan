/*
 * jacobi.c
 *
 *  Created on: Jun 2, 2011
 *      Author: lewy
 */

#include"headers/jacobi.h"

double getDelta(vector_t *XResult, vector_t *XResult_Old, int rowSize, int nrows) {
	int i;
	double sum = 0;
	double tmp = 0;
//	printf("Sprawdzam poprawnosc rozwiazania. ");
	for (i = 0; i < rowSize; i++) {
		printf("row = %d, Xnew %g, Xold %g \n", i,XResult->b[i],XResult_Old->b[i] );
		tmp = fabs(XResult_Old->b[i] - XResult->b[i]);
		sum += tmp;
	}
	sum = sum / rowSize;
	printf("Iteracja\n");
	for (i = 0; i < rowSize; i++) {
		XResult_Old->b[i] = XResult->b[i];
	}
	return sum;
}

void calculateX(int row, int nrows, vector_t *XResult, double *localM, double *localN, int rowSize) {
	int i, j, stop, rank;
	double result = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (nrows > 0) {
		for (j = 0; j < nrows; j++) {
			result = 0;
			for (i = 0; i < rowSize; i++) {
//				printf("RANK %d +> LocalM %g, X %g, localN %g, \n", rank, localM[i], XResult->b[i], localN);
				result = result + localM[i] * XResult->b[i];
			}
			result = result + localN[j];
			XResult->b[row + j] = result;
//			printf("rank = %d, Liczenie, rozwiązanie:%g [%d] \n", rank, XResult->b[row + j], row + j);
		}
	} else {
//		printf("Liczenie. - Proces wykluczony\n");
	}
//	printf("Wiersz %d, rozwiązanie %g \n", row, result);
}

int calculateJacobi(matrix_t A, vector_t B, vector_t* X, int* beginIndexes, int* endIndexes, double precision) {
	int rank, procSize, i, j, nrows, dataSize, rowSize, localStart;
	double *localM, *localN;
	double precisionLocal, accuracy;

	MPI_Status status;
	matrix_t M, D, L, U;
	vector_t N;
	vector_t XResult, XResultOld;
	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	accuracy = 1000;

	if (rank == 0) {
		if(checkMatrix(&A) < 0){
			printf("\n\n Macierz nie jest diagonalnie dominująca ! Nie umiem tego policzyc \n\n\n");
			MPI_Abort(MPI_COMM_WORLD, -1);
			return -1;
		}
		dataSize = A.n * A.n;
		rowSize = A.n;
		precisionLocal = precision;
	}

	MPI_Bcast(&dataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&precisionLocal, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&rowSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
//	printf("My datasize in rank %d is %d, rows = %d\n",rank,dataSize,rowSize);

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
//		printf("#DEBUG policzylem macierze \n");
	}

	XResult.b = calloc(rowSize, sizeof(double));
	XResultOld.b = malloc(rowSize * sizeof(double));

	if (XResult.b == NULL) {
		return -4;
	}

	for (i = 0; i < rowSize; i++) {
		XResultOld.b[i] = 100.0;
	}

	if (rank != 0) {
		beginIndexes = malloc(procSize * sizeof(int));
		endIndexes = malloc(procSize * sizeof(int));
	}

	MPI_Bcast(beginIndexes, procSize, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(endIndexes, procSize, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

//		for (i = 0; i < procSize; i++) {
//			printf("Rank %d, Begin Indexes %d, End Indexes %d \n", rank, beginIndexes[i], endIndexes[i]);
//		}

	if (rank == 0) {
		for (i = 1; i < procSize; i++) {
			nrows = endIndexes[i] - beginIndexes[i] + 1;
			MPI_Send(&nrows, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			printf("LocalStart send %d \n", beginIndexes[i]);
			MPI_Send(&(beginIndexes[i]), 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			for (j = beginIndexes[i]; j <= endIndexes[i]; j++) {
				printf("Wysyłam do rank:%d rowsize %d \n ", i, rowSize);
				MPI_Send(&(M.a[rowSize * j]), rowSize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
				MPI_Send(&(N.b[j]), 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
		}
		nrows = endIndexes[0] - beginIndexes[0] + 1;
		localM = (double*) malloc(rowSize * nrows * sizeof(double));
		localN = (double*) malloc(nrows * sizeof(double));
		int length = rowSize * nrows;
		//printf("lenght %d, rowSize %d, nrows%d", length,rowSize,nrows );
		for (j = 0; j < length; j++) {
			localM[j] = M.a[j];
			//printf("wiersz %d, Saved rank zer, localM %g, localM source %g \n", j,localM[j], M.a[j]);
		}
		for (j = 0; j < nrows; j++) {
			localN[j] = N.b[j];
			//printf("wiersz %d, Saved rank zer, localM %g, localM source %g \n", j,localM[j], M.a[j]);
		}
		localStart = beginIndexes[0];

	} else {
		MPI_Recv(&nrows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&localStart, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		printf("LocalStart rec %d \n",localStart);
		if (nrows > 0) {
			localM = malloc(nrows * rowSize * sizeof(double));
			localN = malloc(nrows * sizeof(double));
		}

		printf("My rank = %d. nrows %d", rank, nrows);
		for (i = 0; i < nrows; i++) {
//			printf("My rank = %d. Odbieram swoja daną nr: %d, RowSize %d\n ", rank, i, rowSize);
			MPI_Recv(localM + (i*rowSize), rowSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Recv(localN + (i), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		}
	}
//	printf("#DEBUG Rank = %d. Rezeslalem dane.  \n", rank);

	printf("Rank %d, rowSize%d, localStart %d  \n", rank,rowSize, localStart);
	MPI_Barrier(MPI_COMM_WORLD);
		if (nrows >= 1) {
			printf("Rank %d.Nrows %d, dataSize %d, rowSize%d, localStart %d  \n", rank, nrows, dataSize, rowSize, localStart);
			for (i = 0; i < rowSize * nrows; i++) {
				printf("Rank %d. Moje lokalne M %g. \n", rank, localM[i]);
			}
			for (i = 0; i < rowSize ; i++) {
				printf("Rank %d. Moje rozwiązanie %g Stare %g LocalM %g\n", rank, XResult.b[i], XResultOld.b[i], localM[i]);
			}
		}
	MPI_Barrier(MPI_COMM_WORLD);
//
//	//LICZENIE
//
//	while (accuracy > precisionLocal) {
////		printf("Rank %d", rank);
//		calculateX(localStart, nrows, &XResult, localM, localN, rowSize);
//
//		MPI_Barrier(MPI_COMM_WORLD);
//
//		int procLength = 0;
//		for (i = 0; i < procSize; i++) {
//			if (nrows > 0 && i != rank) {
////				printf("Rank %d. Robie send do %d, o dlugosci %d. \n", rank, i, nrows);
//				MPI_Send(&(XResult.b[localStart]), nrows, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
//			}
//		}
//
//		for (i = 0; i < procSize; i++) {
//			if (nrows > 0 && i != rank) {
//				procLength = endIndexes[i] - beginIndexes[i] + 1;
////				printf("Rank %d. Robie recv od %d, o dlugosci %d. \n", rank, i, procLength);
//				if (procLength > 0) {
//					MPI_Recv(&(XResult.b[beginIndexes[i]]), procLength, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
//				}
//			}
//		}
//
//		MPI_Barrier(MPI_COMM_WORLD);
//
////		for (i = 0; i < rowSize; i++) {
////			printf("Rank %d, Komorka %d, wynik: %g \n", rank, i, XResult.b[i]);
////		}
//
////		printf("Rank %d", rank);
//		if (rank == 0) {
//			accuracy = getDelta(&XResult, &XResultOld, rowSize, nrows);
////	printf("Iteracja\n");
//		}
//		MPI_Bcast(&accuracy, 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
////	printf("Rank %d Po BCast, %g, %g \n", rank, accuracy, precisionLocal);
//		MPI_Barrier(MPI_COMM_WORLD);
//	}

	if (rank == 0) {
		X->b = malloc(rowSize * sizeof(double));
		for (i = 0; i < rowSize; i++) {
			X->b[i] = XResult.b[i];
			X->n = rowSize;
//			printf("|%g|",X->b[i]);
		}
		freeMemory(&M, &N, &D, &L, &U);
	}
	free(XResult.b);
	free(XResultOld.b);
	if(rank != 0){
	free(beginIndexes);
	free(endIndexes);
	}
	free(localM);

	printf("Rank %d, GaussJordan - koniec mej zacnej funkcji \n", rank);
	MPI_Barrier(MPI_COMM_WORLD);
	return 0;
}

