#include "headers/iofile.h"
#include "headers/def.h"
#include "headers/util.h"
#include <string.h>

int getParams(int , char**,char* , double*);

int main(int argc, char** argv){
	int size,rank,error;
	char* inputFile = NULL;
	double precision;
	matrix_t A;
	vector_t B;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("Hello. I'am a rank %d/%d\n", rank,size);

	if(rank == 0){
		error = getParams(argc,argv,inputFile,&precision);
		if(error != 0){
			printError(error);
		}else{
			error = readFile(argv[1],&A,&B);
			printf("ERROR: %d\nA.a[0][0] = %g\nB.b[0] = %g\nA.n = %d\n",error,A.a[0],B.b[0],A.n);
			freeMatrix(&A);
			freeVector(&B);
		}
	}

	MPI_Finalize();
	return 0;
}

int getParams(int argc, char** argv,char* inputFile, double* precision){
	if(argc < 2 || argc > 3){
		return -1;
	}
	inputFile = argv[1];
	if(argc == 2){
		*precision = DEFAULT_PRECISION;
	}else{
		*precision = atof(argv[2]);
	}
	return 0;
}
