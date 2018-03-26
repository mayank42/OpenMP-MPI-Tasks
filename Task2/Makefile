simulation: include/sim.hpp src/pathGen.cpp
	g++ -c -I include/ src/pathGen.cpp -o build/Sim.o
	g++ build/Sim.o -fopenmp -o bin/Sim

run:
	bin/./Sim $(THREADS)

clean:
	rm -rf build/*
	rm -rf bin/*
	
