#!/usr/bin/env python3

# +-------------------------------------+
# |                                     |
# | Script realizzato da Riccardo Motta |
# |                                     |
# +-------------------------------------+

import argparse
import os
import subprocess
import sys
import time



def error (message, noColor = False):
	if noColor:
		print('\n------------ ERROR ------------\n\n' + message + '\n\nAborting...\n')
	else:
		print('\n\033[1;31m------------ ERROR ------------\033[0m\n\n' + message + '\n\n\033[1;31mAborting...\033[0m\n')
	exit(1)



def check_files (source, input):

	missing = False

	print('\n\033[1;36mChecking existance of source and input files\033[0m')

	sys.stdout.write('Source file: \033[1;33mchecking\033[0m')
	sys.stdout.flush()
	if not os.path.isfile(source):
		sys.stdout.write('\rSource file: \033[1;31mdoes\'t exist\033[0m\n')
		sys.stdout.flush()
		missing = True
	else:
		sys.stdout.write('\rSource file: \033[1;32mexists\033[0m  \n')
		sys.stdout.flush()
	
	sys.stdout.write('Input file: \033[1;33mchecking\033[0m')
	sys.stdout.flush()
	if not os.path.isfile(input):
		sys.stdout.write('\rInput file: \033[1;31mdoes\'t exist\033[0m\n')
		sys.stdout.flush()
		missing = True
	else:
		sys.stdout.write('\rInput file: \033[1;32mexists\033[0m  \n')
		sys.stdout.flush()
	
	if missing:
		error('Some files are missing, check the paths.')



def check_programs (disable_valgrind, disable_callgrind, disable_massif, disable_memcheck, disable_visualization):

	missing = False

	print('\n\033[1;36mChecking if programs are installed and executable from shell\033[0m')

	# GCC
	sys.stdout.write('GCC: \033[1;33mchecking\033[0m')
	sys.stdout.flush()
	process = subprocess.run('gcc --version', shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.DEVNULL)
	if not process.returncode == 0:
		sys.stdout.write('\rGCC: \033[1;31mabsent\033[0m  \n')
		sys.stdout.flush()
		missing = True
	else:
		sys.stdout.write('\rGCC: \033[1;32mexecutable\033[0m\n')
		sys.stdout.flush()
	
	if not disable_valgrind:
		if not (disable_callgrind and disable_massif and disable_memcheck):
			# Valgrind
			sys.stdout.write('Valgrind: \033[1;33mchecking\033[0m')
			sys.stdout.flush()
			process = subprocess.run('valgrind --version', shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.DEVNULL)
			if not process.returncode == 0:
				sys.stdout.write('\rValgrind: \033[1;31mabsent\033[0m  \n')
				sys.stdout.flush()
				missing = True
			else:
				sys.stdout.write('\rValgrind: \033[1;32mexecutable\033[0m\n')
				sys.stdout.flush()
		
		if not disable_callgrind and not disable_visualization:
			# KCachegrind
			sys.stdout.write('KCachegrind: \033[1;33mchecking\033[0m')
			sys.stdout.flush()
			process = subprocess.run('kcachegrind --version', shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.DEVNULL)
			if not process.returncode == 0:
				sys.stdout.write('\rKCachegrind: \033[1;31mabsent\033[0m  \n')
				sys.stdout.flush()
				missing = True
			else:
				sys.stdout.write('\rKCachegrind: \033[1;32mexecutable\033[0m\n')
				sys.stdout.flush()
		
		if not disable_massif and not disable_visualization:
			# Massif-Visualizer
			sys.stdout.write('Massif-Visualizer: \033[1;33mchecking\033[0m')
			sys.stdout.flush()
			process = subprocess.run('massif-visualizer --version', shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.DEVNULL)
			if not process.returncode == 0:
				sys.stdout.write('\rMassif-Visualizer: \033[1;31mabsent\033[0m  \n')
				sys.stdout.flush()
				missing = True
			else:
				sys.stdout.write('\rMassif-Visualizer: \033[1;32mexecutable\033[0m\n')
				sys.stdout.flush()

	if missing:
		error('Some programs are missing, please install them and make them executable from the shell.')
	
	print()



