/**
	\author:	Trasier
	\date:		2019.05.26
*/
#include <bits/stdc++.h>
#include "utils.h"
#include "global.h"
#include "HST.h"
using namespace std;

int nT = 0;
pair<int,int>* depRng = NULL;
path_t* paths = NULL;
int* labels = NULL;
int* mark = NULL;
node_t* nodes = NULL;
vector<vector<int> > tree;

void initTree() {
	int tid;
	
	for (int i=0; i<nT; ++i) {
		tree.push_back(vector<int>());
	}
	
	for (int j=0; j<=H; ++j) {
		for (int i=0; i<nV; ++i) {
			tid = far[i][j];
			tree[tid].push_back(i);
		}
	}
}

void updateTree(vector<int>& rids) {
	for (int i=0; i<rids.size(); ++i) {
		int rid = rids[i], u = requests[rid].oid, v = requests[rid].did;
		pair<int,int> ptmp = getLCA(u, v);
		int nid = ptmp.first;
		node_t& node = nodes[nid];
		node.pop_req(rid);
		
		for (int j=0; j<=H; ++j) {
			if (far[u][j] == far[v][j]) {
				nid = far[u][j];
				--nodes[nid].cnt;
				--nodes[nid].flow;
			} else {
				nid = far[u][j];
				--nodes[nid].flow;
				nid = far[v][j];
				--nodes[nid].flow;
			}
		}
	}
}

void dfsLabel(int rt, int vid, int l, int& cnt, int maxH) {
	if (rt == -1) return ;
	
	mark[rt] = 1;
	
	if (l > 0) {// label children
		vector<int>& vi = tree[rt];
		int _vid, _rt, _l = l-1;
		
		for (int i=0; i<vi.size(); ++i) {
			_vid = vi[i];
			_rt = far[_vid][_l];
			if (!mark[_rt]) {
				dfsLabel(_rt, _vid, _l, cnt, maxH);
			}
		}
	}
	
	// label itself
	labels[rt] = cnt++;
	
	if (l < maxH) {// label farther
		int _vid = vid, _l = l+1, _rt = far[_vid][_l];
		if (!mark[_rt]) {
			dfsLabel(_rt, _vid, _l, cnt, maxH);
		}
	}
}

void genLabel(worker_t& w, int maxH) {
	int ow = w.oid, nid = far[ow][0], cnt = 1;
	
	memset(labels, 0, sizeof(int)*nT);
	memset(mark, 0, sizeof(int)*nT);
	
	dfsLabel(nid, ow, 0, cnt, maxH);
}

int getMinHeight(int vid, int k) {
	int nid;
	
	for (int j=0; j<H; ++j) {
		nid = far[vid][j];
		if (nodes[nid].cnt >= k) {
			return j;
		}
	}
	
	return H;
}

int getMaxPathLength(vector<int>& rids) {
	int n = rids.size(), rid;
	int ret = 0;
	
	for (int i=0; i<n; ++i) {
		rid = rids[i];
		ret = max(ret, paths[rid].size());
	}
	
	return ret;
}

void genVec(int rid) {
	request_t& r = requests[rid];
	int u = r.oid, v = r.did, rt;
	path_t& path = paths[rid];
	
	int n = getPathHeight(u, v), k = 0;
	path.initVec(n<<1);
	
	for (int i=n-1; i>=0; --i) {
		rt = far[u][i], path.vec[k] = labels[rt];
		rt = far[v][i], path.vec[k+1] = labels[rt];
		k += 2;
	}
}

void genVec(vector<int>& rids) {
	for (int i=0; i<rids.size(); ++i) {
		genVec(rids[i]);
	}
}

void clearPath(int rid) {
	paths[rid].clear();
}

void clearPath(vector<int>& rids) {
	for (int i=0; i<rids.size(); ++i) {
		clearPath(rids[i]);
	}
}

void initWorkers() {
	for (int i=0; i<nW; ++i) {
		workers[i].S.push_back(workers[i].oid);
	}
}

void initNodes() {
	int u, v, nid;
	
	for (int i=0; i<nT; ++i) {
		nodes[i].cnt = nodes[i].flow = nodes[i].dep = 0;
	}
	for (int i=0; i<nV; ++i) {
		for (int j=0; j<=H; ++j) {
			nid = far[i][j];
			nodes[nid].dep = j;
		}
	}
	
	for (int i=0; i<nR; ++i) {
		u = requests[i].oid, v = requests[i].did;
		for (int j=0; j<=H; ++j) {
			if (far[u][j] == far[v][j]) {
				nid = far[u][j];
				++nodes[nid].cnt;
				++nodes[nid].flow;
			} else {
				nid = far[u][j];
				++nodes[nid].flow;
				nid = far[v][j];
				++nodes[nid].flow;
			}
		}
	}	
}

