error_dir = ../error/
common_dir = ../common/
worker_dir = worker/
img_dir = img/

CC = gcc
CFLAGS = 
	-std=c2x -D_XOPEN_SOURCE -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -fstack-protector-all -fpie -pie -O0 -g3 -D_FORTIFY_SOURCE=2 -lc -D_POSIX_C_SOURCE=200809L -lpthread -I$(error_dir)

vpath %.c $(error_dir) $(common_dir) $(worker_dir) $(img_dir)
vpath %.h $(error_dir) $(common_dir) $(worker_dir) $(img_dir)
objects = serveur.o error.o common.o queue.o worker.o filter.o
executable = serveur

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) 

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

serveur.o: serveur.c error.h common.h queue.h
error.o: error.c error.h
common.o: common.c common.h
queue.o: queue.c queue.h
worker.o: worker.c worker.h filter.h
filter.o: filter.h filter.c