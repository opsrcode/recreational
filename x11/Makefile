CC := gcc
CFLAGS := -Wall -Wextra -Werror -Wpedantic -ggdb -std=c99
CLIBS :=  $(shell pkg-config --libs x11)
LDFLAGS := $(shell pkg-config --cflags x11)
TARGET := paint
SRCS := ./src/main.c
OBJS := ${SRCS:.c=.o}

${TARGET} : ${OBJS}
	$(CC) ${CFLAGS} ${LDFLAGS} -o ${TARGET} ${OBJS} ${CLIBS}

.PHONY : clean
clean:
	@rm -rf ${TARGET} ${OBJS}
