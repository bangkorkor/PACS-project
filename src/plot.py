# Load the data
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('build/plotting_data.csv')

# Plot Temperature
plt.figure(figsize=(10, 6))
plt.plot(data['t'], label='Temperature', color='red')
plt.title('Temperature')
plt.xlabel('Distance')
plt.ylabel('Temperature (K)')
plt.legend()
plt.grid(True)
plt.savefig('build/Temperature_Plot.png')
plt.close()  # Close the figure to avoid display

# Plot Pressure
plt.figure(figsize=(10, 6))
plt.plot(data['p'], label='Pressure', color='blue')
plt.title('Pressure')
plt.xlabel('Distance')
plt.ylabel('Pressure (Pa)')
plt.legend()
plt.grid(True)
plt.savefig('build/Pressure_Plot.png')
plt.close()  # Close the figure to avoid display