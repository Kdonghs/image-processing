#pragma once
#include <atltypes.h>
#include <vector>

using namespace std;

class Moravec_Data
{
private:
	double value;
	CPoint position;

public:
	Moravec_Data()	{}
	Moravec_Data(double value, CPoint position)
	{
		this->value = value;
		this->position = position;
	}

	const CPoint Get_Position()
	{
		return position;
	}

	CPoint Get_Position(vector<Moravec_Data> vector, int idx)
	{
		return vector.at(idx).position;
	}

	static bool Compare(const Moravec_Data &v1, const Moravec_Data &v2)
	{
		return v1.value > v2.value;
	}

	double GetValue()
	{
		return value;
	}
};