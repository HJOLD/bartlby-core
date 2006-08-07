include ../../Makefile.conf

SHMT = ../shm.o ../config.o ../global.o shmt.o
PORTIER = ../config.o ../global.o  ../shm.o portier.o



.SUFFIXES: .c .S .o

.c.o:

	$(CC) $(EXTRAOPTIONS)   $(INCLUDE_LL)  -c $<

all:    shmt portier

install: all
	$(CPPVA) bartlby_shmt ${BARTLBY_HOME}/bin/
	$(CPPVA) bartlby_portier ${BARTLBY_HOME}/bin/
	
	

shmt: ${SHMT}
	$(CC)  $(EXTRAOPTIONS) $(INCLUDE_LL) -o bartlby_shmt ${SHMT}




portier: ${PORTIER}
	$(CC)  $(LDL) $(EXTRAOPTIONS) $(INCLUDE_LL)   -o bartlby_portier ${PORTIER}
	



	



clean:
	$(RMVFR) *.o 
