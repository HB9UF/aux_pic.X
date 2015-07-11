from matplotlib import pyplot as plt
import numpy as np

plt.xkcd()

fig = plt.figure(figsize=(10,7))
ax = fig.add_subplot(3, 1, 1)
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.xticks([])
plt.yticks([])
ax.set_ylim([0, 1.1])

x = np.linspace(0, 2*np.pi)
y = (np.sin(x)+1)/2.

n = len(x)
k = 3
x_samp = x[::k]
y_samp = y[::k]

plt.plot(x, y+0.05, 'b-', x_samp, y_samp+0.05, 'ro')

ax = fig.add_subplot(3, 1, 2)

ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.xticks([])
plt.yticks([])
ax.set_ylim([0, 1.1])

def get_pwm_lines(i):
    x = np.linspace(0, k/10., 100)
    y = np.linspace(0, 1, 100) < y_samp[i]
    y[0] = 0
    return (x_samp[i] + x, y + 0.05)
    
steps = []
for i in range(len(x_samp)):
    steps.append(get_pwm_lines(i))

steps_x = steps[0][0]
steps_y = steps[0][1]

for i in range(1, len(steps)):
     ret = steps[i]
     steps_x = np.append(steps_x, ret[0])
     steps_y = np.append(steps_y, ret[1])

plt.plot(steps_x, steps_y)

ax = fig.add_subplot(3, 1, 3)

ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.xticks([])
plt.yticks([])
ax.set_ylim([0, 1.1])

def get_avg_lines(i):
    x = np.linspace(0, k/10., 100)
    y = np.repeat(y_samp[i], len(x))
    return (x_samp[i] + x, y + 0.05)
    
steps = []
for i in range(len(x_samp)):
    steps.append(get_avg_lines(i))

steps_x = steps[0][0]
steps_y = steps[0][1]

for i in range(1, len(steps)):
     ret = steps[i]
     steps_x = np.append(steps_x, ret[0])
     steps_y = np.append(steps_y, ret[1])

plt.plot(steps_x, steps_y)

plt.savefig('dds.png', bbox_inches='tight')
plt.show()
