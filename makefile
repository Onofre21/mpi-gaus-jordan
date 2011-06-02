CC = mpicc
FLAGS = -pedantic -w -Wall 
DATA = data/data
SOURCE = src/main.c src/gaussJordan.c src/iofile.c src/util.c src/jacobi.c
OUTPUT = main.o gaussJordan.o iofile.o util.o jacobi.o
BIN = bin/GaussJordan
DEST = bin/
        
all: main gaussJordan iofile util jacobi
	${CC} ${OUTPUT} -lm ${FLAGS} -o ${BIN}
	
main: 
	${CC} -c ${FLAGS} src/main.c src/headers/def.h
gaussJordan: 
	${CC} -c ${FLAGS} src/gaussJordan.c src/headers/gaussJordan.h src/headers/def.h
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
	mpiexec -np 5 ${BIN} ${DATA}3
	
run4:
	mpiexec -np 5 ${BIN} ${DATA}4
	
run5:
	mpiexec -np 5 ${BIN} ${DATA}5
	
clean: 
	rm *.o
	 
