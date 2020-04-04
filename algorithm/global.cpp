/**
	\author:	Trasier
	\date:		2019/05/22
*/
#include "global.h"

int nR = 0;
int nW = 0;
request_t* requests = NULL;
worker_t* workers = NULL;
const double speed = 1.0;
const double EPS = 1e-5;
const double INF = 1e20;
double usedTime = 0.0;
int usedMemory = 0;

int dcmp(double x) {
	if (fabs(x) < EPS)
		return 0;
	return x>0 ? 1:-1;
}

double dist(location_t *V, int x, int y) { 
	location_t& a = V[x];
	location_t& b = V[y];
	return sqrt(1.0*(a.x - b.x)*(a.x - b.x) + 1.0*(a.y - b.y)*(a.y - b.y));
} 

double dist(location_t &a, location_t &b) {
	return sqrt(1.0*(a.x - b.x)*(a.x - b.x) + 1.0*(a.y - b.y)*(a.y - b.y));
}

void freeInput() {
	delete[] requests;
	delete[] workers;
}
