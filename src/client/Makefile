AGENT = ../config.o ../global.o agent.o
PORTIER = ../config.o ../global.o  ../shm.o portier.o

 
.SUFFIXES: .c .S .o

.c.o:

	gcc -g   -I../../include -Wall -g -c $<



portier: ${PORTIER}
	gcc  -g -I../../include/ -o ../../bartlby_portier ${PORTIER}
	
agent: ${AGENT} 
	gcc  -g -I../../include/ -o ../../bartlby_agent ${AGENT}



all:    agent portier
	



clean:
	rm -fv *.o 
