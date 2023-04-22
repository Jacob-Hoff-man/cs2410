CC = g++
CFLAGS = -std=c++11 -c -Wall -pedantic -g
# input parameters - taken from project.pdf
INP_FILE_NAME="prog.dat"
NF=4 # number of instructions fetched per cycle by the fetch unit
NI=16 # instuction queue max capacity of instructions held in the decode unit
NW=4 # number of instructions issued per cycle to reservation stations
NR=16 # number of entries available in the circular reorder buffer (ROB)
NB=4 # number of common data buses

all: main
debug:
	./main $(INP_FILE_NAME) $(NF) $(NI) $(NW) $(NR) $(NB) 1
run: 
	./main $(INP_FILE_NAME) $(NF) $(NI) $(NW) $(NR) $(NB)

main: main.o simulator.o fetch.o decode.o issue.o
	$(CC) -o main main.o simulator.o fetch.o decode.o issue.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

simulator.o: simulator.cpp
	$(CC) $(CFLAGS) simulator.cpp

fetch.o: fetch.cpp
	$(CC) $(CFLAGS) fetch.cpp

decode.o: decode.cpp
	$(CC) $(CFLAGS) decode.cpp

issue.o: issue.cpp
	$(CC) $(CFLAGS) issue.cpp

clean: 
	rm *.o main