/*
 * jacobi.c
 *
 *  Created on: Jun 2, 2011
 *      Author: lewy
 */

#include"headers/jacobi.h"

int calculateJacobi(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes){
	int rank, procSize;
	int i, j;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&procSize);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(rank == 0){
		for(i = 0; i<X->n;i++){
			X->b[i] = 0;
		}
	}

	return 0;
}

