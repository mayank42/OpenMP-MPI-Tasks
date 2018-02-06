import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('Histogram.txt',header=None,names=['Vals'])
data = data['Vals'].values
plt.figure(figsize=(10,10))
plt.plot(data)
plt.ylabel('Normalized Frequency')
plt.xlabel('Gray Level')
plt.savefig('Histogram.png')

