#pragma once
#include "FunctionData.h"
#include <vector>
#include <iostream>

using namespace std;

class Img1_Data
{
private:
	vector<Moravec_Data> keypoint1;
	vector<vector<double>> hist1;

public:
	void Set_keypoint1(const vector<Moravec_Data> &vector);
	vector<Moravec_Data> Get_keypoint1()
	{
		return keypoint1;
	}

	Moravec_Data GetPosition1(int idx)
	{
		if (0 > idx || idx > keypoint1.size())
			std::cout << "肋给等 Idx 立辟" << std::endl;
		return keypoint1[idx];
	}

	vector<vector<double>> Get_hist1() { return hist1; }
};

class Img2_Data
{
private:
	vector<Moravec_Data> keypoint2;
	vector<vector<double>> hist2;

public:
	void Set_keypoint2(const vector<Moravec_Data> &vector);
	vector<Moravec_Data> Get_keypoint2()
	{
		return keypoint2;
	}

	Moravec_Data GetPosition2(int idx)
	{
		if (0 > idx || idx > keypoint2.size())
			std::cout << "肋给等 Idx 立辟" << std::endl;
		return keypoint2[idx];
	}

	vector<vector<double>> Get_hist2() { return hist2; }
};