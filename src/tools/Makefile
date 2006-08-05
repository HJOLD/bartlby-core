include ../../Makefile.conf

SHMT = ../shm.o ../config.o ../global.o shmt.o
PORTIER = ../config.o ../global.o  ../shm.o portier.o
CMD = ../config.o ../global.o cmd.o


.SUFFIXES: .c .S .o

.c.o:

	$(CC) $(EXTRAOPTIONS)   $(INCLUDE_LL)  -c $<


install: all
	$(CPPVA) bartlby_shmt ${BARTLBY_HOME}/bin/
	$(CPPVA) bartlby_portier ${BARTLBY_HOME}/bin/
	$(CPPVA) bartlby_cmd ${BARTLBY_HOME}/bin/
	

shmt: ${SHMT}
	$(CC)  $(EXTRAOPTIONS) $(INCLUDE_LL) -o bartlby_shmt ${SHMT}


cmd: ${CMD}
	$(CC)   $(EXTRAOPTIONS) $(INCLUDE_LL)   -o bartlby_cmd ${CMD}
portier: ${PORTIER}
	$(CC)  $(LDL) $(EXTRAOPTIONS) $(INCLUDE_LL)   -o bartlby_portier ${PORTIER}
	


all:    shmt cmd portier
	



clean:
	$(RMVFR) *.o 
