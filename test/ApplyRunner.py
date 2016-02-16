from __future__ import print_function
import sys
import os
import subprocess
import argparse
from multiprocessing import Pool


def toErr(*objs):
    print("[status] ", *objs, file=sys.stderr)

cmdParser = argparse.ArgumentParser(description='Runs the test instrumentor executable on all input files.')
cmdParser.add_argument('src', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('build', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('compilerIndication', type=str, help="Which compiler is running? [rose/clang]")

# This is the list of test programs which should be applied.
testPrograms = ["ConstructHierarchySelectionTest", "IdentifierSelectorTest", "ConstructElevatorTest", "BooleanCompoundSelectorTest", "CallpathSelectorTest", "UniqueCallpathTransformerTest", "DefaultInstrumentationAdapterTest"]

# The list of targets is read from a targets.lst which resides in a test/input/BinaryName directory
def readTargetFileSpecification(tInstrumentor, directory):
    targets = []
    inFile = open(directory + '/' + tInstrumentor + "/targets.lst")
    for line in inFile:
        targets.append(line.strip())

    return targets

def runTestBinary(arguments, binary, inputDirectory):
    targets = readTargetFileSpecification(binary, inputDirectory);

    failedRuns = []
    for k in targets:
        srcFile = k + ".cpp"
        specFile = k + ".in"

        roseExtraArg = " "
        if arguments.compilerIndication == 'rose':
            roseExtraArg += " --edg:no_warnings "
            roseExtraArg += ' --instro-library-path=' + arguments.build + '/test/.libs'
            roseExtraArg += ' --instro-library-name=InstRO_rtsupport'
            roseExtraArg += ' --instro-include=' + arguments.src + '/support'

        os.environ["INSTRO_TEST_INPUT_FILENAME"] = inputDirectory + '/' + binary + '/' + specFile
        invocationString = "./" + binary + " "  + roseExtraArg + ' ' + inputDirectory + '/' + srcFile

            # we need to add the "--" to the invocation as we do not have JSON compilation databases
        if arguments.compilerIndication == 'clang':
            invocationString += ' --'
            
        toErr("\n[Running]\n" + binary + " " + srcFile)
        if False:
            toErr("Detailed invocation info: " + invocationString)

        errCode = 0
        out = ''
        try:
            out = subprocess.check_output(invocationString, shell=True)
        except subprocess.CalledProcessError as e:
            errCode = e.returncode
            toErr("[Error] Dumping STDOUT \n" + out)

#        errCode = subprocess.call(invocationString, shell=True)

        if errCode == 0 and binary == "DefaultInstrumentationAdapterTest":
            try:
                out = subprocess.call("./a.out", shell=True)
            except subprocess.CalledProcessError as e:
                errCode = e.returncode
                toErr("[Error] Problem when running binary.")

        toErr("[Done]")
        if errCode != 0:
            failedRuns.append((binary, srcFile))

    return failedRuns

# Runs each TestInstrumentor on the given target list
def runApply(arguments):
    baseDir = os.getcwd()[0:os.getcwd().rfind('/')]
    
    inputDirectory = arguments.src + "/test/input"

    os.environ['LD_LIBRARY_PATH'] = arguments.build+'/test/.libs:'+os.environ['LD_LIBRARY_PATH']
    
    failedRuns = []
    for b in testPrograms:
        failedRuns += runTestBinary(arguments, b, inputDirectory)

    if len(failedRuns) == 0:
        print("\n=================================")
        print("==== Tests finished normally ====")
        print("=================================\n")
        return 0
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
