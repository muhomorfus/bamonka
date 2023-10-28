import random
import time
import math

streets = [
    'Suzdalskaya',
    'Tverskaya',
    'Druzhby',
    'Injenerov',
    'Mira',
    'Saltykova',
    'Zhiguley',
    'Stroiteley',
    'Lesnaya',
    'Flotskaya',
    'Baumanskaya',
    'Lomonosova',
    'Romanova',
    'Belaya',
]


def gen_address():
    st = streets[random.randint(0, len(streets) - 1)]
    return '{}, {}'.format(st, random.randint(1, 40))


def gen_area():
    return random.randint(150, 2800) / 10


def gen_number_of_rooms(a):
    s = random.randint(30, 100)
    n = math.floor(a / s)
    if n < 1:
        n = 1
    if n >= 10:
        n = 9
    return n


def gen_cost_per_square_meter():
    return random.randint(70000, 300000)


def gen_class():
    return random.randint(0, 1)


def gen_have_finishing():
    return random.randint(0, 1)


def gen_year_of_construction():
    return random.randint(1940, 2020)


def gen_number_of_owners():
    return random.randint(1, 4)


def gen_number_of_tenants():
    return random.randint(1, 6)


def gen_were_animals():
    return random.randint(0, 1)


def gen_flat():
    address = str(gen_address())
    area_n = gen_area()
    area = str(area_n)
    rooms = str(gen_number_of_rooms(area_n))
    cost = str(gen_cost_per_square_meter())
    kind = str(gen_class())
    arr = [address, area, rooms, cost, kind]
    finishing = str(gen_have_finishing())
    year = str(gen_year_of_construction())
    owners = str(gen_number_of_owners())
    tenants = str(gen_number_of_tenants())
    animals = str(gen_were_animals())

    if kind == '0':
        arr += [finishing]
    else:
        arr += [year, owners, tenants, animals]

    return '; '.join(arr)


n = int(input('Enter num > '))
f = open('{}.txt'.format(n), 'w')
for i in range(n):
    f.write(gen_flat() + '\n')
f.close()




