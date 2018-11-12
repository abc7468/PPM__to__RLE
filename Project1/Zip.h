#pragma once
#include<iostream>
#include<vector>
#include"ppmImage.h"	
using namespace std;

class Zip
{
public:

	unsigned char count; // �񿬼����ΰ��� Ƚ��
	unsigned char ctn; //���ӵ� Ƚ��
	unsigned char tmp;
	unsigned char *tmpR;
	unsigned char *tmpG;
	unsigned char *tmpB;
	int length;

	vector<unsigned char> Rz; //�����带 ���� �����ϴ°�
	vector<unsigned char> Gz;
	vector<unsigned char> Bz;

	vector<unsigned char> Modu; // Ǯ������͵��� ��ü��Ű�°�


	vector<unsigned char> Ru; //�����Ų���� Ǯ����� ��
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