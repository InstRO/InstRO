import os
import subprocess
import argparse

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

# Runs each TestInstrumentor on the given target list
def runApply(arguments):
    baseDir = os.getcwd()[0:os.getcwd().rfind('/')]
    
    inputDirectory = arguments.src + "/test/input"

    os.environ['LD_LIBRARY_PATH'] = arguments.build+'/test/.libs:'+os.environ['LD_LIBRARY_PATH']
    
    failedRuns = []
    for b in testPrograms:
        targets = readTargetFileSpecification(b, inputDirectory);
        for k in targets:
            srcFile = k + ".cpp"
            specFile = k + ".in"

            roseExtraArg = " "
            if arguments.compilerIndication == 'rose':
                roseExtraArg += ' --instro-library-path=' + arguments.build + '/test/.libs'
                roseExtraArg += ' --instro-library-name=InstRO_rtsupport'
                roseExtraArg += ' --instro-include=' + arguments.src + '/support'

            os.environ["INSTRO_TEST_INPUT_FILENAME"] = inputDirectory + '/' + b + '/' + specFile
            invocationString = "./" + b + " " + roseExtraArg + ' ' + inputDirectory + '/' + srcFile

            # we need to add the "--" to the invocation as we do not have JSON compilation databases
            if arguments.compilerIndication == 'clang':
                invocationString += ' --'
            
            print("\n[Running]\n" + b + " " + srcFile)
            if False:
                print("Detailed invocation info: " + invocationString)

            errCode = subprocess.call(invocationString, shell=True)

            if errCode == 0 and b == "DefaultInstrumentationAdapterTest":
                errCode += subprocess.call("./a.out", shell=True)

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
