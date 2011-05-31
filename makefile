CC = mpicc
DATA = data/data
SOURCE = src/main.c src/gaussJordan.c src/iofile.c src/util.c
OUTPUT = main.o gaussJordan.o iofile.o util.o
BIN = bin/GaussJordan
DEST = bin/
        
all: main gaussJordan iofile util
	${CC} ${OUTPUT} -lm -Wall -o ${BIN}
	
main: 
	${CC} -c src/main.c src/headers/def.h
gaussJordan: 
	${CC} -c src/gaussJordan.c src/headers/gaussJordan.h src/headers/def.h
iofile: 
	${CC} -c src/iofile.c src/headers/iofile.h src/headers/def.h
util:  
	${CC} -c src/util.c src/headers/util.h src/headers/def.h

run:
	mpiexec -np 5 ${BIN} ${DATA}
	
clean: 
	rm *.o
	 