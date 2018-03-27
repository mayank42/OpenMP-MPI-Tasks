from __future__ import print_function
import matplotlib.pyplot as plt
import pandas as pd

cols = ['row','l1d_read_acc','l1d_read_mis']
data = pd.read_csv('./log/v0.0/perfLog_48.csv',header=None,names=cols)
data = data.astype(float)

plt.figure(figsize=(8,8))
plt.xlabel('Iteration number (/100)')
plt.ylabel('Hit/Ref ratio (per 100 iteration)')
plt.title('L1 Data Cache Stats')
rat = data[cols[2]]/data[cols[1]]
print(rat.shape)
rat = pd.Series([1.0]*len(rat),dtype='float')-rat
plt.plot(rat.rolling(window=100).mean().values)
plt.savefig('l1dcache_v0.0.png')

