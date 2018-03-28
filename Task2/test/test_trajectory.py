from __future__ import print_function
import pandas as pd
from sys import argv
from math import sqrt
traj1 = pd.read_csv('./log/'+argv[1]+'/trajectory_48.csv',header=None);
traj2 = pd.read_csv('./log/'+argv[2]+'/trajectory_48.csv',header=None);
err = 0.0
for i in range(len(traj1)):
	diff=traj1.iloc[i]-traj2.iloc[i]
	diff = diff*(10**14)
	diff = diff**2
	diff = diff.mean()
	diff = sqrt(diff)
	diff = diff*(10**-14)
	err = err + diff
err = err/(len(traj1))
print('Total error = '+str(err))
		

