#include "headers/util.h"

void freeMatrix(matrix_t* matrix){
	int i;
	for(i = 0; i < matrix->n; ++i){
		free(matrix->a[i]);
	}
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