void initGlobalMemory(int nV, int nW, int nR) {
	// (0)
	initMemory(nV);
	// (1)
	requests = new request_t[nR];
	// (2)
	workers = new worker_t[nW];
	// (3)
	paths = new path_t[nR];
	for (int i=0; i<nR; ++i) {
		paths[i].rid = i;
	}
	// (7)
	depRng = new pair<int,int>[MAX_HEIGHT];
}

void freeGlobalMemory() {
	// (0)
	freeMemory();
	// (1)
	delete[] requests;
	// (2)
	delete[] workers;
	// (3) 
	delete[] paths;
	// (4)
	delete[] labels;
	// (5)
	delete[] mark;
	// (6)
	delete[] nodes;
	// (7)
	delete[] depRng;
}

void readFromFiles(string locFileName, string hstFileName, string inputFileName) {
	ifstream locFile(locFileName.c_str(), ios::in);
	ifstream inputFile(inputFileName.c_str(), ios::in); 

	if (!locFile.is_open()) {
		fprintf(stderr, "FILE %s IS INVALID.", locFileName.c_str());
		exit(1);
	}
	
	if (!inputFile.is_open()) {
		fprintf(stderr, "FILE %s IS INVALID.", inputFileName.c_str());
		exit(1);
	}
	
	locFile >> nV;
	inputFile >> nW >> nR;
	initGlobalMemory(nV, nW, nR);
	
	for(int i=0; i<nV; i++) {
		locFile >> V[i].x >> V[i].y;
	}
	for (int i=0; i<nW; ++i) {
		inputFile >> workers[i].oid >> workers[i].cap; 
	}
	for (int j=0; j<nR; ++j) {
		inputFile >> requests[j].oid >> requests[j].did >> requests[j].wei;
	}	
	inputFile.close();
	locFile.close();

	// (4)
	readHST(hstFileName, true);
	nT = calcTreeSize();
	labels = new int[nT];
	memset(labels, 0, sizeof(int)*nT);
	// (5)
	mark = new int[nT];
	memset(mark, 0, sizeof(int)*nT);
	// (6)
	nodes = new node_t[nT];

	initWorkers();
	initNodes();
	initTree();
	initRng();
}

void dumpResult(string& fileName) {
	dumpResult(fileName, "FESI");
}

void dumpResult(string& fileName, const char *algo) {
	double sumflow=0, maxflow=0, maxTime=0, sumTime=0;
	
	for(int j=0; j<nW; j++){
		double sf = 0, mf = 0, st = 0, mt = 0, tmp;
		vector<int> &S = workers[j].S;
		int _pid = workers[j].oid, pid;
		
		for(int i=1; i<S.size(); ++i){
			pid = (S[i]&1) ? requests[S[i]>>1].did : requests[S[i]>>1].oid;
			tmp = localDist(_pid, pid);
			mt += tmp;
			if (S[i] & 1) {	
				sf += mt;
				mf = max(mf, mt);
			}
			_pid = pid;
		}	
		st = mt;
		maxflow = max(mf, maxflow);	
		sumflow += sf;
		maxTime = max(mt, maxTime);
		sumTime += st;
	}

	sumflow /= speed;
	maxflow /= speed;
	maxTime /= speed;
	sumTime /= speed;
	
	if (!fileName.empty()) {
		FILE* fout = fopen(fileName.c_str(), "w");
		
		if (fout == NULL) {
			fprintf(stderr, "Fail to open %s\n", fileName.c_str());
			exit(1);
		}
		
		fprintf(fout, "%s %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", 
					algo, sumflow, maxflow, sumTime, maxTime, usedTime, usedMemory/1024.0);
		
		fclose(fout);
	} else {
		printf("%s %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", 
					algo, sumflow, maxflow, sumTime, maxTime, usedTime, usedMemory/1024.0);
		
		fflush(stdout);
	}
}

int getPathLength(int u, int v) {
	return getPathHeight(u, v) << 1;
}

