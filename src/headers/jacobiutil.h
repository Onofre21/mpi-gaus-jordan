/*
 * jacobiutil.h
 *
 *  Created on: Jun 5, 2011
 *      Author: lewy
 */

#ifndef JACOBIUTIL_H_
#define JACOBIUTIL_H_

#include "def.h"
#include <math.h>

int allocMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U, vector_t *X, int n) ;
void freeMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U);
void calculateRDiagonal(matrix_t *A, matrix_t *D);
void calculateL(matrix_t *A, matrix_t *L);
void calculateU(matrix_t *A, matrix_t *U) ;
void calculateM(matrix_t *M, matrix_t *D, matrix_t *L, matrix_t *U) ;
void calculateN(vector_t *N, matrix_t *D, vector_t *B) ;

#endif /* JACOBIUTIL_H_ */
