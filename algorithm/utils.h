/**
	\author:	Trasier
	\date:		2019.05.26
*/
#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include "global.h"
#include "HST.h"
using namespace std;

// #define HashMap unordered_map
#define HashMap map

struct path_t {
	int rid;
	vector<int> vec;
	
	void initVec(int n) {
		vec.resize(n);
	}
	
	int size() {
		return vec.size();
	}

	void clear() {
		vec.clear();
	}
	
	bool empty() {
		return vec.empty();
	}
	
	bool operator< (const path_t& b) const {
		const vector<int>& avec = this->vec;
		const vector<int>& bvec = b.vec;
		int i = 0, j = 0;
		const int size = avec.size(), bsize = bvec.size();
		
		while (i<size && j<bsize) {
			if (avec[i] == bvec[j]) {
				++i, ++j;
			} else {
				return avec[i] < bvec[j];
			}
		}
		if (i==size || j==bsize)
			return size < bsize;
		
		return rid < b.rid;
	}
};

struct node_t {
	int cnt, flow, dep;
	vector<int> req;
	
	int pop_req(int x) {
		int k = 0, n = req.size();
		
		for (int i=0; i<n; ++i) {
			if (req[i] != x)
				req[k++] = req[i];
		}
		
		if (k < n) {
			req.pop_back();
			return 1;
		} else {
			return 0;
		}
	}
	
	int pop_req(const vector<int>& xs) {
		if (xs.empty()) return 0;
		
		int k = 0, i = 0, j = 0;
		int n = req.size(), m = xs.size();
		
		while (i<n && j<m) {
			if (req[i] == xs[j]) {
				++i, ++j;
			} else if (req[i] < xs[j]) {
				req[k++] = req[i++];
			} else {
				++j;
			}
		}
		while (i < n)
			req[k++] = req[i++];
		
		if (k < n) {
			req.erase(req.begin()+k, req.end());
			return n-k;
		} else {
			return 0;
		}
	}
};

extern int nT;
extern pair<int,int>* depRng;
extern path_t* paths;
extern int* labels;
extern node_t* nodes;
extern int* mark;
extern vector<vector<int> > tree;

// I/O
void readFromFiles(string locFileName, string hstFileName, string inputFileName);
void dumpResult(string& fileName);
void dumpResult(string& fileName, const char *algo);

// Melloc & Free
void initGlobalMemory(int nV, int nW, int nR);
void freeGlobalMemory();


// Initilization
void initTree();
void initNodes();
void initWorkers();
void initRng();


// The import operations on Metric Space
inline double localDist(int a, int b) {return dist(V, a, b);}
void insertDist(worker_t& w, int rid, int& oPos, int& dPos, double& inc);
void insert(worker_t& w, int rid, int& oPos, int& dPos);
void remove(worker_t& w, int rid);
double getRouteDist(worker_t& w);
double getRouteDist(int wid);
double getRemoveDist(worker_t& w, int rid);

// The import operations on HST
void sortPath(vector<int>& rids);
void genVec(int rid);
void genVec(vector<int>& rids);
void genLabel(worker_t& w, int maxH);
void dfsLabel(int rt, int vid, int l, int& cnt, int maxH);
int getMinHeight(int vid, int k);
void updateTree(vector<int>& rids);

// Other un-important operations on HST
void clearPath(vector<int>& rids);
void clearPath(int rid);
int getPathHeight(int u, int v);
int getPathLength(int u, int v);
int getMaxPathLength(vector<int>& rids);
int calcTreeSize();

#endif
