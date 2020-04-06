CC=gcc

flags=-c -Wall

executableName=test

driver=driver


all: $(driver).o ready_queue_functions.o scheduling_functions.o
	$(CC)  $(driver).o ready_queue_functions.o scheduling_functions.o -o $(executableName)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c


ready_queue_functions.o: ready_queue_functions.c
	$(CC) $(flags) ready_queue_functions.c

scheduling_functions.o: scheduling_functions.c
	$(CC) $(flags) scheduling_functions.c

clean:
	rm -f *.o $(executableName) OUTPUT.txt
