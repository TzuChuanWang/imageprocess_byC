#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <ctime>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

int const width = 256;
int const height = 256;
//
int scalar = 2, percentage = 200;





void histogramplot(int ori_width, int ori_height, unsigned char* orilena, int* histogram, Mat histogramat, int percent = 100, int bit = 8) {
	int i = 0;
	int max_numbit = pow(2, bit);
	int size = ori_width * ori_height;
	while (i < max_numbit)
	{
		histogram[i] = 0;
		i++;
	}
	i = 0;
	int numbit;
	while (i < size) {
		numbit = (int)orilena[i];
		histogram[numbit] = (int)histogram[numbit] + 1;
		//cout << "i: " << i << "histogram[" << numbit << "] num: " << histogram[numbit]<<endl;
		i++;
	}
	/*int all=0;
	for (int i = 0; i < 256; i++)
	{

		cout << i << " num: " << cameraman_512_histogram[i] << endl;
		all = cameraman_512_histogram[i] + all;
	}
	cout << all<<endl;*/
	int max_num = 0;
	i = 0;
	while (i < max_numbit)
	{
		if (max_num < histogram[i])
		{
			max_num = histogram[i];

		}
		i++;
	}
	//cout << max_num;

	i = 0;
	while (i < max_numbit)
	{
		rectangle(histogramat, Point(i, percent), Point((i + 1), (max_num - (int)histogram[i]) * percent / max_num), Scalar(0));
		i++;
	}
}
void histogram_equalization(int ori_width, int ori_height, unsigned char* orilena, int* histogram, int* newhistogram,Mat histogramat, unsigned char* newlena, Mat newhistogramat,int percent = 100, int bit = 8) {
	int i = 0;
	int j = 0;
	int max_numbit = pow(2, bit);
	int size = ori_width * ori_height;
	while (i < max_numbit)
	{
		histogram[i] = 0;
		i++;
	}
	i = 0;
	int numbit;
	while (i < size) {
		numbit = (int)orilena[i];
		histogram[numbit] = (int)histogram[numbit] + 1;
		//cout << "i: " << i << "histogram[" << numbit << "] num: " << histogram[numbit]<<endl;
		i++;
	}
	/*int all=0;
	for (int i = 0; i < 256; i++)
	{

		cout << i << " num: " << cameraman_512_histogram[i] << endl;
		all = cameraman_512_histogram[i] + all;
	}
	cout << all<<endl;*/
	int max_num = 0;
	i = 0;
	while (i < max_numbit)
	{
		if (max_num < histogram[i])
		{
			max_num = histogram[i];
		}
		i++;
	}
	//cout << max_num;

	i = 0;
	while (i < max_numbit)
	{
		rectangle(histogramat, Point(i, percent), Point((i + 1), (max_num - (int)histogram[i]) * percent / max_num), Scalar(0));
		i++;
	}
	//equlization
	double P[256] ;
	double cdf[256] = {0};
	double cdf_min = 1000;


	//計算CDF
	for (i = 1; i < 256; i++) {
		//cout << histogram[i];
		P[i] = (double)histogram[i] / size;
		//cout <<"  "<< P[i];
		cdf[i] = P[i] + cdf[i - 1];
		if (cdf[i] < cdf_min && cdf[i] != 0)  cdf_min = cdf[i];
		//cout << "cdf[" << i << "] = " << cdf[i] << endl;
	}

	//轉換對照表
	for (i = 0; i < 256; i++) {
		newhistogram[i] = ((cdf[i] - cdf_min) * size / (size - (cdf_min * size)) * (max_numbit-1));
		//cout << "newhistogram[" << i << "] = " << newhistogram[i] << endl;
	}
	
	//對照
	for (i = 0; i < size; i++) {
		for (j = 0; j < 256; j++) {
			if ((int)orilena[i] == j) { newlena[i] = newhistogram[j]; }
		}
	}
	//新的histogram

	histogramplot(512, 512, newlena, newhistogram, newhistogramat, percent, 8);
}

