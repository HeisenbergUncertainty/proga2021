import numpy as np
import matplotlib.pyplot as plt

plt.rc('text', usetex=True)
plt.rc('font', family='Times New Roman', weight='normal', size=14)
plt.rcParams['mathtext.fontset'] = 'stix'

out1 = np.genfromtxt('out1.txt', delimiter=" ", skip_header=True)
out2 = np.genfromtxt('out2.txt', delimiter=" ", skip_header=True)

geomprob1 = 0.006
count1 = out1[1:, 0]
prob1 = out1[1:, 1]

#geomprob2 = 0.0045
count2 = out2[1:, 0]
prob2 = out2[1:, 1]

fig, ax = plt.subplots()

x = np.arange(0, 990000, 1000)
ax.plot(x, 0*x + geomprob1, linewidth=1, color='black')

ax.errorbar(
    x=count1,
    y=prob1,
    fmt='o',
    markersize=2,
    linestyle='None',
    color='orange',
    linewidth=0.5,
    capsize=1,
    label='Segment State')

ax.errorbar(
    x=count2,
    y=prob2,
    fmt='o',
    markersize=2,
    linestyle='None',
    color='blue',
    linewidth=0.5,
    capsize=1,
    label='OrState')

ax.legend()
plt.title('')
#plt.ylim(0, 1)
plt.grid(axis='both')
plt.xlabel('Test Count')
plt.ylabel('')
plt.show()
