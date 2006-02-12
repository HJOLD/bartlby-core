include ../../Makefile.conf

SHMT = ../shm.o ../config.o ../global.o shmt.o



.SUFFIXES: .c .S .o

.c.o:

	$(CC) $(EXTRAOPTIONS)   $(INCLUDE_LL)  -c $<




shmt: ${SHMT}
	$(CC)  $(EXTRAOPTIONS) $(INCLUDE_LL) -o ../../shmt ${SHMT}



all:    shmt
	



clean:
	$(RMVFR) *.o 