void logtransformation(int ori_width, int ori_height, unsigned char* orilena, unsigned char* newlena,int c) {
	
	int i = 0;
	int j = 0;
	while (i < ori_width) {
		while (j < ori_height) {
			newlena[i * ori_width + j] =  c*log10(orilena[i * ori_width + j]+1);
			j++;
		}
		i++;
		j = 0;
	}
}
void power_law(int ori_width, int ori_height, unsigned char* orilena, unsigned char* newlena, double c,double r) {

	int i = 0;
	int j = 0;
	while (i < ori_width) {
		while (j < ori_height) {
			newlena[i * ori_width + j] = c * pow(orilena[i * ori_width + j],r);
			j++;
		}
		i++;
		j = 0;
	}
}
void negative_img(int ori_width, int ori_height, unsigned char* orilena, unsigned char* newlena) {
	int size = ori_width * ori_height;
	
	int i = 0;
	int j = 0;
	while (i < ori_width) {
		while (j < ori_height) {
			newlena[i * ori_width + j] = 255-orilena[i * ori_width + j];
			j++;
		}
		i++;
		j = 0;
	}
}
double msecul(int ori_width, int ori_height, unsigned char* orilena, unsigned char* newlena) {

	int i = 0;
	int j = 0;
	double mse = 0;
	int size = ori_width * ori_height;
	while (i < ori_width) {
		while (j < ori_height) {
			mse = mse + pow((orilena[i * ori_width + j] - newlena[i * ori_width + j]), 2);
			j++;
		}
		i++;
		j = 0;
	}
	mse = mse / size;
	return mse;
}
void Decimal_to_binary(int ori_width, int ori_height, unsigned char* orilena, unsigned char* newlena_binary,int ori_bit=8) {
	int size = ori_width * ori_height;
	unsigned char* tempbitlena = new unsigned char[size];
	int i = 0;
	int j = 0;
	while (i < ori_width) {
		while (j < ori_height) {
			tempbitlena[i * ori_width + j] = orilena[i * ori_width + j] ;
			j++;
		}
		i++;
		j = 0;
	}
	int n = 0 ;
	int l = 0;
	i = 0;
	j = 0;
	while (i < ori_width) {
		while (j < ori_height) {
			if (orilena[i * ori_width + j] != 0) {
				while (tempbitlena[i * ori_width + j] > 0)   //迴圈過程的始終，判斷n是否能被2除盡
				{
					newlena_binary[n] = tempbitlena[i * ori_width + j] % 2;   //用陣列儲存每次除以2之後的餘數，即斷定奇偶性，對應二進位制位上數值
					n++;
					tempbitlena[i * ori_width + j] = tempbitlena[i * ori_width + j] / 2;
					//cout << "here";
					while ((tempbitlena[i * ori_width + j] == 0) && (n % ori_bit != 0)) {
						newlena_binary[n] = 0;
						n++;
					}
				}
			}
			else if(orilena[i * ori_width + j] == 0){
				while (l < 8) { newlena_binary[n] = 0; l++; n++; }
			}
			j++; l = 0;
		}
			i++;
			j = 0;
	}
	
}
void binary_to_Decimal(int ori_width, int ori_height, unsigned char* orilena_binary, unsigned char* newlena_Decimal, int new_bit = 8) {
	int size = ori_width * ori_height;
	unsigned char* tempbitlena = new unsigned char[size];
	int i = 0;
	int j = 0;
	int n = 0;
	int l = 0;
	
		while (i < ori_width) {
			while (j < ori_height) {
				n = 8 * (i * ori_width + j);
				newlena_Decimal[i * ori_width + j] = orilena_binary[n] + orilena_binary[n + 1] * 2 + orilena_binary[n + 2] * 4 + orilena_binary[n + 3] * 8 + orilena_binary[n + 4] * 16 + orilena_binary[n + 5] * 32 + orilena_binary[n + 6] * 64 + orilena_binary[n + 7] * 128;
				//cout << "here";
				j++;
			}
			i++;
			j = 0;
		}
	
}
void bitplane_comb(int ori_width, int ori_height, unsigned char* orilena1_binary, unsigned char* orilena2_binary, unsigned char* bitplanelena_comb_binary, int hide_bit=3,int ori_bit = 8){
	int size = ori_width * ori_height;
	int i = 0, n = 0;
	while (i < 8*size) {
		if (i % ori_bit < hide_bit) {
			n = 8 - hide_bit;
			bitplanelena_comb_binary[i] = orilena2_binary[i+ n];
			//cout << "here";
		}
		else {
			bitplanelena_comb_binary[i] = orilena1_binary[i];
			//cout << "here";
		}
		i++;
	}
}
void bitplane_break(int ori_width, int ori_height, unsigned char* ori_binary, unsigned char* bitplanebreak_small_binary, unsigned char* bitplanebreak_big_binary, int hide_bit = 3, int ori_bit = 8) {
	int size = ori_width * ori_height;
	int i = 0,n=0;
	while (i < 8 * size) {
		if (i % ori_bit < hide_bit) {
			bitplanebreak_big_binary[i] = 0;
		}
		else {
			bitplanebreak_big_binary[i] = ori_binary[i];
		}
		if (i % ori_bit < (ori_bit - hide_bit)) {
			bitplanebreak_small_binary[i] = 0;
			//cout << "here";
		}
		else{
			n = 8 - hide_bit;
			bitplanebreak_small_binary[i] = ori_binary[i-n];
			//cout << "here";
		}
		i++;
	}
}
double psnrcul(int ori_width, int ori_height, unsigned char* orilena, unsigned char* newlena) {
	
	int i = 0;
	int j = 0;
	double mse = 0;
	double psnr = 0;
	int size = ori_width * ori_height;
	while (i < ori_width) {
		while (j < ori_height) {
			mse = mse + pow((orilena[i * ori_width + j] - newlena[i * ori_width + j]), 2);
			j++;
		}
		i++;
		j = 0;
	}
	mse = mse / size;
	psnr = 10 * log10(255 * 255 / mse);
	return psnr;
}
int main() {
	//Input File
	int const size = width * height;
	char OriFile[] = "lena_256.raw";
	char No;
	int i = 0;
	int j = 0;
	int l = 0;
	int k = 0;
	FILE* lenaFile;
	errno_t err;
	err = fopen_s(&lenaFile, OriFile, "rb");
	if (err != NULL)
	{
		cout << "Loading File Error!";
		system("PAUSE");
		exit(0);
	}

	unsigned char* lena = new unsigned char[size];
	fread(lena, sizeof(unsigned char), size, lenaFile);
	Mat lenaMat = Mat(height, width, CV_8UC1, lena);
	/*imshow("lenaFile", lenaMat);
	waitKey();*/
	//destroyWindow("lenaFile");
	char OriFile1[] = "baboon_256.raw";
	i = 0;
	j = 0;
	l = 0;
	k = 0;
	FILE* baboonFile;
	err = fopen_s(&baboonFile, OriFile1, "rb");
	if (err != NULL)
	{
		cout << "Loading File Error!";
		system("PAUSE");
		exit(0);
	}

	unsigned char* baboon = new unsigned char[size];
	fread(baboon, sizeof(unsigned char), size, baboonFile);
	Mat baboonMat = Mat(height, width, CV_8UC1, baboon);
	///////////////////////////////////////////////////////////////////
	
	FILE* saveFile;
	
	cout << "請先輸入第幾大題:" << endl;
	cout << "此次有1 2 3 4 " << endl;
	cin >> No;
	switch (No)
	{
		///////////////////
	case '1':
	{
		cout << "目前在第一大題，請輸入第幾小題:" << endl;
		cout << "此次有1 2 " << endl;
		cin >> No;
		switch (No) {
		case'1':
		{
			cout << "目前第1大題中第1小題" << endl;
			double mse = 0;
			double psnr = 0;
			unsigned char* baboon_binary = new unsigned char[8 * size];
			unsigned char* lena_binary = new unsigned char[8 * size];
			unsigned char* comb_binary = new unsigned char[8 * size];
			Decimal_to_binary(width, height, baboon, baboon_binary);
			Decimal_to_binary(width, height, lena, lena_binary);
			bitplane_comb(width, height, lena_binary, baboon_binary, comb_binary, 4);
			unsigned char* comb_Decimal = new unsigned char[size];

			binary_to_Decimal(width, height, comb_binary, comb_Decimal);
			Mat lena_with_baboonMat = Mat(height, width, CV_8UC1, comb_Decimal);
			imshow("lena_with_baboonFile", lena_with_baboonMat);
			waitKey();
			destroyWindow("lena_with_baboonFile");
			imwrite("1.1.png", lena_with_baboonMat);
			saveFile = fopen("lena_with_baboon.raw", "wb");
			fwrite(comb_Decimal, 1, size, saveFile);
			mse=msecul(256,256,lena, comb_Decimal);
			psnr = psnrcul(256, 256, lena, comb_Decimal);
			cout << "MSE :" <<mse<< endl;
			cout << "PSNR :" << psnr<<" db" << endl;
			break;
			
		}
		case '2':
		{
			
			cout << "目前第1大題中第2小題" << endl;
			double mse_breakbaboon = 0;
			double psnr_breakbaboon = 0;
			double mse_breaklena = 0;
			double psnr_breaklena = 0;
			unsigned char* baboon_binary = new unsigned char[8 * size];
			unsigned char* lena_binary = new unsigned char[8 * size];
			unsigned char* comb_binary = new unsigned char[8 * size];
			Decimal_to_binary(width, height, baboon, baboon_binary);
			Decimal_to_binary(width, height, lena, lena_binary);
			bitplane_comb(width, height, lena_binary, baboon_binary, comb_binary, 4);
			unsigned char* comb_Decimal = new unsigned char[size];
			binary_to_Decimal(width, height, comb_binary, comb_Decimal);
			Mat lena_with_baboonMat = Mat(height, width, CV_8UC1, comb_Decimal);

			//
			unsigned char* bitplanebreak_baboon_binary = new unsigned char[8 * size];
			unsigned char* bitplanebreak_lena_binary = new unsigned char[8 * size];
			unsigned char* bitplanebreak_baboon_Decimal = new unsigned char[size];
			unsigned char* bitplanebreak_lena_Decimal = new unsigned char[size];
			bitplane_break(width, height, comb_binary, bitplanebreak_baboon_binary, bitplanebreak_lena_binary, 4);
			binary_to_Decimal(width, height, bitplanebreak_baboon_binary, bitplanebreak_baboon_Decimal);
			binary_to_Decimal(width, height, bitplanebreak_lena_binary, bitplanebreak_lena_Decimal);
			Mat breakbaboonMat = Mat(height, width, CV_8UC1, bitplanebreak_baboon_Decimal);
			Mat breaklenaMat = Mat(height, width, CV_8UC1, bitplanebreak_lena_Decimal);
			imshow("extractbaboonFile", breakbaboonMat);
			waitKey();
			destroyWindow("extractbaboonFile");
			imwrite("1.2extractbaboon.png", breakbaboonMat);
			saveFile = fopen("extractbaboon.raw", "wb");
			fwrite(bitplanebreak_baboon_Decimal, 1, size, saveFile);
			imshow("extractlenaFile", breaklenaMat);
			waitKey();
			destroyWindow("extractlenaFile");
			imwrite("1.2extractlena.png", breaklenaMat);
			saveFile = fopen("extractlena.raw", "wb");
			fwrite(bitplanebreak_lena_Decimal, 1, size, saveFile);
			mse_breaklena = msecul(256, 256, lena, bitplanebreak_lena_Decimal);
			psnr_breaklena = psnrcul(256, 256, lena, bitplanebreak_lena_Decimal);
			mse_breakbaboon = msecul(256, 256, baboon, bitplanebreak_baboon_Decimal);
			psnr_breakbaboon = psnrcul(256, 256, baboon, bitplanebreak_baboon_Decimal);
			cout << "MSE_breaklena :" << mse_breaklena << endl;
			cout << "PSNR_breaklena :" << psnr_breaklena << " db" << endl;
			cout << "MSE_breakbaboon :" << mse_breakbaboon << endl;
			cout << "PSNR_breakbaboon :" << psnr_breakbaboon << " db" << endl;
			break;
		}
		
		}//1-小switch
		break;
	}
	case'2':
	{
		//2
		cout << "目前第2題" << endl;
		char OriFile2[] = "a512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* a512File;
		err = fopen_s(&a512File, OriFile2, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}

		unsigned char* a512 = new unsigned char[512 * 512];
		fread(a512, sizeof(unsigned char), 512 * 512, a512File);
		Mat a512Mat = Mat(512, 512, CV_8UC1, a512);

		char OriFile3[] = "b512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* b512File;
		err = fopen_s(&b512File, OriFile3, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}

		unsigned char* b512 = new unsigned char[512 * 512];
		fread(b512, sizeof(unsigned char), 512 * 512, b512File);
		Mat b512Mat = Mat(512, 512, CV_8UC1, b512);

		char OriFile4[] = "c512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* c512File;
		err = fopen_s(&c512File, OriFile4, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}
		unsigned char* c512 = new unsigned char[512 * 512];
		fread(c512, sizeof(unsigned char), 512 * 512, c512File);
		Mat c512Mat = Mat(512, 512, CV_8UC1, c512);

		char OriFile5[] = "d512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* d512File;
		err = fopen_s(&d512File, OriFile5, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}
		unsigned char* d512 = new unsigned char[512 * 512];
		fread(d512, sizeof(unsigned char), 512 * 512, d512File);
		Mat d512Mat = Mat(512, 512, CV_8UC1, d512);

		char OriFile6[] = "e512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* e512File;
		err = fopen_s(&e512File, OriFile6, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}
		unsigned char* e512 = new unsigned char[512 * 512];
		fread(e512, sizeof(unsigned char), 512 * 512, e512File);
		Mat e512Mat = Mat(512, 512, CV_8UC1, e512);

		char OriFile7[] = "f512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* f512File;
		err = fopen_s(&f512File, OriFile7, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}
		unsigned char* f512 = new unsigned char[512 * 512];
		fread(f512, sizeof(unsigned char), 512 * 512, f512File);
		Mat f512Mat = Mat(512, 512, CV_8UC1, f512);

		char OriFile8[] = "g512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* g512File;
		err = fopen_s(&g512File, OriFile8, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}
		unsigned char* g512 = new unsigned char[512 * 512];
		fread(g512, sizeof(unsigned char), 512 * 512, g512File);
		Mat g512Mat = Mat(512, 512, CV_8UC1, g512);

		char OriFile9[] = "h512x512.raw";
		i = 0;
		j = 0;
		l = 0;
		k = 0;
		FILE* h512File;
		err = fopen_s(&h512File, OriFile9, "rb");
		if (err != NULL)
		{
			cout << "Loading File Error!";
			system("PAUSE");
			exit(0);
		}
		unsigned char* h512 = new unsigned char[512 * 512];
		fread(h512, sizeof(unsigned char), 512 * 512, h512File);
		Mat h512Mat = Mat(512, 512, CV_8UC1, h512);

		unsigned char* a512_binary = new unsigned char[8 * 512 * 512];
		unsigned char* b512_binary = new unsigned char[8 * 512 * 512];
		unsigned char* c512_binary = new unsigned char[8 * 512 * 512];
		unsigned char* d512_binary = new unsigned char[8 * 512 * 512];
		unsigned char* e512_binary = new unsigned char[8 * 512 * 512];
		unsigned char* f512_binary = new unsigned char[8 * 512 * 512];
		unsigned char* g512_binary = new unsigned char[8 * 512 * 512];
		unsigned char* h512_binary = new unsigned char[8 * 512 * 512];


		negative_img(512, 512, h512, h512);
		negative_img(512, 512, e512, e512);
		negative_img(512, 512, f512, f512);
		negative_img(512, 512, d512, d512);
		/*negative_img(512, 512, b512, b512);
		negative_img(512, 512, c512, c512);
		negative_img(512, 512, d512, d512);
		negative_img(512, 512, f512, f512);
		negative_img(512, 512, g512, g512);*/
		Decimal_to_binary(512, 512, a512, a512_binary);
		//cout << (int)a512_binary[7] << (int)a512_binary[6] << (int)a512_binary[5] << (int)a512_binary[4] << (int)a512_binary[3] << (int)a512_binary[2] << (int)a512_binary[1] << (int)a512_binary[0]<<endl;
		Decimal_to_binary(512, 512, b512, b512_binary);
		//cout << (int)b512_binary[15] << (int)b512_binary[14] << (int)b512_binary[13] << (int)b512_binary[12] << (int)b512_binary[11] << (int)b512_binary[10] << (int)b512_binary[9] << (int)b512_binary[8]<< endl;
		Decimal_to_binary(512, 512, c512, c512_binary);
		Decimal_to_binary(512, 512, d512, d512_binary);
		Decimal_to_binary(512, 512, e512, e512_binary);
		Decimal_to_binary(512, 512, f512, f512_binary);
		//cout << (int)f512_binary[15] << (int)f512_binary[14] << (int)f512_binary[13] << (int)f512_binary[12] << (int)f512_binary[11] << (int)f512_binary[10] << (int)f512_binary[9] << (int)f512_binary[8] << endl;
		Decimal_to_binary(512, 512, g512, g512_binary);
		Decimal_to_binary(512, 512, h512, h512_binary);
		unsigned char* temp_binary = new unsigned char[8 * 512 * 512];
		bitplane_comb(512, 512, b512_binary, g512_binary, temp_binary, 7);
		bitplane_comb(512, 512, temp_binary, h512_binary, temp_binary, 6);
		bitplane_comb(512, 512, temp_binary, c512_binary, temp_binary, 5);
		bitplane_comb(512, 512, temp_binary, a512_binary, temp_binary, 4);
		bitplane_comb(512, 512, temp_binary, e512_binary, temp_binary, 3);
		bitplane_comb(512, 512, temp_binary, d512_binary, temp_binary, 2);
		bitplane_comb(512, 512, temp_binary, f512_binary, temp_binary, 1);

		//cout << (int)temp_binary[7] << (int)temp_binary[6] << (int)temp_binary[5] << (int)temp_binary[4] << (int)temp_binary[3] << (int)temp_binary[2] << (int)temp_binary[1] << (int)temp_binary[0] << endl;
		unsigned char* temp_Decimal = new unsigned char[512 * 512];
		binary_to_Decimal(512, 512, temp_binary, temp_Decimal);

		Mat tempMat = Mat(512, 512, CV_8UC1, temp_Decimal);
		imshow("originalFile", tempMat);
		waitKey();
		destroyWindow("originalFile");
		imwrite("2.png", tempMat);
		saveFile = fopen("original.raw", "wb");
		fwrite(temp_Decimal, 1, size, saveFile);
		break;

	}//case2的
	case '3':
	{
		cout << "目前在第三大題，請輸入第幾小題:" << endl;
		cout << "此次有a b " << endl;
		cin >> No;
		switch (No) {
		case'a':
		{
			cout << "目前第三大題中第a小題" << endl;
			char OriFile2[] = "livingroom_d512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* livingroomFile;
			err = fopen_s(&livingroomFile, OriFile2, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}

			unsigned char* livingroom_512 = new unsigned char[512 * 512];
			fread(livingroom_512, sizeof(unsigned char), 512 * 512, livingroomFile);
			unsigned char* templivingroom = new unsigned char[512 * 512];
			
			Mat livingroomMat = Mat(512, 512, CV_8UC1, livingroom_512);
			logtransformation(512, 512, livingroom_512, templivingroom, 100);//c=100
			Mat livingroom_logtransMat = Mat(512, 512, CV_8UC1, templivingroom);
			imshow("livingroomFile", livingroom_logtransMat);
			waitKey();
			destroyWindow("livingroomFile");
			imwrite("livingroom_logtrans100Mat.png", livingroom_logtransMat);
			
			logtransformation(512, 512, livingroom_512, templivingroom, 20);//c=20
			livingroom_logtransMat = Mat(512, 512, CV_8UC1, templivingroom);
			imshow("livingroomFile", livingroom_logtransMat);
			waitKey();
			destroyWindow("livingroomFile");
			imwrite("livingroom_logtrans20Mat.png", livingroom_logtransMat);

			char OriFile3[] = "cameraman_b512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* cameramanFile;
			err = fopen_s(&cameramanFile, OriFile3, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}

			unsigned char* cameraman_b512 = new unsigned char[512 * 512];
			unsigned char* tempcameraman = new unsigned char[512 * 512];
			fread(cameraman_b512, sizeof(unsigned char), 512 * 512, cameramanFile);
			Mat cameramanMat = Mat(512, 512, CV_8UC1, cameraman_b512);
			logtransformation(512, 512, cameraman_b512, tempcameraman, 100);
			Mat cameraman_logtransMat = Mat(512, 512, CV_8UC1, tempcameraman);
			imshow("cameramanFile", cameraman_logtransMat);
			waitKey();
			destroyWindow("cameramanFile");
			imwrite("cameraman_logtrans100Mat.png", cameraman_logtransMat);

			
			logtransformation(512, 512, cameraman_b512, tempcameraman, 20);
			cameraman_logtransMat = Mat(512, 512, CV_8UC1, tempcameraman);
			imshow("cameramanFile", cameraman_logtransMat);
			waitKey();
			destroyWindow("cameramanFile");
			imwrite("cameraman_logtrans20Mat.png", cameraman_logtransMat);
			break;
		}
		case 'b':
		{
			//1.2
			cout << "目前第三大題中第b小題" << endl;
			char OriFile2[] = "livingroom_d512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* livingroomFile;
			err = fopen_s(&livingroomFile, OriFile2, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}
			unsigned char* livingroom_512 = new unsigned char[512 * 512];
			unsigned char* templivingroom = new unsigned char[512 * 512];
			fread(livingroom_512, sizeof(unsigned char), 512 * 512, livingroomFile);
			Mat livingroomMat = Mat(512, 512, CV_8UC1, livingroom_512);
			power_law(512, 512, livingroom_512, templivingroom, 100, 0.2);//c=100 r=0.2
			Mat livingroom_powerlawMat = Mat(512, 512, CV_8UC1, templivingroom);
			imshow("livingroomFilec100r0.2_powerlaw", livingroom_powerlawMat);
			waitKey();
			destroyWindow("livingroomFilec100r0.2_powerlaw");
			imwrite("c100_livingroom_powerlaw0.2Mat.png", livingroom_powerlawMat);

			power_law(512, 512, livingroom_512, templivingroom, 0.0000025, 10);//c=0.0000025 r=10
			livingroom_powerlawMat = Mat(512, 512, CV_8UC1, templivingroom);
			imshow("livingroomFilec0.0000025r10_powerlaw", livingroom_powerlawMat);
			waitKey();
			destroyWindow("livingroomFilec0.0000025r10_powerlaw");
			imwrite("c0.0000025_livingroom_powerlaw10Mat.png", livingroom_powerlawMat);

			power_law(512, 512, livingroom_512, templivingroom, 1000, 0.2);//c=1000 r=0.2
			livingroom_powerlawMat = Mat(512, 512, CV_8UC1, templivingroom);
			imshow("livingroomFilec1000r0.2_powerlaw", livingroom_powerlawMat);
			waitKey();
			destroyWindow("livingroomFilec1000r0.2_powerlaw");
			imwrite("c1000_livingroom_powerlaw0.2Mat.png", livingroom_powerlawMat);

			power_law(512, 512, livingroom_512, templivingroom, 0.0000000000000000000001, 10);//c=0.0000025 r=10
			livingroom_powerlawMat = Mat(512, 512, CV_8UC1, templivingroom);
			imshow("livingroomFilec0.0000000000000000000001r10_powerlaw", livingroom_powerlawMat);
			waitKey();
			destroyWindow("livingroomFilec0.0000000000000000000001r10_powerlaw");
			imwrite("c0.0000000000000000000001_livingroom_powerlaw10Mat.png", livingroom_powerlawMat);
			char OriFile3[] = "cameraman_b512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* cameramanFile;
			err = fopen_s(&cameramanFile, OriFile3, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}
			unsigned char* cameraman_b512 = new unsigned char[512 * 512];
			unsigned char* tempcameraman = new unsigned char[512 * 512];
			fread(cameraman_b512, sizeof(unsigned char), 512 * 512, cameramanFile);
			Mat cameramanMat = Mat(512, 512, CV_8UC1, cameraman_b512);
			power_law(512, 512, cameraman_b512, tempcameraman, 100, 0.2);//c=100 r=0.2
			Mat cameraman_power_lawMat = Mat(512, 512, CV_8UC1, tempcameraman);
			imshow("cameraman_powerlaw", cameraman_power_lawMat);
			waitKey();
			destroyWindow("cameraman_powerlaw");
			imwrite("c100_cameraman_powerlaw0.2Mat.png", cameraman_power_lawMat);

			power_law(512, 512, cameraman_b512, tempcameraman, 1000, 0.2);//c=1000 r=0.2
			cameraman_power_lawMat = Mat(512, 512, CV_8UC1, tempcameraman);
			imshow("cameraman_powerlaw", cameraman_power_lawMat);
			waitKey();
			destroyWindow("cameraman_powerlaw");
			imwrite("c1000_cameraman_powerlaw0.2Mat.png", cameraman_power_lawMat);

			power_law(512, 512, cameraman_b512, tempcameraman, 0.0000025, 10);//c=0.0000025 r=10
			cameraman_power_lawMat = Mat(512, 512, CV_8UC1, tempcameraman);
			imshow("cameraman_powerlaw", cameraman_power_lawMat);
			waitKey();
			destroyWindow("cameraman_powerlaw");
			imwrite("c0.0000025_cameraman_powerlaw10Mat.png", cameraman_power_lawMat);

			power_law(512, 512, cameraman_b512, tempcameraman, 0.0000000000000000000002, 10);//c=0.0000000000000000000002r=10
			cameraman_power_lawMat = Mat(512, 512, CV_8UC1, tempcameraman);
			imshow("cameraman_powerlaw", cameraman_power_lawMat);
			waitKey();
			destroyWindow("cameraman_powerlaw");
			imwrite("c0.0000000000000000000001_cameraman_powerlaw10Mat.png", cameraman_power_lawMat);
			break;
		}

		}//3-小switch
		break;
	}
	case '4':
	{
		cout << "目前在第四大題，請輸入第幾小題:" << endl;
		cout << "此次有a b " << endl;
		cin >> No;
		switch (No) {
		case'a':
		{
			cout << "目前第四大題中第a小題" << endl;
			char OriFile2[] = "livingroom_d512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* livingroomFile;
			err = fopen_s(&livingroomFile, OriFile2, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}

			unsigned char* livingroom_512 = new unsigned char[512 * 512];
			fread(livingroom_512, sizeof(unsigned char), 512 * 512, livingroomFile);
			int livingroom_512_histogram[256];
			int percent = 100;
			Mat livingroom_histogramat(percent, 256, CV_8U, Scalar(127));
			histogramplot(512, 512, livingroom_512, livingroom_512_histogram, livingroom_histogramat, percent, 8);
			imshow("livingroom_histogramat", livingroom_histogramat);
			waitKey();
			destroyWindow("livingroom_histogramat");
			imwrite("livingroom_histogramat.png", livingroom_histogramat);

			char OriFile3[] = "cameraman_b512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* cameramanFile;
			err = fopen_s(&cameramanFile, OriFile3, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}

			unsigned char* cameraman_b512 = new unsigned char[512 * 512];
			fread(cameraman_b512, sizeof(unsigned char), 512 * 512, cameramanFile);
			Mat cameraman_b512Mat = Mat(512, 512, CV_8UC1, cameraman_b512);
			int cameraman_512_histogram[256];
			int max_num = 0;
			percent = 100;
			Mat cameraman_histogramat(percent, 256, CV_8U, Scalar(127));
			histogramplot(512, 512, cameraman_b512, cameraman_512_histogram, cameraman_histogramat, percent, 8);

			/*int all=0;
			for (int i = 0; i < 256; i++)
			{

				cout << i << " num: " << cameraman_512_histogram[i] << endl;
				all = cameraman_512_histogram[i] + all;
			}
			cout << all<<endl;*/
			imshow("cameraman_histogramat", cameraman_histogramat);
			waitKey();
			destroyWindow("cameraman_histogramat");
			imwrite("cameraman_histogramat.png", cameraman_histogramat);
			break;
		}
		case 'b':
		{
			//1.2
			cout << "目前第四大題中第b小題" << endl;
			char OriFile2[] = "livingroom_d512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* livingroomFile;
			err = fopen_s(&livingroomFile, OriFile2, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}

			unsigned char* livingroom_512 = new unsigned char[512 * 512];
			fread(livingroom_512, sizeof(unsigned char), 512 * 512, livingroomFile);
			int livingroom_512_histogram[256];
			int newlivingroom_512_histogram[256];
			int percent = 100;
			unsigned char* newlivingroom_512 = new unsigned char[512 * 512];
			Mat livingroom_histogramat(percent, 256, CV_8U, Scalar(127));
			Mat livingroom_newhistogramat(percent, 256, CV_8U, Scalar(127));
			histogram_equalization(512, 512, livingroom_512, livingroom_512_histogram, newlivingroom_512_histogram, livingroom_histogramat, newlivingroom_512, livingroom_newhistogramat);
			Mat livingroom_newhistograeqlmat = Mat(512, 512, CV_8UC1, newlivingroom_512);

			imshow("livingroom_newhistogramat", livingroom_newhistogramat);
			waitKey();
			destroyWindow("livingroom_newhistogramat");
			imwrite("livingroom_newhistogramat.png", livingroom_newhistogramat);

			imshow("livingroom_newhistograeqlmat", livingroom_newhistograeqlmat);
			waitKey();
			destroyWindow("livingroom_newhistograeqlmat");
			imwrite("livingroom_newhistograeqlmat.png", livingroom_newhistograeqlmat);
		




			char OriFile3[] = "cameraman_b512.raw";
			i = 0;
			j = 0;
			l = 0;
			k = 0;
			FILE* cameramanFile;
			err = fopen_s(&cameramanFile, OriFile3, "rb");
			if (err != NULL)
			{
				cout << "Loading File Error!";
				system("PAUSE");
				exit(0);
			}

			unsigned char* cameraman_b512 = new unsigned char[512 * 512];
			fread(cameraman_b512, sizeof(unsigned char), 512 * 512, cameramanFile);
			Mat cameraman_b512Mat = Mat(512, 512, CV_8UC1, cameraman_b512);
			int cameraman_512_histogram[256];
			percent = 100;

			int cameraman_newhistogram[256];
			unsigned char* newcameraman_512 = new unsigned char[512 * 512];
			Mat cameraman_newhistogramat(percent, 256, CV_8U, Scalar(127));
			Mat cameraman_histogramat(percent, 256, CV_8U, Scalar(127));
			histogram_equalization(512, 512, cameraman_b512, cameraman_512_histogram, cameraman_newhistogram, cameraman_histogramat, newcameraman_512, cameraman_newhistogramat);
			Mat cameraman_newhistograeqlmat = Mat(512, 512, CV_8UC1, newcameraman_512);

			imshow("cameraman_newhistogramat", cameraman_newhistogramat);
			waitKey();
			destroyWindow("cameraman_newhistogramat");
			imwrite("cameraman_newhistogramat.png", cameraman_newhistogramat);

			imshow("cameraman_newhistograeqlmat", cameraman_newhistograeqlmat);
			waitKey();
			destroyWindow("cameraman_newhistograeqlmat");
			imwrite("cameraman_newhistograeqlmat.png", cameraman_newhistograeqlmat);
			break;
		}

		}//4-小switch
		break;
	}
	}//大題switch
	return  main();
}//main