def buildAndRun (source, input, no_output, store_output, output_file):

	print('\033[1;36mBuilding and running the project to ckeck the output\033[0m')

	sys.stdout.write('Building the project: \033[1;33mrunning\033[0m')
	sys.stdout.flush()
	process = subprocess.run('gcc -DEVAL -Wall -Werror -O2 -pipe -g3 ' + source + ' -o ' + source[:-2] + ' -lm', shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.PIPE)
	if not process.returncode == 0:
		sys.stdout.write('\rBuilding the project: \033[1;31mfailed\033[0m \n')
		sys.stdout.flush()
		error('Compilation failed with exit code \033[1;31m' + str(process.returncode) + '\033[0m and message:\n' + str(process.stderr.decode())[:-1])
	sys.stdout.write('\rBuilding the project: \033[1;32mdone\033[0m   \n')
	sys.stdout.flush()

	sys.stdout.write('Running the project: \033[1;33mrunning\033[0m')
	sys.stdout.flush()
	process = subprocess.run('./' + source[:-2], stdin = open(input, 'r'), shell = True, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
	if not process.returncode == 0:
		sys.stdout.write('\rRunning the project: \033[1;31mfailed\033[0m \n')
		sys.stdout.flush()
		print('Execution failed with exit code \033[1;31m' + str(process.returncode) + '\033[0m and message:\n' + str(process.stderr.decode())[:-1])
	else:
		sys.stdout.write('\rRunning the project: \033[1;32mdone\033[0m   \n')
		sys.stdout.flush()

		if not no_output:
			if store_output and output_file:
				sys.stdout.write('Saving the output into the file: \033[1;33mrunning\033[0m')
				sys.stdout.flush()
				file = open(output_file, 'w')
				file.write(process.stdout.decode())
				file.close()
				sys.stdout.write('\rSaving the output into the file: \033[1;32mdone\033[0m   \n')
				sys.stdout.flush()
			else:
				print('\n\033[1;35m-----BEGIN PROGRAM OUTPUT-----\033[0m')
				print(process.stdout.decode()[:-1])
				print('\033[1;35m----- END PROGRAM OUTPUT -----\033[0m')



def valgrind (source, input, disable_callgrind, disable_massif, disable_memcheck, disable_visualization):

	print('\n\033[1;36mRunning Valgrind checks and analysis\033[0m')

	if not disable_memcheck:
		sys.stdout.write('Memcheck check: \033[1;33mrunning\033[0m')
		sys.stdout.flush()
		process = subprocess.run('valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./' + source[:-2], stdin = open(input, 'r'), shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.PIPE)
		if not process.returncode == 0:
			sys.stdout.write('\rMemcheck check: \033[1;31mfailed\033[0m \n')
			sys.stdout.flush()
			error('Memcheck test failed with exit code \033[1;31m' + str(process.returncode) + '\033[0m and message:\n' + str(process.stderr.decode())[:-1])
		sys.stdout.write('\rMemcheck check: \033[1;32mpassed\033[0m \n')
		sys.stdout.flush()

	if not disable_callgrind:
		sys.stdout.write('Callgrind analysis: \033[1;33mrunning\033[0m')
		sys.stdout.flush()
		process = subprocess.run('valgrind --tool=callgrind --callgrind-out-file=result.callgrind ./' + source[:-2], stdin = open(input, 'r'), shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.PIPE)
		if not process.returncode == 0:
			sys.stdout.write('\rCallgrind analysis: \033[1;31mfailed\033[0m \n')
			sys.stdout.flush()
			error('Callgrind analysis failed with exit code \033[1;31m' + str(process.returncode) + '\033[0m and message:\n' + str(process.stderr.decode())[:-1])
		sys.stdout.write('\rCallgrind analysis: \033[1;32mcompleted\033[0m\n')
		sys.stdout.flush()

		if not disable_visualization:
			sys.stdout.write('Displaying Callgrind result: \033[1;33mstarting\033[0m')
			sys.stdout.flush()
			subprocess.Popen(['kcachegrind result.callgrind'], shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.DEVNULL)
			sys.stdout.write('\rDisplaying Callgrind result: \033[1;32mdone\033[0m    \n')
			sys.stdout.flush()

	if not disable_massif:
		sys.stdout.write('Massif analysis: \033[1;33mrunning\033[0m')
		sys.stdout.flush()
		process = subprocess.run('valgrind --tool=massif --massif-out-file=result.massif ./' + source[:-2], stdin = open(input, 'r'), shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.PIPE)
		if not process.returncode == 0:
			sys.stdout.write('\rMassif analysis: \033[1;31mfailed\033[0m \n')
			sys.stdout.flush()
			error('Callgrind analysis failed with exit code \033[1;31m' + str(process.returncode) + '\033[0m and message:\n' + str(process.stderr.decode())[:-1])
		sys.stdout.write('\rMassif analysis: \033[1;32mcompleted\033[0m\n')
		sys.stdout.flush()

		if not disable_visualization:
			sys.stdout.write('Displaying Massif result: \033[1;33mstarting\033[0m')
			sys.stdout.flush()
			subprocess.Popen(['massif-visualizer result.massif'], shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.DEVNULL)
			sys.stdout.write('\rDisplaying Massif result: \033[1;32mdone\033[0m    \n')
			sys.stdout.flush()



def timing (source, input, tests_number):

	execution_times = []

	print('\n\033[1;36mExecution time estimation based on', tests_number, 'tests\033[0m')

	sys.stdout.write('Running: \033[1;33m0 of ' + str(tests_number) + '\033[0m')
	sys.stdout.flush()

	for index in range(1, tests_number + 1):

		sys.stdout.write('\rRunning: \033[1;33m' + str(index) + ' of ' + str(tests_number) + '\033[0m')
		sys.stdout.flush()

		start = time.time()
		subprocess.run('./' + source[:-2], stdin = open(input, 'r'), shell = True, stdout = subprocess.PIPE, stderr = subprocess.DEVNULL)
		finish = time.time()

		execution_times.append(finish - start)

	sys.stdout.write('\rRunning: \033[1;32m' + str(tests_number) + ' of ' + str(tests_number) + '\033[0m\n')
	sys.stdout.flush()

	if tests_number > 1:
		print('Average: \033[1;33m' + str(round(sum(execution_times) / len(execution_times), 3)) + ' ms\033[0m')
		print('Minimum: \033[1;32m' + str(round(min(execution_times), 3)) + ' ms\033[0m')
		print('Maximum: \033[1;31m' + str(round(max(execution_times), 3)) + ' ms\033[0m')
	else:
		print('Time: \033[1;33m' + str(round(execution_times[0], 3)) + ' ms\033[0m')



if __name__ == '__main__':

	parser = argparse.ArgumentParser(description = 'An automated testing suite for the project')

	parser.add_argument('source', help = 'source file (.c)')
	parser.add_argument('input', help = 'input file')

	parser.add_argument('--disable_valgrind', action = 'store_true', help = 'disables Callgrind, Massif and Memcheck analysis')

	parser.add_argument('--disable_callgrind', action = 'store_true', help = 'disables Callgrind analysis')
	parser.add_argument('--disable_massif', action = 'store_true', help = 'disables Massif analysis')
	parser.add_argument('--disable_memcheck', action = 'store_true', help = 'disables Memcheck analysis')

	parser.add_argument('--disable_visualization', action = 'store_true', help = 'disables the visualization of Callgrind and/or Massif, according to the previous settings')

	parser.add_argument('--tests_number', type = int, default = 10, help = 'number of executions to calculate the medium running time (only if TESTS_NUMBER isn\'t 0)')

	parser.add_argument('--no_output', action = 'store_true', help = 'avoids to store or print the output')
	parser.add_argument('--store_output', action = 'store_true', help = 'stores the output as a file (only if --no_output isn\'t set)')
	parser.add_argument('--output_file', help = 'output file name (only if --store_output is set and --no_output isn\'t set)')

	args = parser.parse_args()

	if not sys.platform == 'linux' and not sys.platform == 'linux2':
		if sys.platform == 'win32':
			error('This script runs only under Linux.', noColor = True)
		else:
			error('This script runs only under Linux.')

	if args.tests_number < 0:
		error('TESTS_NUMBER must be a positive number, or at least 0 if you want to disable this.')

	check_files(args.source, args.input)

	check_programs(args.disable_valgrind, args.disable_callgrind, args.disable_massif, args.disable_memcheck, args.disable_visualization)

	buildAndRun(args.source, args.input, args.no_output, args.store_output, args.output_file)

	if not args.disable_valgrind:
		valgrind(args.source, args.input, args.disable_callgrind, args.disable_massif, args.disable_memcheck, args.disable_visualization)

	if not args.tests_number == 0:
		timing(args.source, args.input, args.tests_number)

	print()
