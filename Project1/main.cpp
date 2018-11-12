#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<vector>
#include<fstream>
#include <cstring>
#include"PPMImage.h"
#include"Zip.h"
#pragma warning(disable:4996)


using namespace std;


int main() {

	vector<int> test;
	PPMImage* ppm = new PPMImage;
	Zip* zip = new Zip;
	//zip->ReadPPM("C:/Users/admin/Desktop/github/PPM_to_RLE/input4.ppm", "C:/Users/admin/Desktop/github/PPM_to_RLE/result4.bin", ppm);

	zip->MakePPM("C:/Users/admin/Desktop/github/PPM_to_RLE/result1.bin", "C:/Users/admin/Desktop/github/PPM_to_RLE/unzip1.ppm",ppm, zip->Modu);

}