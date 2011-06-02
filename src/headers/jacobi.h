/*
 * jacobi.h
 *
 *  Created on: Jun 2, 2011
 *      Author: lewy
 */

#ifndef JACOBI_H_
#define JACOBI_H_

#include "def.h"
#include <math.h>


int calculateJacobi(matrix_t A,vector_t B, vector_t* X, int* beginIndexes, int* endIndexes);

#endif /* JACOBI_H_ */
