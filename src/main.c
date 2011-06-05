#include "headers/iofile.h"
#include "headers/def.h"
#include "headers/util.h"
#include <string.h>
#include "headers/gaussJordan.h"

int main(int argc, char** argv) {
	int size, rank, error, equalsSize;
	char* inputFile = NULL;
	double precision;
	matrix_t A, A_zast;
	vector_t B, B_zast, XGaussSeq, XGauss, XJacobi;
	int *beginIndexes, *endIndexes;
	struct timeval start, end;
	struct timezone timezone;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		error = getParams(argc, argv, inputFile, &precision);
		if (error != 0) {
			printError(error);
			MPI_Abort(MPI_COMM_WORLD, -1);
			return -1;
		}
		error = readFile(argv[1], &A, &B);
		if (error != 0) {
			printError(error);
			MPI_Abort(MPI_COMM_WORLD, -2);
			return -2;
		}
		error = calculateIndexes(A.n, &beginIndexes, &endIndexes, &equalsSize);
		if (error != 0) {
			printError(error);
			MPI_Abort(MPI_COMM_WORLD, -3);
			return -3;
		}
	}

	/*
	 * Jordan sekwencyjny
	 */
	if (rank == 0) {
		duplicateMatrix(&A_zast,&A);
		duplicateVector(&B_zast,&B);
		gettimeofday(&start, &timezone);
		error = calculateGaussJordanSequence(A_zast, B_zast, &XGaussSeq);
		gettimeofday(&end, &timezone);
		freeVector(&B_zast);
		freeMatrix(&A_zast);
		if (error < 0) {
			printError(error);
		} else {
			printResultsSequence("Gauss-Jordan", XGaussSeq, start, end);
			freeVector(&XGaussSeq);
		}
	}

	/*
	 * Jordan równoległy
	 */
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		duplicateMatrix(&A_zast,&A);
		duplicateVector(&B_zast,&B);
		gettimeofday(&start, &timezone);
	}
	calculateGauss(A_zast, B_zast, &XGauss, beginIndexes, endIndexes);
	if (rank == 0) {
		gettimeofday(&end, &timezone);
		freeVector(&B_zast);
		freeMatrix(&A_zast);
		printResults("Gauss-Jordan", XGauss, start, end);
		freeVector(&XGauss);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	/*
	 * Jacobi
	 */
/*	if (rank == 0) {
		duplicateMatrix(&A_zast,&A);
		duplicateVector(&B_zast,&B);
		gettimeofday(&start, &timezone);
	}
	calculateJacobi(A_zast, B_zast, &XJacobi, beginIndexes, endIndexes);
	if (rank == 0) {
		gettimeofday(&end, &timezone);
		freeVector(&B_zast);
		freeMatrix(&A_zast);
		printResults("Jacobi", XJacobi, start, end);
		free(XJacobi.b);
	}
	MPI_Barrier(MPI_COMM_WORLD);
*/
	if (rank == 0) {
		freeMatrix(&A);
		freeVector(&B);
		free(beginIndexes);
		free(endIndexes);
	}
	MPI_Finalize();
	return 0;
}
