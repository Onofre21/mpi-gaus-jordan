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
	vector_t B, X;
	int *beginIndexes, *endIndexes;
	struct timeval start,end;
	struct timezone timezone;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(rank == 0){
		error = getParams(argc,argv,inputFile,&precision);
		if(error != 0){
			printf("Error params\n");
			printError(error);
			MPI_Abort(MPI_COMM_WORLD,-1);
			return -1;
		}
		error = readFile(argv[1],&A,&B);
		if(error != 0){
			printf("Error readFile\n");
			printError(error);
			MPI_Abort(MPI_COMM_WORLD,-2);
			return -2;
		}
		error = calculateIndexes(A.n,&beginIndexes,&endIndexes,&equalsSize);
		if(error != 0){
			printf("Error calculate indexes\n");
			printError(error);
			MPI_Abort(MPI_COMM_WORLD,-3);
			return -3;
		}
	}

	if(rank==0){
		gettimeofday(&start,&timezone);
	}
	error = calculateGauss(A,B,&X,beginIndexes,endIndexes,equalsSize);
	if(rank==0){
		gettimeofday(&end,&timezone);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0){
		if(error<0){
			printError(error);
		}else{
			printResults("Gauss-Jordan",X,start,end);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// robota Lewego
	//TODO sprawdzenie czy macierz się nadaje dla Jacobiego
	//TODO obliczenia JACOBI
	//TODO WYdruk JAcobi
	if(rank == 0){
		freeMatrix(&A);
		freeVector(&B);
		free(beginIndexes);
		free(endIndexes);
	}
	MPI_Finalize();
	return 0;
}
