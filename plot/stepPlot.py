from matplotlib import pyplot as plt
import numpy as np

data = open("walking_test1_edited.txt")

dataPoints = []

while True:
    line = data.readline()
    endpos = line.find("z=")
    if endpos == -1: break
    value = line[endpos + 2:]
    value = float(value)
    dataPoints.append(value)

plt.plot(dataPoints)
plt.title("Z-Position over time")
plt.show()

data.close()