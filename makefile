CC ?= gcc
CFLAGS ?= -O3 -std=c17 -flto -Wall -Wextra -pipe
TARGET := demo.out

SRCS := demo.c Sudoku.c sudoku_solver.c
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm $(OBJS) $(DEPS) $(TARGET)
