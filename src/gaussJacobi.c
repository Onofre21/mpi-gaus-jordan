/*
 * gaussJacobi.c
 *
 *  Created on: 25-05-2011
 *      Author: alien
 */

#include"headers/gaussJordan.h"

int calculateGauss(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes,int equalSize){
	int procSize,i,j,rank,dataSize;
	double *data;
	MPI_Comm_size(MPI_COMM_WORLD,&procSize);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank == 0){
		dataSize = A.n+1;
	}
	MPI_Bcast(&dataSize,1,MPI_INTEGER,0,MPI_COMM_WORLD);
	if(rank == 0){
		for(i = 0; i < procSize; i++){
			//beginIndexes[i];
			//TODO przygotowanie danych
			//todo wysłanie danych

		}
	}else{
		printf("dataSize %d\n",dataSize);
		//TODO odebranie danych
	}
	//todo obliczenia
	//todo zebranie wyników
	//todo zapakowanie wyników

	return 0;
}
