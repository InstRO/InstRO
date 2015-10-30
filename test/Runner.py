import subprocess
import os
import tempfile
import argparse

# The InstRO test runner should do:
# - configure with the different flavors (depending on arguments)
# - build the different flavors
# - for each flavor:
#   - invoke the test translators/executables on the respective inputs
# - if any: run unit-tests

cmdParser = argparse.ArgumentParser(description='Run InstRO test suite. It compiles add flavors possible with the given arguments.')
cmdParser.add_argument('--rose', type=str, dest='rosepath', help="/path/to/rose/installation")
cmdParser.add_argument('--llvm-src', type=str, dest='llvmsrc', help="/path/to/llvm/source")
cmdParser.add_argument('--llvm-install', type=str, dest='llvminstall', help="/path/to/llvm/installation")
cmdParser.add_argument('--rapidjson', type=str, dest='rapidjson', help="/path/to/rapidjson/repository")
cmdParser.add_argument('--boost', type=str, dest='boost', help="/path/to/boost/installation")

def createAndCDToTempDir():
    tDir = tempfile.mkdtemp()
    os.chdir(tDir)
    return tDir


def buildWithConfigureLine(configLine, baseDir):
    fullQualInvocLine = baseDir + "/" + configLine
    retCode = subprocess.call(fullQualInvocLine, shell=True)
    if retCode != 0:
        raise Exception("The build failed while configure was run: " + fullQualInvocLine)
    
    # If configure succeeded we can now build InstRO
    print(os.getcwd())
    retCode = subprocess.call("make -j8", shell=True)
    if retCode != 0:
        raise Exception("Building failed with err code " + retCode)

def buildWithRose(arguments, baseDir):
    numBuilds = 1
    if arguments.rapidjson != None:
        numBuilds += 1


    for i in xrange(0, numBuilds):
        # we need to clean the build directory
        subprocess.call("rm -rf *", shell=True)
        configureLine = "configure "
        configureLine += "--with-rose=" + arguments.rosepath + " "
        configureLine += "--with-boost=" + arguments.boost + " "

        if arguments.rapidjson != None and i == 1:
            configureLine += "--with-rapidjson=" + arguments.rapidjson + " "

        buildWithConfigureLine(configureLine, baseDir)

    subprocess.call("rm -rf *", shell=True)


def buildWithClang(arguments, baseDir):
    numBuilds = 1
    if arguments.rapidjson != None:
        numBuilds += 1


    for i in xrange(0, numBuilds):
        # we need to clean the build directory
        subprocess.call("rm -rf *", shell=True)
        configureLine = "configure "
        configureLine += "--with-llvm-src=" + arguments.llvmsrc + " "
        configureLine += "--with-llvm-install=" + arguments.llvminstall + " "
        configureLine += "--with-boost=" + arguments.boost + " "

        if arguments.rapidjson != None and i == 1:
            configureLine += "--with-rapidjson=" + arguments.rapidjson + " "

        buildWithConfigureLine(configureLine, baseDir)
        
    subprocess.call("rm -rf *", shell=True)


# We get the parsed commandline arguments and build the flavors accordingly
def configureAndBuildFlavor(arguments):
    baseDir = os.getcwd()[0:os.getcwd().rfind('/')]
    tempDirectory = createAndCDToTempDir()
    # We want to check whether we have to build using ROSE, Clang, and RapidJson
    if arguments.boost == None:
        raise Exception("no boost path specified")

    if arguments.rosepath != None:
        buildWithRose(arguments, baseDir)

    if arguments.llvmsrc != None and arguments.llvminstall != None:
        buildWithClang(arguments, baseDir)

    os.chdir(baseDir)
    os.rmdir(tempDirectory)

args = cmdParser.parse_args()
configureAndBuildFlavor(args)
