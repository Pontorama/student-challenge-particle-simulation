import sys
import collections

file1 = sys.argv[1]
file2 = sys.argv[2]

i = 0
with open(file1) as f1:
    with open(file2) as f2:
        while line1 := f1.readline():
            i += 1
            line2 = f2.readline()
            line1 = line1.replace(" ", "")
            line1 = line1.replace("\n", "")
            line2 = line2.replace(" ", "")
            line2 = line2.replace("\n", "")
            pairs1 = line1.split(",")
            pairs2 = line2.split(",")
            if not collections.Counter(pairs1) == collections.Counter(pairs2):
                print(i)
                print(pairs1)
                print(pairs2)
                print("----------")
                input()