int getPathHeight(int u, int v) {
	int ret = 0;
	
	for (int i=0; i<=H; ++i) {
		if(far[u][i] == far[v][i]) {
			return ret;
		}
		++ret;
	}
	
	return -1;
}

static void append(worker_t& w, int rid, int& oPos, int& dPos, double& inc) {
	oPos = dPos = w.S.size() - 1;
	int sid = *w.S.rbegin(), uid;
	
	if (w.S.size() == 1)
		uid = sid;
	else
		uid = (sid&1) ? requests[sid>>1].did : requests[sid>>1].oid;
	inc = localDist(uid, requests[rid].oid) + localDist(requests[rid].oid, requests[rid].did); 
}

void insertDist(worker_t& w, int rid, int& oPos, int& dPos, double& inc) {
	if (w.S.size() <= 1) {
		append(w, rid, oPos, dPos, inc);
		return ;
	}
	
	request_t* R = requests;
	request_t& r = R[rid];
	vector<int>& S = w.S;
	vector<double> picked;
	
	picked.push_back(0.0);
	for (int i=1; i<S.size(); ++i) { 
		double carry = *picked.rbegin();
		if (S[i] & 1) {
			carry -= R[S[i]>>1].wei;
		} else {
			carry += R[S[i]>>1].wei;
		}
        picked.push_back(carry);
    }
	
	double tmp;
	inc = INF, oPos = dPos = -1;
	
	for (int i=0; i<S.size(); ++i) { 
		if (picked[i]+r.wei > w.cap)
			continue;
		
		if (i == S.size()-1) {
			int uid = R[S[i]>>1].did;
			tmp = localDist(uid, r.oid) + localDist(r.oid, r.did);
		} else if (i == 0) {
			int uid = S[i];
			int vid = (S[i+1]&1) ? R[S[i+1]>>1].did : R[S[i+1]>>1].oid;
			tmp = localDist(uid, r.oid) + localDist(r.oid, r.did) + localDist(r.did, vid) - localDist(uid, vid);
		} else {
			int uid = (S[i]&1) ? R[S[i]>>1].did : R[S[i]>>1].oid;
			int vid = (S[i+1]&1) ? R[S[i+1]>>1].did : R[S[i+1]>>1].oid;
			tmp = localDist(uid, r.oid) + localDist(r.oid, r.did) + localDist(r.did, vid) - localDist(uid, vid);
		}
		
		if (tmp < inc) {
			inc = tmp, oPos = i, dPos = i;
		}
	}
	
	vector<double> Dio;
	vector<int> Plc;

	for (int j=0; j<S.size(); ++j) {
		if (picked[j]+r.wei > w.cap) {
			Dio.push_back(INF);
			Plc.push_back(-1);
			continue;
		}
		
		if (j == 0) {
			int uid = S[j];
			int vid = (S[j+1]&1) ? R[S[j+1]>>1].did : R[S[j+1]>>1].oid;
			double det = localDist(uid, r.oid) + localDist(r.oid, vid) - localDist(uid, vid);
			Dio.push_back(det);
			Plc.push_back(j);
			continue;
		}
		
		double det, det_;
		
		
		if (j == S.size()-1) {
			int uid = (S[j]&1) ? R[S[j]>>1].did : R[S[j]>>1].oid;
			det = localDist(uid, r.did);
			det_ = localDist(uid, r.oid);
		} else {
			int uid = (S[j]&1) ? R[S[j]>>1].did : R[S[j]>>1].oid;
			int vid = (S[j+1]&1) ? R[S[j+1]>>1].did : R[S[j+1]>>1].oid;
			det = localDist(uid, r.did) + localDist(r.did, vid) - localDist(uid, vid);
			det_ = localDist(uid, r.oid) + localDist(r.oid, vid) - localDist(uid, vid);
		}
		
		double tmp = det + *Dio.rbegin();
		
		if (tmp < inc) {
			inc = tmp, oPos = *Plc.rbegin(), dPos = j;
		}
		
		if (det_ < *Dio.rbegin()) {
			Dio.push_back(det_);
			Plc.push_back(j);
		} else {
			Dio.push_back(*Dio.rbegin());
			Plc.push_back(*Plc.rbegin());
		}
	}
}

void insert(worker_t& w, int rid, int& oPos, int& dPos) {
	vector<int> S = w.S;
	w.S.clear();
	
	for (int i=0; i<S.size(); ++i) {
		w.S.push_back(S[i]);
		if (i == oPos) {
			w.S.push_back(rid<<1);
		}
		if (i == dPos) {
			w.S.push_back(rid<<1|1);
		}
	}
}

