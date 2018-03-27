import pandas as pd
from sys import argv
from math import sqrt
traj1 = pd.read_csv('./log/'+argv[1]+'/trajectory_48.csv',headers=None);
traj2 = pd.read_csv('./log/'+argv[2]+'/trajectory_48.csv',headers=None);
err = 0.0
for i in range(len(traj1)):
	diff=traj1.iloc[i]-traj2.iloc[i]
	diff = diff*(10**14)
	diff = diff.abs()
	diff = diff**2
	diff = diff.sum()
	diff = sqrt(diff)
	err = err + diff

print('Total error = '+str(err))
		

