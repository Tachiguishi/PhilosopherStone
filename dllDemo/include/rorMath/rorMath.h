#ifndef __rorMath_H__
#define __rorMath_H__

#pragma once

#ifdef RORMATH_EXPORTS
#define RORMATH_DLL_API __declspec(dllexport)
#else
#define RORMATH_DLL_API __declspec(dllimport)
#endif

namespace RorMath1 {

extern const double RORMATH_DLL_API PI;

double RORMATH_DLL_API Add(double a, double b);

class RORMATH_DLL_API Aggregate {
public:
	Aggregate();
	~Aggregate();
public:
	void AddItem(int item);
	int Sum();
	int Avg();
	int Max();
	int Min();
private:
	int sum;
	int avg;
	int max;
	int min;
	int count;
};

}

#endif // !__rorMath_H__
