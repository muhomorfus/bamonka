import matplotlib.pyplot as plt

try:
    plt.figure(1, figsize = (12, 7))

    times = []
    labels = []

    with open("../lab_03/time.txt", "r", encoding = "utf-8") as f:
        i = 0
        for line in f:
            line = line.strip()
            if line == "":
                continue

            if i % 2 == 0:
                labels.append(line)
            else:
                times.append(float(line) * 1000000)

            i += 1

    y = range(len(times))

    plt.bar(y, times)
    plt.xticks(y, labels)

    plt.ylabel("Время в мкс.")
    plt.show()

except Exception as e:
    print(e)
    input()
