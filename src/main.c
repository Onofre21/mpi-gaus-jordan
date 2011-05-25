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

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("Hello. I'am a rank %d/%d\n", rank,size);

	if(rank == 0){
		error = getParams(argc,argv,inputFile,&precision);
		if(error != 0){
			printError(error);
			MPI_Finalize();
			return -1;
		}
		error = readFile(argv[1],&A,&B);
		if(error != 0){
			printError(error);
			MPI_Finalize();
			return -2;
		}
		error = calculateIndexes(A.n,&beginIndexes,&endIndexes,&equalsSize);
		if(error != 0){
			printError(error);
			MPI_Finalize();
			return -2;
		}
		printf("ERROR: %d\nA.a[0][0] = %g\nB.b[0] = %g\nA.n = %d\n begin index 0 = %d\n end index 0 = %d\nequalsize = %d\n",error,A.a[0],B.b[0],A.n,beginIndexes[3],endIndexes[3],equalsSize);
	}
	//TODO obliczenia GAUSSEM
	error = calculateGauss(A,B,&X,beginIndexes,endIndexes,equalsSize);

	//TODO WYDRUK GAUSSA


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
