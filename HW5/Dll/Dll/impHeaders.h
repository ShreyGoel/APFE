#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

__declspec(dllexport) int arbitrage(double *prices, double n, double K, double r, int N, double *x);

__declspec(dllexport) struct vars
{
	int N;
	int n;
	int K;
	double *obj; //K + 1 by n matrix
	double *x;
	double r;

};

__declspec(dllexport) vars read_file(string file_name);

