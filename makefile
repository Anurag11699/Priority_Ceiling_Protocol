CC=gcc

flags=-c -Wall

executableName=test

driver=driver


all: $(driver).o priority_queue_functions.o scheduling_functions.o
	$(CC)  $(driver).o priority_queue_functions.o scheduling_functions.o -o $(executableName)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c


ready_queue_functions.o: priority_queue_functions.c
	$(CC) $(flags) priority_queue_functions.c

scheduling_functions.o: scheduling_functions.c
	$(CC) $(flags) scheduling_functions.c

clean:
	rm -f *.o $(executableName) OUTPUT.txt
