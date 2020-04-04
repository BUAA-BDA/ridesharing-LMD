#!/usr/bin/env python
import commands
import os
import sys
from random import randint
import multiprocessing

class constForBatchRun:
	execNames = ["fesif"]
	MAX_SAMPLE = 10
	
class CFBR(constForBatchRun):
	pass


def run(execName, locFile, hstFile, dataFile, logFile):
	cmdLine = "./%s %s %s %s" % (execName, locFile, dataFile, hstFile)
	print cmdLine
	line = commands.getoutput(cmdLine)
	print line
	with open(logFile, "w") as fout:
		fout.write(line)

def getLocSuffix(s):
	tmpList = s.split('_')
	return tmpList[-1]

def batchRun(srcFilePath, locFilePath, hstFilePath, desFilePath, dataSetN, nprocess):
	pool = multiprocessing.Pool(processes = nprocess)

	if not os.path.exists(desFilePath):
		os.mkdir(desFilePath)
	dirNames = os.listdir(srcFilePath)
	dirNames.remove("location")
	for dataSetId in xrange(dataSetN):
		for execName in CFBR.execNames:
			tmpFilePath = os.path.join(desFilePath, execName)
			if not os.path.exists(tmpFilePath):
				os.mkdir(tmpFilePath)
			for dirName in dirNames:
				locSuffix = getLocSuffix(dirName)
				tmpFilePath = os.path.join(desFilePath, execName, dirName)
				if not os.path.exists(tmpFilePath):
					os.mkdir(tmpFilePath)
				srcFileName = "data_%02d.txt" % (dataSetId)
				locFileName = "loc_%02d.txt" % (dataSetId)
				desFileName = os.path.join(desFilePath, execName, dirName, srcFileName)
				locFileName = os.path.join(locFilePath, locSuffix, locFileName)
				srcFileName = os.path.join(srcFilePath, dirName, srcFileName)
				if os.path.exists(desFileName):
					continue
				idx = randint(0, CFBR.MAX_SAMPLE-1)
				hstFileName = "hst_%04d.txt" % (idx)
				_hstFileName = "loc_%02d" % (dataSetId)
				hstFileName = os.path.join(hstFilePath, locSuffix, _hstFileName, hstFileName)
				pool.apply_async(run, (execName, locFileName, hstFileName, srcFileName, desFileName, ))

	pool.close()
	pool.join()


def exp0(dataSetN=8, nprocess=8):
	setFilePath = "./synData"
	locFilePath = os.path.join(setFilePath, "location")
	desFilePath = "./synResult"
	hstFilePath = "./synHST"
	batchRun(setFilePath, locFilePath, hstFilePath, desFilePath, dataSetN, nprocess)
	
	
if __name__ == "__main__":
	dataSetN = 1
	nprocess = 16
	exp0(dataSetN, nprocess)
	
