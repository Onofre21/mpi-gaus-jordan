#include "headers/iofile.h"

int readFile(char* filename, matrix_t* A, vector_t* B){
	int n,i,j;
	FILE* file = fopen(filename,"r");
	if(file == NULL){
		return -2;
	}

	if(fscanf(file,"%d\n",&n) !=1){
		return -3;
	}

	A->n = n;
	A->a = malloc(n*n*sizeof(double));
	if(A->a == NULL){
		return -4;
	}

	B->n = n;
	B->b = malloc(n*sizeof(double));
	if(B->b == NULL){
		return -4;
	}

	for(i = 0; i< n; ++i){
		for(j = 0; j < n; ++j){
			if(fscanf(file,"%lf",&(A->a[i*n+j])) != 1){
				/*free mem*/
				return -3;
			}
		}
	}

	for(i = 0; i < n ; ++i ){
		if(fscanf(file,"%lf",&(B->b[i])) != 1){
			/*free mem*/
			return -3;
		}
	}

	fclose(file);
	return 0;
}

void printError(int error){
	printf("Program natrafił na błąd numer %d:\n",error);
	switch (error){
		case -1:
			printf("Złe wywołanie programu, powinno być: mpirun -np 5 ./GaussJacobiDiff plik_wejsciowy [precyzja]");
			break;
		case -2:
			printf("Nie można otworzyć pliku wejściowego\n");
			break;
		case -3:
			printf("Zła składnia pliku wejściowego\n");
			break;
		case -4:
			printf("Nie można zaalokować pamięci\n");
			break;
		case -5:
			printf("Układ równań ma nieskończenie wiele rozwiązań lub jest sprzeczny\n");
			break;
	}
}

void printResults(char* header,vector_t X,struct timeval start, struct timeval end){
	int i = 0;
	int sec,size;
	long milisec;
	printf("Zakończono obliczenia metodą %s.\n",header);
	printf("Otrzymano następujący wektor:\n");
	for(; i < X.n; i++){
		printf("X[%d] = %g\n",i,X.b[i]);
	}
	sec = end.tv_sec-start.tv_sec;
	if(end.tv_usec < start.tv_usec){
		sec--;
	}
	milisec = sec*1000000 + (end.tv_usec-start.tv_usec);
	//printf("%d %d %d %d\n",start.tv_sec,start.tv_usec,end.tv_sec,end.tv_usec);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	printf("Metoda %s - czas pracy: %ld mikrosekund dla rzędu układu %d i %d procesów\n",header, milisec,X.n,size);
}

void printResultsSequence(char* header,vector_t X,struct timeval start, struct timeval end){
	int i = 0;
	int sec,size;
	long milisec;
	printf("Zakończono obliczenia sekwencyjną metodą %s.\n",header);
	printf("Otrzymano następujący wektor:\n");
	for(; i < X.n; i++){
		printf("X[%d] = %g\n",i,X.b[i]);
	}
	sec = end.tv_sec-start.tv_sec;
	if(end.tv_usec < start.tv_usec){
		sec--;
	}
	//printf("%d %d %d %d\n",start.tv_sec,start.tv_usec,end.tv_sec,end.tv_usec);
	milisec = sec*1000000 + (end.tv_usec-start.tv_usec);
	printf("Metoda sekwencyjna %s - czas pracy: %ld mikrosekund dla rzędu układu %d\n",header, milisec,X.n);
}

void printTimeDiff(struct timeval times[4]){
	/*TODO wypisać info o różnicy czasów*/
}
