import re
import matplotlib.pyplot as plt
p = re.compile('Total time elapsed   :   (\d\d\d\d)')
threads = ['16','24','32','40','48','60','96']
time = []
with open('./Output/Output 1','r') as f:
	for line in f:
		m = p.match(line)
		if m!=None:
			time.append(m.group(1))

plt.figure(figsize=(8,8))
plt.bar(range(7),[int(a) for a in time])
plt.ylabel('Time to completion (seconds)')
plt.title('Total time')
plt.xticks(range(7),threads)
plt.xlabel('Threads')
plt.savefig('total_time.png')
