import pandas as pd
import matplotlib.pyplot as plt
import sys

# Load the data
data = pd.read_csv('build/plotting_data.csv')

# Extract parameters from command-line arguments
params = sys.argv[1:]

# Separate and format parameters
formatted_params = []
for param in params:
    key, value = param.split('=')
    try:
        # Try to convert value to float for numeric parameters
        value = float(value)
        formatted_params.append(f"{key}={value:.2f}")
    except ValueError:
        # If conversion fails, it is a string parameter
        formatted_params.append(f"{key}={value}")

# Create a string representation of the formatted parameters for the plot title
param_str = ', '.join(formatted_params)

# Plot Temperature
plt.figure(figsize=(10, 6))
plt.plot(data['t'], label='Temperature Profile', color='red')
plt.title(f'Temperature\nParameters used: ({param_str})')
plt.xlabel('Num_Mods')
plt.ylabel('Temperature (K)')
plt.legend()
plt.grid(True)
plt.savefig('build/Temperature_Plot.png')
plt.close()  # Close the figure to avoid display

# Plot Pressure
plt.figure(figsize=(10, 6))
plt.plot(data['p'], label='Pressure Profile', color='blue')
plt.title(f'Pressure\nParameters used: ({param_str})')
plt.xlabel('Num_Mods')
plt.ylabel('Pressure (Pa)')
plt.legend()
plt.grid(True)
plt.savefig('build/Pressure_Plot.png')
plt.close()  # Close the figure to avoid display