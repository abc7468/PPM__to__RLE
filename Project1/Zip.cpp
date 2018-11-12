#define _CRT_SECURE_NO_WARNINGS
#include "Zip.h"
#include<cstdio>
#include<vector>
#include<iostream>
#include<cstdlib>
#include <fstream>
#include <cstring>
#include<windows.h>
#define TRUE	1
#define FALSE	0

using namespace std;

int Zip::ReadPPM(const char * fileNm, const char* fileNm2, PPMImage * img)
{

	FILE* fp;
	if (fileNm == NULL) {
		printf("ReadPPM 호출 에러\n");
		return FALSE;
	}

	fp = fopen(fileNm, "rb");
	if (fp == NULL) {
		printf("파일을 열 수 없습니다 : %s\n", fileNm);
		return FALSE;
	}

	fscanf(fp, "%c%c\n", &img->M, &img->N);

	//PPM파일의 매직넘버 P6
	if (img->M = 'P') {
		if (img->N != '6') {
			printf("PPM이미지 포멧이 아닙니다. : %c%c\n", img->M, img->N);
			return FALSE;
		}
	}

	fscanf(fp, "%d %d\n", &img->width, &img->height);
	fscanf(fp, "%d\n", &img->max);

	if (img->max != 255) {
		printf("올바른 이미지 포멧이 아닙니다.\n");
		return FALSE;
	}


	img->pixels = (unsigned char**)calloc(img->height, sizeof(unsigned char*));

	for (int i = 0; i < img->height; i++) {
		img->pixels[i] = (unsigned char*)calloc(img->width * 3, sizeof(unsigned char));
	}

	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width * 3; j++) {
			fread(&img->pixels[i][j], sizeof(unsigned char), 1, fp);
		}
	}
	int height = img->height;
	int width = img->width;
	img->length = height * width;
	length = img->length;
	tmpR = (unsigned char*)malloc(sizeof(unsigned char)*length);
	tmpG = (unsigned char*)malloc(sizeof(unsigned char)*length);
	tmpB = (unsigned char*)malloc(sizeof(unsigned char)*length);



	int count = 0; // RGB를 구분해주는 변수
	//Zip구조체에 R,G,B 따로 입력
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width * 3; j++) {
			if (j % 3 == 0) {
				tmpR[count] = img->pixels[i][j];
			}
			else if (j % 3 == 1) {
				tmpG[count] = img->pixels[i][j];
			}
			else if (j % 3 == 2) {
				tmpB[count] = img->pixels[i][j];
				count = count + 1;
			}
		}
	}
	fclose(fp);


	compress(fileNm, fileNm2, img);

	return TRUE;
}

void Zip::MakeRLE(const char * fileNm, PPMImage * img)
{
	FILE* out;

	out = fopen(fileNm, "ab+");
	fprintf(out, "%c%c\n%d %d\n%d", img->M, img->N, img->width, img->height, img->max);//여기가 문제입니다. 여기서 R,G,B세번다 헤더가 들어갑니다 슈벌..
	for (int i = 0; i < Rz.size(); i++) {
		fputc(Rz[i], out);
	}
	for (int i = 0; i < Gz.size(); i++) {
		fputc(Gz[i], out);
	}
	for (int i = 0; i < Bz.size(); i++) {
		fputc(Bz[i], out);
	}
	fclose(out);
}

void Zip::MakePPM(const char * fileNm, const char * fileNm2, PPMImage * img, vector<unsigned char> RLE)
{
	FILE *in;
	int ch1;
	char M, N;
	int width, height;
	int max;
	char del[20];
	if ((in = fopen(fileNm, "rb")) == NULL) {
		fputs("파일 열기 에러!", stderr);
		exit(1);
	}
	fscanf(in, "%c%c\n", &M, &N);
	fscanf(in, "%d %d\n", &width, &height);
	fscanf(in, "%d\n", &max);

	sprintf(del, "%c%c\n%d %d\n%d\n", M, N, width, height, max);

	fseek(in, strlen(del) - 1, SEEK_SET);

	while ((ch1 = fgetc(in)) != EOF) {
		RLE.push_back(ch1);

	}

	for (int i = 0; i < RLE.size(); i++) {
		if (RLE[i] == 0) {
			i = i + 1;
			int line = RLE[i];
			for (int j = i + 1; j < i + line + 1; j++) {
				Modu.push_back(RLE[j]);
			}
			i = i + line;
		}
		else {
			int loop = RLE[i];
			for (int j = 0; j < loop; j++) {
				Modu.push_back(RLE[i + 1]);
			}
			i = i + 1;
		}
	}

	int len = Modu.size() / 3;

	for (int i = 0; i < len; i++) {
		Ru.push_back(Modu[i]);
	}
	for (int i = len; i < len * 2; i++) {
		Gu.push_back(Modu[i]);
	}
	for (int i = len * 2; i < len * 3; i++) {
		Bu.push_back(Modu[i]);
	}
	int count = 0;
	for (int i = 0; i < 3 * len; i++) {
		if (i % 3 == 0) {
			Merge.push_back(Ru[count]);
		}
		else if (i % 3 == 1) {
			Merge.push_back(Gu[count]);
		}
		if (i % 3 == 2) {
			Merge.push_back(Bu[count]);
			count = count + 1;
		}
	}


	FILE* out;
	out = fopen(fileNm2, "wb+");

	fprintf(out, "%c%c\n%d %d\n%d\n", M, N, width, height, max);
	for (int i = 0; i < Merge.size(); i++) {
		fputc(Merge[i], out);
	}
	fclose(out);

}



