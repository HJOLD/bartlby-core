FIREBIRD = ../global.o firebird.o ../config.o
MYSQL = ../global.o ../config.o mysql.o



.SUFFIXES: .c .S .o

.c.o:

	gcc -g  -I/usr/include/mysql/ -I../../include -Wall -g -c $<



firebird: ${FIREBIRD}
	gcc  -lfbclient -fPIC -shared  -g -I../../include/ -o ../../firebird.so ${FIREBIRD}

mysql: ${MYSQL}
	gcc -I /usr/include/mysql/ -lmysqlclient -fPIC -shared  -g -I../../include/ -o ../../mysql.so ${MYSQL}



all:    firebird mysql
	



clean:
	rm -fv *.o 
