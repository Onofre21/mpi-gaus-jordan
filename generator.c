#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int
main(int argc, char **argv)
{
	FILE           *f;
	int             i, j, m, ile, max;
	double          tmp;
	char 		name[20];
	srand(time(NULL));
	
	max = atoi(argv[1]);	

	if(max < 2){
		return 0;
	}
	
		sprintf(name, "data/dataJ%d",max);
		f = fopen(name, "w");
		fprintf(f, "%d\n", max);
		for (i = 0; i < max; i++) {
			for (j = 0; j < max; j++) {
				tmp = (rand() / ((double) (RAND_MAX))) * 20.0 - 100.0;
				if (i == j) {
					tmp = tmp * 15000;
				}
				fprintf(f, "%lf ", tmp);
			}
			fprintf(f, "\n");
			printf("Wygenerowalem %d iteracje\n",i);
		}
		for (i = 0; i < max; i++) {
			tmp = (rand() / ((double) (RAND_MAX))) * 200.0 - 100.0;
			fprintf(f, "%lf\n", tmp);
		}
		fclose(f);
	return 0;
}