int Zip::compress(const char * fileNm, const char * fileNm2, PPMImage * img)
{
	int tmp = NULL;
	storeR(tmpR);
	storeG(tmpG);
	storeB(tmpB);
	MakeRLE(fileNm2, img);
	calEff(fileNm, fileNm2);
	return 0;
}

void Zip::storeR(unsigned char* tmpRGB) {
	count = 0; //반복되지 않는것들의 연속되는 횟수
	ctn = 0; //반복되는 것들의 연속되는 횟수


	for (int i = 0; i < length - 1; i++) { //length = 가로 * 세로 => R데이터의 갯수
		if (tmpRGB[i] == tmpRGB[i + 1]) { // 같은 데이터가 연속된다면 반복되는 조건문

			if (count == 1) {
				Rz.push_back(1);
				Rz.push_back(tmpRGB[i - 1]);
				count = 0;
			}
			if (count == 2) {
				Rz.push_back(1);
				Rz.push_back(tmpRGB[i - 2]);
				Rz.push_back(1);
				Rz.push_back(tmpRGB[i - 1]);
				count = 0;
			}

			else if (count != 0 && count != 1 && count != 2) {

				Rz.push_back(0);
				Rz.push_back(count);
				for (int j = count; j > 0; j--) {
					Rz.push_back(tmpRGB[i - j]);
				}
				count = 0; //초기화
			}

			ctn = ctn + 1; //조건문 안에 들어오면 무조건 ctn + 1
			if (i == length - 2) { //끝부분은 따로 처리
				tmp = tmpRGB[i];
				if (tmp == tmpRGB[i + 1]) {
					ctn = ctn + 1;
					Rz.push_back(ctn);
					Rz.push_back(tmp);
					break;
				}
				else {
					Rz.push_back(ctn);
					Rz.push_back(tmp);
					Rz.push_back(1);
					Rz.push_back(tmpRGB[i + 1]);
					break;
				}
			}
			if (ctn == 255) { //16진수로 나타낼 수 있는 가장 큰값인 255를 초과하면 
							  //안되기 때문에 255에서 처리 후 다시 0부터 시작
				ctn = ctn + 1;
				Rz.push_back(ctn);
				Rz.push_back(tmpRGB[i]);
				ctn = 0; //초기화
				continue;
			}
		}


		else {//서로 다른 데이터가 이어지면 반복되는 조건문
			if (ctn != 0) { //이 조건문에 도달한다면 연속되는 조건문에 도달했다는 뜻
				ctn = ctn + 1;
				Rz.push_back(ctn);
				Rz.push_back(tmpRGB[i]);
				ctn = 0; //초기화
			}
			else {
				count = count + 1;//조건문에 들어온다면 무조건 count + 1
				if (count == 255) {
					Rz.push_back(0);
					Rz.push_back(count);
					for (int j = count; j > 0; j--) {
						Rz.push_back(tmpRGB[i - j]);
					}
					count = 0; //초기화
				}
			}
			if (i == length - 2) { //끝부분은 따로 처리
				tmp = tmpRGB[i];
				if (tmp == tmpRGB[i + 1]) {
					Rz.push_back(0);
					Rz.push_back(count);
					for (int j = count; j > 0; j--) {
						Rz.push_back(tmpRGB[i - j]);
					}
					Rz.push_back(2);
					Rz.push_back(tmpRGB[i]);
					break;
				}
				else {
					count = count + 1;
					Rz.push_back(0);
					Rz.push_back(count);
					for (int j = count; j > 0; j--) {
						Rz.push_back(tmpRGB[i - j]);
					}
					break;
				}
			}
		}
	}
}

