CC = gcc

CFLAGS = -std=c2x -D_XOPEN_SOURCE -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -fstack-protector-all -fpie -pie -O2 -D_FORTIFY_SOURCE=2 -g3 -lc -D_POSIX_C_SOURCE=200809L -lpthread

TARGET = client

SRCS = client.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@ 

clean:
	rm -f $(TARGET) $(OBJS) 
