s = '''
brek: (integer) 11464
cons: (integer) 13736
fish: (integer) 12200
aufd: (integer) 13864
brow: (integer) 12040
crui: (integer) 12280
erns: (integer) 12792
batz: (integer) 12424
fees: (integer) 12056
auer: (integer) 24680
'''

print(sum([int(l.split('(integer) ')[1]) for l in s.strip().split('\n')]))