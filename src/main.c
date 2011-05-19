#include "headers/iofile.h"
#include "headers/def.h"

int main(int argc, char** argv){
	int size,rank;
	matrix_t A;
	vector_t B;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("Hello. I'am a rank %d/%d\n", rank,size);

	if(rank == 0){
		if(argc != 3){
			fprintf(stderr,"Nie prawidłowe wywołanie programu:\n\tPowinno być: GaussJacobiDiff plik_wejsciowy dokładność\n");
		}else{
			int error = readFile(argv[1],&A,&B);
			printf("ERROR: %d\nA.a[0][0] = %g\nB.b[0] = %g\nA.n = %d\n",error,A.a[0],B.b[0],A.n);
			freeMatrix(&A);
			freeVector(&B);
		}
	}

	MPI_Finalize();
	return 0;
}
