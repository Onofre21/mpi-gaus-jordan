#include "headers/reader.h"

int readFile(char* filename, matrix_t* A, vector_t* B){
	int n,m,i,j;
	FILE* file = fopen(filename,"r");
	if(file == NULL){
		return -1;
	}

	if(fscanf(file,"%d %d\n",&n,&m) !=2){
		return -2;
	}

	A->n = n;
	A->m = m;
	A->a = malloc(n*sizeof(double*));
	if(A->a == NULL){
		return -3;
	}
	for(i = 0; i < n; ++i){
		A->a[i] = malloc(m*sizeof(double));
		if(A->a[i] == NULL){
			for(j = i-1; j>=0; --j){
				free(A->a[i]);
			}
			free(A->a);
			return -3;
		}
	}

	B->n = n;
	B->b = malloc(n*sizeof(double));
	if(B->b == NULL){
		return -3;
	}

	for(i = 0; i< n; ++i){
		for(j = 0; j < m; ++j){
			if(fscanf(file,"%lf",&(A->a[i][j])) != 1){
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
