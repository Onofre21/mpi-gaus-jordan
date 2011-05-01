#include "headers/reader.h"
#include "headers/def.h"

int main(int argc, char** argv){
	int size,rank;
	matrix_t gaussA,jacobiA;
	vector_t gaussB,gaussX,jacobiB,jacobiX;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("Hello. I'am a rank %d/%d\n", rank,size);

	if(rank == 0){
		if(argc != 3){
			fprintf(stderr,"Nie prawidłowe wywołanie programu:\n\tPowinno być: GaussJacobiDiff plik_wejsciowy plik_wyjsciowy\n");
		}else{
			readFile(argv[1],&gaussA,&gaussB);
			printf("A.a[0][0] = %g\nB.b[0] = %g\nA.n = %d\n",gaussA.a[0][0],gaussB.b[0],gaussA.n);
			freeMatrix(&gaussA);
			freeVector(&gaussB);
		}
	}

	MPI_Finalize();
	return 0;
}
