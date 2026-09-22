with open("results.txt", "r") as f:
    times = f.readlines()
    times = [t.strip() for t in times]
    nodes = [1,2,4,8,16]
    print("\tRuntime\tNode*s\tspeedup\tparallelization\tel cost\tmoney")
    for t, n in zip(times, nodes):
        t = float(t)
        n = float(n)
        print(f"{t}s\t{t*n}nodes\t{t/float(times[0])*100}x\t{(t*n)/float(times[0])}\t{t/3600*n*600}Wh\t{t*n/3600*4}euro")
