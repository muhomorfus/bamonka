def bezie(x1, y1, x2, y2, x3, y3):
    t = 0
    while t <= 1:
        print((1 - t) ** 2 * x1 + 2 * (1 - t) * t * x2 + t ** 2 * x3, ', ',
              (1 - t) ** 2 * y1 + 2 * (1 - t) * t * y2 + t ** 2 * y3, ',',
              sep='')
        t += 0.05
