OBJS = main.o mainFunctions.o stateList.o recordList.o bloomFilter.o skipList.o
SOURCE = main.c mainFunctions.c stateList.c recordList.c bloomFilter.c skipList.c
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

stateList.o:stateList.c
	$(CC) $(FLAGS) stateList.c	

recordList.o:recordList.c
	$(CC) $(FLAGS) recordList.c

bloomFilter.o:bloomFilter.c
	$(CC) $(FLAGS) bloomFilter.c

skipList.o:skipList.c
	$(CC) $(FLAGS) skipList.c		

clean:@echo "Cleaning up..."
	rm -vf $(OBJS) $(OUT) 