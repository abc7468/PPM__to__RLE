#pragma once
#include<iostream>
#include<vector>
#include"ppmImage.h"	
using namespace std;

class Zip
{
public:

	unsigned char count; // 비연속적인것의 횟수
	unsigned char ctn; //연속된 횟수
	unsigned char tmp;
	unsigned char *tmpR;
	unsigned char *tmpG;
	unsigned char *tmpB;
	int length;

	vector<unsigned char> Rz; //압축모드를 따로 저장하는곳
	vector<unsigned char> Gz;
	vector<unsigned char> Bz;

	vector<unsigned char> Modu; // 풀어놓은것들을 합체시키는곳


	vector<unsigned char> Ru; //압축시킨것을 풀어놓는 곳
	vector<unsigned char> Gu;
	vector<unsigned char> Bu;


	vector<unsigned char> Merge;

	int compress(const char * fileNm, const char * fileNm2, PPMImage * img);
	int ReadPPM(const char* fileNm, const char* fileNm2, PPMImage* img);
	void MakeRLE(const char * fileNm, PPMImage * img);
	void MakePPM(const char * fileNm, const char * fileNm2, PPMImage * img, vector<unsigned char> RLE);
	void storeR(unsigned char* tmpRGB);
	void storeG(unsigned char* tmpRGB);
	void storeB(unsigned char* tmpRGB);
	void calEff(const char * fileNm, const char * fileNm2);

	Zip();
	~Zip();


};