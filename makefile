CC = mpicc
DATA = data/data
SOURCE = src/main.c src/gaussJordan.c src/iofile.c src/util.c
OUTPUT = main.o gaussJordan.o iofile.o util.o
BIN = bin/GausJordan
        
all: ${OUTPUT} 
	${CC} ${OUTPUT} -lm -Wall -o ${BIN}
	
main.o: src/main.c src/headers/def.h
	${CC} -c src/main.c
gaussJordan.o: src/gaussJordan.c src/headers/gaussJordan.h src/headers/def.h
	${CC} -c src/gaussJordan.c
iofile.o: src/iofile.c src/headers/iofile.h src/headers/def.h
	${CC} -c src/iofile.c
util.o: src/util.c src/headers/util.h src/headers/def.h
	${CC} -c src/util.c

run:
	mpiexec -np 5 ${BIN} ${DATA}
	
clean: 
	rm *.o
	 