#include "stdafx.h"
#include "HOG.h"

void HOG::Sobel(const BYTE _img[][256], double (&img_x)[256][256], double(&img_y)[256][256], int height, int width)
{
	int maskSobelX[3][3] = { {-1,0,1},
							{-2,0,2},
							{-1,0,1} };
	int maskSobelY[3][3] = { {-1,-2,-1},
							{0,0,0},
							{1,2,1} };

	double** result = new double*[height];
	for (int i = 0; i < height; i++)
		result[i] = new double[width];


	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			int sum_X = 0;	int sum_Y = 0;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					sum_X += (_img[y + (i - 1)][x + (j - 1)] * maskSobelX[i][j]);
					sum_Y += (_img[y + (i - 1)][x + (j - 1)] * maskSobelY[i][j]);
				}
			}

			if (sum_X < 0)
				sum_X = abs(sum_X);
			if (sum_X > 255)
				sum_X = 255;

			if (sum_Y < 0)
				sum_Y = abs(sum_Y);
			if (sum_Y > 255)
				sum_Y = 255;

			img_x[y][x] = sum_X;
			img_y[y][x] = sum_Y;

			int sum = sum_X + sum_Y;
			if (sum > 255)
				sum = 255;

			result[y][x] = sum;
		}
	}

	for (int i = 0; i < height; ++i)
		delete[] result[i];
	delete[] result;
}

void HOG::Get_edgeinform(const BYTE _img[][256], double(&img_x)[256][256], double(&img_y)[256][256],
	double(&magnitude)[256][256], double(&angle)[256][256], int height, int width)
{
	
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++)
		{
			double d_x = img_x[y][x];
			double d_y = img_y[y][x];

			double _magnitude = sqrt(pow(d_x, 2) + pow(d_y, 2));
			double _angle = atan(d_y / d_x) + 90;

			double _int;
		}
	}

}

void HOG::HOG_Algorithm(/*const BYTE _img[][256], const vector<Moravec_Data> &keypoint,*/
	int height, int width)
{
	int a = 0;
	//Moravec_Data value;

	/*double img_gx[256][256];
	double img_gy[256][256];

	Sobel(_img, img_gx, img_gy, height, width);

	double magnitude[256][256];
	double angle[256][256];

	Get_edgeinform(_img, img_gx, img_gy, magnitude, angle, height, width);*/

}
