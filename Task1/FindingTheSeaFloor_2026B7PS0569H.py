# AARUSH VASHISTH
# 2026B7PS0569H

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Read the CSV file and extract Points (x) and Depth (y)
data = pd.read_csv('Depth Data.csv')
x = data['Point']
y = pd.to_numeric(data['Depth (m)'], errors='coerce') # Handles the "#VALUE!"

# Using subplots for FuncAnimation
fig, ax = plt.subplots()
ax.set_title("Athena's Intern - Finding the Sea Floor")
ax.set_xlabel("Points (in sec)")
ax.set_ylabel("Depth (in m)")

# Finding average of the two neighbouring values and using mask to store the y values which cross the threshold of 60m
avg = (y.shift(1) + y.shift(-1))/2
mask = abs(y - avg) > 60
y[mask] = float('nan')

# Report for the bad points in the CSV file
bad_points = y.isna().sum()
# print(f"Data Cleaning Report: Found and removed {bad_points} anomalous data points.")
# Output: Data Cleaning Report: Found and removed 7 anomalous data points.

# Interpolate the bad values
y = y.interpolate()

# Rolling mean to smoothen out the graph, ie reduce noise
y = y.rolling(window=5, min_periods=1).mean()

line, = ax.plot([], [], lw=2)

# Initializing function - sets up the blank canvas before animation function runs
def init():
    line.set_data([], [])
    return line,

# Animating function - gets called for every frame by FuncAnimation
def animate(i):
    line.set_data(x.iloc[:i], y.iloc[:i])
    # Using recalculating limits and autoscale (zooming out) instead of predefined limits
    ax.relim()
    ax.autoscale_view()
    return line,

ani = FuncAnimation(fig, animate, init_func=init, frames=len(x)+1, interval=1000, blit=False, repeat=False)

plt.tight_layout()
plt.show()