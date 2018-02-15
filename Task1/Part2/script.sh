#!/bin/bash
window=10
min_intensity=20
if [ $# -lt 1 ]
then
	echo "Using default values for window (10) & minimum intensity(20)"
	echo "usage: $0 [window] [minimum intensity]"
elif [ $# -lt 2 ]
then
	echo "Using default value for minimum intensity(20)"
else
	window=$1
	min_intensity=$2
fi
echo -n "Building code..."
g++ -fopenmp -std=c++11 Asgn2.cpp -o hist
out=$?
if [ $out -ne 0 ]
then 
	echo "Build failed. Exiting."
	exit $out
fi
echo "Done."
echo "Generating histogram..."
./hist Julia_IIM_6_circle.pgm $window $min_intensity
out=$?
if [ $out -ne 0 ]
then
	echo "Histogram generation failed. Exiting."
	exit $out
else
	echo "Done."
fi
echo -n "Generating graph..."
python graph.py
out=$?
if [ $out -ne 0 ]
then
	echo "[ Fail ]"
	echo "Exiting."
	exit $out
else
	echo "Done."
fi

echo -n "Cleaning files..."
rm hist
echo "Done."
