lscpu &>>specifications.txt
echo "Find specifications in specifications.txt"
echo "Building code   :"
make 

echo "-------------------------------------------------------"
#echo "Simulating with 1 thread."
#start_time="$(date -u +%s)"
#make run THREADS=1
#SIM1ID=$!
#end_time="$(date -u +%s)"
#elapsed="$(($end_time-$start_time))"
#echo "Total time elapsed   :   $elapsed"
#echo "-------------------------------------------------------"
#
#echo "Simulating with 8 threads."
#start_time="$(date -u +%s)"
#make run THREADS=8
#SIM8ID=$!
#end_time="$(date -u +%s)"
#elapsed="$(($end_time-$start_time))"
#echo "Total time elapsed   :   $elapsed"
#echo "-------------------------------------------------------"

echo "Simulating with 16 threads."
start_time="$(date -u +%s)"
make run THREADS=16
SIM16ID=$!
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total time elapsed   :   $elapsed"
echo "-------------------------------------------------------"

echo "Simulating with 24 threads."
start_time="$(date -u +%s)"
make run THREADS=24
SIM24ID=$!
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total time elapsed   :   $elapsed"
echo "-------------------------------------------------------"

echo "Simulating with 32 threads."
start_time="$(date -u +%s)"
make run THREADS=32
SIM32ID=$!
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total time elapsed   :   $elapsed"
echo "-------------------------------------------------------"

echo "Simulating with 40 threads."
start_time="$(date -u +%s)"
make run THREADS=40
SIM40ID=$!
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total time elapsed   :   $elapsed"
echo "-------------------------------------------------------"

echo "Simulating with 48 threads."
start_time="$(date -u +%s)"
make run THREADS=48
SIM48ID=$!
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total time elapsed   :   $elapsed"
echo "-------------------------------------------------------"

echo "Simulating with 60 threads."
start_time="$(date -u +%s)"
make run THREADS=60
SIM60ID=$!
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total time elapsed   :   $elapsed"
echo "-------------------------------------------------------"

echo "Simulating with 96 threads."
start_time="$(date -u +%s)"
make run THREADS=96
SIM96ID=$!
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total time elapsed   :   $elapsed"
echo "-------------------------------------------------------"

echo "Cleaning up   :"
make clean
