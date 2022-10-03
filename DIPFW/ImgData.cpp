#include "stdafx.h"
#include "ImgData.h"



void Img1_Data::Set_keypoint1(const vector<Moravec_Data> &vector)
{
	this->keypoint1.assign(vector.begin(), vector.end());
}

void Img2_Data::Set_keypoint2(const vector<Moravec_Data> &vector)
{
	this->keypoint2.assign(vector.begin(), vector.end());
}