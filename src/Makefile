OBJS = bartlby.o config.o global.o sched.o shm.o check.o trigger.o daemon.o replication.o perf.o downtime.o

.SUFFIXES: .c .S .o

.c.o:

	gcc -g  -I../include/  -Wall -g -c $<


install: all
	cp -v ../bartlby ${BARTLBY_HOME}/bin/
	cp -v ../bartlby ${BARTLBY_HOME}/
	

bartlby: ${OBJS}
	gcc  -g -I../include/ -ldl  -o ../bartlby ${OBJS}



all:    bartlby


clean:
	rm -fv *.o 
