OBJS = bartlby.o config.o global.o sched.o shm.o check.o trigger.o daemon.o

.SUFFIXES: .c .S .o

.c.o:

	gcc -g  -I../include/  -Wall -g -c $<



bartlby: ${OBJS}
	gcc  -g -I../include/ -ldl  -o ../bartlby ${OBJS}



all:    bartlby


clean:
	rm -fv *.o 
