from __future__ import print_function
import matplotlib.pyplot as plt
import pandas as pd

cols = ['total_cycles','last_cache_ref','last_cache_mis','l1d_read_acc','l1d_read_mis','l1d_write_acc','k1']
data = pd.read_csv('../log/perfLog_48.csv',header=None,names=cols)
data = data.astype(float)
plt.figure(figsize=(8,8))
plt.xlabel('Iteration number (/100)')
plt.ylabel('Average total cycles (per 100 iteration)')
plt.title('CPU Cycle Stats')
plt.plot(data[cols[0]].rolling(window=100).mean().values)
plt.savefig('cycles_v0.0.png')

plt.figure(figsize=(8,8))
plt.xlabel('Iteration number (/100)')
plt.ylabel('Hit/Ref ratio (per 100 iteration)')
plt.title('Last Level Cache Stats')
rat = data[cols[2]]/data[cols[1]]
print(rat.shape)
rat = pd.Series([1.0]*len(rat),dtype='float')-rat
plt.plot(rat.rolling(window=100).mean().values)
plt.savefig('llcache_v0.0.png')

plt.figure(figsize=(8,8))
plt.xlabel('Iteration number (/100)')
plt.ylabel('Hit/Ref ratio (per 100 iteration)')
plt.title('L1 Data Cache Stats')
rat = data[cols[4]]/data[cols[3]]
print(rat.shape)
rat = pd.Series([1.0]*len(rat),dtype='float')-rat
plt.plot(rat.rolling(window=100).mean().values)
plt.savefig('l1dcache_v0.0.png')

