SHMT = ../shm.o ../config.o ../global.o shmt.o



.SUFFIXES: .c .S .o

.c.o:

	gcc -g   -I../../include -Wall -g -c $<




shmt: ${SHMT}
	gcc  -g -I../../include/ -o ../../shmt ${SHMT}



all:    shmt
	



clean:
	rm -fv *.o 
