CFLAGS=-Wall -I/usr/local/include -I/usr/local/include/mysql -O -pipe -mcpu=pentiumpro -D_THREAD_SAFE
LDFLAGS=-Wall -L/usr/local/lib -lsmbclient -L/usr/local/lib/mysql -lmysqlclient -lcrypt -lm  -lz

OBJECTS=scanner.o \
	utils/block.o utils/entry.o utils/functions.o \
	database/database.o database/database_mysql.o \
	engines/engine.o engines/engine_samba.o

all: ${OBJECTS}
	g++ ${CFLAGS} ${LDFLAGS} -o scanner  ${OBJECTS}
