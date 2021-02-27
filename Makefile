OBJS = main.o
# OBJS = main.o functions.o treeFunctions.o heapFunctions.o mainFunctions.o
# SOURCE = main.c functions.c treeFunctions.c heapFunctions.c mainFunctions.c
SOURCE = main.c
HEADER = structs.h
OUT = vaccineMonitor
CC = gcc
FLAGS = -g3 -c -Wall

all:$(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

vaccineMonitor.o:main.c
	$(CC) $(FLAGS) main.c

# functions.o:functions.c
# 	$(CC) $(FLAGS) functions.c

# treeFunctions.o:treeFunctions.c
# 	$(CC) $(FLAGS) treeFunctions.c

# heapFunctions.o:heapFunctions.c
# 	$(CC) $(FLAGS) heapFunctions.c

# mainFunctions.o:mainFunctions.c
# 	$(CC) $(FLAGS) mainFunctions.c		

clean:@echo "Cleaning up..."
	rm -vf $(OBJS) $(OUT) 