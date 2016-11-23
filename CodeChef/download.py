
N = int(input())
times = []
while N > 0:
    s, e = input().split()
    times.append(( int(s), int(e) ))
    N -= 1
times.sort(key = lambda x: x[0])

Q = int(input())
while Q > 0:

    l = list(map(int, input().split()))[1:]
    copy = set()

    # for ins in l:
        # for beg, end in times:
            # if ins < beg:
                # break
            # if ins >= beg and ins <= end:
                # copy.add((beg,end))
    for ins in l:
        copy =  copy.union(filter(lambda x: ins >= x[0] and ins <= x[1], times))

    print(len(copy))
    Q -= 1
        