void remove(worker_t& w, int rid) {
	vector<int> S = w.S;
	w.S.clear();
	
	for (int i=0; i<S.size(); ++i) {
		if (i>0 && (S[i]>>1)==rid) {
			continue;
		}
		w.S.push_back(S[i]);
	}
}

double getRouteDist(int wid) {
	worker_t& w = workers[wid];
	double ret = 0;
	vector<int>& S = w.S;
	int _pid = workers[wid].oid, pid;
	
	for(int i=1; i<S.size(); ++i){
		pid = (S[i]&1) ? requests[S[i]>>1].did : requests[S[i]>>1].oid;
		ret += localDist(_pid, pid);
		_pid = pid;
	}	
	
	return ret;
}

double getRouteDist(worker_t& w) {
	double ret = 0;
	vector<int>& S = w.S;
	int _pid = w.oid, pid;
	
	for(int i=1; i<S.size(); ++i){
		pid = (S[i]&1) ? requests[S[i]>>1].did : requests[S[i]>>1].oid;
		ret += localDist(_pid, pid);
		_pid = pid;
	}	
	
	return ret;
}

int calcTreeSize() {
	int ret = 0;
	
	for (int i=0; i<nV; ++i) {
		ret = max(ret, far[i][0]);
	}
	
	return ret+1;
}

void initRng() {
	for (int i=0; i<=H; ++i) {
		depRng[i].first = nT + 5;
		depRng[i].second = -1;
	}
	
	for (int i=0; i<nT; ++i) {
		int k = nodes[i].dep;
		depRng[k].first = min(depRng[k].first, i);
		depRng[k].second = max(depRng[k].second, i);
	}
}

void sortPath(vector<int>& rids) {
	const int n = rids.size();
	vector<path_t> par(n, path_t());
	int rid;
	
	for (int i=0; i<n; ++i) {
		rid = par[i].rid = rids[i];
		par[i].vec = paths[rid].vec;
	}
	
	sort(par.begin(), par.end());
	
	for (int i=0; i<rids.size(); ++i) {
		rids[i] = par[i].rid;
		par[i].clear();
	}
}

double getRemoveDist(worker_t& w, int rid) {
	request_t& r = requests[rid];
	vector<int>& S = w.S;
	int n = S.size(), oPos = -1, dPos = -1;
	int ox, oy, dx, dy;
	double ret;
	
	for (int i=1; i<n; ++i) {
		if (S[i] == (rid<<1))
			oPos = i;
		else if (S[i] == (rid<<1|1))
			dPos = i;
	}
	
	if (oPos+1 == dPos) {
		// left point of origin
		if (oPos == 1) {
			ox = S[0];
		} else {
			ox = (S[oPos-1]&1) ? requests[S[oPos-1]>>1].did : requests[S[oPos-1]>>1].oid;
		}
		// right point of destination
		if (dPos == n-1) {
			dy = ox;
		} else {
			dy = (S[dPos+1]&1) ? requests[S[dPos+1]>>1].did : requests[S[dPos+1]>>1].oid;
		}
		if (dPos == n-1) {
			ret = localDist(ox, r.oid) + localDist(r.oid, r.did);
		} else {
			ret = localDist(ox, r.oid) + localDist(r.oid, r.did) + localDist(r.did, dy) - localDist(ox, dy);
		}
	} else {
		// ending points of origin
		if (oPos == 1) {
			ox = S[0];
		} else {
			ox = (S[oPos-1]&1) ? requests[S[oPos-1]>>1].did : requests[S[oPos-1]>>1].oid;
		}	
		oy = (S[oPos+1]&1) ? requests[S[oPos+1]>>1].did : requests[S[oPos+1]>>1].oid;
		// ending points of destination
		dx = (S[dPos-1]&1) ? requests[S[dPos-1]>>1].did : requests[S[dPos-1]>>1].oid;
	 	if (dPos == n-1) {
			dy = dx;
		} else {
			dy = (S[dPos+1]&1) ? requests[S[dPos+1]>>1].did : requests[S[dPos+1]>>1].oid;
		}	
		ret = localDist(ox, r.oid) + localDist(r.oid, oy) - localDist(ox, oy)
				+ localDist(dx, r.did) + localDist(r.did, dy) - localDist(dx, dy);
	}
	
	return ret;
}
