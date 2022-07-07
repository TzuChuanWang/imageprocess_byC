#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <ctime>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include "opencv2/imgcodecs.hpp"
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI acos(-1)
using namespace std;
using namespace cv;

int const all_width = 512;
int const all_height = 512;
//

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
void read_file(int width, int height, const char filename[], unsigned char* dst) {
	int size = width * height;
	FILE* file;
	errno_t err;
	err = fopen_s(&file, filename, "rb");
	if (err != NULL)
	{
		cout << "Loading File Error!";
		system("PAUSE");
		exit(0);
	}
	fread(dst, sizeof(unsigned char), size, file);
}

double homomorphic_fliter_function(int all_heigh,int all_width, int now_heigh, int now_width, double c, double rH, double rL, double D0) {
	
	
	
	double D = sqrt(pow((now_heigh - all_heigh / 2), 2) + pow((now_width - all_width / 2), 2));
	double h = rL + (rH - rL) * (1 - exp(-c * D * D / D0 / D0));
	return h;
}


void homomorphic_flitering(int heigh, int width, unsigned char* src, Mat& dst, Mat& invDFT) {
	//log//
	int const size = heigh * width;
	float* cc = new float[size];
	for (int i = 0; i < size; i++) {
		cc[i] = log((float)src[i] + 1);
	}
	Mat house = Mat(heigh, width, CV_8UC1, src);
	//
	Mat imgi, imgo, pad, complexI;
	imgi = Mat(heigh, width, CV_32FC1, cc);
	int R = getOptimalDFTSize(imgi.rows);
	int C = getOptimalDFTSize(imgi.cols);
	copyMakeBorder(imgi, pad, 0, R - imgi.rows, 0, C - imgi.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);
	//phase
	/*phase(planes[0], planes[1], ph, false);
	normalize(ph, ph, 0, 1, NORM_MINMAX);
	ph.convertTo(ph, CV_8UC1, 255, 0);*/
	//
	//magnitude(planes[0], planes[1], planes[0]);
	//imgo = planes[0];
	//imgo += Scalar::all(1);
	//log(imgo, imgo);
	//imgo = imgo(Rect(0, 0, imgo.cols & -2, imgo.rows & -2));

	//int x = imgo.cols / 2;
	//int y = imgo.rows / 2;
	//Mat tmp;
	//Mat q0(imgo, Rect(0, 0, x, y));
	//Mat q1(imgo, Rect(x, 0, x, y));
	//Mat q2(imgo, Rect(0, y, x, y));
	//Mat q3(imgo, Rect(x, y, x, y));
	//q0.copyTo(tmp);
	//q3.copyTo(q0);
	//tmp.copyTo(q3);
	//q1.copyTo(tmp);
	//q2.copyTo(q1);
	//tmp.copyTo(q2);
	////cout << imgo << endl;
	//normalize(imgo, imgo, 0, 1, NORM_MINMAX);
	////cout << imgo << endl;
	//imgo.convertTo(imgo, CV_8UC1, 255, 0);
	//imgo.copyTo(dst);
	//unsigned char* dft_realData = new unsigned char[size];
	//dft_realData = planes[0].data;
	//unsigned char* dft_imageData = new unsigned char[size];
	//dft_imageData = planes[1].data;
	////////////////////////////////////////////////////////////////////
	float* dft_realData = new float[size];
	dft_realData = (float*)planes[0].data;
	float* dft_imageData = new float[size];
	dft_imageData = (float*)planes[1].data;
	float rL = 0.25;
	float rH = 2;
	float c = 1;
	float D0 =80;
	float* dst_realData = new float[size];
	float* dst_imageData = new float[size];
	for (int i = 0; i < heigh; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//dstData[j + i * width] = dftData[ j  + i  * width];

			dst_realData[j + i * width] = dft_realData[j + i * width] * homomorphic_fliter_function(heigh, width, i, j, c, rL, rH, D0);
			dst_imageData[j + i * width] = dft_imageData[j + i * width] * homomorphic_fliter_function(heigh, width, i, j, c, rL, rH, D0);
			//dst_realData[j + i * width] = (float)dft_realData[j + i * width] * 1;
			//dst_imageData[j + i * width] = (float)dft_imageData[j + i * width] * 1;
		}
	}

	planes[0] = Mat(heigh, width, CV_32F, dst_realData);
	planes[1] = Mat(heigh, width, CV_32F, dst_imageData);
	//////////////////////////////////////////////////////////////////////////////
	//idft
	Mat complexI12;
	merge(planes, 2 ,complexI12);
	Mat tmp;
	//invDFT = Mat(heigh, width, CV_8UC1, dstData);
	idft(complexI12, tmp, DFT_SCALE | DFT_REAL_OUTPUT);//離散傅立葉逆變換

	float* temp = new float[size];
	temp = (float*)tmp.data;
	for (int i = 0; i < size; i++) {
		temp[i] = exp(temp[i]+1);
	}
	invDFT = Mat(heigh, width, CV_32F,temp);
	normalize(invDFT, invDFT, 0, 1, NORM_MINMAX);
	////cout << imgo << endl;
	invDFT.convertTo(invDFT, CV_8UC1, 255, 0);