void Zip::storeG(unsigned char* tmpRGB) {
	count = 0;
	ctn = 0;


	for (int i = 0; i < length - 1; i++) {
		if (tmpRGB[i] == tmpRGB[i + 1]) {
			if (count != 0) {

				Gz.push_back(0);
				Gz.push_back(count);
				for (int j = count; j > 0; j--) {
					Gz.push_back(tmpRGB[i - j]);
				}
				count = 0;
			}
			ctn = ctn + 1;
			if (i == length - 2) {
				tmp = tmpRGB[i];
				if (tmp == tmpRGB[i + 1]) {
					ctn = ctn + 1;
					Gz.push_back(ctn);
					Gz.push_back(tmp);
					break;
				}
				else {
					Gz.push_back(ctn);
					Gz.push_back(tmp);
					Gz.push_back(1);
					Gz.push_back(tmpRGB[i + 1]);
					break;
				}
			}
		}


		else {
			if (ctn != 0) {
				ctn = ctn + 1;
				Gz.push_back(ctn);
				Gz.push_back(tmpRGB[i]);
				ctn = 0;
			}
			else {
				count = count + 1;
			}
			if (i == length - 2) {
				tmp = tmpRGB[i];
				if (tmp == tmpRGB[i + 1]) {
					Gz.push_back(0);
					Gz.push_back(count);
					for (int j = count; j > 0; j--) {
						Gz.push_back(tmpRGB[i - j]);
					}
					Gz.push_back(2);
					Gz.push_back(tmpRGB[i]);
					break;
				}
				else {
					count = count + 1;
					Gz.push_back(0);
					Gz.push_back(count);
					for (int j = count; j > 0; j--) {
						Gz.push_back(tmpRGB[i - j]);
					}
					break;
				}
			}
		}
	}
}
void Zip::storeB(unsigned char* tmpRGB) {
	count = 0;
	ctn = 0;


	for (int i = 0; i < length - 1; i++) {
		if (tmpRGB[i] == tmpRGB[i + 1]) {
			if (count != 0) {

				Bz.push_back(0);
				Bz.push_back(count);
				for (int j = count; j > 0; j--) {
					Bz.push_back(tmpRGB[i - j]);
				}
				count = 0;
			}
			ctn = ctn + 1;
			if (i == length - 2) {
				tmp = tmpRGB[i];
				if (tmp == tmpRGB[i + 1]) {
					ctn = ctn + 1;
					Bz.push_back(ctn);
					Bz.push_back(tmp);
					break;
				}
				else {
					Bz.push_back(ctn);
					Bz.push_back(tmp);
					Bz.push_back(1);
					Bz.push_back(tmpRGB[i + 1]);
					break;
				}
			}
		}


		else {
			if (ctn != 0) {
				ctn = ctn + 1;
				Bz.push_back(ctn);
				Bz.push_back(tmpRGB[i]);
				ctn = 0;
			}
			else {
				count = count + 1;
			}
			if (i == length - 2) {
				tmp = tmpRGB[i];
				if (tmp == tmpRGB[i + 1]) {
					Bz.push_back(0);
					Bz.push_back(count);
					for (int j = count; j > 0; j--) {
						Bz.push_back(tmpRGB[i - j]);
					}
					Bz.push_back(2);
					Bz.push_back(tmpRGB[i]);
					break;
				}
				else {
					count = count + 1;
					Bz.push_back(0);
					Bz.push_back(count);
					for (int j = count; j > 0; j--) {
						Bz.push_back(tmpRGB[i - j]);
					}
					break;
				}
			}
		}
	}
}

void Zip::calEff(const char * fileNm, const char * fileNm2)
{
	FILE *input;
	FILE *output;
	float inputValue;
	float outputValue;
	float result;
	Sleep(5000);
	input = fopen(fileNm, "r+");
	output = fopen(fileNm2, "r+");
	fseek(input, 0, SEEK_END);
	fseek(output, 0, SEEK_END);
	inputValue = ftell(input);
	outputValue = ftell(output);
	result = 100 - outputValue / inputValue * 100;
	printf("입력한 PPM : %s\n압축된 파일 : %s\n\n", fileNm, fileNm2);
	printf("PPM크기 : %.0f byte\n압축파일크기:%.0f byte\n압축률 : %.2f %%\n", inputValue, outputValue, result);

}


Zip::Zip()
{
}


Zip::~Zip()
{
}