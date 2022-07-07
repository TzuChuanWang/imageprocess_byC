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

int const width = 256;
int const height = 256;
//
int scalar = 2, percentage = 200;
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

void dft(int width, int height, unsigned char* src, unsigned char* result, unsigned char* dft_phase) {
	const int size = width * height;
	double* temp = new double[size] {};
	double* real = new double [size] {};
	double* imag = new double[size] {};
	double max = 0;
	double phase_max = 0;
	double* phase_temp = new double[size] {};
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			for (int x = 0; x < height; x++)
			{
				for (int y = 0; y < width; y++)
				{
					double powerX = v * x *1.0* (2 * PI) / height;
					double powerY = u * y *1.0* (2 * PI) / width;
					real[u + v * width] += pow(-1, x + y) * src[y + x * width] * cos(powerX + powerY);
					imag[u + v * width] += pow(-1, x + y) * src[y + x * width] * sin(powerX + powerY);
				}
			}					
		}
	}
	for (int i = 0; i < size; i++) {
		temp[i] = sqrt(real[i] * real[i] + imag[i] * imag[i]);
	}

	
	for (int i = 0; i < size; i++) {
		temp[i] = log(temp[i] + 1);
		if (temp[i] > max) {
			max=temp[i];
		}
	}
	for (int i = 0; i < size; i++) {
		result[i] = int(temp[i] * 255.0 / max);
	}
	QueryPerformanceCounter(&nEndTime);// 計時截止
	double time = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart;
	cout << "dft use : " << time << " ms" << endl;
	//dft phase
	for (int i = 0; i < size; i++) {

		/*phase_temp[i] = atan(imag[i]/real[i]);
		phase_temp[i] = phase_temp[i] *180/PI;*/
		
		phase_temp[i] = atan(-imag[i] / real[i]);
		phase_temp[i] = phase_temp[i] + 0.5 * PI;
		phase_temp[i] = phase_temp[i] * 255;
		phase_temp[i] = phase_temp[i] / PI;
	}
	for (int i = 0; i < size; i++) {
		phase_temp[i] = log(phase_temp[i] + 1);
		if (phase_temp[i] > phase_max) {
			phase_max = phase_temp[i];
		}
	}
	for (int i = 0; i < size; i++) {
		dft_phase[i] = int(phase_temp[i] * 255.0 / phase_max);
	}
}
int opencv_dft(int h, int w,unsigned char* src, Mat& dst, Mat& ph ) {
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	Mat imgi, imgo, pad, complexI;
	imgi = Mat(h, w, CV_8UC1, src);
	int R = getOptimalDFTSize(imgi.rows);
	int C = getOptimalDFTSize(imgi.cols);
	copyMakeBorder(imgi, pad, 0, R - imgi.rows, 0, C - imgi.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);
	//phase
	phase(planes[0], planes[1], ph, false);
	normalize(ph, ph, 0, 1, NORM_MINMAX);
	ph.convertTo(ph, CV_8UC1, 255, 0);
	//
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

	//test phase
	
	QueryPerformanceCounter(&nEndTime);// 計時截止
	double time = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart;
	cout << "opencv dft use : " << time << " ms" << endl;
	return 1;
}
void idft(int width, int height, unsigned char* src, unsigned char* result) {
	const int size = width * height;
	double* temp = new double[size] {};
	double* real = new double [size] {};
	double* imag = new double[size] {};
	double* idft_tempreal = new double[size] {};
	double* idft_tempimag = new double[size] {};
	double max = 0;
	double phase_max = 0;
	double* phase_temp = new double[size] {};
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;
	//QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	//QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			for (int x = 0; x < height; x++)
			{
				for (int y = 0; y < width; y++)
				{
					double powerX = v * x * 1.0 * (2 * PI) / height;
					double powerY = u * y * 1.0 * (2 * PI) / width;
					real[u + v * width] += pow(-1, x + y) * src[y + x * width] * cos(powerX + powerY);
					imag[u + v * width] += pow(-1, x + y) * src[y + x * width] * sin(powerX + powerY);
				}
			}
		}
	}
	
	

	//idft
	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			for (int x = 0; x < height; x++)
			{
				for (int y = 0; y < width; y++)
				{
					double powerX = v * x * 1.0 * (2 * PI) / height;
					double powerY = u * y * 1.0 * (2 * PI) / width;
					idft_tempreal[u + v * width] += (real[y + x * width] * cos(powerX + powerY) + sin(powerX + powerY) * imag[y + x * width]);
					idft_tempimag[u + v * width] += (cos(powerX + powerY) * imag[y + x * width] - sin(powerX + powerY) * real[y + x * width]);
				}
			}

		}
	}
	for (int i = 0; i < size; i++) {
		temp[i] = sqrt(idft_tempreal[i] * idft_tempreal[i] + idft_tempimag[i] * idft_tempimag[i]);
	}


	for (int i = 0; i < size; i++) {
		//temp[i] = log(temp[i] + 1);
		if (temp[i] > max) {
			max = temp[i];
		}
	}
	for (int i = 0; i < size; i++) {
		result[i] = int(temp[i] * 255.0 / max);
	}
	
}

