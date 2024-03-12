all: main

# toutes les librairies
LIB = $(LIBP) -lm -lblas -llapack

COPT = -O3 -Wall

default: main

clean: 
	rm *.o
	rm main

main : main.cpp Source.o Beam.o Cell.o createMap.o Wall.o createMap.o Graphics.o utils.o Map.o Optimization.o Chromosome.o
	g++ $(COPT) -o $@ $^

%.o: %.c
	g++ $(COPT) -c $< -o $@