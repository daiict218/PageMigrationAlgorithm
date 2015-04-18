#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

with open("example1.txt") as f:
    data = f.read()

data = data.split('\n')

x = [row.split(' ')[0] for row in data]
#print x
y = [row.split(' ')[1] for row in data]
#print x,y
fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Memory Power Consumption Graph")    
ax1.set_xlabel('iteration')
ax1.set_ylabel('Power')

ax1.plot(x,y, c='b', label='the data')

leg = ax1.legend()

plt.show()