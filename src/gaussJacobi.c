/*
 * gaussJacobi.c
 *
 *  Created on: 25-05-2011
 *      Author: alien
 */

#include"headers/gaussJordan.h"

int calculateGauss(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes,int equalSize){
	int procSize,i,rank;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank == 0){
		for(i = 0; i < size; i++){
		//TODO przygotowanie danych
		//todo wysłanie danych
		}
	}else{
		//TODO odebranie danych
	}
	//todo obliczenia
	//todo zebranie wyników
	//todo zapakowanie wyników

	return 0;
}
