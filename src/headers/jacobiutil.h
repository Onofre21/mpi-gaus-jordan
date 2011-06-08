/*
 * jacobiutil.h
 *
 *  Created on: Jun 5, 2011
 *      Author: lewy
 */

#ifndef JACOBIUTIL_H_
#define JACOBIUTIL_H_

#include "def.h"

int allocMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U, vector_t *X, int n) ;
void freeMemory(matrix_t *M, vector_t *N, matrix_t *D, matrix_t *L, matrix_t *U);
void calculateRDiagonal(matrix_t *A, matrix_t *D);
void calculateL(matrix_t *A, matrix_t *L);
void calculateU(matrix_t *A, matrix_t *U) ;
void calculateM(matrix_t *M, matrix_t *D, matrix_t *L, matrix_t *U) ;
void calculateN(vector_t *N, matrix_t *D, vector_t *B) ;
int checkMatrix(matrix_t *A);
int allcMemoryAll(vector_t *XResult, vector_t *XResultOld, int **beginIndexes, int **endIndexes, int rowSize, int rank, int procSize);
void freeMemoryAll(int **beginIndexes, int **endIndexes, vector_t *XResult, vector_t *XResultOld);

#endif /* JACOBIUTIL_H_ */
