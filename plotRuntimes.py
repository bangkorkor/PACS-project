# code for plotting the runtimes 

import pandas as pd
import matplotlib.pyplot as plt

standard_runtimes = [70003,60088,60682,61871,61031,79923,59911,65497,91273,61514,64557,61706,62615,63007 ,63325,62719,60557,62525,78450 ,65163,63588,63466,62925,62690,60515,97845,61726,64990,61888,61212,61057]
optimized_runtimes = [37684,35208,33498,37886,35047,33893,38032,42781,40444,37512,40891,44615,61475,46788,37231,47288,86890,78785,41959,48846,45381,38571,44147,38101,99440,38017,55011,39562,49066,48706,37504]

plt.figure(figsize=(12, 8))
plt.plot(standard_runtimes, label='Standard Runtimes', color='red')
plt.plot(optimized_runtimes, label='Optimized Runtimes', color='blue')
# i only want the x-axis to be the attempt number
plt.xticks([i for i in range(1, len(standard_runtimes))])
plt.title(f'Runtimes Comparison')

plt.xlabel('Attempt Number')
plt.ylabel('Runtime (microsecond)')
plt.legend()
plt.grid(True)
plt.savefig('build/Runtimes_Plot.png')

plt.close()  # Close the figure to avoid display

# print the average runtime for each and print how much quicker the optimized version is
average_standard = sum(standard_runtimes) / len(standard_runtimes)
average_optimized = sum(optimized_runtimes) / len(optimized_runtimes)
print(f'Average Standard Runtime: {average_standard:.2f} microseconds')
print(f'Average Optimized Runtime: {average_optimized:.2f} microseconds')
print(f'Optimized version is {average_standard / average_optimized:.2f} times quicker')

