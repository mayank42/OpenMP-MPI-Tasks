from __future__ import print_function
import pandas as pd
import matplotlib.pyplot as plt
timefiles = ['timeLog_16.csv','timeLog_24.csv','timeLog_32.csv','timeLog_40.csv','timeLog_48.csv','timeLog_60.csv','timeLog_96.csv']

plt.figure(figsize=(8,8))
plt.xlabel('Iteration number')
plt.ylabel('Average Time taken (window=100)')
for name in timefiles:
	timeData = pd.read_csv(name,header=None,names=['Iteration','Time'])
    	print('Plotting   :   '+name)
	plt.plot(timeData['Time'].rolling(window=100).mean().values)
plt.legend(['16 Threads','24 Threads','32 Threads','40 Threads','48 Threads','60 Threads','96 Threads'])
plt.savefig('Time.png')

