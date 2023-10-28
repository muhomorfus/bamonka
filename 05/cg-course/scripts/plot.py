import matplotlib
import matplotlib.pyplot as plt

file = open("result.txt", "r", encoding = "utf-8") 

x = []
y = []

for line in file.read().split("\n"):
    splitted = line.split()
    if len(splitted) == 2:
        x.append(int(splitted[0]))
        y.append(int(splitted[1]))

plt.xlabel("Количество потоков")
plt.ylabel("Время, мс")
plt.grid(True)

plt.scatter(x, y, s=10)

plt.plot(x, y)
plt.savefig("result.svg")

file.close()

