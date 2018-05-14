import os
import subprocess
import shutil
from common import cpp_compile, run_source, official_source_name, problem_name, score
#generat teste in fct de sursa corecta


if __name__ == '__main__':
    cpp_compile(official_source_name())
    try:
        shutil.rmtree('./tmp', ignore_errors=False)
    except:
        pass
    os.mkdir('./tmp')

    for test in range(20):
        print("Test %d:" % test)

        subprocess.check_call(["cp", "./tests/%d-%s.in" % (test, problem_name()), "./tmp/"+problem_name()+".in"])
        print("  Running "+official_source_name()+" for output ... ", end="")
        if run_source(official_source_name()) == False:
            print("Error")
            exit(1)
        subprocess.check_call(["cp", "./tmp/"+official_source_name()+".out", "./tmp/"+problem_name()+".ok"])
        os.chdir('./tmp')
        os.chdir('../')

        testno = "./tests/%d-" % test
        subprocess.check_call(["cp", "./tmp/"+problem_name()+".in", testno+problem_name()+".in"])
        subprocess.check_call(["cp", "./tmp/"+official_source_name()+".out", testno+problem_name()+".ok"])
        print("Done")
