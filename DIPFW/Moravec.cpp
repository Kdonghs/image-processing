#include "stdafx.h"
#include "Moravec.h"
#include <math.h>
#include <iostream>
#include <sstream>

#define DBL_MAX          1.7976931348623158e+308 // max value

using namespace std;

double Moravec::Get_pow(double num)
{
	double result = 0;
	result = num * num;
	return result;
}

void Moravec::Moravec_Algorithm(const BYTE _img[][256], double (&_result)[256][256], 
	int height, int width, int mask_size)
{
	CPoint pointers[4] = { {-1, 0}, {0, -1}, {0, 1}, {1, 0} };	// CPoint(x, y)
	
	double** now_mask = new double*[mask_size];	
		for (int i = 0; i < mask_size; i++)
		now_mask[i] = new double[mask_size];

	double** s = new double*[mask_size];	// 제곱차 합 연산 결과 저장할 배열.
	for (int i = 0; i < mask_size; i++)
		s[i] = new double[mask_size];

	double** _diff = new double*[mask_size];	// 제곱차 합 결과 저장할 배열.
	for (int i = 0; i < mask_size; i++)
		_diff[i] = new double[mask_size];
	
	for (int y = 2; y < height - 2; y++)
	{
		for (int x = 2; x < width - 2; x++)
		{			

			for (int i = 0; i < mask_size; i++)
			{
				for (int j = 0; j < mask_size; j++)	// 3x3 현재 위치의 마스크 떼기.
				{
					now_mask[i][j] = _img[y - (int)(mask_size / 2)][x - (int)(mask_size / 2)];
				}
			}

			for (CPoint p : pointers)
			{
				double** now_neighbor = new double*[mask_size];
				for (int i = 0; i < mask_size; i++)
					now_neighbor[i] = new double[mask_size];

				for (int i = 0; i < mask_size; i++)
				{
					for (int j = 0; j < mask_size; j++)	// 현재 (v, u) 위치의 마스크 떼기.
					{
						now_neighbor[i][j]
							= _img[(y + p.y) - (int)(mask_size / 2)][(x + p.x) - (int)(mask_size / 2)];
					}
				}

				for (int i = 0; i < mask_size; i++)
				{
					for (int j = 0; j < mask_size; j++)		// 제곱차 계산.
					{
						_diff[i][j] = pow(now_mask[i][j] - now_neighbor[i][j], 2);
					}
				}

				double result_sum = 0;
				for (int i = 0; i < mask_size; i++)
				{
					for (int j = 0; j < mask_size; j++)		// 제곱차들의 합 계산.
					{
						result_sum += _diff[i][j];
					}
				}
				
				s[p.y + 1][p.x + 1] = result_sum;
			}

			double min = DBL_MAX;
			CPoint min_position;

			for (int u = 0; u < 3; u++)
			{
				for (int v = 0; v < 3; v++)
				{
					if ((u == 0 && v == 1) || (u == 1 && v == 0) || (u == 1 && v == 2) || (u == 2 && v == 1))
					{
						if (s[u][v] < min)
						{
							min = s[u][v];
							min_position.y = u;	min_position.x = v;
						}
					}
				}
			}
			double result = s[min_position.y][min_position.x];
			_result[y][x] = result;
		}
	}
	
	// 동적메모리 할당 해제.
	for (int i = 0; i < mask_size; ++i)
		delete[] now_mask[i];
	delete[] now_mask;

	for (int i = 0; i < mask_size; ++i)
		delete[] s[i];
	delete[] s;

	for (int i = 0; i < mask_size; ++i)
		delete[]_diff[i];
	delete[]_diff;	
}

vector<Moravec_Data> Moravec::Select_N(const BYTE(&_img)[256][256], const double(&_result)[256][256]
	,int height, int width)
{
	vector<Moravec_Data> vector;
	for (int y = 2; y < height-2; y++)
	{
		for (int x = 2; x < width - 2; x++)
		{
			Moravec_Data value{ _result[y][x], CPoint(x, y) };
			vector.push_back(value);
		}
	}

	sort(vector.begin(), vector.end(), Moravec_Data::Compare);

	std::vector<Moravec_Data> selected;
	Moravec_Data value;		int count = 0;

	cout << vector.size() << endl;
	for (auto data : vector)
	{
		count == 0;
		
		if (data.GetValue() > 42000)
		{
			CPoint now = data.Get_Position();
			
			selected.push_back(data);
		}
	}

	return selected;
}