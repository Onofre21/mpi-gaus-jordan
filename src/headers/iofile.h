/*
 * iofile.h
 *
 *  Created on: 30-04-2011
 *      Author: alien
 */

#ifndef IOFILE_H_
#define IOFILE_H_

#include "def.h"
#include<sys/time.h>

int readFile(char*, matrix_t*, vector_t*);
void printError(int);
void printResults(char*,vector_t X, struct timeval , struct timeval );
void printResultsSequence(char*,vector_t X,struct timeval, struct timeval);

#endif /* IOFILE_H_ */
