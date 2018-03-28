from __future__ import print_function
import pandas as pd
import matplotlib.pyplot as plt
from sys import argv
import numpy as np

times = argv[3:]

plt.figure(figsize=(8,8))
plt.xlabel('Iteration number')
plt.ylabel('Average Time taken (window=100)')
plt.title('Iteration Time Graph')
cutter = lambda x: min(x,float(argv[1]))
vcut = np.vectorize(cutter)

for name in times:
	timeData = pd.read_csv('./log/'+name+'/timeLog_48.csv',header=None,names=['Iteration','Time'])
	plt.plot(vcut(timeData['Time'].rolling(window=100).mean().values))

if len(times)==1:
	plt.savefig('./Graphs/time_'+argv[3]+'.png')
else:
	plt.legend(times)
	plt.savefig('./Graphs/time_comp_'+argv[2]+'.png')


