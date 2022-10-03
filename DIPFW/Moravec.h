#pragma once
#include <vector>
#include "ImgData.h"
#include "FunctionData.h"
#include <algorithm>

typedef unsigned char       BYTE;
using namespace std;

class Moravec
{

private:
	static double Get_pow(double num);

public:
	static void Moravec_Algorithm(const BYTE _img[][256], double (&_result)[256][256], int height, int width, int mask_size );
	static vector<Moravec_Data> Select_N(const BYTE(&_img)[256][256], const double(&_result)[256][256], int height, int width);
	
};