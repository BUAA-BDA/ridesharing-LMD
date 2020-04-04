#!/usr/bin/env python

# import numpy as np
from random import randint, sample, shuffle, expovariate, uniform, gauss
import sys
import os
import math

class constForDataSet:
	caseN = 30
	locRng = [0, 200]
	RList = [2000, 4000, 6000, 8000, 10000]
	WList = [60, 100, 150, 200, 300]
	crList = [1, 1.5, 2, 2.5, 3]
	cwList = [4, 6, 8, 10, 15]
	meanList = [50, 75, 100, 125, 150]
	muList = [50, 75, 100, 125, 150]
	sigmaList = [10, 20, 30, 40, 50]
	lambdaList = [50, 75, 100, 125, 150]
	defaultValues = [
		RList[len(RList)/2],
		WList[len(WList)/2],
		crList[len(crList)/2],
		cwList[len(cwList)/2],
		meanList[len(meanList)/2],
		muList[len(muList)/2],
		sigmaList[len(sigmaList)/2],
		lambdaList[len(lambdaList)/2],
	]
	nw, no, nd = [max(WList), 50, 120]
	

class CFDS(constForDataSet):
	pass

class worker(object):

	def __init__(self, oid=0, cap=0):
		self.oid = oid
		self.cap = cap
		
	def __str__(self):
		return "%s %s" % (self.oid, self.cap)
		
		
class request(object):

	def __init__(self, oid=0, did=0, wei=0):
		self.oid = oid
		self.did = did
		self.wei = wei
		
	def __str__(self):
		return "%s %s %s" % (self.oid, self.did, self.wei)
		

class baseGenerator:

	def gen(self, n):
		pass

class randomGenerator(baseGenerator):

	def __init__(self, lb, rb):
		self.lb = lb
		self.rb = rb

	def gen(self, n):
		ret = [0] * n
		for i in xrange(n):
			ret[i] = self.__gen()
		return ret
		
	def __gen(self):
		return randint(self.lb, self.rb)

class normalGenerator(baseGenerator):

	def __init__(self, mu, sigma):
		self.mu = mu
		self.sigma = sigma

	def gen(self, n, lb, rb):
		ret = [0] * n
		for i in xrange(n):
			ret[i] = self.__gen(lb, rb)
		return ret
		
	def __gen(self, lb, rb):
		ret = int(gauss(self.mu, self.sigma))
		if ret < lb:
			ret = lb
		if ret > rb:
			ret = rb
		return ret

	def setMu(self, mu):
		self.mu = mu

	def setSigma(self, sigma):
		self.sigma = sigma


class uniformGenerator(baseGenerator):

	def __init__(self, mean):
		self.mean = mean

	def gen(self, n, lb, rb):
		ret = [0] * n
		for i in xrange(n):
			ret[i] = self.__gen(lb, rb)
		return ret
		
	def __gen(self, lb, rb):
		if self.mean <= (lb+rb)/2:
			low = lb
			high = 2*self.mean - lb
		else:
			high = rb
			low = 2*self.mean - high
		ret = int(uniform(low, high))
		if ret < lb:
			ret = lb
		if ret > rb:
			ret = rb
		return ret

	def setLow(self, low):
		self.low = low

	def setHigh(self, high):
		self.high = high

class expGenerator(baseGenerator):

	def __init__(self, mu):
		self.mu = mu

	def gen(self, n, lb, rb):
		ret = [0] * n
		for i in xrange(n):
			ret[i] = self.__gen(lb, rb)
		return ret
	
	def __gen(self, lb, rb):
		ret = int(expovariate(1.0 / self.mu))
		if ret < lb:
			ret = lb
		if ret > rb:
			ret = rb
		return ret
	
	def setMu(self, mu):
		self.mu = mu


