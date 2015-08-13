
CC=gcc
INCLUDE=gameoflife.h
FLAGS= -I.

all: simulation

simulation: simulation.o gameoflife.o
	$(CC) -o $@ simulation.o gameoflife.o $(FLAGS)

%.o:%.c $(INCLUDE)
	$(CC) -c -o $@ $< $(FLAGS)
