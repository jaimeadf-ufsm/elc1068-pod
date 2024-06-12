import matplotlib.pyplot as plt
import numpy as np

minimum_size = 0
maximum_size = 10000000

increment = 100000

quicksort_filename = 'data/quicksort_random_benchmarks.txt'
mergesort_filename = 'data/mergesort_random_benchmarks.txt'
quicksert_filename = 'data/quicksert_random_benchmarks.txt'
mergix_filename = 'data/mergix_random_benchmarks.txt'

sizes = np.arange(minimum_size, maximum_size + 1, increment)

def compute_benchmark_averages(benchmark_filename):
    with open(benchmark_filename, 'r') as f:
        lines = f.readlines()
        samples = np.array(list(map(lambda line: np.array(list(map(float, line.split(',')))), lines[1:])))
        averages = np.array(list(map(lambda s : np.average(s, axis=0), samples)))

        return averages

fig, ax = plt.subplots(1, 1, figsize=(8, 5))

ax.set_title('Vetor aleatório')
ax.set_xlabel('Tamanho (10^5 elementos)')
ax.set_ylabel('Tempo de execução (ciclos de clock)')

ax.ticklabel_format(style='plain')
ax.xaxis.set_ticks(np.arange(minimum_size, maximum_size + 1, increment * 10) / increment)

ax.scatter(sizes / increment, compute_benchmark_averages(quicksort_filename), s=0.5, label='Quicksort')
ax.scatter(sizes / increment, compute_benchmark_averages(mergesort_filename), s=0.5, label='Mergesort')
ax.scatter(sizes / increment, compute_benchmark_averages(quicksert_filename), s=0.5, label='Quicksert (23)')
ax.scatter(sizes / increment, compute_benchmark_averages(mergix_filename), s=0.5, label='Mergix (97)')

plt.legend(loc='best')
plt.show()