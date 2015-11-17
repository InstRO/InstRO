import os
import subprocess
import argparse

cmdParser = argparse.ArgumentParser(description='Runs the test instrumentor executable on all input files.')
cmdParser.add_argument('src', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('build', type=str, help="/path/to/instro/repo")


def runApply(arguments):
    # For each unique name in test/input
    # export environment variable to point to $$.in file
    # apply TestInstrumentor to $$.cpp file
    # Test return code
    baseDir = os.getcwd()[0:os.getcwd().rfind('/')]
    inputDirectory = arguments.src + "/test/input"
    fileList = os.listdir(inputDirectory)
    workList = dict()
    for f in fileList:
        if f[0] == '.':
            continue
        name = f[0 : f.rfind('.')]
        if workList.has_key(name):
            workList[name].append(f)
        else:
            workList[name] = [f]
 
    for k in workList:
        element = workList[k]
        srcFile = ""
        specFile = ""
        for e in element:
            if e.rfind(".in") != -1:
                specFile = e
            else:
                srcFile = e

        os.environ["INSTRO_TEST_INPUT_FILENAME"] = inputDirectory + '/' + specFile
        invocationString = "./SelectionTest " + inputDirectory + '/' + srcFile
        subprocess.call(invocationString, shell=True)

    print("\n==== Tests finished normally ====\n")

# we use two command line parameters to get build and source directory
args = cmdParser.parse_args()
runApply(args)
