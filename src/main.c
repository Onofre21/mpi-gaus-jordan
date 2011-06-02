#include "headers/iofile.h"
#include "headers/def.h"
#include "headers/util.h"
#include <string.h>
#include "headers/gaussJordan.h"

int main(int argc, char** argv){
	int size,rank,error,equalsSize;
	char* inputFile = NULL;
	double precision;
	matrix_t A;
	vector_t B, XGaussSeq,XGauss,XJacobi;
	int *beginIndexes, *endIndexes;
	struct timeval start,end;
	struct timezone timezone;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(rank == 0){
		error = getParams(argc,argv,inputFile,&precision);
		if(error != 0){
			printError(error);
			MPI_Abort(MPI_COMM_WORLD,-1);
			return -1;
		}
		error = readFile(argv[1],&A,&B);
		if(error != 0){
			printError(error);
			MPI_Abort(MPI_COMM_WORLD,-2);
			return -2;
		}
		error = calculateIndexes(A.n,&beginIndexes,&endIndexes,&equalsSize);
		if(error != 0){
			printError(error);
			MPI_Abort(MPI_COMM_WORLD,-3);
			return -3;
		}
	}

//	if(rank==0){
//		gettimeofday(&start,&timezone);
//		error = calculateGaussJordanSequence(A,B,&XGaussSeq);
//		gettimeofday(&end,&timezone);
//		if(error<0){
//			printError(error);
//		}else{
//			printResultsSequence("Gauss-Jordan",XGaussSeq,start,end);
//			freeVector(&XGaussSeq);
//		}
//	}
//
//
//	MPI_Barrier(MPI_COMM_WORLD);
//	if(rank==0){
//		gettimeofday(&start,&timezone);
//	}
//	calculateGauss(A,B,&XGauss,beginIndexes,endIndexes);
//	if(rank==0){
//		gettimeofday(&end,&timezone);
//	}
//	if(rank == 0){
//		printResults("Gauss-Jordan",XGauss,start,end);
//		freeVector(&XGauss);
//	}
//	MPI_Barrier(MPI_COMM_WORLD);

	/*
	 * Jacobi
	 */
	if(rank==0){
			gettimeofday(&start,&timezone);
		}
	calculateJacobi(A,B,&XJacobi,beginIndexes,endIndexes);
//	if(rank==0){
//		gettimeofday(&end,&timezone);
//	}
//	if(rank == 0){
//		printResults("Jacobi",XJacobi,start,end);
//		freeVector(&XJacobi);
//	}
	MPI_Barrier(MPI_COMM_WORLD);


	if(rank == 0){
		freeMatrix(&A);
		freeVector(&B);
		free(beginIndexes);
		free(endIndexes);
	}
	MPI_Finalize();
	return 0;
}
