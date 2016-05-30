from __future__ import print_function
import sys
import os
import subprocess
import argparse
import shutil
import time
from multiprocessing import Pool


def toErr(*objs):
	print("[status] ", *objs, file=sys.stderr)

cmdParser = argparse.ArgumentParser(description='Runs the test instrumentor executable on all input files.')
cmdParser.add_argument('src', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('build', type=str, help="/path/to/instro/repo")
cmdParser.add_argument('compilerIndication', type=str, help="Which compiler is running? [rose/clang]")
cmdParser.add_argument('optionals', nargs='*', default=[])

# This is the list of test programs which should be applied.
testPrograms = ["ConstructHierarchySelectionTest", "IdentifierSelectorTest", "ConstructElevatorTest", "BooleanCompoundSelectorTest", "CallpathSelectorTest", "UniqueCallpathTransformerTest", "FunctionWrapperTest", "DefaultInstrumentationAdapterTest"]

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

	# Add flags we want to pass to rose only translators
	roseExtraArg = ' --edg:no_warnings'
	roseExtraArg += ' -rose:o ' + src2srcOutFile
	roseExtraArg += ' --instro-library-path=../' + arguments.build + '/test/.libs'
	roseExtraArg += ' --instro-library-name=InstRO_rtsupport'

	# Add flags we want to pass to scorep/rose only translators
	# Skip final compile step for scorep test, since we need to compile using scorep!
	if binary == 'ScorepRegionAdapterTest':
		roseExtraArg += ' -rose:skipfinalCompileStep'
  	
	invocationString = '../' + binary + ' '
	
	if arguments.compilerIndication == 'rose':
		# We add the necessary location infos (is actually still necessary?)
		if os.path.isabs(arguments.src):
			roseExtraArg += ' --instro-include=' + arguments.src + '/support'
		else:
			roseExtraArg += ' --instro-include=../' + arguments.src + '/support'

		invocationString += roseExtraArg + ' ' + inputDirectory + '/' + srcFile + ' -o ' + outFile

	elif arguments.compilerIndication == 'clang':
		invocationString += inputDirectory + '/' + srcFile
		# we need to add the "--" to the invocation as we do not have JSON compilation databases
		invocationString += ' --'
            
	os.environ['INSTRO_TEST_INPUT_FILENAME'] = inputDirectory + '/' + binary + '/' + specFile
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

	# we have to compile the source that was translated manually, as we need to invoke
	# scorep and pass the --user --nocompiler instrumentation flags.
	if errCode == 0 and binary == 'ScorepRegionAdapterTest':
		try:
			scorepInvocStr = 'scorep --user --nocompiler g++ ' + src2srcOutFile + ' -o ' + outFile
			out = subprocess.check_output(scorepInvocStr, shell=True)
		except subprocess.CalledProcessError as e:
			errCode = e.returncode
			toErr('[Error] Not able to call ScoreP. Make sure is available in PATH')

	if errCode == 0 and (binary == "DefaultInstrumentationAdapterTest" or binary == "ScorepRegionAdapterTest"):
		# Scorep runtime prints errors to stderr, so we need to pipe it to stdout for this python command.
		# It seems that, although errorneous, 0 is returned, thus we check output for Score-P errors
		# and raise the exception by hand.
		# The sleep is to get around potential timestamp problems, when the test runner tries to delete a
		# score-p directory, while score-p still copies the experiment data.
		try:
			out = subprocess.check_output('./'+outFile, stderr=subprocess.STDOUT, shell=True)
			time.sleep(1.0)
			if out.find('[Score-P]') != -1:
				raise subprocess.CalledProcessError(-1, 'running ' + outFile, out)
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

	# Depending on the additional information, add tests
	if 'scorep' in arguments.optionals:
		print('Adding ScorepRegionAdapterTest to the list of tests to run')
		testPrograms.append('ScorepRegionAdapterTest')

	baseDir = os.getcwd()[0:os.getcwd().rfind('/')]

	inputDirectory = arguments.src + "/test/input"

	newLDLIBS = os.path.abspath(arguments.build)+'/test/.libs'
	currentLDLIBPATH = os.environ.get('LD_LIBRARY_PATH')
	if currentLDLIBPATH is None:
		# environment variable might not be set beforehand
		os.environ['LD_LIBRARY_PATH'] = newLDLIBS
	else:
		os.environ['LD_LIBRARY_PATH'] = newLDLIBS+':'+currentLDLIBPATH

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
