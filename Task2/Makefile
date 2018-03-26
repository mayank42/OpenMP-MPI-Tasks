CC=g++
CFLAGS=
ifndef TARGET
TARGET=Sim
endif

$(TARGET): build/Sim.o
	$(CC) $(CFLAGS) build/Sim.o -fopenmp -o bin/$(TARGET)
build/Sim.o: include/sim.hpp src/pathGen.cpp
	$(CC) $(CFLAGS) -Iinclude/ -c src/pathGen.cpp -o build/Sim.o

run:
	bin/./$(TARGET) $(THREADS)

clean:
	rm -rf build/*
	rm -rf bin/*
	
