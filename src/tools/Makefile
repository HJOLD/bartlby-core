include ../../Makefile.conf

SHMT = ../shm.o ../config.o ../global.o shmt.o



.SUFFIXES: .c .S .o

.c.o:

	$(CC) $(EXTRAOPTIONS)   $(INCLUDE_LL)  -c $<


install: all
	$(CPPVA) ../../bartlby_shmt ${BARTLBY_HOME}/
	

shmt: ${SHMT}
	$(CC)  $(EXTRAOPTIONS) $(INCLUDE_LL) -o ../../bartlby_shmt ${SHMT}



all:    shmt
	



clean:
	$(RMVFR) *.o 
