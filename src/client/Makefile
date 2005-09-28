AGENT = ../config.o ../global.o agent.o
PORTIER = ../config.o ../global.o  ../shm.o portier.o
CMD = ../config.o ../global.o cmd.o
 
.SUFFIXES: .c .S .o

.c.o:

	gcc -g   -I../../include -Wall -g -c $<


cmd: ${CMD}
	gcc   -g -I../../include/   -o ../../bartlby_cmd ${CMD}
portier: ${PORTIER}
	gcc  -ldl -g -I../../include/   -o ../../bartlby_portier ${PORTIER}
	
agent: ${AGENT} 
	gcc   -g -I../../include/   -o ../../bartlby_agent ${AGENT}



all:    agent portier cmd
	



clean:
	rm -fv *.o 
