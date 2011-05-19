#include "headers/iofile.h"

int readFile(char* filename, matrix_t* A, vector_t* B){
	int n,i,j;
	FILE* file = fopen(filename,"r");
	if(file == NULL){
		return -1;
	}

	if(fscanf(file,"%d\n",&n) !=1){
		return -2;
	}

	A->n = n;
	A->a = malloc(n*n*sizeof(double));
	if(A->a == NULL){
		return -3;
	}

	B->n = n;
	B->b = malloc(n*sizeof(double));
	if(B->b == NULL){
		return -3;
	}

	for(i = 0; i< n; ++i){
		for(j = 0; j < n; ++j){
			if(fscanf(file,"%lf",&(A->a[i*n+j])) != 1){
				/*free mem*/
				return -4;
			}
		}
	}

	for(i = 0; i < n ; ++i ){
		if(fscanf(file,"%lf",&(B->b[i])) != 1){
			/*free mem*/
			return -4;
		}
	}

	fclose(file);
	return 0;
}
