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

int duplicateMatrix(matrix_t* dest,matrix_t* source){
	int i,j;
	dest->m = source->m;
	dest->n = source->n;
	dest->a = malloc(dest->n * sizeof(double*));
	if(dest->a == NULL){
		return -1;
	}
	for(i = 0; i < dest->n; ++i){
		dest->a[i] = malloc(dest->m*sizeof(double));
		if(dest->a[i] == NULL){
			for(j=i-1; j >=0; --j){
				free(dest->a[i]);
			}
			free(dest->a);
			return -1;
		}
		for(j = 0; j < dest->m; ++j){
			dest->a[i][j] = source->a[i][j];
		}
	}

	return 0;
}

int duplicateVector(vector_t* dest,vector_t* source){
	int i;
	dest->n = source->n;
	dest->b = malloc(dest->n*sizeof(double));
	if(dest->b == NULL){
		return -1;
	}
	for(i = 0;i < dest->n;++i){
		dest->b[i] = source->b[i];
	}
	return 0;
}
