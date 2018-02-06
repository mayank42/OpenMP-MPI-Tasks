import pandas as pd
import matplotlib.pyplot as plt
#Schedule graph
data = pd.read_csv('Result1.txt')
data = data.values
plt.figure(figsize=(10,10))
plt.plot(data[:,1])
plt.plot(data[:,2]);
plt.plot(data[:,3]);
plt.xlabel('Matrix dimension')
plt.ylabel('Time')
plt.legend(['Static Scheduling','Dynamic Scheduling','Guided Scheduling'])
plt.savefig('Schedule_Graph.png')
#Reduction graph
data = pd.read_csv('Result11.txt')
data = data.values
plt.figure(figsize=(10,10))
plt.plot(data[:,1])
plt.plot(data[:,2])
plt.xlabel('Matrix dimension')
plt.ylabel('Time')
plt.legend(['Without Reduction','With Reduction'])
plt.savefig('Reduction_Graph.png')
#Collapse graph
data = pd.read_csv('Result12.txt')
data = data.values
plt.figure(figsize=(10,10))
plt.plot(data[:,1])
plt.plot(data[:,2])
plt.xlabel('Matrix dimension')
plt.ylabel('Time')
plt.legend(['Without Collapse','With Collapse'])
plt.savefig('Collapse_Graph.png')
