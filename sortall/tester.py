# python3 tester.py .?-program1 ./_program2 N seed
import sys, random, os, filecmp

def main():
    if len(sys.argv) < 4:
        exit()

    c1, c2 = str(sys.argv[1]), str(sys.argv[2])
    n = int(sys.argv[3])
    s = random.randint(1, 2**30) if len(sys.argv) < 5 else sys.argv[4]

    print("N = %d, S = %d" % (n, s))

    num_tests = 0
    while num_tests <= 20:
        print("Running %d.." % (num_tests + 1))
        os.system("./_test_gen %d %d > sortall.in" % (n, s))
        os.system(c1 + " < sortall.in > sortall.ok")
        os.system(c2 + " < sortall.in > sortall.out")

        if not filecmp.cmp("sortall.out", "sortall.ok"):
            print("Mismatch!")
            exit()
        num_tests += 1


if __name__ == "__main__":
    main()
