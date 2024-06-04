import pandas as pd
import matplotlib.pyplot as plt
import sys

print("Script started...")  # Debug print

# Load the data
data = pd.read_csv('build/plotting_data.csv')

# Function to check for convergence in the data
def check_convergence(data_series, threshold=0.01):
    # Calculate the absolute differences between consecutive measurements
    diffs = abs(data_series.diff().iloc[1:])
    # Check if all differences are below the threshold
    return diffs.max() < threshold

# Extract parameters from command-line arguments
params = sys.argv[1:]

# Separate and format parameters
formatted_params = []
numeric_params = []
string_params = []
for param in params:
    key, value = param.split('=')
    try:
        # Try to convert value to float for numeric parameters
        value = float(value)
        numeric_params.append(f"{key}={value:.2f}")
    except ValueError:
        # If conversion fails, it is a string parameter
        string_params.append(f"{key}={value}")

# Create string representations of the formatted parameters for the plot title
numeric_param_str = ', '.join(numeric_params)
string_param_str = ', '.join(string_params)

# Check for convergence
temp_converged = check_convergence(data['t'])
pres_converged = check_convergence(data['p'])
convergence_status_t = "Converged" if temp_converged else "Not Converged"
convergence_status_p = "Converged" if pres_converged else "Not Converged"

# Print convergence status
print("Temperature Convergence Status:", convergence_status_t)
print("Pressure Convergence Status:", convergence_status_p)

# Plot Temperature
plt.figure(figsize=(12, 8))
plt.plot(data['t'], label='Temperature Profile', color='red')
plt.title(f'Temperature Profile - {convergence_status_t}\nNumeric: {numeric_param_str}\nString: {string_param_str}')
plt.xlabel('Num_Mods')
plt.ylabel('Temperature (K)')
plt.legend()
plt.grid(True)
plt.savefig('build/Temperature_Plot.png')
print("Temperature plot saved.")  # Debug print
plt.close()  # Close the figure to avoid display

# Plot Pressure
plt.figure(figsize=(12, 8))
plt.plot(data['p'], label='Pressure Profile', color='blue')
plt.title(f'Pressure Profile - {convergence_status_p}\nNumeric: {numeric_param_str}\nString: {string_param_str}')
plt.xlabel('Num_Mods')
plt.ylabel('Pressure (Pa)')
plt.legend()
plt.grid(True)
plt.savefig('build/Pressure_Plot.png')
print("Pressure plot saved.")  # Debug print
plt.close()  # Close the figure to avoid display

print("Script completed.")  # Debug print
