from __future__ import print_function
import pandas as pd
import matplotlib.pyplot as plt
from sys import argv

times = argv[1:]

plt.figure(figsize=(8,8))
plt.xlabel('Iteration number')
plt.ylabel('Average Time taken (per 100 iteration)')
plt.title('Iteration Time Graph')

for name in times:
	timeData = pd.read_csv('./log/'+name+'/timeLog_48.csv',header=None,names=['Iteration','Time'])
	plt.plot(timeData['Time'].rolling(window=100).mean().values)

if len(times)==1:
	plt.savefig('./Graphs/time_'+argv[1]+'.png')
else:
	plt.legend(times)
	plt.savefig('./Graphs/time_comp.png')


