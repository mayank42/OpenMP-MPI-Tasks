#!/bin/bash
if [ $# -lt 2 ]
then
	echo "usage: $0 <window> <minimum intensity>"
	exit 1
fi
echo -n "Building code..."
g++ -fopenmp -std=c++11 Asgn2.cpp -o hist
out=$?
if [ $out -ne 0 ]
then 
	exit $out
fi
echo "Done."


if [ $out -eq 0 ] 
then
	echo  "Generating histogram..."
	./hist Julia_IIM_6_circle.pgm $1 $2 
	out=$?
	echo "Done."
fi

if [ $out -eq 0 ] 
then
	echo -n "Generating graph..."
	python graph.py
	echo "Done."
fi

echo -n "Cleaning files..."
rm hist
echo "Done."
