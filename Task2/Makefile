CC=g++
CFLAGS=-Wall -Werror -g
MINOR=2
ifndef TARGET
TARGET=Sim
endif
ifndef MAJOR
MAJOR=0
endif
ifndef MINOR
MINOR=0
endif
VERSION=v$(MAJOR).$(MINOR)
ifndef THREADS
THREADS=48
endif


$(TARGET): build/$(VERSION)/Sim.o
	@mkdir bin/$(VERSION)
	$(CC) $(CFLAGS) build/$(VERSION)/Sim.o -fopenmp -o bin/$(VERSION)/$(TARGET)
build/$(VERSION)/Sim.o: include/sim.hpp src/$(VERSION)/pathGen.cpp
	@mkdir build/$(VERSION)
	$(CC) $(CFLAGS) -Iinclude/ -c -fopenmp src/$(VERSION)/pathGen.cpp -o build/$(VERSION)/Sim.o

run:
	@mkdir log/$(VERSION)
	bin/$(VERSION)/./$(TARGET) $(THREADS)

clean:
	rm -rf build/*
	rm -rf bin/*
	rm -rf log/$(VERSION)
