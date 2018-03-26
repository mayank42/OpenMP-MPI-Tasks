THREADS=48
echo "Cleaning up first"
make clean
make
echo "-------------------------------------------------------"
echo "Simulating with $THREADS threads"
START_TIME="$(date -u +%s)"
make run THREADS=$THREADS
END_TIME="$(date -u +%s)"
ELAPSED="$((END_TIME-START_TIME))"
echo "Total time elapsed   :   $ELAPSED"
echo "-------------------------------------------------------"


