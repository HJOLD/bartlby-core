include ../Makefile.conf

OBJS = bartlby.o config.o global.o sched.o shm.o check.o trigger.o daemon.o replication.o perf.o downtime.o ack.o event.o

.SUFFIXES: .c .S .o

.c.o:

	$(CC) $(EXTRAOPTIONS)  $(INCLUDE_L)   -c $<


install: all
	$(CPPVA) ../bartlby ${BARTLBY_HOME}/bin/
	$(CPPVA) ../bartlby ${BARTLBY_HOME}/
	$(CPPVA) ../mysql.shema ${BARTLBY_HOME}/
	
	

bartlby: ${OBJS}
	$(CC)  $(EXTRAOPTIONS) $(INCLUDE_L) $(LDL)  -o ../bartlby ${OBJS}



all:    bartlby



clean:
	$(RMVFR) *.o 
