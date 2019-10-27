
import random
import sys
M = int(str(sys.argv[1]))
#M = 4 #número de ilhas

N = 1000  #valor disponível

print(N, " ", M)

for i in range(0, M):
    print(random.randrange(50,700), " ", random.randrange(100))