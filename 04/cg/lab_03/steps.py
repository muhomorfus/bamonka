import matplotlib
import matplotlib.pyplot as plt

try:
    def get_method_info(method_info):
        strings = [x.strip() for x in method_info.split("\n")]
        name = strings[0]
        strings = strings[1:]

        x = []
        y = []

        for string in strings:
            if string == "":
                continue

            some_x, some_y = [float(x) for x in string.split()]
            x.append(some_x)
            y.append(some_y)

        return name, x, y


    def draw_method_info(fig_num, method_info):
        name, x, y = get_method_info(method_info)

        plt.figure(fig_num)
        plt.title(f"{name}")
        plt.xlabel("Угол наклона прямой")
        plt.ylabel("Число ступеней")
        plt.grid(True)
        plt.plot(x, y)


    with open("../lab_03/steps.txt", "r", encoding = "utf-8") as f:
        all_file = f.read()
        methods_info = all_file.split("---\n")

        i = 1
        for info in methods_info:
            draw_method_info(i, info)
            i += 1

    plt.show()

except Exception as e:
    print(e)
    input()

