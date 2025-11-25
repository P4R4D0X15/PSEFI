error_dir = ../error/
common_dir = ../common/
holdall_dir = ../holdall/

CC = gcc
CFLAGS = 
	-std=c2x -D_XOPEN_SOURCE -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -fstack-protector-all -fpie -pie -O2 -D_FORTIFY_SOURCE=2 -g3 -lc -D_POSIX_C_SOURCE=200809L -lpthread -I$(error_dir)

vpath %.c $(error_dir) $(common_dir) $(holdall_dir)
vpath %.h $(error_dir) $(common_dir) $(holdall_dir)
objects = client.o error.o common.o holdall.o queue.o
executable = client

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) 

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

client.o: client.c error.h common.h
error.o: error.c error.h
common.o: common.c common.h
holdall.o: holdall.c holdall.h
queue.o: queue.c queue.h