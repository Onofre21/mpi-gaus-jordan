/*
 * gaussJacobi.c
 *
 *  Created on: 25-05-2011
 *      Author: alien
 */

#include"headers/gaussJordan.h"

int calculateGauss(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes,int equalSize){
	int procSize,i,j,rank,dataSize,nrows;
	double *data;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&procSize);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank == 0){
		dataSize = A.n+1;
	}
	MPI_Bcast(&dataSize,1,MPI_INTEGER,0,MPI_COMM_WORLD);
	if(rank == 0){
		for(i = 1; i < procSize; i++){
			nrows = endIndexes[i] - beginIndexes[i]+1;
			MPI_Send(&nrows,1,MPI_INT,i,0,MPI_COMM_WORLD);
			for(j = beginIndexes[i]; j <= endIndexes[j];j++){
				MPI_Send(&(A.a[A.n*j]),A.n,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
				MPI_Send(&(B.b[j]),1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			}
		}
		data = malloc((endIndexes[0]-beginIndexes[0]+1)*dataSize*sizeof(double));
		nrows = endIndexes[0] - beginIndexes[0]+1;
		for(i = beginIndexes[0]; i <= endIndexes[0]; i++){
			for(j = 0; j < dataSize-1; j++){
				data[i*dataSize+j] = A.a[i*A.n+j];
			}
			data[(i+1)*dataSize-1] = B.b[i];
		}
	}else{
		MPI_Recv(&nrows,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		data = malloc(nrows*dataSize*sizeof(double));
		for( i = 0; i < nrows; i++){
			MPI_Recv(data+i*dataSize,dataSize-1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
			MPI_Recv(data+((i+1)*dataSize)-1,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		}
	}

	/*for(i = 0; i < nrows; i++){
		for(j = 0; j < dataSize;j++){
			printf("RANK #%d: data[%d][%d] = %g\n",rank,i,j,data[i*dataSize+j]);
		}
	}*/
	//todo obliczenia
	//todo zebranie wyników
	//todo zapakowanie wyników

	return 0;
}
