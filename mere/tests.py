import os
import subprocess
import shutil
from common import cpp_compile, run_source, official_source_name, problem_name
#generat tests in fct de sursa corecta


if __name__ == '__main__':
    cpp_compile(official_source_name())
    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    TESTS = [line.strip() for line in open("./tests/teste.txt") if len(line.strip()) > 0 and line.strip()[0] != '#']
    for test in range(len(TESTS)):
        print("Test %d:" % test)
        ARGS = TESTS[test].split()

        print("  Generating ...................... ", end="")
        subprocess.check_call(ARGS, stdout=open("./tmp/"+problem_name()+".in", "w"))
        print("Done")
        print("  Running "+official_source_name()+" for output ... ", end="")
        if run_source(official_source_name()) == False:
            print("Error")
            exit(1)

        testno = "./tests/%d-" % test
        subprocess.check_call(["cp", "./tmp/"+problem_name()+".in", testno+problem_name()+".in"])
        subprocess.check_call(["cp", "./tmp/"+official_source_name()+".out", testno+problem_name()+".ok"])
        print("Done")
