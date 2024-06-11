import matplotlib.pyplot as plt
import numpy as np

input_filename = 'data/quicksert_thresholds.txt'

minimum_threshold = 0
maximum_threshold = 1000

with open(input_filename, 'r') as f:
    lines = f.readlines()
    thresholds = np.arange(minimum_threshold, maximum_threshold + 1, 1)
    samples = np.array(list(map(lambda line: np.array(list(map(float, line.split(',')))), lines[1:])))
    averages = np.array(list(map(lambda s : np.average(s, axis=0), samples)))

fig, ax = plt.subplots()

ax.set_title('10^6 elementos')

ax.set_xlabel('Limiar')
ax.set_ylabel('Tempo de execução (ciclos de clock)')

ax.ticklabel_format(style='plain')
ax.xaxis.set_ticks(np.arange(minimum_threshold, maximum_threshold + 1, 100))

ax.scatter(thresholds, averages,  s=0.1, label='Média dos tempos de execução')

print(np.argmin(averages))

plt.show()