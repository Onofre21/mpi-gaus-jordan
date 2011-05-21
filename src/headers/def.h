/*
 * def.h
 *
 *  Created on: 30-04-2011
 *      Author: alien
 */

#ifndef DEF_H_
#define DEF_H_
#include <mpich2/mpi.h>

#define DEFAULT_PRECISION 1e-6
typedef struct a{
	double *a;
	int n;
} matrix_t;

typedef struct b{
	double *b;
	int n;
}vector_t;


#endif /* DEF_H_ */