def genLocations(n, gtor):
	ret = []
	st = set()
	while len(st) < n:
		X = gtor.gen(n-len(st), CFDS.locRng[0], CFDS.locRng[1])
		Y = gtor.gen(n-len(st), CFDS.locRng[0], CFDS.locRng[1])
		for i in xrange(n-len(st)):
			t = (X[i], Y[i])
			if t not in st:
				ret.append(t)
				st.add(t)
	return ret

	
def genDataFileName(R, W, cr, cw, para, flag):
	return "%s_%s_%s_%s_%s%s" % (R, W, cr, cw, flag, para)

def genLocFileName(para, flag):
	return "%s%s" % (flag, para)
	
	
def genWeights(nR, maxw):
	# maxc = CFDS.defaultValues[2]
	clb, crb = 0, CFDS.crList.index(maxw)
	cgt = randomGenerator(clb, crb)
	
	wids = cgt.gen(nR)
	ret = [CFDS.crList[wids[i]] for i in xrange(nR)]
	return ret
	
	
def genCaps(nW, maxc):
	# maxc = CFDS.defaultValues[3]
	clb, crb = 0, CFDS.cwList.index(maxc)
	cgt = randomGenerator(clb, crb)
	
	cids = cgt.gen(nW)
	ret = [maxc for i in xrange(nW)]
	return ret
	
	
def genRequests(nR):
	olb, orb = CFDS.nw, CFDS.nw+CFDS.no-1
	dlb, drb = CFDS.nw+CFDS.no, CFDS.nw+CFDS.no+CFDS.nd-1
	ogt = randomGenerator(olb, orb)
	dgt = randomGenerator(dlb, drb)
	
	oids = ogt.gen(nR)
	dids = dgt.gen(nR)
	wids = genWeights(nR, CFDS.defaultValues[2])
	ret = [request(oids[i], dids[i], wids[i]) for i in xrange(nR)]
	
	return ret
	
	
def genWorkers(nW):
	wlb, wrb = 0, CFDS.nw-1
	wgt = randomGenerator(wlb, wrb)
	
	oids = wgt.gen(nW)
	cids = genCaps(nW, CFDS.defaultValues[3])
	ret = [worker(oids[i], cids[i]) for i in xrange(nW)]
	
	return ret
	
def dumpLocation(desFileName, nV, locations):
	lines = []
	with open(desFileName, "w") as fout:
		line = "%d\n" % (nV)
		lines.append(line)
		for i in xrange(nV):
			loc = locations[i]
			line = "%s %s\n" % (loc[0], loc[1])
			lines.append(line)
		fout.writelines(lines)
	
	
def dumpData(desFileName, nW, nR, workers, requests, caps, weis):
	lines = []
	with open(desFileName, "w") as fout:
		line = "%s %s\n" % (nW, nR)
		lines.append(line)
		for i in xrange(nW):
			line = "%s %s\n" % (workers[i].oid, caps[i])
			lines.append(line)
		for j in xrange(nR):
			line = "%s %s %s\n" % (requests[j].oid, requests[j].did, weis[j])
			lines.append(line)
		fout.writelines(lines)
		
	
