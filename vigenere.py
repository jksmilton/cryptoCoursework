def incidentOfCoincidence(text, step):
	mapOfCounts = dict([('a',0), ('b',0), ('c',0), ('d',0), ('e',0), ('f',0), ('g',0),
		('h',0), ('i',0), ('j',0), ('k',0), ('l',0), ('m',0), ('n',0), ('o',0),
		('p',0), ('q',0), ('r',0), ('s',0), ('t',0), ('u',0), ('v',0), ('w',0),
		('x',0), ('y',0), ('z',0)])
	i = 0
	n = 0

	while i < len(text):
		mapOfCounts[text[i]] = mapOfCounts[text[i]] + 1
		i = i + step
		n = n + 1

	sumOfcounts = 0

	for v in mapOfCounts.values():
		sumOfcounts = v * (v - 1)

	divisor = n * (n - 1) / 26
	sumOfcounts = sumOfcounts / divisor
	return sumOfcounts

def getStrippedFile(fileName):
	f = open(fileName, 'r')
	output = f.read()
	while(not(f.closed)):
		f.close()
	return output.replace(" ", "")