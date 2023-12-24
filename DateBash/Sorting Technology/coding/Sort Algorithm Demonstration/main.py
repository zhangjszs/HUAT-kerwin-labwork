import sorting as s
import visualizer as vs
import numpy as np
import random

def get_random_array(length):
    n = list(range(length))
    random.shuffle(n)
    return n

array_size = 50

algorithm_dict = {
    1: s.bubble_sort,
    2: s.heap_sort,
    3: s.selection_sort,
    4: s.insertion_sort,
    5: s.quick_sort,
    6: s.merge_sort
}

while True:
    print('Choose an algorithm:')
    for key, value in algorithm_dict.items():
        print(f'{key}. {value.__name__}')

    algorithm_number = int(input('Enter the number of the algorithm (0 to exit): '))

    if algorithm_number == 0:
        break

    algorithm = algorithm_dict.get(algorithm_number)

    if algorithm is None:
        print('Invalid algorithm number.')
        continue

    arr = s.Array(get_random_array(array_size))
    algorithm(arr)
    plotter = vs.Plotter(arr.pile, title=algorithm.__name__, repeat=True, interval=1)
    plotter.plot()