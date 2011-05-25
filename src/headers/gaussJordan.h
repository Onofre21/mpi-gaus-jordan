/*
 * gaussJordan.h
 *
 *  Created on: 25-05-2011
 *      Author: alien
 */

#ifndef GAUSSJORDAN_H_
#define GAUSSJORDAN_H_

#include "def.h"

int calculateGauss(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes,int equalSize);

#endif /* GAUSSJORDAN_H_ */