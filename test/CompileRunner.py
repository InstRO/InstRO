import subprocess
import os
import tempfile
import shutil
import argparse
import time

# The InstRO test runner should do:
# - configure with the different flavors (depending on arguments)
# - build the different flavors
# - for each flavor:
#   - invoke the test translators/executables on the respective inputs
# - if any: run unit-tests

cmdParser = argparse.ArgumentParser(description='The script compiles all flavors possible with the given arguments.')
cmdParser.add_argument('--rose', type=str, dest='rosepath', help="/path/to/rose/installation")
cmdParser.add_argument('--llvm-src', type=str, dest='llvmsrc', help="/path/to/llvm/source")
cmdParser.add_argument('--llvm-install', type=str, dest='llvminstall', help="/path/to/llvm/installation")
cmdParser.add_argument('--rapidjson', type=str, dest='rapidjson', help="/path/to/rapidjson/repository")
cmdParser.add_argument('--boost', type=str, dest='boost', help="/path/to/boost/installation")
cmdParser.add_argument('--jobs', type=int, dest='numJobs', help='Option is forwarded to -j option of build system')

runnerLog = []

def createAndCDToTempDir():
    tDir = tempfile.mkdtemp()
    os.chdir(tDir)
    return tDir


def buildWithConfigureLine(configLine, baseDir, numJobs):
    fullQualInvocLine = baseDir + "/" + configLine
    FNULL = open(os.devnull, 'w')
    
    try:
        print('Configuring ... ')
        out = subprocess.check_output(fullQualInvocLine, shell=True, stderr=FNULL)
        runnerLog.append('Configuring with ' + fullQualInvocLine)
    except subprocess.CalledProcessError:
        runnerLog.append('Configure failed: ' + fullQualInvocLine)
    
    try:
        print('Building ... ')
        out = subprocess.check_output("make -j" + str(numJobs), shell=True, stderr=FNULL)
    except subprocess.CalledProcessError:
        runnerLog.append('Building failed with configure ' + fullQualInvocLine)

    try:
        print('Running tests ... ')
        out = subprocess.check_output("make check -j" + str(numJobs), shell=True, stderr=FNULL)
        runnerLog.append(out)
    except subprocess.CalledProcessError as e:
			runnerLog.append('Tests failed: \n' + e.output)

    FNULL.close()

def buildWithRose(arguments, baseDir):
    numBuilds = 1
    if arguments.rapidjson != None:
        numBuilds += 1
    
    numJobs = 24
    if arguments.numJobs != None:
        numJobs = arguments.numJobs

    for i in xrange(0, numBuilds):
        # we need to clean the build directory
        subprocess.call("rm -rf *", shell=True)
        configureLine = "configure "
        configureLine += "--with-rose=" + arguments.rosepath + " "
        configureLine += "--with-boost=" + arguments.boost + " "
        configureLine += "--enable-examples "

        if arguments.rapidjson != None and i == 1:
            configureLine += "--with-rapidjson=" + arguments.rapidjson + " "

        buildWithConfigureLine(configureLine, baseDir, numJobs)

    subprocess.call("rm -rf *", shell=True)


def buildWithClang(arguments, baseDir):
    numBuilds = 1
    if arguments.rapidjson != None:
        numBuilds += 1

    numJobs = 24
    if arguments.numJobs != None:
        numJobs = arguments.numJobs

    for i in xrange(0, numBuilds):
        # we need to clean the build directory
        subprocess.call("rm -rf *", shell=True)
        configureLine = "configure "
        configureLine += "--with-llvm-src=" + arguments.llvmsrc + " "
        configureLine += "--with-llvm-install=" + arguments.llvminstall + " "
        configureLine += "--with-boost=" + arguments.boost + " "
        configureLine += "--enable-examples "

        if arguments.rapidjson != None and i == 1:
            configureLine += "--with-rapidjson=" + arguments.rapidjson + " "

        buildWithConfigureLine(configureLine, baseDir, numJobs)
        
    subprocess.call("rm -rf *", shell=True)

def checkArguments(arguments):
    if arguments.boost != None and os.path.isdir(arguments.boost) == False:
        raise Exception("Provided Boost path isn't an accesible directory.")

    if arguments.rosepath != None and os.path.isdir(arguments.rosepath) == False:
        raise Exception("Provided ROSE path isn't an accessible directory.")

    if arguments.llvmsrc != None and os.path.isdir(arguments.llvmsrc) == False:
        raise Exception("Provided LLVM source directory isn't accessible.")

    if arguments.llvminstall != None and os.path.isdir(arguments.llvminstall) == False:
        raise Exception("Provided LLVM install directory isn't accessible.")

    if arguments.rapidjson != None and os.path.isdir(arguments.rapidjson) == False:
        raise Exception("Provided rapidjson directory is not a directory.")

def generateLogFileAndPrintToScreen():
    outFile = 'test/TestSuite-compiletest-' + str(int(time.time()))
    of = open(outFile, 'w')

    sumHeader = "\n#####===================#####\n==== Global Test Summary ====\nThis has also been written to file " + outFile
    of.write(sumHeader + '\n')
    print(sumHeader)
    for s in runnerLog:
        strArr = s.splitlines()
        for ss in strArr:
            if ss.find('make') != -1 or ss.find('Making') != -1 or ss.find('python ') != -1:
                continue
            of.write(ss + '\n')
            print(ss)
        if ss.find('Configuring ') == -1:
            separator = '#########################################\n'
            of.write(separator + '\n')
            print(separator)

    of.close()


# We get the parsed commandline arguments and build the flavors accordingly
def configureAndBuildFlavor(arguments):
    baseDir = os.getcwd()[0:os.getcwd().rfind('/')]
    tempDirectory = createAndCDToTempDir()
    checkArguments(arguments)

    try:
        # We want to check whether we have to build using ROSE, Clang, and RapidJson
        if arguments.boost == None:
            raise Exception("no boost path specified")

        if arguments.rosepath != None:
            buildWithRose(arguments, baseDir)

        if arguments.llvmsrc != None and arguments.llvminstall != None:
            buildWithClang(arguments, baseDir)

    finally:
        os.chdir(baseDir)
        shutil.rmtree(tempDirectory)


    generateLogFileAndPrintToScreen()

args = cmdParser.parse_args()
configureAndBuildFlavor(args)
