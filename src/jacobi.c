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
	for (i = 0; i < rowSize; i++) {
		tmp = fabs(XResult_Old->b[i] - XResult->b[i]);
		sum += tmp;
	}
	sum = sum / rowSize;
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
				result += localM[i] * XResult->b[i];
			}
			result = result + localN[j];
			XResult->b[row + j] = result;
		}
	}
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
		if (checkMatrix(&A) < 0) {
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

	if(allcMemoryAll(&XResult, &XResultOld, &beginIndexes, &endIndexes, rowSize, rank, procSize) != 0){
		freeMemory(&M, &N, &D, &L, &U);
		printError(-4);
		return -1;
	}

	MPI_Bcast(beginIndexes, procSize, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(endIndexes, procSize, MPI_INT, 0, MPI_COMM_WORLD);

	nrows = endIndexes[rank] - beginIndexes[rank] + 1;
	localStart = beginIndexes[rank];

	localM = malloc(rowSize * nrows * sizeof(double));
	localN = malloc(nrows * sizeof(double));

	/*
	 * Rozsyłanie macierzy pomocniczych
	 */
	if (rank == 0) {
		for (i = 1; i < procSize; i++) {
			nrows = endIndexes[i] - beginIndexes[i] + 1;
			for (j = beginIndexes[i]; j <= endIndexes[i]; j++) {
				MPI_Send(&(M.a[rowSize * j]), rowSize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
				MPI_Send(&(N.b[j]), 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
		}
		nrows = endIndexes[0] - beginIndexes[0] + 1;
		int length = rowSize * nrows;
		for (j = 0; j < length; j++) {
			localM[j] = M.a[j];
		}
		for (j = 0; j < nrows; j++) {
			localN[j] = N.b[j];
		}
		localStart = beginIndexes[0];

	} else {
		for (i = 0; i < nrows; i++) {
			MPI_Recv(localM + (i * rowSize), rowSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Recv(localN + (i), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	/*
	 * Liczenie właściwe
	 */
	while (accuracy > precisionLocal) {
		calculateX(localStart, nrows, &XResult, localM, localN, rowSize);

		MPI_Barrier(MPI_COMM_WORLD);
		int procLength = 0;
		for (i = 0; i < procSize; i++) {
			if (nrows > 0 && i != rank) {
				MPI_Send(&(XResult.b[localStart]), nrows, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
		}

		for (i = 0; i < procSize; i++) {
			if (nrows > 0 && i != rank) {
				procLength = endIndexes[i] - beginIndexes[i] + 1;
				if (procLength > 0) {
					MPI_Recv(&(XResult.b[beginIndexes[i]]), procLength, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
				}
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);
		if (rank == 0) {
			accuracy = getDelta(&XResult, &XResultOld, rowSize, nrows);
		}
		MPI_Bcast(&accuracy, 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	/*
	 * Konczenie pracy
	 */
	if (rank == 0) {
		X->b = malloc(rowSize * sizeof(double));
		for (i = 0; i < rowSize; i++) {
			X->b[i] = XResult.b[i];
			X->n = rowSize;
		}
		freeMemory(&M, &N, &D, &L, &U);
	}

	freeMemoryAll(&beginIndexes, &endIndexes, &XResult, &XResultOld);
	free(localM);
	free(localN);
	MPI_Barrier(MPI_COMM_WORLD);
	return 0;
}

