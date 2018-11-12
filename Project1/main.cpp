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
	zip->ReadPPM("C:/Users/admin/source/repos/input3.ppm", "C:/Users/admin/source/repos/result3.bin", ppm);

	//zip->MakePPM("C:/Users/admin/source/repos/result4.bin", "C:/Users/admin/source/repos/unzip4.ppm",ppm, zip->Modu);

}