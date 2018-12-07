// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "testingHeader.h"
#include "impHeaders.h"


int main()
{
	string file_name = "arb.dat";
	vars var = read_file(file_name);
	int retcode = arbitrage(var.obj, var.n, var.K, var.r, var.N, var.x);

	BACK:

	return retcode;

}

vars read_file(string file_name)
{
	vars var;
	ifstream fin(file_name);
	string num;
	vector<string> vec;


	while (fin >> num)
	{
		if (num != " ") {
			vec.push_back(num);
		}
	}

	var.n = stoi(vec[1]);
	var.K = stoi(vec[3]);
	var.r = stod(vec[5]);
	var.N = stoi(vec[7]);
	var.obj = (double *)calloc(var.n*(var.K + 1), sizeof(double));
	var.x = (double *)calloc(var.n, sizeof(double));

	for (int i = 0; i <= var.K; i++)
	{
		for (int j = 0; j < var.n; j++)
		{
			var.obj[j + i * var.n] = stod(vec[9 + j + i * (var.n + 1)]);
		}
	}

	return var;
}