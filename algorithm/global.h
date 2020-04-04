/**
	\author:	Trasier
	\date:		2019/05/22
*/
#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
using namespace std;

// typedef unorderd_map<int,int> hii;
typedef long long LL;
typedef pair<int,int> pii;
typedef pair<double,double> pdd;
typedef pair<int,double> pid;
typedef pair<double,int> pdi;

typedef struct {
    double x, y;
} location_t;

typedef struct {
    int oid, did; 	// origin and destination
    double wei; 		// weight
} request_t;

typedef struct {
    int oid;		// initial location
    double cap;		// capacity
	vector<int> S;
} worker_t;


extern int nR;
extern int nW;
extern request_t* requests;
extern worker_t* workers;
extern const double speed;
extern const double EPS;
extern const double INF;
extern double usedTime;
extern int usedMemory;

int dcmp(double x);
double dist(location_t *V, int x, int y);
double dist(location_t& a, location_t& b);
void freeInput();

#endif
