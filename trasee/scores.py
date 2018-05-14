#!/usr/bin/env python3
import os
import subprocess
import shutil
import tabulate
import argparse
from common import cpp_compile, run_source, get_sources, score, problem_name
from sys import stdout


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Get scores')
    parser.add_argument('people', type=str, nargs='*')
    parser.add_argument('--all', dest='all', action='store_true')
    args = parser.parse_args()
    if not args.all and args.people == []:
        parser.print_help()
        exit(0)
    SOURCES = get_sources()
    if not args.all:
        SOURCES = list(set(SOURCES) & set(args.people))

    for source in SOURCES:
        cpp_compile(source)

    # initialize ./tmp folder
    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    TESTS = [test.name[:-3] for test in os.scandir('./tests') if test.name[-3:] == '.in']
    SCORES = {source: 0 for source in SOURCES}
    RESULTS = {source: [] for source in SOURCES}
    for test in range(len(TESTS)):
        print(".", end="")
        stdout.flush()
        subprocess.check_call(["cp", "./tests/" + str(test) + "-"+problem_name()+".in", "./tmp/"+problem_name()+".in"])
        subprocess.check_call(["cp", "./tests/" + str(test) + "-"+problem_name()+".ok", "./tmp/"+problem_name()+".ok"])

        for source in SOURCES:
            if run_source(source) == False:
                RESULTS[source] += ['X']
            else:
                subprocess.check_call(["cp", "./tmp/" + source + ".out", "./tmp/"+problem_name()+".out"])
                s = score(source)
                RESULTS[source] += [str(s)]
                SCORES[source] += s
    print()
    SCORES = [[name, score] for name, score in SCORES.items()]
    RESULTS = [[name] + results for name, results in RESULTS.items()]

    print(tabulate.tabulate(SCORES, ["Nume", "Scor"], tablefmt="grid"))
    print(tabulate.tabulate(RESULTS, ["Nume"] + list(range(len(TESTS))), tablefmt="grid"))
