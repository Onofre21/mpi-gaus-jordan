/*
 * gaussJacobi.c
 *
 *  Created on: 25-05-2011
 *      Author: alien
 */

#include"headers/gaussJordan.h"

int calculateGauss(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes){
	int procSize,i,j,k,rank,dataSize,nrows,flag=0;
	int *markedRows, *columnChecked;
	double *data,*pivotRow;
	WhichRank in, out;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&procSize);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank == 0){
		dataSize = A.n+1;
	}
	//rozeslanie rozmiaru wiersza: dla ulatwienia wiersz to ilosc kolumn macierzy A + 1 jako wartosc z wektora B
	MPI_Bcast(&dataSize,1,MPI_INT,0,MPI_COMM_WORLD);
	if(rank == 0){
		//dla kazdego procesu
		for(i = 1; i < procSize; i++){
			nrows = endIndexes[i] - beginIndexes[i]+1;
			//przesylam ilosc wierszy
			MPI_Send(&nrows,1,MPI_INT,i,0,MPI_COMM_WORLD);
			//rozeslanie odpowiednich wierszy do kazdego procesu
			for(j = beginIndexes[i]; j <= endIndexes[i];j++){
				MPI_Send(&(A.a[A.n*j]),A.n,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
				MPI_Send(&(B.b[j]),1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			}
		}
		//proces zerowy zapisuje wlasne dane
		nrows = endIndexes[0] - beginIndexes[0]+1;
		data = (double*)malloc((nrows)*dataSize*sizeof(double));
		for(i = beginIndexes[0]; i <= endIndexes[0]; i++){
			for(j = 0; j < dataSize-1; j++){
				data[i*dataSize+j] = A.a[i*A.n+j];
			}
			data[(i+1)*dataSize-1] = B.b[i];
		}
	}else{
		//odebranie ilosci wierszy do obliczen przez proces
		MPI_Recv(&nrows,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		if(nrows >0){
			data = (double*)malloc(nrows*dataSize*sizeof(double));
		}
		//odebranie wierszy do obliczen
		for( i = 0; i < nrows; i++){
			MPI_Recv(data+i*dataSize,dataSize-1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
			MPI_Recv(data+((i+1)*dataSize)-1,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		}
	}

	//znaczniki do obliczen
	if(nrows > 0){
		//czy wiersz byl juz wybrany jako posiadajacy najwiekszy element w kolumnie
		markedRows = (int*)malloc(nrows*sizeof(int));
	}
	columnChecked = (int*)malloc((dataSize-1)*sizeof(int));
	pivotRow = (double*)malloc(dataSize*sizeof(double));

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
		if(rank==0 && fabs(pivotRow[i]) < EPSILON){
			printError(-5);
			MPI_Abort(MPI_COMM_WORLD,1);
			//TODO Napisać komentarz dlaczego tak
			return -1;
			/*if(nrows >0){
				printf("%p %p in rank %d\n",markedRows,data,rank);
				free(markedRows);
				free(data);
			}
			MPI_Barrier(MPI_COMM_WORLD);
			free(columnChecked);
			free(pivotRow);
			MPI_Barrier(MPI_COMM_WORLD);
			return -1;*/

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

	MPI_Barrier(MPI_COMM_WORLD);
	/*for(i = 0; i < nrows; i++){
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
			int rows = endIndexes[i] - beginIndexes[i] +1;
			for(j = 0; j < rows; j++){
				int which;
				MPI_Recv(pivotRow,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
				MPI_Recv(&which,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
				X->b[which] = pivotRow[0];
			}
		}
		/*for(i = 0; i < X->n; i++){
			printf("X[%i] = %g\n",i,X->b[i]);
		}*/
	}else{
		for(i = 0; i < nrows;i++){
			pivotRow[i] = data[(i+1)*dataSize-1]/data[i*dataSize+columnChecked[i]];
			MPI_Send(pivotRow+i,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
			MPI_Send(columnChecked+i,1,MPI_INT,0,0,MPI_COMM_WORLD);
		}
	}


	if(nrows >0){
		free(markedRows);
		free(data);
	}
	free(columnChecked);
	free(pivotRow);
	return 0;
}

int calculateGaussJordanSequence(matrix_t A,vector_t B, vector_t* X){
	int i,j,k,which;
	double tmp;
	int *markedRows = (int*) malloc(A.n * sizeof(int));
	int *columnChecked = (int*) malloc(A.n * sizeof(int));
	for(i =0; i < A.n; i++){
		markedRows[i] = 0;
	}
	// i to kolumna, j to wiersz
	for(i = 0; i < A.n; i++){
		tmp = 0.0;
		for(j = 0; j < A.n; j++){
			if(!markedRows[j] && fabs(A.a[j*A.n+i]) > tmp){
				tmp = fabs(A.a[j*A.n+i]);
				which = j;
			}
		}
		if(tmp<EPSILON){
			free(markedRows);
			free(columnChecked);
			return -5;
		}
		markedRows[which] = 1;
		columnChecked[which] = i;
		for(j = 0; j < A.n; j++){
			if(!(markedRows[j] && columnChecked[j] == i)){
				tmp = A.a[j*A.n+i] / A.a[which*A.n+i];
				A.a[j*A.n+i] = 0;
				for( k = i+1; k < A.n; k++){
					A.a[j*A.n+k] -= A.a[which*A.n+k]*tmp;
				}
				B.b[j] -= B.b[which]*tmp;
			}
		}
	}
	X->n = A.n;
	X->b = malloc(A.n*sizeof(double));

	for(i = 0; i < X->n; i++){
		X->b[columnChecked[i]] = B.b[i]/A.a[ i*A.n+columnChecked[i]];
	}
	/*for(i = 0; i < X->n; i++){
		for(j = 0; j < X->n; j++){
			printf("A[%d][%d] = %g\n",i,j,A.a[i*A.n+j]);
		}
		printf("B[%i]=%g\n",i,B.b[i]);
		printf("X[%d] = %g\n",i,X->b[i]);
	}*/

	free(markedRows);
	free(columnChecked);
	return 0;
}
