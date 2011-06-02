#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main(int argc, char** argv){
	FILE* f = fopen(argv[1],"w");
	int i,j,ile = atoi(argv[2]);
	double tmp;
	srand(time(NULL));
	fprintf(f,"%d\n",ile);
	for(i = 0; i < ile; i++){
		for(j = 0; j < ile; j++){
			tmp = (rand() /((double)(RAND_MAX)))*200.0-100.0;
			fprintf(f,"%lf ",tmp);
		}	
		fprintf(f,"\n");			
	}
	for(i = 0; i < ile; i++){
		tmp = (rand() /((double)(RAND_MAX)))*200.0-100.0;
		fprintf(f,"%lf\n",tmp);
	}
	return 0;				
}
