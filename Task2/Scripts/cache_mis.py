from __future__ import print_function
import matplotlib.pyplot as plt
import pandas as pd
from sys import argv

perfs = argv[2:]
plt.figure(figsize=(8,8))
plt.xlabel('Iteration number')
plt.ylabel('Average cache miss (window=100)')
plt.title('L1 Data Cache Stats')

for ver in perfs:
	cols = ['row','l1d_read_acc','l1d_read_mis']
	data = pd.read_csv('./log/'+ver+'/perfLog_48.csv',header=None,names=cols)
	data = data.astype(float)
	
	rat = data[cols[2]]
	plt.plot(rat.rolling(window=100).mean().values)


if len(perfs)==1:
	plt.savefig('./Graphs/l1dcache_mis_'+argv[2]+'.png')
else:
	plt.legend(perfs)
	plt.savefig('./Graphs/l1dcache_mis_comp_'+argv[1]+'.png')

