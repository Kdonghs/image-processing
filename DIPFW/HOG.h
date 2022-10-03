#pragma once
#include <atltypes.h>
#include "ImgData.h"
#include <vector>

using namespace std;

class HOG_Data
{
public:
	vector<double> HOG;
	CPoint pt;
	HOG_Data(vector<double> HOG, CPoint pt)
		: HOG(std::move(HOG)), pt(pt)
	{

	}
};

class HOG
{
private:
	void Sobel(const BYTE _img[][256], double (&img_x)[256][256], double(&img_y)[256][256], int height, int width);
	void Get_edgeinform(const BYTE _img[][256], double(&img_x)[256][256], double(&img_y)[256][256],
		double(&magnitude)[256][256], double(&angle)[256][256], int height, int width);
	vector<double> Stack_Histogram(double(&magnitude)[256][256], double(&angle)[256][256]);
public:
	void HOG_Algorithm(/*const BYTE _img[][256], const vector<Moravec_Data> &keypoint,*/
		int height, int width);
};