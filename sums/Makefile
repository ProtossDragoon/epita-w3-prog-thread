# Compiler
CC = gcc

# Compiler flags
# -std=c99: C standard version
# -g: make debuggable
# -fsanitize=address: help to find memory leak
# -Wall: show all warnings
# -Wextra: show extra warnings
# -O3: set high optimization
CFLAGS = -std=c99 -g -fsanitize=address -Wall -Wextra -O3

# Linker flags
LDFLAGS = -fsanitize=address -pthread

# File variables
OBJ = split.o dnc.o
DEP = ${OBJ:.o=.d}
EXC = split dnc

# Instructions
all: ${EXC}

split: split.o
dnc: dnc.o

.PHONY: clean
clean:
	${RM} ${OBJ} ${DEP} ${EXC}
