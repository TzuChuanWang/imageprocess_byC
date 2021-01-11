#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES
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
double band_reject_fliter_ideal(int all_heigh, int all_width, int now_heigh, int now_width, double D0,double W) {


	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;
	double D = sqrt(pow((now_heigh - all_heigh / 2), 2) + pow((now_width - all_width / 2), 2));
	double h =0;
	if ((D0 - W/ 2 <= D) &&(D <= D0 + W / 2)) { h = 0; }
	else h = 1;
	return h;
}
double band_reject_fliter_butterworth(int all_heigh, int all_width, int now_heigh, int now_width, double D0, double W) {


	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;
	double D = sqrt(pow((now_heigh - all_heigh / 2), 2) + pow((now_width - all_width / 2), 2));
	double h ;
	h = 1 / (1+  pow((D * W/(D * D - D0 * D0)),2));
	return h;
}
double band_reject_fliter_gaussian(int all_heigh, int all_width, int now_heigh, int now_width, double D0, double W) {


	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;
	double D = sqrt(pow((now_heigh - all_heigh / 2), 2) + pow((now_width - all_width / 2), 2));
	double h;
	h = 1-exp(-0.5*pow(((D*D-D0*D0)/D/W),2));
	return h;
}
double notch_reject_fliter_ideal(int all_heigh, int all_width, int now_heigh, int now_width, double D0, double W,int u0=81,int v0=16) {


	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;

	double D1 = pow((now_heigh - all_heigh / 2-v0), 2) + pow((now_width - all_width / 2-u0), 2);
	double D2 = pow((now_heigh - all_heigh / 2+v0), 2) + pow((now_width - all_width / 2+u0), 2);
	double h;
	if ((D1< D0) || (D2 < D0 )) { h = 0; }
	else h = 1;
	return h;
}
double notch_reject_fliter_butterworth(int all_heigh, int all_width, int now_heigh, int now_width, double D0, int u0 = 81, int v0 = 16) {


	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;
	int n = 1;
	double D1 = pow((now_heigh - all_heigh / 2 - v0), 2) + pow((now_width - all_width / 2 - u0), 2);
	double D2 = pow((now_heigh - all_heigh / 2 + v0), 2) + pow((now_width - all_width / 2 + u0), 2);
	double h;
	h = 1 / (1 + pow(D0 * D0 / D1 / D2, 2));
	return h;
}
double notch_reject_fliter_gaussian(int all_heigh, int all_width, int now_heigh, int now_width, double D0, int u0 = 81, int v0 =16) {


	if (now_heigh < all_heigh / 2) { now_heigh = now_heigh + all_heigh / 2; }
	else  now_heigh = now_heigh - all_heigh / 2;
	if (now_width < all_width / 2) { now_width = now_width + all_width / 2; }
	else now_width = now_width - all_width / 2;
	
	double D1 = pow((now_heigh - all_heigh / 2 - v0), 2) + pow((now_width - all_width / 2 - u0), 2);
	double D2 = pow((now_heigh - all_heigh / 2 + v0), 2) + pow((now_width - all_width / 2 + u0), 2);
	double h;
	h = 1 - exp(1/2*(D1*D2/D0/D0));
	return h;
}
double inverse_fliter_hr(int all_heigh, int all_width, int now_heigh, int now_width) {

	double dist = 0;
	double hr = 0;
	double hi = 0;
	double temp = 0;
	/*if (now_width == 0) { hr = 1; }
	else {
		temp = M_PI * -0.01 * (now_width);
		hr = (double)((1.0 / temp) * sin(temp) * cos(temp));
		hi = (double)((1.0 / temp) * sin(temp) * -sin(temp));
		dist = (double)(hr * hr + hi * hi);
		hr = (double)(hr / dist);
		hi = (double)(-hi / dist);
	}*/
	if (now_width == 0) { hr = 1; }
	else {
		temp = M_PI * -0.01 * now_width;
		hr = (double)((1.0 / temp) * sin(temp) * cos(temp));
		//hi = (double)((1.0 / temp) * sin(temp) * -sin(temp));
		//dist = (double)(hr * hr + hi * hi);
		//hr = (double)(hr / dist);
		//hi = (double)(-hi / dist);
	}
	return hr;
	
}
double inverse_fliter_hi(int all_heigh, int all_width, int now_heigh, int now_width) {

	

	double dist = 0;
	double hr = 0;
	double hi = 0;
	double temp = 0;
	if (now_width == 0) { hi = 1; }
	else {
		temp = M_PI * -0.01 * (now_width);
		//hr = (double)((1.0 / temp) * sin(temp) * cos(temp));
		hi = (double)((1.0 / temp) * sin(temp) * -sin(temp));
		//dist = (double)(hr * hr + hi * hi);
		//hr = (double)(hr / dist);
		//hi = (double)(-hi / dist);
	}
	//cout << -hi << endl;
	return hi;
	
}
void periodic_noise(int heigh, int width, unsigned char* src, Mat& dst, Mat& invDFT, int fliternum, double D0,double W) {
	//log//
	int const size = heigh * width;
	float* cc = new float[size];
	for (int i = 0; i < size; i++) {
		cc[i] = src[i];
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
	float* dst_realData = new float[size];
	float* dst_imageData = new float[size];
	double h = 0;
	for (int i = 0; i < heigh; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//dstData[j + i * width] = dftData[ j  + i  * width];
			if (fliternum == 1) { h = band_reject_fliter_ideal(heigh, width, i, j, D0, W);  }
			else if (fliternum == 2) { h = band_reject_fliter_butterworth(heigh, width, i, j, D0, W); }
			else if (fliternum == 3) { h = band_reject_fliter_gaussian(heigh, width, i, j, D0, W);  }
			else if (fliternum == 4) { h = notch_reject_fliter_ideal(heigh, width, i, j, D0, W);  }
			else if (fliternum == 5) { h = notch_reject_fliter_gaussian(heigh, width, i, j, D0);  }
			else if (fliternum == 6) { h = notch_reject_fliter_butterworth(heigh, width, i, j, D0);}

			
			else { h = 1;  }
			dst_realData[j + i * width] = dft_realData[j + i * width] * h;
			dst_imageData[j + i * width] = dft_imageData[j + i * width] * h;
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
void deblur_inverse(int heigh, int width, unsigned char* src, Mat& dst, Mat& invDFT) {
	//log//
	int const size = heigh * width;
	float* cc = new float[size];
	for (int i = 0; i < size; i++) {
		cc[i] = src[i];
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
	float* dst_realData = new float[size];
	float* dst_imageData = new float[size];
	double hr = 0;
	double hi = 0;
	for (int i = 0; i < heigh; i++)
	{
		for (int j = 0; j < width; j++)
		{
			hr = inverse_fliter_hr(heigh, width, i, j);
			hi = inverse_fliter_hi(heigh, width, i, j);

			dst_realData[j + i * width] = dft_realData[j + i * width] * hr - dft_imageData[j + i * width] * hi;
			dst_imageData[j + i * width] = dft_imageData[j + i * width] * hr + dft_realData[j + i * width] * hi;
		}
	}

	planes[0] = Mat(heigh, width, CV_32F, dst_realData);
	planes[1] = Mat(heigh, width, CV_32F, dst_imageData);
	//////////////////////////////////////////////////////////////////////////////
	//idft
	Mat complexI2;

	merge(planes, 2, complexI2);
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



	//invDFT = Mat(heigh, width, CV_8UC1, dstData);
	Mat tmp;
	idft(complexI2, tmp, DFT_SCALE | DFT_REAL_OUTPUT);//離散傅立葉逆變換


		
	normalize(tmp, invDFT, 0, 1, NORM_MINMAX);
	////cout << imgo << endl;
	invDFT.convertTo(invDFT, CV_8UC1, 255, 0);

}
void deblur_wiener(int heigh, int width, unsigned char* src, Mat& dst, Mat& invDFT, double K = 0.1) {
	int const size = heigh * width;
	float* cc = new float[size];
	for (int i = 0; i < size; i++) {
		cc[i] = src[i];
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
	float* dst_realData = new float[size];
	float* dst_imageData = new float[size];
	double hr = 0;
	double hi = 0;
	double wie_h = 0;
	for (int i = 0; i < heigh; i++)
	{
		for (int j = 0; j < width; j++)
		{
			hr = inverse_fliter_hr(heigh, width, i, j);
			hi = inverse_fliter_hi(heigh, width, i, j);

			hr = inverse_fliter_hr(heigh, width, i, j);
			hi = inverse_fliter_hi(heigh, width, i, j);
			wie_h = hr * hr + hi * hi;
			hr = hr / (wie_h + K)  ;
			hi = hi / (wie_h + K) ;

			dst_realData[j + i * width] = dft_realData[j + i * width] * hr - dft_imageData[j + i * width] * hi;
			dst_imageData[j + i * width] = dft_imageData[j + i * width] * hr + dft_realData[j + i * width] * hi;
		}
	}

	planes[0] = Mat(heigh, width, CV_32F, dst_realData);
	planes[1] = Mat(heigh, width, CV_32F, dst_imageData);
	//////////////////////////////////////////////////////////////////////////////
	//idft
	Mat complexI2;

	merge(planes, 2, complexI2);
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



	//invDFT = Mat(heigh, width, CV_8UC1, dstData);
	Mat tmp;
	idft(complexI2, tmp, DFT_SCALE | DFT_REAL_OUTPUT);//離散傅立葉逆變換



	normalize(tmp, invDFT, 0, 1, NORM_MINMAX);
	////cout << imgo << endl;
	invDFT.convertTo(invDFT, CV_8UC1, 255, 0);

}

int main() {


	int const size = all_width * all_height;
	unsigned char* motion_flower512 = new unsigned char[size];
	read_file(512, 512, "motion_flower.raw", motion_flower512);
	Mat motion_flower512Mat = Mat(512, 512, CV_8UC1, motion_flower512);

	unsigned char* motion_flower512_dft = new unsigned char[size];
	unsigned char* motion_flower512_idft = new unsigned char[size];
	Mat motion_flower512_dftMat = Mat(512, 512, CV_8UC1, motion_flower512_dft);
	Mat motion_flower512_idftMat = Mat(512, 512, CV_8UC1, motion_flower512_idft);
	//deblur_inverse(512, 512, motion_flower512, motion_flower512_dftMat, motion_flower512_idftMat, 7, 50, 60);

	//deblur_wiener(512, 512, motion_flower512, motion_flower512_dftMat, motion_flower512_idftMat, 7, 50, 60);
	
	//deblur_inverse(512, 512, motion_flower512, motion_flower512_dftMat, motion_flower512_idftMat);
	//deblur_wiener_test(512, 512, motion_flower512, motion_flower512_dftMat, motion_flower512_idftMat);
	deblur_inverse(512, 512, motion_flower512, motion_flower512_dftMat, motion_flower512_idftMat);
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
		cout << "目前在第一大題，請輸入第幾小題:" << endl;
		cout << "此次有a b " << endl;
		cin >> No;
		switch (No) {
		case'a':
		{
			cout << "目前第一大題中第a小題" << endl;
			int const size = all_width * all_height;
			unsigned char* kirby512 = new unsigned char[size];
			read_file(512, 512, "kirby512.raw", kirby512);
			Mat kirby512Mat = Mat(512, 512, CV_8UC1, kirby512);

			unsigned char* kirby512_dft = new unsigned char[size];
			unsigned char* kirby512_idft = new unsigned char[size];
			Mat kirby512_dftMat = Mat(512, 512, CV_8UC1, kirby512_dft);
			Mat kirby512_idftMat = Mat(512, 512, CV_8UC1, kirby512_idft);
			periodic_noise(512, 512, kirby512, kirby512_dftMat, kirby512_idftMat, 4, 80, 60);
			imwrite("periodic_noise_notch_reject_fliter_ideal.png", kirby512_idftMat);
			periodic_noise(512, 512, kirby512, kirby512_dftMat, kirby512_idftMat, 5, 50, 60);
			//imwrite("periodic_noise_notch_reject_fliter_gaussian.png", kirby512_idftMat);
			periodic_noise(512, 512, kirby512, kirby512_dftMat, kirby512_idftMat, 6, 80, 60);
			imwrite("periodic_noise_notch_reject_fliter_butterworth.png", kirby512_idftMat);
			break;

		}
		case 'b':
		{
			
			cout << "目前第一大題中第b小題" << endl;
			int const size = all_width * all_height;
			unsigned char* kirby512 = new unsigned char[size];
			read_file(512, 512, "kirby512.raw", kirby512);
			Mat kirby512Mat = Mat(512, 512, CV_8UC1, kirby512);

			unsigned char* kirby512_dft = new unsigned char[size];
			unsigned char* kirby512_idft = new unsigned char[size];
			Mat kirby512_dftMat = Mat(512, 512, CV_8UC1, kirby512_dft);
			Mat kirby512_idftMat = Mat(512, 512, CV_8UC1, kirby512_idft);
			periodic_noise(512, 512, kirby512, kirby512_dftMat, kirby512_idftMat, 1, 83, 30);
			//opencv_dft(all_height, all_width, kirby512, kirby512_dftMat, kirby512_idftMat);
			imwrite("periodic_noise_band_reject_fliter_ideal.png", kirby512_idftMat);
			periodic_noise(512, 512, kirby512, kirby512_dftMat, kirby512_idftMat, 2, 83, 30);
			imwrite("periodic_noise_band_reject_fliter_butterworth.png", kirby512_idftMat);
			periodic_noise(512, 512, kirby512, kirby512_dftMat, kirby512_idftMat, 3, 83, 30);
			//imwrite("periodic_noise_band_reject_fliter_gaussian.png", kirby512_idftMat);
			break;
		}

		}//1-小switch
		break;

	}//case1的
	case'2':
	{
		cout << "目前在第二題" << endl;
		int const size = all_width * all_height;
		unsigned char* motion_flower512 = new unsigned char[size];
		read_file(512, 512, "motion_flower.raw", motion_flower512);
		Mat motion_flower512Mat = Mat(512, 512, CV_8UC1, motion_flower512);

		unsigned char* motion_flower512_dft = new unsigned char[size];
		unsigned char* motion_flower512_idft = new unsigned char[size];
		Mat motion_flower512_dftMat = Mat(512, 512, CV_8UC1, motion_flower512_dft);
		Mat motion_flower512_idftMat = Mat(512, 512, CV_8UC1, motion_flower512_idft);
		deblur_inverse(512, 512, motion_flower512, motion_flower512_dftMat, motion_flower512_idftMat);
		imwrite("inverse_fliter.png", motion_flower512_idftMat);

		deblur_wiener(512, 512, motion_flower512, motion_flower512_dftMat, motion_flower512_idftMat);
		imwrite("wiener_fliter.png", motion_flower512_idftMat);
		break;

	}//case2的

	}//大題switch
	return  main();
}//main


//main

