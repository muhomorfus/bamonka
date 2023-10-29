from functools import reduce


class BinNumber:
    def __init__(self, string):
        if string.startswith('-'):
            self.negative = True
            string = string.lstrip('-')
        else:
            self.negative = False

        parts = string.split('.')
        int_str = parts[0]
        self.integer = [
            int(int_str[i]) for i in range(len(int_str) - 1, -1, -1)
        ]

        if len(parts) <= 1:
            self.floating = [0]
        else:
            float_str = parts[1]
            self.floating = [
                int(float_str[i]) for i in range(len(float_str) - 1, -1, -1)
            ]

    def copy(self):
        self_copy = BinNumber('0')
        self_copy.integer = self.integer
        self_copy.floating = self.floating
        self_copy.negative = self.negative

        return self_copy

    def add_zeros_to_integer(self, n):
        self.integer = add_zeros(self.integer, n)

    def add_zeros_to_floating(self, n):
        self.floating = add_zeros(self.floating, n, on_start=True)

    def text(self):
        def new_reducer():
            def func(part):
                return reduce(lambda res, cur: str(cur) + res, part, '')

            return lambda part: func(part)

        reducer = new_reducer()
        result = reducer(self.integer) + '.' + reducer(self.floating)
        result = result.strip('0')

        if result.startswith('.'):
            result = '0' + result

        if result.endswith('.'):
            result = result.rstrip('.')

        if self.negative and not self.is_zero():
            result = '-' + result

        return result

    def is_zero(self):
        for n in self.to_sticky():
            if n == 1:
                return False

        return True

    def to_sticky(self):
        return self.floating + self.integer

    def from_sticky(self, sticky):
        self.floating = sticky[:len(self.floating)]
        self.integer = sticky[len(self.floating):]

    def abs_sum(self, bn):
        normalize_sizes(self, bn)

        self_sticky = self.to_sticky()
        bn_sticky = bn.to_sticky()
        sum_sticks(self_sticky, bn_sticky)

        return self.from_sticky(self_sticky)

    def abs_diff(self, bn):
        normalize_sizes(self, bn)

        if self.abs_less(bn):
            bn.abs_diff(self)
            self.copy_abs(bn)
            self.negative = True
            return

        self_sticky = self.to_sticky()
        bn_sticky = bn.to_sticky()

        for i in range(len(self_sticky)):
            self_sticky[i] -= bn_sticky[i]
            if self_sticky[i] < 0:
                self_sticky[i] += 2
                for j in range(i + 1, len(self_sticky)):
                    if self_sticky[j] == 0:
                        self_sticky[j] = 1
                    else:
                        self_sticky[j] -= 1
                        break

        self.from_sticky(self_sticky)

    def abs_multiple(self, bn):
        normalize_sizes(self, bn)

        self_sticky = self.to_sticky()
        bn_sticky = bn.to_sticky()

        result = []

        for bns in bn_sticky:
            if bns == 1:
                sum_sticks(result, self_sticky)
            self_sticky = [0] + self_sticky

        len_floating_new = len(self.floating) + len(bn.floating)
        self.floating = result[:len_floating_new]
        self.integer = result[len_floating_new:]

    def copy_abs(self, bn):
        self.integer = bn.integer[:]
        self.floating = bn.floating[:]

    def sum(self, bn):
        self_copy = self.copy()
        bn_copy = bn.copy()

        if self_copy.negative:
            if bn_copy.negative:
                self_copy.abs_sum(bn_copy)
                self_copy.negative = True
            else:
                self_copy.negative = not self_copy.abs_less(bn_copy)
                self_copy.abs_diff(bn_copy)
        else:
            if bn_copy.negative:
                self_copy.negative = self_copy.abs_less(bn_copy)
                # print(self_copy.negative)
                # print(self_copy.text(), bn_copy.text())
                self_copy.abs_diff(bn_copy)
            else:
                self_copy.abs_sum(bn_copy)
                self_copy.negative = False

        return self_copy

    def diff(self, bn):
        self_copy = self.copy()
        bn_copy = bn.copy()

        bn_copy.negative = not bn_copy.negative
        return self_copy.sum(bn_copy)

    def multiple(self, bn):
        self_copy = self.copy()
        bn_copy = bn.copy()

        self_copy.negative = self_copy.negative != bn_copy.negative

        self_copy.abs_multiple(bn)

        return self_copy

    def abs_less(self, bn):
        self_copy = self.copy()
        bn_copy = bn.copy()

        normalize_sizes(self_copy, bn_copy)

        for i in range(len(self_copy.integer) - 1, -1, -1):
            if self_copy.integer[i] > bn_copy.integer[i]:
                return False
            elif self_copy.integer[i] < bn_copy.integer[i]:
                return True

        for i in range(len(self_copy.floating) - 1, -1, -1):
            if self_copy.floating[i] > bn_copy.floating[i]:
                return False
            elif self_copy.floating[i] < bn_copy.floating[i]:
                return True

        return False

    def less(self, bn):
        if self.negative and not bn.negative:
            return True

        if not self.negative and bn.negative:
            return False

        if self.negative and bn.negative:
            return not self.abs_less(bn)

        return self.abs_less(bn)


def add_zeros(arr, n, on_start=False):
    num_of_zeros = n - len(arr)
    zeros = [0] * num_of_zeros

    if on_start:
        return zeros + arr
    else:
        return arr + zeros


def normalize_sizes(a, b):
    max_integer_digits = max(len(a.integer), len(b.integer)) + 1
    max_floating_digits = max(len(a.floating), len(b.floating))

    a.add_zeros_to_integer(max_integer_digits)
    b.add_zeros_to_integer(max_integer_digits)

    a.add_zeros_to_floating(max_floating_digits)
    b.add_zeros_to_floating(max_floating_digits)


def sum_sticks(s1, s2):
    max_size = max(len(s1), len(s2))
    s1 += [0] * (max_size - len(s1))
    s2 += [0] * (max_size - len(s2))

    shift = 0

    for i in range(max_size):
        cur = s1[i] + s2[i] + shift
        s1[i] = cur % 2
        shift = cur // 2

    return s1
