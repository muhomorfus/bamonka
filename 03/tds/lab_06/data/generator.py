import random
import sys

words_file = open("/usr/share/dict/words", "r")
words = words_file.read().splitlines()
words_file.close()

n = int(input())
filename = "{}.txt".format(n)
if len(sys.argv) > 1:
    filename = sys.argv[1]

file_out = open(filename, "w")
for i in range(n):
    k = random.randint(0, len(words))
    word = words[k]
    words[k], words[-1] = words[-1], words[k]
    words = words[:-1]
    file_out.write("{}\n".format(word.lower()))
file_out.close()
