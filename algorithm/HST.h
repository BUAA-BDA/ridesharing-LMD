/**
	\Author: Trasier
	\Date:	2019/05/21
**/

#ifndef HST_H
#define HST_H

#include <bits/stdc++.h>
#include "global.h"
using namespace std;

extern const int MAX_SAMPLE;
extern const int MAX_HEIGHT;
extern const int MAX_NODE;
extern int nV;
extern int H;
extern location_t* V;
extern double* exp2s;
extern double* sum2s;
extern int* pi;
extern int* reverse_pi;
extern int** far;
extern double dmax;
extern double gama;

void initLocation(string &fileName);
void initMemory(int n);
void freeMemory();
void printHST(int H);
void constructHST(bool load=false);
void constructHST_fast(bool load);
void constructHST_slow(bool load);
void randomization();
void calcDmax();
double distOnHST(int u, int v);
void readHST(string &fileName, bool flag=true);
void dumpHST(string &fileName, bool flag=true);

void loadHST();
void backupHST();
pair<int,int> getLCA(int u, int v);
int levelOfLCA(int u, int v);
double distAtLevel(int level);
double calcScore(request_t* R, int nR);

#endif