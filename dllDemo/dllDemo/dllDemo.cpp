// dllDemo.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "rorMath/rorMath.h"
#pragma comment(lib, "RorMath.lib")

int main()
{
	std::cout << RorMath1::PI << std::endl;
	double c = RorMath1::Add(4.5, 7.2);
	std::cout << c << std::endl;
	RorMath1::Aggregate agg;
	for (int i = 0; i < 50; i++) {
		agg.AddItem(i);
	}
	std::cout << agg.Max() << " " << agg.Sum() << " " << agg.Avg() << std::endl;

	// dumpbin -exports RorMath2.dll
	HMODULE hDLL = LoadLibrary(TEXT("RorMath2.dll"));
	if (!hDLL) { std::cout << "load Rormath2.dll failed." << std::endl; return 0; }

	typedef const double* pPI;
	pPI pi = (pPI)GetProcAddress(hDLL, "?PI@RorMath2@@3NB");
	if (!pi) std::cout << "load PI failed" << std::endl;
	else std::cout << *pi << std::endl;

	typedef double(*pAdd)(double a, double b);
	pAdd fp1 = pAdd(GetProcAddress(hDLL, "?Add@RorMath2@@YANNN@Z"));
	if (!fp1) std::cout << "load Add failed" << std::endl;
	else {
		double d = fp1(4.5, 7.2);
		std::cout << d << std::endl;
	}

	pAdd fp2 = pAdd(GetProcAddress(hDLL, "cAdd"));
	if (!fp2) std::cout << "load cAdd failed" << std::endl;
	else {
		double d2 = fp2(4.5, 7.2);
		std::cout << d2 << std::endl;
	}

	FreeLibrary(hDLL);
	system("pause");
    return 0;
}

