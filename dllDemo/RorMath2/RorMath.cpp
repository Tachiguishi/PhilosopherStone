#include <limits.h>
#include <algorithm>
#include "..\include\rorMath\rorMath2.h"
// RorMath.cpp : Defines the exported functions for the DLL application.
//

namespace RorMath2 {

const double PI = 3.147;

double Add(double a, double b)
{
	return a + b;
}

double cAdd(double a, double b)
{
	return a + b;
}

Aggregate::Aggregate() :
	sum(0),
	avg(0),
	max(INT_MIN),
	min(INT_MAX),
	count(0)
{
}

Aggregate::~Aggregate()
{
}

void Aggregate::AddItem(int item)
{
	count++;
	sum += item;
	avg = sum / count;
	max = std::max(max, item);
	min = std::min(min, item);
}

int Aggregate::Sum()
{
	return sum;
}

int Aggregate::Avg()
{
	return avg;
}

int Aggregate::Max()
{
	return max;
}

int Aggregate::Min()
{
	return min;
}

}
