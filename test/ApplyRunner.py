from __future__ import print_function
import sys
import os
import subprocess
import argparse
import shutil
from multiprocessing import Pool


def toErr(*objs):
	print("[status] ", *objs, file=sys.stderr)

cmdParser = argparse.ArgumentParser(description='Runs the test instrumentor executable on all input files.')
cmdParser.add_argument('src', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('build', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('compilerIndication', type=str, help="Which compiler is running? [rose/clang]")

# This is the list of test programs which should be applied.
testPrograms = ["ConstructHierarchySelectionTest", "IdentifierSelectorTest", "ConstructElevatorTest", "BooleanCompoundSelectorTest", "CallpathSelectorTest", "UniqueCallpathTransformerTest", "DefaultInstrumentationAdapterTest", "ScorepRegionAdapterTest"]

# The list of targets is read from a targets.lst which resides in a test/input/BinaryName directory
def readTargetFileSpecification(tInstrumentor, directory):
	targets = []
	inFile = open(directory + '/' + tInstrumentor + "/targets.lst")
	for line in inFile:
		targets.append(line.strip())

	return targets

def runTest(k, arguments, binary, inputDirectory):
	failedRuns = []
	srcFile = k + ".cpp"
	specFile = k + ".in"
	outFile = binary + '_' + k + '.out'
	src2srcOutFile = binary + '_' + srcFile


	roseExtraArg = ""
	if arguments.compilerIndication == 'rose':
		roseExtraArg += " --edg:no_warnings "
		roseExtraArg += ' --instro-library-path=../' + arguments.build + '/test/.libs'
		roseExtraArg += ' --instro-library-name=InstRO_rtsupport'
		if os.path.isabs(arguments.src):
			roseExtraArg += ' --instro-include=' + arguments.src + '/support'
		else:
			roseExtraArg += ' --instro-include=../' + arguments.src + '/support'

  	roseExtraArg += ' -rose:o ' + src2srcOutFile

	os.environ["INSTRO_TEST_INPUT_FILENAME"] = inputDirectory + '/' + binary + '/' + specFile
	invocationString = "../" + binary + " "  + roseExtraArg + ' ' + inputDirectory + '/' + srcFile + ' -o ' + outFile
	# we need to add the "--" to the invocation as we do not have JSON compilation databases
	if arguments.compilerIndication == 'clang':
		invocationString += ' --'
            
	toErr("Running\n" + binary + " " + srcFile)
	if False:
		toErr("Detailed invocation info: " + invocationString)

	errCode = 0
	out = ''
	try:
		out = subprocess.check_output(invocationString, shell=True)
	except subprocess.CalledProcessError as e:
		errCode = e.returncode
		toErr('[Error] code ' + str(errCode) + ' Dumping STDOUT \n' + out + '\n' + e.output)

	if errCode == 0 and binary == "DefaultInstrumentationAdapterTest":
		try:
			out = subprocess.check_output('./'+outFile, shell=True)
		except subprocess.CalledProcessError as e:
			errCode = e.returncode
			toErr("[Error] Problem when running binary: " + e.output)

	toErr("Done")
	if errCode != 0:
		failedRuns.append((binary, srcFile))
	
	if os.path.isfile(outFile):
		os.remove(outFile)
	if os.path.isfile(k+'.o'):
		os.remove(k+'.o')
	if os.path.isfile(src2srcOutFile):
		os.remove(src2srcOutFile)

	return failedRuns

def runTestBinary(arguments, binary, inputDirectory):
	targets = readTargetFileSpecification(binary, inputDirectory)

	os.mkdir(binary+'_dir')
	restoreDir = os.getcwd()
	if os.path.isdir(binary+'_dir'):
		os.chdir(binary+'_dir')
	else:
		raise Exception('creating binary dir failed', 'srsly!')

	if not os.path.isabs(inputDirectory):
		inputDirectory = '../' + inputDirectory
#	else:
#		raise Exception('expected relative path here', 'would be easier')

	failedRuns = []
	for k in targets:
		failedRuns += runTest(k, arguments, binary, inputDirectory)

	os.chdir(restoreDir)
	if os.path.isdir(binary+'_dir'):
		shutil.rmtree(binary+'_dir')

	return failedRuns

# Runs each TestInstrumentor on the given target list
def runApply(arguments):
	baseDir = os.getcwd()[0:os.getcwd().rfind('/')]

	inputDirectory = arguments.src + "/test/input"

	os.environ['LD_LIBRARY_PATH'] = os.path.abspath(arguments.build)+'/test/.libs:'+os.environ['LD_LIBRARY_PATH']

	pool = Pool()
	failedRuns = []
	collector = []
	for b in testPrograms:
		if os.path.isfile(b):
			try:
				collector.append(pool.apply_async(runTestBinary, (arguments, b, inputDirectory)))
			except Exception as e:
				print('Caught exception in running binaries ' + str(e))
		else:
			failedRuns += [(b, 'not available')]

	for elem in collector:
		fr = elem.get()
		if len(fr) != 0:
			failedRuns += fr

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
