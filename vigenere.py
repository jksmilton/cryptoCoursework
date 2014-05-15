def createLangDic():
	dictionary = set()
	with open('en-GB-wlist.txt', 'r') as f:
		for line in f:
			dictionary.add(line[:len(line)-1])
	return dictionary
maxKeyLength = 12
langDict = createLangDic()
letterToNum = dict([('a',0), ('b',1), ('c',2), ('d',3), ('e',4), ('f',5), ('g',6),
		('h',7), ('i',8), ('j',9), ('k',10), ('l',11), ('m',12), ('n',13), ('o',14),
		('p',15), ('q',16), ('r',17), ('s',18), ('t',19), ('u',20), ('v',21), ('w',22),
		('x',23), ('y',24), ('z',25)])
numToLetter = dict([(0, 'a'), (1, 'b'), (2, 'c'), (3, 'd'), (4, 'e'), (5, 'f'), (6, 'g'),
		(7, 'h'), (8, 'i'), (9, 'j'), (10, 'k'), (11, 'l'), (12, 'm'), (13, 'n'), (14, 'o'),
		(15, 'p'), (16, 'q'), (17, 'r'), (18, 's'), (19, 't'), (20, 'u'), (21, 'v'), (22, 'w'),
		(23, 'x'), (24, 'y'), (25, 'z')])
passRate = 0.75
def incidentOfCoincidence(text, step):
	mapOfCounts = dict([('a',0), ('b',0), ('c',0), ('d',0), ('e',0), ('f',0), ('g',0),
		('h',0), ('i',0), ('j',0), ('k',0), ('l',0), ('m',0), ('n',0), ('o',0),
		('p',0), ('q',0), ('r',0), ('s',0), ('t',0), ('u',0), ('v',0), ('w',0),
		('x',0), ('y',0), ('z',0)])
	n, start, incidence = 0, 0, 0
	while start < step:
		i = start
		while i < len(text):
			mapOfCounts[text[i]] = mapOfCounts[text[i]] + 1
			i = i + step
			n = n + 1

		sumOfcounts = 0
		for v in mapOfCounts.values():
			sumOfcounts = v * (v - 1)

		divisor = n * (n - 1) / 26
		sumOfcounts = sumOfcounts / divisor
		incidence = incidence + sumOfcounts
		start = start + 1
	incidence = incidence / step

	return incidence

def getStrippedFile(fileName):
	f = open(fileName, 'r')
	output = f.read()
	while(not(f.closed)):
		f.close()
	return (output.replace(" ", ""), output)

def getKeyLengthCandidates(text, numToTest):
	results = dict()
	i = 1
	while i < maxKeyLength:
		results[i] = incidentOfCoincidence(text, i)
		i = i + 1
	sortedResults = sorted(results.items(), key=lambda x: x[1])
	keyLengthsToTest, notWanted = zip(*(sortedResults[-numToTest:]))
	return keyLengthsToTest, notWanted

def createColumns(text, keySize):
	i = 0
	result = ['' for x in range(keySize)]
	while i < len(text):
		index = i % keySize
		result[index] = result[index] + text[i]
		i = i + 1
	return result

def bruteForce(columns, original, counter):
	candidates = []
	line = columns[counter]
	for i in range(0,26):
		j = 0
		newLine = ''
		while j < len(line):
			newLine = newLine + numToLetter[(letterToNum[line[j]] + i) % 26]
			j = j + 1
		crntColumns = columns[:counter] + [newLine] + columns[counter + 1:]
		if counter < len(columns) - 1:
			candidates.extend(bruteForce(crntColumns, original, counter + 1))
		else:
			candidateAttempt = rebuild(crntColumns, original)
			dicMatch = dictionaryCheck(candidateAttempt)
			#print(candidateAttempt)
			#print(dicMatch)
			if  dicMatch > passRate:
				print('here')
				candidates.append(candidateAttempt)
	return candidates

def rebuild(columns, original):
	keyLength = len(columns)
	words = original.split()
	strBuilder = ''
	i = 0
	for word in words:
		j = 0
		while j < len(word):
			strBuilder = strBuilder + columns[i%keyLength][i//keyLength]
			i = i+1
			j = j+1
		strBuilder = strBuilder + ' '
		#print(strBuilder)
	return strBuilder



def dictionaryCheck(text):
	words = text.split()
	totalWords = len(words)
	totalCorrect = 0
	for word in words:
		if word in langDict:
			totalCorrect = totalCorrect + 1
	return totalCorrect/totalWords
