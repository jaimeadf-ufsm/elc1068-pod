import matplotlib.pyplot as plt
import numpy as np

minimum_size = 0
maximum_size = 10000000

increment = 100000

value_filename = 'data/mergix_random_benchmarks.txt'
reference_filename = 'data/mergesort_random_benchmarks.txt'

sizes = np.arange(minimum_size, maximum_size + 1, increment)

def compute_benchmark_averages(benchmark_filename):
    with open(benchmark_filename, 'r') as f:
        lines = f.readlines()
        samples = np.array(list(map(lambda line: np.array(list(map(float, line.split(',')))), lines[1:])))
        averages = np.array(list(map(lambda s : np.average(s, axis=0), samples)))

        return averages
    
value_averages = compute_benchmark_averages(value_filename)
reference_averages = compute_benchmark_averages(reference_filename)

improvements = (reference_averages - value_averages) / reference_averages * 100

fig, ax = plt.subplots(1, 1, figsize=(8, 5))

ax.set_title('Mergix (97) vs. Mergesort')
ax.set_xlabel('Tamanho (10^5 elementos)')
ax.set_ylabel('Porcentagem de melhoria (%)')

ax.ticklabel_format(style='plain')
ax.xaxis.set_ticks(np.arange(minimum_size, maximum_size + 1, increment * 10) / increment)

ax.scatter((sizes / increment)[1:], improvements[1:], s=0.5)

ax.set_ylim(bottom=0)

plt.show()