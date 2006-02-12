include ../../Makefile.conf

AGENT = ../config.o ../global.o agent.o
PORTIER = ../config.o ../global.o  ../shm.o portier.o
CMD = ../config.o ../global.o cmd.o
 
.SUFFIXES: .c .S .o

.c.o:

	$(CC) $(EXTRAOPTIONS)   $(INCLUDE_LL)  -c $<


cmd: ${CMD}
	$(CC)   $(EXTRAOPTIONS) $(INCLUDE_LL)   -o ../../bartlby_cmd ${CMD}
portier: ${PORTIER}
	$(CC)  $(LDL) $(EXTRAOPTIONS) $(INCLUDE_LL)   -o ../../bartlby_portier ${PORTIER}
	
agent: ${AGENT} 
	$(CC)   $(EXTRAOPTIONS) $(INCLUDE_LL)   -o ../../bartlby_agent ${AGENT}


install: all
	$(CPPVA) ../../bartlby_cmd ${BARTLBY_HOME}/bin/
	$(CPPVA) ../../bartlby_portier ${BARTLBY_HOME}/bin/
	$(CPPVA) ../../bartlby_agent ${BARTLBY_HOME}/bin/
	
all:    agent portier cmd
	



clean:
	$(RMVFR) *.o 
