#include "headers/reader.h"
#include "headers/def.h"

int main(int argc, char** argv){
	int size,rank;
	matrix_t gaussA,jacobi;
	vector_t gaussB,gaussX,jacobiB,jacobiX;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("Hello. I'am a rank %d\n", rank);

	if(rank == 0){
		if(argc != 4){
			fprintf(stderr,"Nie prawidłowe wywołanie programu:\n\tPowinno być: GaussJacobiDiff plik_macierzA plik_wektorB plik_wyjsciowy\n");
		}
	}




	MPI_Finalize();
	return 0;
}
