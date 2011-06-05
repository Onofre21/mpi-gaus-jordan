CC = mpicc
FLAGS = -pedantic -w -Wall 
DATA = data/data
SOURCE = src/main.c src/gaussJordan.c src/iofile.c src/util.c src/jacobiutil.c src/jacobi.c 
OUTPUT = main.o gaussJordan.o iofile.o util.o jacobi.o jacobiutil.o
BIN = bin/GaussJacobiDiff
DEST = bin/
        
all: main gaussJordan iofile util jacobiutil jacobi
	${CC} ${OUTPUT} -lm ${FLAGS} -o ${BIN}
	
main: 
	${CC} -c ${FLAGS} src/main.c src/headers/def.h
gaussJordan: 
	${CC} -c ${FLAGS} src/gaussJordan.c src/headers/gaussJordan.h src/headers/def.h
jacobiutil:
	${CC} -c ${FLAGS} src/jacobiutil.c src/headers/jacobiutil.h src/headers/def.h
jacobi: 
	${CC} -c ${FLAGS} src/jacobi.c src/headers/jacobi.h src/headers/def.h
iofile: 
	${CC} -c ${FLAGS} src/iofile.c src/headers/iofile.h src/headers/def.h
util:  
	${CC} -c ${FLAGS} src/util.c src/headers/util.h src/headers/def.h

run:
	mpiexec -np 5 ${BIN} ${DATA}
	
run2:
	mpiexec -np 5 ${BIN} ${DATA}2
	
run3:
	mpiexec -np 5 ${BIN} data/dataInvalid1
	
run4:
	mpiexec -np 5 ${BIN} data/dataInvalid2
	
run5:
	mpiexec -np 5 ${BIN} ${DATA}6
	
run6:
	mpiexec -np 5 ${BIN} ${DATA}Jacobi
	
clean: 
	rm *.o