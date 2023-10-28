import matplotlib.pyplot as plt
x1 = []
y1 = []
x2 = []
y2 = []

with open('mysort.txt', 'r') as f1:
    for line in f1:
        a,b = [int(x) for x in line.split()]
        x1.append(a)
        y1.append(b)

with open('qsort.txt', 'r') as f1:
    for line in f1:
        a,b = [int(x) for x in line.split()]
        x2.append(a)
        y2.append(b)

plt.plot(x1, y1, '-g', label = 'mysort', lw = 3)
plt.plot(x2, y2, '-r', label = 'qsort', lw = 3)

plt.title('Сравнение быстродействия функций mysort и qsort', fontsize = 14)
plt.xlabel('Размер массива', fontsize = 12)
plt.ylabel('Время сортировки, мкс', fontsize = 12)
plt.legend(loc = 'upper right')
plt.grid()
plt.show()
