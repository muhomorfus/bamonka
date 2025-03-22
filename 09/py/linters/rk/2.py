import struct
import io


def f_x(x):
    return 1


def integral(func, start, end, steps):
    if end < start:
        start, end = end, start

    step = (end - start) / steps

    result = 0

    x = start
    while x < end + step / 2:
        result += (func(x) + func(x + step)) / 2 * step
        x += step

    return result


def integral_with_eps(func, start, end, eps):
    first = integral(func, start, end, 2)
    second = integral(func, start, end, 4)

    delta = abs(second - first)
    result = second
    steps = 4

    while abs(delta) >= abs(eps):
        steps *= 2
        current = integral(func, start, end, steps)
        delta = abs(current - result)
        result = current

    return result, steps


fmt = "hf"
offset = struct.calcsize(fmt)

with open("in.txt", "wb") as f:
    f.write(struct.pack(fmt, 1, 0.01))
    f.write(struct.pack(fmt, 2, 0.01))
    f.write(struct.pack(fmt, 3, 0.01))
    f.write(struct.pack(fmt, 4, 0.01))
    f.write(struct.pack(fmt, 6, 0.01))
    f.write(struct.pack(fmt, 5, 0.01))

with open("in.txt", "rb+") as f:
    f.seek(0, io.SEEK_END)
    size = f.tell()
    current_offset = 0
    f.seek(current_offset)

    max_integral_offset = None
    max_integral = None

    while f.tell() < size:
        data = f.read(offset)
        a, b = struct.unpack(fmt, data)
        print(a, b)

        res, steps = integral_with_eps(f_x, 0, a, b)
        print("Res: ", res, steps)

        f.seek(current_offset)
        f.write(struct.pack(fmt, steps, res))

        if max_integral is None or max_integral < res:
            max_integral = res
            max_integral_offset = current_offset

        current_offset += offset
        f.seek(current_offset)

    current_offset = max_integral_offset
    f.seek(current_offset)
    while f.tell() < size - offset:
        f.seek(current_offset + offset)
        a, b = struct.unpack(fmt, f.read(offset))
        f.seek(current_offset)
        f.write(struct.pack(fmt, a, b))
        current_offset += offset

    f.truncate(size - offset)

print("\n\n\nTest")

with open("in.txt", "rb") as f:
    f.seek(0, io.SEEK_END)
    size = f.tell()
    current_offset = 0
    f.seek(current_offset)

    while f.tell() < size:
        data = f.read(offset)
        a, b = struct.unpack(fmt, data)
        print(a, b)

        current_offset += offset
        f.seek(current_offset)