int main() {

	

	///////////////////////////////////////////////////////////////////


	char No;
	cout << "請先輸入第幾大題:" << endl;
    cout << "此次有2 " << endl;
	cin >> No;
	switch (No)
	{
		///////////////////
	
	case'2':
	{
		cout << "目前在第二大題，請輸入第幾小題:" << endl;
		cout << "此次有a b c" << endl;
		cin >> No;
		switch (No) {
		case'a':
		{
			cout << "目前第2大題中第a小題" << endl;
			int const size = width * height;


			//2	

			unsigned char* rect256 = new unsigned char[size];
			read_file(256, 256, "rect256.raw", rect256);
			Mat rect256Mat = Mat(256, 256, CV_8UC1, rect256);

			unsigned char* rect256_dft = new unsigned char[size];
			unsigned char* rect256_dft_phase = new unsigned char[size];
			dft(256, 256, rect256, rect256_dft, rect256_dft_phase);/////////////記得開回來
			//dft_phase(256, 256, rect256,rect256_dft_phase);
			Mat rect256_dftMat = Mat(256, 256, CV_8UC1, rect256_dft);
			Mat rect256_dft_phaseMat = Mat(256, 256, CV_8UC1, rect256_dft_phase);
			imwrite("rect256_dft.png", rect256_dftMat);
			imwrite("rect256_dft_phase.png", rect256_dft_phaseMat);

			unsigned char* circle256 = new unsigned char[size];
			read_file(256, 256, "circle256.raw", circle256);
			Mat circle256Mat = Mat(256, 256, CV_8UC1, circle256);

			unsigned char* circle256_dft = new unsigned char[size];
			unsigned char* circle256_dft_phase = new unsigned char[size];
			dft(256, 256, circle256, circle256_dft, circle256_dft_phase);
			Mat circle256_dftMat = Mat(256, 256, CV_8UC1, circle256_dft);
			Mat circle256_dft_phaseMat = Mat(256, 256, CV_8UC1, circle256_dft_phase);
			imwrite("circle256_dft.png", circle256_dftMat);
			imwrite("circle256_dft_phase.png", circle256_dft_phaseMat);

			unsigned char* square256 = new unsigned char[size];
			read_file(256, 256, "square256.raw", square256);
			Mat square256Mat = Mat(256, 256, CV_8UC1, square256);

			unsigned char* square256_dft = new unsigned char[size];
			unsigned char* square256_dft_phase = new unsigned char[size];
			dft(256, 256, square256, square256_dft, square256_dft_phase);
			Mat square256_dftMat = Mat(256, 256, CV_8UC1, square256_dft);
			Mat square256_dft_phaseMat = Mat(256, 256, CV_8UC1, square256_dft_phase);
			imwrite("square256_dft.png", square256_dftMat);
			imwrite("square256_dft_phase.png", square256_dft_phaseMat);

			unsigned char* square256_rota = new unsigned char[size];
			read_file(256, 256, "square256_rota.raw", square256_rota);
			Mat square256_rotaMat = Mat(256, 256, CV_8UC1, square256_rota);

			unsigned char* square256_rota_dft = new unsigned char[size];
			unsigned char* square256_rota_dft_phase = new unsigned char[size];
			dft(256, 256, square256_rota, square256_rota_dft, square256_rota_dft_phase);
			Mat square256_rota_dftMat = Mat(256, 256, CV_8UC1, square256_rota_dft);
			Mat square256_rota_dft_phaseMat = Mat(256, 256, CV_8UC1, square256_rota_dft_phase);
			imwrite("square256_rota_dft.png", square256_rota_dftMat);
			imwrite("square256_rota_dft_phase.png", square256_rota_dft_phaseMat);
			break;

		}
		case 'b':
		{
			int const size = width * height;
			cout << "目前第2大題中第b小題" << endl;
			unsigned char* rect256 = new unsigned char[size];
			read_file(256, 256, "rect256.raw", rect256);
			Mat rect256Mat = Mat(256, 256, CV_8UC1, rect256);

			unsigned char* rect256_dft = new unsigned char[size];
			unsigned char* rect256_dft_phase = new unsigned char[size];
			Mat rect256_dftMat = Mat(256, 256, CV_8UC1, rect256_dft);
			Mat rect256_dft_phaseMat = Mat(256, 256, CV_8UC1, rect256_dft_phase);
			opencv_dft(256, 256, rect256, rect256_dftMat, rect256_dft_phaseMat);
			//dft_phase(256, 256, rect256,rect256_dft_phase);
			
			imwrite("rect256_dft_opencv.png", rect256_dftMat);
			imwrite("rect256_dft_phase_opencv.png", rect256_dft_phaseMat);
			unsigned char* circle256 = new unsigned char[size];
			read_file(256, 256, "circle256.raw", circle256);
			Mat circle256Mat = Mat(256, 256, CV_8UC1, circle256);

			unsigned char* circle256_dft = new unsigned char[size];
			unsigned char* circle256_dft_phase = new unsigned char[size];
			Mat circle256_dftMat = Mat(256, 256, CV_8UC1, circle256_dft);
			Mat circle256_dft_phaseMat = Mat(256, 256, CV_8UC1, circle256_dft_phase);
			opencv_dft(256, 256, circle256, circle256_dftMat, circle256_dft_phaseMat);
			imwrite("circle256_dft_opencv.png", circle256_dftMat);
			imwrite("circle256_dft_phase_opencv.png", circle256_dft_phaseMat);

			unsigned char* square256 = new unsigned char[size];
			read_file(256, 256, "square256.raw", square256);
			Mat square256Mat = Mat(256, 256, CV_8UC1, square256);

			unsigned char* square256_dft = new unsigned char[size];
			unsigned char* square256_dft_phase = new unsigned char[size];
			Mat square256_dftMat = Mat(256, 256, CV_8UC1, square256_dft);
			Mat square256_dft_phaseMat = Mat(256, 256, CV_8UC1, square256_dft_phase);
			opencv_dft(256, 256, square256, square256_dftMat, square256_dft_phaseMat);
			imwrite("square256_dft.png_opencv.png", square256_dftMat);
			imwrite("square256_dft_phase.png_opencv.png", square256_dft_phaseMat);

			unsigned char* square256_rota = new unsigned char[size];
			read_file(256, 256, "square256_rota.raw", square256_rota);
			Mat square256_rotaMat = Mat(256, 256, CV_8UC1, square256_rota);

			unsigned char* square256_rota_dft = new unsigned char[size];
			unsigned char* square256_rota_dft_phase = new unsigned char[size];
			Mat square256_rota_dftMat = Mat(256, 256, CV_8UC1, square256_rota_dft);
			Mat square256_rota_dft_phaseMat = Mat(256, 256, CV_8UC1, square256_rota_dft_phase);
			opencv_dft(256, 256, square256_rota, square256_rota_dftMat, square256_rota_dft_phaseMat);
		
			imwrite("square256_rota_dft_opencv.png", square256_rota_dftMat);
			imwrite("square256_rota_dft_phase_opencv.png", square256_rota_dft_phaseMat);
			break;
		}
		case 'c':
		{

			cout << "目前第2大題中第c小題" << endl;
			int const size = width * height;
			double psnr_rect = 0;
			double mse_rect = 0;
			FILE* saveFile;
			unsigned char* rect256 = new unsigned char[size];
			read_file(256, 256, "rect256.raw", rect256);
			Mat rect256Mat = Mat(256, 256, CV_8UC1, rect256);

			unsigned char* rect256_idft = new unsigned char[size];
			idft(256, 256, rect256, rect256_idft);
			Mat rect256_idftMat = Mat(256, 256, CV_8UC1, rect256_idft);
			psnr_rect = psnrcul(256, 256, rect256, rect256_idft); //測psnr
			mse_rect = msecul(256, 256, rect256, rect256_idft);//測mse
			imwrite("rect256_idft.png", rect256_idftMat);
			saveFile = fopen("rect256_idft.raw", "wb");
			fwrite(rect256_idft, 1, size, saveFile);
			cout << "rect256 MSE : " << mse_rect << endl;
			cout << "rect256 PSNR : " << psnr_rect << " db" << endl;
			///////////////////////////////////////////////////////////////////
			double psnr_circle256 = 0;
			double mse_circle256 = 0;
			unsigned char* circle256 = new unsigned char[size];
			read_file(256, 256, "circle256.raw", circle256);
			Mat circle256Mat = Mat(256, 256, CV_8UC1, circle256);

			unsigned char* circle256_idft = new unsigned char[size];
			idft(256, 256, circle256, circle256_idft);
			Mat circle256_idftMat = Mat(256, 256, CV_8UC1, circle256_idft);
			psnr_circle256 = psnrcul(256, 256, circle256, circle256_idft); //測psnr
			mse_circle256 = msecul(256, 256, circle256, circle256_idft);//測mse
			imwrite("circle256_idft.png", circle256_idftMat);
			saveFile = fopen("circle256_idft.raw", "wb");
			fwrite(circle256_idft, 1, size, saveFile);
			cout << "circle256 MSE : " << mse_circle256 << endl;
			cout << "circle256 PSNR : " << psnr_circle256 << " db" << endl;
			///////////////////////////////////////////////////////////////////
			double psnr_square256 = 0;
			double mse_square256 = 0;
			unsigned char* square256 = new unsigned char[size];
			read_file(256, 256, "square256.raw", square256);
			Mat square256Mat = Mat(256, 256, CV_8UC1, square256);

			unsigned char* square256_idft = new unsigned char[size];
			idft(256, 256, square256, square256_idft);
			Mat square256_idftMat = Mat(256, 256, CV_8UC1, square256_idft);
			psnr_square256 = psnrcul(256, 256, square256, square256_idft); //測psnr
			mse_square256 = msecul(256, 256, square256, square256_idft);//測mse
			imwrite("square256_idft.png", square256_idftMat);
			saveFile = fopen("square256_idft.raw", "wb");
			fwrite(square256_idft, 1, size, saveFile);
			cout << "square256 MSE : " << mse_square256 << endl;
			cout << "square256 PSNR : " << psnr_square256 << " db" << endl;
			///////////////////////////////////////////////////////////////////
			double psnr_square256_rota = 0;
			double mse_square256_rota = 0;
			unsigned char* square256_rota = new unsigned char[size];
			read_file(256, 256, "square256_rota.raw", square256_rota);
			Mat square256_rotaMat = Mat(256, 256, CV_8UC1, square256_rota);

			unsigned char* square256_rota_idft = new unsigned char[size];
			idft(256, 256, square256_rota, square256_rota_idft);
			Mat square256_rota_idftMat = Mat(256, 256, CV_8UC1, square256_rota_idft);
			psnr_square256_rota = psnrcul(256, 256, square256_rota, square256_rota_idft); //測psnr
			mse_square256_rota = msecul(256, 256, square256_rota, square256_rota_idft);//測mse
			imwrite("square256_rota_idft.png", square256_rota_idftMat);
			saveFile = fopen("square256_rota_idft.raw", "wb");
			fwrite(square256_rota_idft, 1, size, saveFile);
			cout << "square256_rota MSE : " << mse_square256_rota << endl;
			cout << "square256_rota PSNR : " << psnr_square256_rota << " db" << endl;
			break;
		}
		}//1-小switch
		break;

	}//case2的
	
	}//大題switch
	return  main();
}//main


//main

