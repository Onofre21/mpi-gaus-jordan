/*
 * gaussJacobi.c
 *
 *  Created on: 25-05-2011
 *      Author: alien
 */

#include"headers/gaussJordan.h"

int calculateGauss(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes,int equalSize){
	int procSize,i,j,k,rank,dataSize,nrows;
	int *markedRows, *columnChecked;
	double *data,*pivotRow;
	WhichRank in, out;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&procSize);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank == 0){
		dataSize = A.n+1;
	}
	MPI_Bcast(&dataSize,1,MPI_INT,0,MPI_COMM_WORLD);
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

	markedRows = malloc(nrows*sizeof(int));
	columnChecked = malloc((dataSize-1)*sizeof(int));
	pivotRow = malloc(dataSize*sizeof(double));

	for(i =0; i < nrows; i++){
		markedRows[i] = 0;
	}

	//dla każdej kolumny
	for(i = 0; i < dataSize-1 ; i++){
		double tmp = 0.0;
		int which = -1;
		for(j = 0; j < nrows;j++){
			double d = data[j*dataSize+i];
			if(!markedRows[j] && (fabs(d) > tmp)){
				tmp = fabs(d);
				which = j;
			}
		}
		in.max = tmp;
		in.rank = rank;
		MPI_Allreduce(&in,&out,1,MPI_DOUBLE_INT,MPI_MAXLOC,MPI_COMM_WORLD);
		if(rank == out.rank){
			markedRows[which] = 1;
			columnChecked[which] = i;
			for(j = 0; j < dataSize; j++){
				pivotRow[j] = data[which*dataSize+j];
			}
		}
		MPI_Bcast(pivotRow,dataSize,MPI_DOUBLE,out.rank,MPI_COMM_WORLD);
		if(fabs(pivotRow[i]) < EPSILON){
			return -1;
		}
		for(j = 0; j < nrows; j++){
			if(!(markedRows[j] && columnChecked[j] == i)){
				double tmp = data[j*dataSize+i] / pivotRow[i];
				data[j*dataSize+i] = 0;
				for( k = i+1; k < dataSize; k++){
					data[j*dataSize+k] -=pivotRow[k]*tmp;
				}
			}
		}
	}

	/*MPI_Barrier(MPI_COMM_WORLD);
	for(i = 0; i < nrows; i++){
		for(j = 0; j < dataSize;j++){
			printf("RANK #%d: data[%d][%d] = %g\n",rank,i,j,data[i*dataSize+j]);
		}
	}*/

	if(rank == 0){
		X->n = A.n;
		X->b = malloc(A.n*sizeof(double));
		for(i = 0; i < nrows;i++){
			pivotRow[i] = data[(i+1)*dataSize-1]/data[i*dataSize+columnChecked[i]];
			X->b[columnChecked[i]] = pivotRow[i];
		}
		for(i = 1; i < procSize; i++){
			int rows = endIndexes[i] -beginIndexes[i] +1;
			for(j = 0; j < rows; j++){
				int which;
				MPI_Recv(pivotRow,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
				MPI_Recv(&which,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
				X->b[which] = pivotRow[0];
			}
		}
		for(i = 0; i < X->n; i++){
			printf("X[%i] = %g\n",i,X->b[i]);
		}
	}else{
		for(i = 0; i < nrows;i++){
			pivotRow[i] = data[(i+1)*dataSize-1]/data[i*dataSize+columnChecked[i]];
			MPI_Send(pivotRow+i,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
			MPI_Send(columnChecked+i,1,MPI_INT,0,0,MPI_COMM_WORLD);
		}
	}



	free(markedRows);
	free(columnChecked);
	free(pivotRow);
	free(data);
	return 0;
}
