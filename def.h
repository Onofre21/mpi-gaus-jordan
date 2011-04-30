/*
 * def.h
 *
 *  Created on: 30-04-2011
 *      Author: alien
 */

#ifndef DEF_H_
#define DEF_H_

#include "mpich2/mpi.h"

typedef struct a{
	double **a;
	int n;
	int m;
} matrixA_t;

typedef struct b{
	double *b;
	int n;
}vectorB_t;

typedef struct x{
	double *x;
	int n;
}vectorX_t;

#endif /* DEF_H_ */
