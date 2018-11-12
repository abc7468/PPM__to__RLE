#pragma once
#include<vector>
#include<iostream>
using namespace std;
class PPMImage
{
public:
	char M, N; // 매직넘버
	int width;
	int height;
	int max;
	int length;
	unsigned char **pixels;
	unsigned char *pixelsR;
	unsigned char *pixelsG;
	unsigned char *pixelsB;





	PPMImage();
	~PPMImage();

};