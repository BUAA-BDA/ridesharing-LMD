#!/usr/bin/env python
import commands
import os
import sys
from random import randint
import multiprocessing


def run(execName, locFile, dataFile):
	cmdLine = "./%s %s %s" % (execName, locFile, dataFile)
	print cmdLine
	line = commands.getoutput(cmdLine)
	print line


def getLocSuffix(s):
	tmpList = s.split('_')
	return tmpList[-1]


def batchRun(locFilePath, desFilePath, dataSetN, nprocess):
	pool = multiprocessing.Pool(processes = nprocess)

	if not os.path.exists(desFilePath):
		os.mkdir(desFilePath)
	dirNames = os.listdir(locFilePath)
	for dataSetId in xrange(dataSetN):
		execName = "chst"
		for dirName in dirNames:
			tmpFilePath = os.path.join(desFilePath, dirName)
			if not os.path.exists(tmpFilePath):
				os.mkdir(tmpFilePath)
			srcFileName = "loc_%02d.txt" % (dataSetId)
			tmpFileName = "loc_%02d" % (dataSetId)
			srcFileName = os.path.join(locFilePath, dirName, srcFileName)
			tmpFilePath = os.path.join(desFilePath, dirName, tmpFileName)
			if not os.path.exists(tmpFilePath):
				os.mkdir(tmpFilePath)
			else:
				continue
			pool.apply_async(run, (execName, srcFileName, tmpFilePath, ))

	pool.close()
	pool.join()


def exp0(dataSetN=8, nprocess=8):
	setFilePath = "./synData"
	locFilePath = os.path.join(setFilePath, "location")
	desFilePath = "./synHST"
	batchRun(locFilePath, desFilePath, dataSetN, nprocess)

def exp1(dataSetN=8, nprocess=8):
	setFilePath = "./olistData"
	locFilePath = os.path.join(setFilePath, "location")
	desFilePath = "./olistHST"
	batchRun(locFilePath, desFilePath, dataSetN, nprocess)
	
	
if __name__ == "__main__":
	dataSetN = 1
	nprocess = 10
	exp0(dataSetN, nprocess)
	exp1(dataSetN, nprocess)
	
