AGENT = ../config.o ../global.o agent.o
PASSIVE = ../config.o ../global.o passive.o


.SUFFIXES: .c .S .o

.c.o:

	gcc -g   -I../../include -Wall -g -c $<




agent: ${AGENT}
	gcc  -g -I../../include/ -o ../../bartlby_agent ${AGENT}



all:    agent
	



clean:
	rm -fv *.o 
