import os
import subprocess
import argparse

cmdParser = argparse.ArgumentParser(description='Runs the test instrumentor executable on all input files.')
cmdParser.add_argument('src', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('build', type=str, help="/path/to/instro/repo")

# This is the list of test programs which should be applied.
testPrograms = ["ConstructHierarchySelectionTest", "IdentifierSelectorTest", "ConstructElevatorTest", "BooleanCompoundSelectorTest", "CallpathSelectorTest"]

# The list of targets is read from a targets.lst which resides in a test/input/BinaryName directory
def readTargetFileSpecification(tInstrumentor, directory):
    targets = []
    inFile = open(directory + '/' + tInstrumentor + "/targets.lst")
    for line in inFile:
        targets.append(line.strip())

    return targets

# Runs each TestInstrumentor on the given target list
def runApply(arguments):
    baseDir = os.getcwd()[0:os.getcwd().rfind('/')]
    
    inputDirectory = arguments.src + "/test/input"
    
    failedRuns = []
    for b in testPrograms:
        targets = readTargetFileSpecification(b, inputDirectory);
        for k in targets:
            srcFile = k + ".cpp"
            specFile = k + ".in"

            os.environ["INSTRO_TEST_INPUT_FILENAME"] = inputDirectory + '/' + b + '/' + specFile
            invocationString = "./" + b + " " + inputDirectory + '/' + srcFile
            
            print("\n[Running]\n" + b + " " + srcFile)

            errCode = subprocess.call(invocationString, shell=True)
            print("[Done]")
            if errCode != 0:
                failedRuns.append((b, srcFile))

    if len(failedRuns) == 0:
        print("\n=================================")
        print("==== Tests finished normally ====")
        print("=================================\n")
    else:
        print("\n|=== Tests failed ===")
        print("|= Number of failing tests due to error: " + str(len(failedRuns)))
        print("|--- Failing testcases:")
        print("|---")
        for fr in failedRuns:
            print("|--- " + fr[0] + "\t" + fr[1])
        print("|=== End report ===\n")
        return -1

# we use two command line parameters to get build and source directory
args = cmdParser.parse_args()
runApply(args)
