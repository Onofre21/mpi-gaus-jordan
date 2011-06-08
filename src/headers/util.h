/*
 * util.c
 *
 *  Created on: 01-05-2011
 *      Author: alien
 */

#ifndef UTIL_C_
#define UTIL_C_

#include <stdlib.h>
#include "def.h"

void freeMatrix(matrix_t*);
void freeVector(vector_t*);
vector_t createVectorX(int n);
int duplicateVector(vector_t* dest,vector_t* source);
int duplicateMatrix(matrix_t* dest,matrix_t* source);
int calculateIndexes(int, int**, int**,int*);
int getParams(int argc, char** argv,char* inputFile, double* precision);


#endif /* UTIL_C_ */
