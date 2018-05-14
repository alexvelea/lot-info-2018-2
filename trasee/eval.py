#!/usr/bin/env python3
import os
import subprocess
import shutil
import random
import argparse
from common import get_sources, run_source, score, compile_all, cpp_compile, problem_name

def gcd(a, b):
    if b == 0:
        return a
    return gcd(b, a % b)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Get scores')
    parser.add_argument('people', type=str, nargs='*')
    parser.add_argument('--all', dest='all', action='store_true')
    args = parser.parse_args()
    if not args.all and args.people == []:
        parser.print_help()
        exit(0)
    Sources = get_sources()
    if not args.all:
        Sources = list(set(Sources) & set(args.people))
    for source in Sources:
        cpp_compile(source)

    TESTS = int(input("TESTS = "))
    MAXN = int(input("MAXN = "))
    MAXM = int(input("MAXM = "))

    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    for test in range(1, TESTS + 1):
        print("Case #%d" % test, end="")

        N = random.randint(1, MAXN)
        M = random.randint(1, MAXM)

        while True:
            if N == 1 or M == 1:
                break

            if gcd(N, M) != 1:
                break;
            N = random.randint(1, MAXN)
            M = random.randint(1, MAXM)

        print("%d %d" % (N, M), file=open("./tmp/trasee.in", "w"))

        output = subprocess.check_output(["./eel"], stdin=open("./tmp/trasee.in")).decode('utf-8')
        many = int(output.split()[0].strip())
        K = random.randint(1, many)
        print("(N = %d, M = %d, K = %d): " % (N, M, K), end="")
        print("%d %d %d" % (N, M, K), file=open("./tmp/trasee.in", "w"))

        for source in Sources:
            if run_source(source) == False:
                print("Runtime error on %s" % source)
                exit(1)

        subprocess.check_call(["cp", "./tmp/%s.out" % Sources[0], "./tmp/"+problem_name()+".ok"])

        for source in Sources:
            if score(source) != 10:
                print("Wrong answer: %s" % source)
                exit(1)
        print("OK")
