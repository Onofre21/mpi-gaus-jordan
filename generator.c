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
	
	for (m = 3; m < max; m++) {
		ile = m;
		sprintf(name, "dataJ%d",ile);
		f = fopen(name, "w");
		fprintf(f, "%d\n", ile);
		for (i = 0; i < ile; i++) {
			for (j = 0; j < ile; j++) {
				tmp = (rand() / ((double) (RAND_MAX))) * 200.0 - 100.0;
				if (i == j) {
					tmp = tmp * 150;
				}
				fprintf(f, "%lf ", tmp);
			}
			fprintf(f, "\n");
		}
		for (i = 0; i < ile; i++) {
			tmp = (rand() / ((double) (RAND_MAX))) * 200.0 - 100.0;
			fprintf(f, "%lf\n", tmp);
		}
		fclose(f);
	}
	return 0;
}
