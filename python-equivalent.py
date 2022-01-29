import math
from sys import argv
from threading import Thread
from os.path import exists as fileExists
import time




runthread = True
primes = []

def main():
	n = 0
	ifilename = ''
	if len(argv) == 1:
		global runthread
		n = int(input('Numbers: '))
		global primes
		ifilename = input('File: ')

	elif len(argv) == 3:
		ifilename = argv[1]
		n = int(argv[2])

	else:
		exit()


	i = 2
	primes = []
	if fileExists(ifilename):
		print('Restoring vector')
		ifile = open(ifilename, 'r')
		if not ifile.closed:

			j = 0
			total = 0
			for line in ifile.read().split('\n'):
				j += 1
				total += len(line) + 1
				if not j & 0xfffff:
					print(str(total / 0x100000) + "mb")
				if not line == '': primes.append(int(line))

			i = primes[len(primes) - 1]
			ifile.close()

		else: print('Unable to open file')


	print('Calculating...')
	runthread = True
	Thread(target=trd).start()
	while len(primes) < n:
		def goto():







			root = int(math.sqrt(i))
			for ci in primes:
				if i % ci == 0:
					return
				if ci > root:
					break

			primes.append(i)
		goto()
		i += 1



	runthread = False
	print('Writing...')
	if True:
		file = open(ifilename, 'w')
		data = ''
		count = 0
		for i in range(len(primes)):
			data += str(primes[i]) + '\n'
			if count & 0x1000000:
				print(str(float(i) / 0x100000) + 'm')
				file.write(data)
				count = 0
				data = ''
			count += 1


		file.write(data)
		file.close()





	print('Done!')


def trd():
	before = 0
	while runthread:
		print(str(math.floor(len(primes) / 1000) / 1000) + 'm | ' + str(float(len(primes) - before) / 100) + 'k')
		before = len(primes)
		time.sleep(.1)
main()