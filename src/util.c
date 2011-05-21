#include "headers/util.h"
#include "mpich2/mpi.h"

void freeMatrix(matrix_t* matrix){
	free(matrix->a);
}

void freeVector(vector_t* vector){
	free(vector->b);
}

vector_t createVectorX(int n){
	vector_t X;
	X.n = n;
	X.b = calloc(n,sizeof(double));
	return X;
}

int calculateIndexes(int n, int** beginIndexes, int** endIndexes, int* equalsSize){
	int size, begin, last = -1, i, normalSize,additional;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if((*beginIndexes = malloc(size*sizeof(int))) == NULL){
		return -1;
	}
	if((*endIndexes = malloc(size*sizeof(int))) == NULL){
		free(*beginIndexes);
		return -2;
	}
	normalSize = n/size;
	additional = n%size;
	for(i = 0; i < n; i++){
		(*beginIndexes)[i] = begin = last+1;
		(*endIndexes)[i] = last = begin + normalSize + (additional > i ? 0:-1);
		//FIXME tu jest jakiś błąd
	}
	*equalsSize = normalSize + (additional ? 1:0);
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
