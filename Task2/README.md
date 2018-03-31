## Description
This is part of an experiment with optimizing OpenMP program on solving the N-body problem. The specifications of the system on which this is run is given in specifications.txt. Standard outputs can be found in Output folder.

### The workflow

* Step 1: tracking the thread time ( v0.0 )  

   First I have run a naive implmentation with a different number of threads. The total time taken in each case is depicted in the     chart below:  
   <img src="https://github.com/mayank42/OpenMP-MPI-Tasks/blob/master/Task2/Graphs/total_time.png" width="600" height="500" />  
   Also the per iteration dynamics can be seen in the graph below:  
   <img src="https://github.com/mayank42/OpenMP-MPI-Tasks/blob/master/Task2/Graphs/iteration_time.png" width="600" height="500" />  
   
   
* Step 2: loop fusion ( v0.1 )  

   I have fused the first three loops from version 0.1. This is increased a performance a little bit, specially in iteration time. Below is a per iteration time comparison graph ( the heights are cut off to focus on comparision ):  
   <img src="https://github.com/mayank42/OpenMP-MPI-Tasks/blob/master/Task2/Graphs/time_comp_1.png" width="600" height="500" />  
   Below is a comparision of L1 data cache hit ratio:  
   <img src="https://github.com/mayank42/OpenMP-MPI-Tasks/blob/master/Task2/Graphs/l1dcache_comp_1.png" width="600" height="500" />  
   
* Step 3: cache line false sharing ( v0.2 )  

   In this version I have tried to reduce the false sharing of data among threads by specifying the minimum chunk size of scheduling the for loop. The minimum chunk size was chosen so that the data required in those many iterations fit into an integral number of cache line - here 64. Below is the time comparision graph:  
   <img src="https://github.com/mayank42/OpenMP-MPI-Tasks/blob/master/Task2/Graphs/time_comp_2.png" width="600" height="500" />  
   Below is a comparision of L1 data cache hit ratio:  
   <img src="https://github.com/mayank42/OpenMP-MPI-Tasks/blob/master/Task2/Graphs/l1dcache_comp_2.png" width="600" height="500" />      
   Suprisingly the cache miss has increased while the cpu time has decreased. This leaves a heavy room for improvement. Perhaps rather than stopping false sharing we've just decreased some overhead for openmp threading.
   
* Step 4: TODO   

