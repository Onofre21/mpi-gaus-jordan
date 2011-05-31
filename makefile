CC = mpicc
DATA = data/data
SOURCE = src/gaussJordan.c src/iofile.c src/main.c src/util.c
HEADERS = src/headers/def.h src/headers/gaussJordan.h src/headers/iofile.h src/headers/util.h
BIN = bin/GausJordan

build:
	${CC} ${HEADERS} ${SOURCE} -o ${BIN}
	
run:
	mpiexec -np 5 ${BIN} ${DATA}
	 