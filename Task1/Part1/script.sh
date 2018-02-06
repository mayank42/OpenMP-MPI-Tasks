#!/bin/bash
echo -n "Building files..."
g++ -fopenmp -std=c++11 Asgn1.cpp -o genSched
g++ -fopenmp -std=c++11 Asgn11.cpp -o guidedRed
g++ -fopenmp -std=c++11 Asgn12.cpp -o guidedCol
echo "Done."

echo -n "Generating matrices..."
python genMatrix.py
echo "Done."


echo -n "Generating results..."
./genSched 200 A.txt B.txt
./guidedRed 60 A.txt B.txt
./guidedCol 200 A.txt B.txt
echo "Done."

echo -n "Generating graphs..."
python graph.py
echo "Done."

echo -n "Cleaning files..."
rm genSched guidedRed guidedCol A.txt B.txt
echo "Done."
