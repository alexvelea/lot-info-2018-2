import sys, os, random

def main():
    official = str(sys.argv[1])
    tests = [
        [3, 3, 3, 0, 0],
        [8, 4, 4, 4, 4],
        ###############
        [100, 100, 100, 0], 
        [99, 45, 80, 13],
        ############### 
        [1000, 1000, 1000, 0],
        [1000, 500, 500, 253],
        [997, 267, 534, 2],
        ###############
        [5000, 5000, 5000, 0],
        [5000, 2500, 2500, 2500],
        [4992, 3, 3750, 1000],
        ###############
        [46919, 17500, 30000, 10000],
        [44620, 124, 20000, 2500],
        [37687, 1000, 35000, 50],
        [50000, 3, 50000, 0],
        [50000, 25000, 25000, 12500],
        [50000, 25000, 25000, 2],
        [50000, 50000, 50000, 0],
        ###############
        [41234, 41234, 41234, 0],
        [45001, 45001, 45001, 0],
        [50000, 50000, 50000, 0]
    ]

    for (idx, test) in enumerate(tests):
        input_file = str(idx) + "-sortall.in"
        output_file = str(idx) + "-sortall.ok"
        if len(test) != 5:
            N, X0, N0, X1 = test
            S = random.randint(0, 2**30)
        else:
            N, X0, N0, X1, S = test
        print("Generating %d %d %d %d %d.." % (N, X0, N0, X1, S))
        os.system("./_test_gen %d %d %d %d %d > sortall.in" % (N, X0, N0, X1, S))
        os.system("./" + official)
        os.system("cp sortall.in " + input_file)
        os.system("cp sortall.out " + output_file)


if __name__ == "__main__":
    main()
