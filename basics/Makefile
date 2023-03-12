# Compiler
CC = gcc

# Compiler flags
# -std=c99: C standard version
# -g: make debuggable
# -fsanitize=address: help to find memory leak
# -Wall: show all warnings
# -Wextra: show extra warnings
CFLAGS = -std=c99 -g -fsanitize=address -Wall -Wextra

# Linker flags
LDFLAGS = -fsanitize=address -pthread

# File variables
OBJ = hello.o hello_id.o hello_parity.o
DEP = ${OBJ:.o=.d} 
EXC = hello hello_id hello_parity 

# Instructions
all: ${EXC} 

hello: hello.o
hello_id: hello_id.o
hello_parity: hello_parity.o

.PHONY: clean
clean:
	${RM} ${OBJ} ${DEP} ${EXC}
