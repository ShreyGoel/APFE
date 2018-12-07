#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct vars
{
	int N;
	int n;
	int K;
	double *obj; //K + 1 by n matrix
	double *x;
	double r;

};

vars read_file(string file_name);