//	invDFT.convertTo(idftout, CV_8U);
	//invDFT = Mat(heigh, width, CV_8UC1, dstData);

	return;
	
	}

double ideal_LPF(int all_heigh, int all_width, int now_heigh, int now_width, double D0, double h = 0) {

	double D = 0;
	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;
	D = sqrt(pow((now_heigh - all_heigh / 2), 2) + pow((now_width - all_width / 2), 2));
	if (D > D0) { h = 0; }
	else h = 1;
	 
	return h;
}
void flitering_frequency_domain_LPF(int heigh, int width, unsigned char* src, Mat& dst, Mat& invDFT,double D0) {
	//log//
	int const size = heigh * width;
	float* cc = new float[size];
	for (int i = 0; i < size; i++) {
		cc[i] = src[i] ;
	}
	Mat house = Mat(heigh, width, CV_8UC1, src);
	//
	Mat imgi, imgo, pad, complexI;
	imgi = Mat(heigh, width, CV_32FC1, cc);
	int R = getOptimalDFTSize(imgi.rows);
	int C = getOptimalDFTSize(imgi.cols);
	copyMakeBorder(imgi, pad, 0, R - imgi.rows, 0, C - imgi.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);
	
	float* dft_realData = new float[size];
	dft_realData = (float*)planes[0].data;
	float* dft_imageData = new float[size];
	dft_imageData = (float*)planes[1].data;
	float rL = 0.25;
	float rH = 2;
	float c = 1;
	float* dst_realData = new float[size];
	float* dst_imageData = new float[size];
	for (int i = 0; i < heigh; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//dstData[j + i * width] = dftData[ j  + i  * width];
			dst_realData[j + i * width] = dft_realData[j + i * width] * ideal_LPF(heigh, width, i, j, D0);
			dst_imageData[j + i * width] = dft_imageData[j + i * width] * ideal_LPF(heigh, width, i, j, D0);
			//dst_realData[j + i * width] = (float)dft_realData[j + i * width] * 1;
			//dst_imageData[j + i * width] = (float)dft_imageData[j + i * width] * 1;
		}
	}

	planes[0] = Mat(heigh, width, CV_32F, dst_realData);
	planes[1] = Mat(heigh, width, CV_32F, dst_imageData);
	//////////////////////////////////////////////////////////////////////////////
	//idft
	Mat complexI12;
	merge(planes, 2, complexI12);
	magnitude(planes[0], planes[1], planes[0]);
	imgo = planes[0];
	imgo += Scalar::all(1);
	log(imgo, imgo);
	imgo = imgo(Rect(0, 0, imgo.cols & -2, imgo.rows & -2));

	int x = imgo.cols / 2;
	int y = imgo.rows / 2;
	Mat tmp;
	Mat q0(imgo, Rect(0, 0, x, y));
	Mat q1(imgo, Rect(x, 0, x, y));
	Mat q2(imgo, Rect(0, y, x, y));
	Mat q3(imgo, Rect(x, y, x, y));
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	//cout << imgo << endl;
	normalize(imgo, imgo, 0, 1, NORM_MINMAX);
	//cout << imgo << endl;
	imgo.convertTo(imgo, CV_8UC1, 255, 0);
	imgo.copyTo(dst);
	
	//invDFT = Mat(heigh, width, CV_8UC1, dstData);
	idft(complexI12, tmp, DFT_SCALE | DFT_REAL_OUTPUT);//離散傅立葉逆變換


	float* temp = new float[size];
	temp = (float*)tmp.data;
	/*for (int i = 0; i < size; i++) {
		temp[i] = exp(temp[i] + 1);
	}*/
	invDFT = Mat(heigh, width, CV_32F, temp);
	normalize(invDFT, invDFT, 0, 1, NORM_MINMAX);
	////cout << imgo << endl;
	invDFT.convertTo(invDFT, CV_8UC1, 255, 0);
	
}
double Gaussian_HPF(int all_heigh, int all_width, int now_heigh, int now_width, double D0, double h = 0) {

	double D = 0;
	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;
	D = sqrt(pow((now_heigh - all_heigh / 2), 2) + pow((now_width - all_width / 2), 2));
	h = 1 - exp(-pow(D,2)/2/(pow(D0,2)));

	return h;
}
void flitering_frequency_domain_Gaussian_HPF(int heigh, int width, unsigned char* src, Mat& dst, Mat& invDFT, double D0) {
	//log//
	int const size = heigh * width;
	float* cc = new float[size];
	for (int i = 0; i < size; i++) {
		cc[i] = src[i] ;
	}
	Mat house = Mat(heigh, width, CV_8UC1, src);
	//
	Mat imgi, imgo, pad, complexI;
	imgi = Mat(heigh, width, CV_32FC1, cc);
	int R = getOptimalDFTSize(imgi.rows);
	int C = getOptimalDFTSize(imgi.cols);
	copyMakeBorder(imgi, pad, 0, R - imgi.rows, 0, C - imgi.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);

	float* dft_realData = new float[size];
	dft_realData = (float*)planes[0].data;
	float* dft_imageData = new float[size];
	dft_imageData = (float*)planes[1].data;
	float rL = 0.25;
	float rH = 2;
	float c = 1;
	float* dst_realData = new float[size];
	float* dst_imageData = new float[size];
	for (int i = 0; i < heigh; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//dstData[j + i * width] = dftData[ j  + i  * width];
			dst_realData[j + i * width] = dft_realData[j + i * width] * Gaussian_HPF(heigh, width, i, j, D0);
			dst_imageData[j + i * width] = dft_imageData[j + i * width] * Gaussian_HPF(heigh, width, i, j, D0);
			//dst_realData[j + i * width] = (float)dft_realData[j + i * width] * 1;
			//dst_imageData[j + i * width] = (float)dft_imageData[j + i * width] * 1;
		}
	}

	planes[0] = Mat(heigh, width, CV_32F, dst_realData);
	planes[1] = Mat(heigh, width, CV_32F, dst_imageData);
	//////////////////////////////////////////////////////////////////////////////
	//idft
	Mat complexI12;

	merge(planes, 2, complexI12);
	magnitude(planes[0], planes[1], planes[0]);
	imgo = planes[0];
	imgo += Scalar::all(1);
	log(imgo, imgo);
	imgo = imgo(Rect(0, 0, imgo.cols & -2, imgo.rows & -2));

	int x = imgo.cols / 2;
	int y = imgo.rows / 2;
	Mat tmp;
	Mat q0(imgo, Rect(0, 0, x, y));
	Mat q1(imgo, Rect(x, 0, x, y));
	Mat q2(imgo, Rect(0, y, x, y));
	Mat q3(imgo, Rect(x, y, x, y));
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	//cout << imgo << endl;
	normalize(imgo, imgo, 0, 1, NORM_MINMAX);
	//cout << imgo << endl;
	imgo.convertTo(imgo, CV_8UC1, 255, 0);
	imgo.copyTo(dst);

	

	//invDFT = Mat(heigh, width, CV_8UC1, dstData);
	idft(complexI12, tmp, DFT_SCALE | DFT_REAL_OUTPUT);//離散傅立葉逆變換


	float* temp = new float[size];
	temp = (float*)tmp.data;
	/*for (int i = 0; i < size; i++) {
		temp[i] = exp(temp[i] + 1);
	}*/
	invDFT = Mat(heigh, width, CV_32F, temp);
	normalize(invDFT, invDFT, 0, 1, NORM_MINMAX);
	////cout << imgo << endl;
	invDFT.convertTo(invDFT, CV_8UC1, 255, 0);
	
}
int main() {

	
	
	///////////////////////////////////////////////////////////////////
	

	char No;
	cout << "請先輸入第幾大題:" << endl;
	cout << "此次有1 2 " << endl;
	cin >> No;
	switch (No)
	{
		///////////////////
	case'1':
	{
		cout << "目前在第一大題" << endl;
		int const size = all_width * all_height;
		unsigned char* house512 = new unsigned char[size];
		read_file(512, 512, "house512.raw", house512);
		Mat house512Mat = Mat(512, 512, CV_8UC1, house512);

		unsigned char* house512_dft = new unsigned char[size];
		unsigned char* house512_idft = new unsigned char[size];
		Mat house512_dftMat = Mat(512, 512, CV_8UC1, house512_dft);
		Mat house512_idftMat = Mat(512, 512, CV_8UC1, house512_idft);
		homomorphic_flitering(512, 512, house512, house512_dftMat, house512_idftMat);



		imwrite("homomorphic_flitering.png", house512_idftMat);
		break;

	}//case1的
	case'2':
	{
		cout << "目前在第二大題，請輸入第幾小題:" << endl;
		cout << "此次有a b " << endl;
		cin >> No;
		switch (No) {
		case'a':
		{
			cout << "目前第2大題中第a小題" << endl;
			int const size = all_width * all_height;
			unsigned char* aerialcity512 = new unsigned char[size];
			read_file(512, 512, "aerialcity512.raw", aerialcity512);
			Mat house512Mat = Mat(512, 512, CV_8UC1, aerialcity512);

			unsigned char* aerialcity512_dft = new unsigned char[size];
			unsigned char* aerialcity512_idft = new unsigned char[size];
			Mat aerialcity512_dftMat = Mat(512, 512, CV_8UC1, aerialcity512_dft);
			Mat aerialcity512_idftMat = Mat(512, 512, CV_8UC1, aerialcity512_idft);
			flitering_frequency_domain_LPF(512, 512, aerialcity512, aerialcity512_dftMat, aerialcity512_idftMat, 5);
			cout<<"flitering_frequency_domain_DFT_5 mse : "<<msecul(all_width,all_height, aerialcity512, aerialcity512_idftMat.data)<<endl;
			cout << "flitering_frequency_domain_DFT_5 psnr : "<<psnrcul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data)<<" db"<<endl;
			imwrite("flitering_frequency_domain_DFT_5.png", aerialcity512_dftMat);
			imwrite("flitering_frequency_domain_LPF_5.png", aerialcity512_idftMat);

			flitering_frequency_domain_LPF(512, 512, aerialcity512, aerialcity512_dftMat, aerialcity512_idftMat, 25);
			cout << "flitering_frequency_domain_DFT_25 mse : " << msecul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << endl;
			cout << "flitering_frequency_domain_DFT_25 psnr : " << psnrcul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data)<<" db" << endl;;
			imwrite("flitering_frequency_domain_DFT_25.png", aerialcity512_dftMat);
			imwrite("flitering_frequency_domain_LPF_25.png", aerialcity512_idftMat);

			flitering_frequency_domain_LPF(512, 512, aerialcity512, aerialcity512_dftMat, aerialcity512_idftMat, 125);
			cout << "flitering_frequency_domain_DFT_125 mse : " << msecul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << endl;
			cout << "flitering_frequency_domain_DFT_125 psnr : " << psnrcul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data)<<" db" << endl;;
			imwrite("flitering_frequency_domain_DFT_125.png", aerialcity512_dftMat);
			imwrite("flitering_frequency_domain_LPF_125.png", aerialcity512_idftMat);

			break;

		}
		case 'b':
		{
			int const size = all_width * all_height;
			cout << "目前第2大題中第b小題" << endl;
			unsigned char* aerialcity512 = new unsigned char[size];
			read_file(512, 512, "aerialcity512.raw", aerialcity512);
			Mat house512Mat = Mat(512, 512, CV_8UC1, aerialcity512);

			unsigned char* aerialcity512_dft = new unsigned char[size];
			unsigned char* aerialcity512_idft = new unsigned char[size];
			Mat aerialcity512_dftMat = Mat(512, 512, CV_8UC1, aerialcity512_dft);
			Mat aerialcity512_idftMat = Mat(512, 512, CV_8UC1, aerialcity512_idft);
			flitering_frequency_domain_Gaussian_HPF(512, 512, aerialcity512, aerialcity512_dftMat, aerialcity512_idftMat, 5);
			cout << "flitering_frequency_domain_Gaussian_DFT_5 mse : " << msecul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << endl;
			cout << "flitering_frequency_domain_Gaussian_DFT_5 psnr : " << psnrcul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << " db" << endl;;
			imwrite("flitering_frequency_domain_Gaussian_DFT_5.png", aerialcity512_dftMat);
			imwrite("flitering_frequency_domain_Gaussian_HPF_5.png", aerialcity512_idftMat);

			flitering_frequency_domain_Gaussian_HPF(512, 512, aerialcity512, aerialcity512_dftMat, aerialcity512_idftMat, 25);
			cout << "flitering_frequency_domain_Gaussian_DFT_25 mse : " << msecul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << endl;
			cout << "flitering_frequency_domain_Gaussian_DFT_25 psnr : " << psnrcul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << " db" << endl;;
			imwrite("flitering_frequency_domain_Gaussian_DFT_25.png", aerialcity512_dftMat);
			imwrite("flitering_frequency_domain_Gaussian_HPF_25.png", aerialcity512_idftMat);

			flitering_frequency_domain_Gaussian_HPF(512, 512, aerialcity512, aerialcity512_dftMat, aerialcity512_idftMat, 125);
			cout << "flitering_frequency_domain_Gaussian_DFT_125 mse : " << msecul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << endl;
			cout << "flitering_frequency_domain_Gaussian_DFT_125 : " << psnrcul(all_width, all_height, aerialcity512, aerialcity512_idftMat.data) << " db" << endl;;
			imwrite("flitering_frequency_domain_Gaussian_DFT_125.png", aerialcity512_dftMat);
			imwrite("flitering_frequency_domain_Gaussian_HPF_125.png", aerialcity512_idftMat);
			break;
		}
		
		}//2-小switch
		break;

	}//case2的

	}//大題switch
	return  main();
}//main


//main

