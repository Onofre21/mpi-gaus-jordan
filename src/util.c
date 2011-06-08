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
		return -4;
	}
	if((*endIndexes = malloc(size*sizeof(int))) == NULL){
		free(*beginIndexes);
		return -4;
	}
	normalSize = n/size;
	additional = n%size;
	//printf("normalSize = %d, additional = %d\n",normalSize,additional);
	for(i = 0; i < size; i++){
		(*beginIndexes)[i] = begin = last+1;
		(*endIndexes)[i] = last = begin + normalSize + (additional > i ? 0 :-1);
	//	printf("beginIndexes[%i] = %i\n",i,(*beginIndexes)[i]);
	//	printf("endIndexes[%i] = %i\n",i,(*endIndexes)[i]);
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
		if(*precision<0){
			return -1;
		}
	}
	return 0;
}

int duplicateMatrix(matrix_t* dest,matrix_t* source){
	int i,j;
	dest->a = malloc(source->n*source->n*sizeof(double));
	dest->n = source->n;
	for(i = 0; i < dest->n;i++){
		for(j = 0; j < dest->n; j++){
			dest->a[i*dest->n+j] = source->a[i*source->n+j];
		}
	}
	return 0;
}

int duplicateVector(vector_t* dest,vector_t* source){
	int i;
	dest->b = malloc(source->n*sizeof(double));
	dest->n = source->n;
	for(i = 0; i < dest->n; i++){
		dest->b[i] = source->b[i];
	}
	return 0;
}
