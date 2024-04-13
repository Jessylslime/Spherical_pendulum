
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
import sys

matplotlib.use('agg')

arguments = sys.argv[1:]


num_of_test = arguments[0]
time, theta, fi, v_th, v_fi = np.loadtxt('C:/games/User_Files/QT/work/Data/' + num_of_test + '/data.txt', unpack=True)

with open('C:/games/User_Files/QT/work/Data/' + num_of_test + '/param.txt', 'r') as file:
    lines = file.readlines()
params = dict()

for line in lines:
    name, value = line.split(":")
    params[name] = float(value)

r = params["r"]
time = np.array(time)
theta = np.array(theta)
fi = np.array(fi)
v_fi = np.array(v_fi)
v_th = np.array(v_th)
#v = np.sqrt((r*v_fi*np.sin(theta))**2 + (r*v_th)**2)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

u = np.linspace(0, 2 * np.pi, 100)
v = np.linspace(0, np.pi, 100)

x = r * np.outer(np.cos(u), np.sin(v))
y = r * np.outer(np.sin(u), np.sin(v))
z = r * np.outer(np.ones(np.size(u)), np.cos(v))

x_curve = r*np.cos(fi)*np.sin(theta)
y_curve = r*np.sin(fi)*np.sin(theta)
z_curve = -r*(np.cos(theta))

ax.plot(x_curve, y_curve, z_curve, color='r')
ax.plot_surface(x, y, z, color='b', alpha=0.1)
ax.set_aspect('equal')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.savefig('C:/games/User_Files/QT/work/Data/' + num_of_test + '/graph.png')