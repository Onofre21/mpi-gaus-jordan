#include "headers/util.h"

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
