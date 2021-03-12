OBJS = main.o mainFunctions.o recordList.o bloomFilter.o skipList.o
# OBJS = main.o functions.o treeFunctions.o heapFunctions.o mainFunctions.o
# SOURCE = main.c functions.c treeFunctions.c heapFunctions.c mainFunctions.c
SOURCE = main.c mainFunctions.c recordList.c bloomFilter.c skipList.c
HEADER = structs.h functions.h
OUT = vaccineMonitor
CC = gcc
FLAGS = -g3 -c -Wall

all:$(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

vaccineMonitor.o:main.c
	$(CC) $(FLAGS) main.c

mainFunctions.o:mainFunctions.c
	$(CC) $(FLAGS) mainFunctions.c

recordList.o:recordList.c
	$(CC) $(FLAGS) recordList.c

bloomFilter.o:bloomFilter.c
	$(CC) $(FLAGS) bloomFilter.c

skipList.o:skipList.c
	$(CC) $(FLAGS) skipList.c		

clean:@echo "Cleaning up..."
	rm -vf $(OBJS) $(OUT) 