def genSynData(locFilePath, desFilePath, caseId):
	## initialization
	_R, _W, _cr, _cw, _mean, _mu, _sigma, _lambda = CFDS.defaultValues
	maxR = max(CFDS.RList)
	maxW = max(CFDS.WList)
	workers = genWorkers(maxW)
	requests = genRequests(maxR)
	_caps = genCaps(maxW, _cw)
	_weis = genWeights(maxR, _cr)
	
	
	# vary number of requests
	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _mu, "mu"
	for R in CFDS.RList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		dumpData(desFileName, W, R, workers, requests, caps, weis)
		
	# vary number of workers
	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _mu, "mu"
	for W in CFDS.WList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		dumpData(desFileName, W, R, workers, requests, caps, weis)

	# vary maximum weights
	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _mu, "mu"
	for cr in CFDS.crList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		weis = genWeights(R, cr)
		dumpData(desFileName, W, R, workers, requests, caps, weis)


	# vary maximum capacity
	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _mu, "mu"
	for cw in CFDS.cwList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		caps = genCaps(W, cw)
		dumpData(desFileName, W, R, workers, requests, caps, weis)
	
	
	# vary the uniform distributions
	nV = CFDS.nw + CFDS.no + CFDS.nd
	para, flag = _mean, "mean"
	for para in CFDS.meanList:
		tmpFilePath = genLocFileName(para, flag)
		tmpFilePath = os.path.join(locFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)	
		desFileName = "loc_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		gtor = uniformGenerator(para)
		locations = genLocations(nV, gtor)
		dumpLocation(desFileName, nV, locations)
		
	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _mean, "mean"
	for para in CFDS.meanList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		dumpData(desFileName, W, R, workers, requests, caps, weis)		
			
	# vary the normal distributions (\mu)
	nV = CFDS.nw + CFDS.no + CFDS.nd
	para, sigma, flag = _mu, _sigma, "mu"
	for para in CFDS.muList:
		tmpFilePath = genLocFileName(para, flag)
		tmpFilePath = os.path.join(locFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)	
		desFileName = "loc_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		gtor = normalGenerator(para, sigma)
		locations = genLocations(nV, gtor)
		dumpLocation(desFileName, nV, locations)

	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _mu, "mu"
	for para in CFDS.muList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		dumpData(desFileName, W, R, workers, requests, caps, weis)	

		
	# vary the normal distributions (\sigma)
	nV = CFDS.nw + CFDS.no + CFDS.nd
	mu, para, flag = _mu, _sigma, "sig"
	for para in CFDS.sigmaList:
		tmpFilePath = genLocFileName(para, flag)
		tmpFilePath = os.path.join(locFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)	
		desFileName = "loc_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		gtor = normalGenerator(mu, para)
		locations = genLocations(nV, gtor)
		dumpLocation(desFileName, nV, locations)

	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _sigma, "sig"
	for para in CFDS.sigmaList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		dumpData(desFileName, W, R, workers, requests, caps, weis)	

	# vary the exponential distributions (\sigma)
	nV = CFDS.nw + CFDS.no + CFDS.nd
	mu, para, flag = _mu, _lambda, "exp"
	for para in CFDS.lambdaList:
		tmpFilePath = genLocFileName(para, flag)
		tmpFilePath = os.path.join(locFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)	
		desFileName = "loc_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		gtor = expGenerator(para)
		locations = genLocations(nV, gtor)
		dumpLocation(desFileName, nV, locations)

	R, W, caps, weis, cr, cw, para, flag = _R, _W, _caps, _weis, _cr, _cw, _sigma, "exp"
	for para in CFDS.lambdaList:
		tmpFilePath = genDataFileName(R, W, cr, cw, para, flag)
		tmpFilePath = os.path.join(desFilePath, tmpFilePath)
		if not os.path.exists(tmpFilePath):
			os.mkdir(tmpFilePath)
		desFileName = "data_%02d.txt" % (caseId)
		desFileName = os.path.join(tmpFilePath, desFileName) 
		if os.path.exists(desFileName):
			continue
		dumpData(desFileName, W, R, workers, requests, caps, weis)	

		
def genSynDataSet(locFilePath, desFilePath, caseN):
	for caseId in xrange(caseN):
		genSynData(locFilePath, desFilePath, caseId)
	
	
def exp1():
	desFilePath = "./synData"
	if not os.path.exists(desFilePath):
		os.mkdir(desFilePath)
	locFilePath = os.path.join(desFilePath, "location")
	if not os.path.exists(locFilePath):
		os.mkdir(locFilePath)
	# os.remove(locFilePath)
	# os.remove(desFilePath)
	genSynDataSet(locFilePath, desFilePath, CFDS.caseN)
	

if __name__ == "__main__":
	exp1()
