#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <ctime>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

int const width = 256;
int const height = 256;



void neighborbig(int ori_width, int ori_height,float scale,unsigned char* orilena , unsigned char* newlena) {//放大
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	int new_width = ori_width * scale;
	int new_height = ori_height * scale;
	while(k < new_width) {
		while (l < new_height){
				newlena[k * new_width + l] = orilena[i * ori_width + j];
				l++;
				j= l / scale;
				//cout << " k:"<<k<<" l:"<<l;
		}
		l = 0;
		k++;
		i=k/scale;		
	}
}
void neighborshrink(int ori_width, int ori_height, float scale, unsigned char* orilena, unsigned char* newlena){
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int y = 1/ scale;
	int new_width = ori_width * scale;
	int new_height = ori_height * scale;
	while (i < new_width) {
		while (j < new_height) {
			newlena[i * new_width + j] = orilena[(k * y) * ori_width + (l *y)];
			j = j + 1;
			l = l + 1;
		}
		i++;
		k = k + 1;
		l = 0;
		j = 0;
	}
}
void bilinear(int ori_width, int ori_height, float scale, unsigned char* orilena, unsigned char* newlena) {
	
	int new_width = ori_width * scale;
	int new_height = ori_height * scale;
	
	double i = 0;
	double j = 0;
	int inew = 0;
	int jnew = 0;
	int ap = 0;
	int bp = 0;
	double a = 0;
	double b = 0;
	int ox1, oy1,  ox2, oy2;
	int x, y, x1, y1, x2, y2;
	int w = 0;
	while (inew < new_width) {
		while (jnew < new_height) {
			i = inew / scale;
			j = jnew / scale;
			ox1 = inew / scale;
			oy1 = jnew / scale;
			ox2 = ox1 + 1;
			oy2 = oy1 + 1;
			x = inew;
			y = jnew;
			x1 = ox1;
			x2 = ox1+1 ;
			y1 = oy1;
			y2 = oy1+1 ;
			ap = x2-x1;
			bp = y2 - y1;
			a = i - x1;
			b = j - y1;
			//
			
			newlena[inew * new_width + jnew] = ((ap - a) * (bp - b) * orilena[ox1 * ori_width + oy1] + b * (ap - a) * orilena[ox1 * ori_width + oy2] + a * (bp - b) * orilena[ox2 * ori_width + oy1]  + a * b * orilena[ox1 * ori_width + oy1]) /(bp*ap);
			
			jnew++;
			
		}
		inew++;		
		jnew = 0;
	}
}
double bilcubicbig_weight(double a, double x) {
	if (fabs(x) <= 1)
	{
		return (a + 2) * pow(fabs(x), 3) - (a + 3) * pow(fabs(x), 2) + 1;
	}
	else if (fabs(x) > 1 && fabs(x) < 2)
	{
		return( a * pow(fabs(x), 3) - 5 * a * pow(fabs(x), 2) + 8 * a * fabs(x) - 4 * a);
	}
	else
	{
		return 0;
	}
}
void bilcubic(int ori_width, int ori_height, float scale, unsigned char* orilena, unsigned char* newlena) {
	int i = 0,j=0,t=0;
	double a =-0.5; 
	double temp = 0;
	int inew = 0,jnew=0;
	int ox1, oy1,  oy2;
	int new_width = ori_width * scale;
	int new_height = ori_height * scale;
	double xd, yd,px,py;
	while (inew < new_width) {
		while (jnew < new_height) {
			py = inew / scale;
			px = jnew / scale;
			ox1 = jnew / scale;
			oy1 = inew / scale;
			xd = px - ox1 ;
			yd = py - oy1 ;			
			temp=orilena[(oy1 - 1) * ori_width + (ox1 - 1)] * bilcubicbig_weight(a, xd + 1)*  bilcubicbig_weight(a, yd + 1) + orilena[(oy1 - 1) * ori_width + (ox1)] * bilcubicbig_weight(a, xd )* bilcubicbig_weight(a, yd + 1)+ orilena[(oy1 - 1) * ori_width + (ox1+1)]   * bilcubicbig_weight(a, 1-xd)   * bilcubicbig_weight(a, yd + 1)+ orilena[(oy1 - 1) * ori_width + (ox1+2)]   * bilcubicbig_weight(a, 2-xd)   * bilcubicbig_weight(a, yd + 1)+
				 orilena[(oy1 )    * ori_width + (ox1 - 1)] * bilcubicbig_weight(a, xd + 1) * bilcubicbig_weight(a, yd ) +    orilena[(oy1)     * ori_width + (ox1)] * bilcubicbig_weight(a, xd) * bilcubicbig_weight(a, yd )   + orilena[(oy1 )    * ori_width + (ox1 + 1)] * bilcubicbig_weight(a, 1 - xd) * bilcubicbig_weight(a, yd)    + orilena[(oy1 )	* ori_width + (ox1 + 2)] * bilcubicbig_weight(a, 2 - xd) * bilcubicbig_weight(a, yd )+
				 orilena[(1 + oy1) * ori_width + (ox1 - 1)] * bilcubicbig_weight(a, xd + 1) * bilcubicbig_weight(a, 1-yd) +   orilena[(1 + oy1 )* ori_width + (ox1)] * bilcubicbig_weight(a, xd) * bilcubicbig_weight(a, 1 - yd)+ orilena[(1 + oy1) * ori_width + (ox1 + 1)] * bilcubicbig_weight(a, 1 - xd) * bilcubicbig_weight(a, 1 - yd)+ orilena[(1 + oy1) * ori_width + (ox1 + 2)] * bilcubicbig_weight(a, 2 - xd) * bilcubicbig_weight(a, 1 - yd)+
				 orilena[(2 + oy1 )* ori_width + (ox1 - 1)] * bilcubicbig_weight(a, xd + 1) * bilcubicbig_weight(a, 2-yd) +   orilena[(2 + oy1) * ori_width + (ox1)] * bilcubicbig_weight(a, xd) * bilcubicbig_weight(a, 2 - yd)+ orilena[(2 + oy1) * ori_width + (ox1 + 1)] * bilcubicbig_weight(a, 1 - xd) * bilcubicbig_weight(a, 2 - yd)+ orilena[(2 + oy1) * ori_width + (ox1 + 2)] * bilcubicbig_weight(a, 2 - xd) * bilcubicbig_weight(a, 2- yd );
			newlena[inew * new_width + jnew] = (int)temp;

			jnew++;
		}	
		inew++;
		jnew = 0;
	}	
}
double msecul(int ori_width, int ori_height, unsigned char* orilena, unsigned char* newlena){

	int i = 0;
	int j = 0;
	double mse = 0;
	int size = ori_width * ori_height;
	while (i < ori_width) {
		while (j < ori_height) {
			mse = mse+pow((orilena[i * ori_width + j]- newlena[i * ori_width + j] ),2) ;
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
	psnr = 10 * log10(255 *255 / mse);
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
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;

	unsigned char* lena = new unsigned char[size];
	fread(lena, sizeof(unsigned char), size, lenaFile);
	FILE* saveFile;

	Mat lenaMat = Mat(height, width, CV_8UC1, lena);
	/*imshow("lenaFile", lenaMat);
	waitKey();*/
	//destroyWindow("lenaFile");

	////////////////
	//unsigned char* testlena = new unsigned char[128 * 128];
	//neighborbig(256, 256, 2.5,lena, testlena);
	//neighborshrink(256, 256, 0.5, lena, testlena);
	//bilinear(256, 256, 0.5, lena, testlena);

	/*Mat testlenaMat = Mat(128,128, CV_8UC1, testlena);
	imshow("lenaFile", testlenaMat);
	waitKey();
	destroyWindow("testlenaFile");*/
	cout << "請先輸入第幾大題:" << endl;
	cout << "此次有1 2 3 5 " << endl;
	cin >> No;
	switch (No)
	{
		///////////////////
	case '1':
	{
		cout << "目前在第一大題，請輸入第幾小題:" << endl;
		cout << "此次有1 2 3" << endl;
		cin >> No;
		switch (No) {
		case'1':
		{
			cout << "目前第1大題中第1小題 zoomeye2:1" << endl;
			cout << "按任意鍵重新選擇...." << endl;
			unsigned char* eyelena = new unsigned char[size];
			int i = 0;
			int j = 0;
			while (i < width) {
				while (j < height) {
					eyelena[i * width + j] = lena[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 126;
			j = 123;
			int k = 120;
			int l = 112;

			while (i < 138) {
				while (j < 145) {
					eyelena[k * width + l] = lena[i * width + j];
					eyelena[(k + 1) * width + l] = lena[i * width + j];
					eyelena[k * width + l + 1] = lena[i * width + j];
					eyelena[(k + 1) * width + 1 + l] = lena[i * width + j];
					j = j + 1;
					l = l + 2;
				}
				i++;
				k = k + 2;
				l = 112;
				j = 123;
			}
			i = 129;
			j = 157;
			k = 124;
			l = 149;

			while (i < 139) {
				while (j < 173) {
					eyelena[k * width + l] = lena[i * width + j];
					eyelena[(k + 1) * width + l] = lena[i * width + j];
					eyelena[k * width + l + 1] = lena[i * width + j];
					eyelena[(k + 1) * width + 1 + l] = lena[i * width + j];
					j = j + 1;
					l = l + 2;
				}
				i++;
				k = k + 2;
				l = 149;
				j = 157;
			}

			Mat eyelenaMat = Mat(height, width, CV_8UC1, eyelena);
			imshow("eyelenaFile", eyelenaMat);
			waitKey();
			destroyWindow("eyelenaFile");
			imwrite("1.1.png", eyelenaMat);
			break;
		}
		case '2':
		{
			//1.2
			cout << "目前第1大題中第2小題 shrinklena1:2" << endl;

			unsigned char* shrinklena = new unsigned char[128 * 128];
			neighborshrink(256, 256, 0.5, lena, shrinklena);
			Mat shrinklenaMat = Mat(128, 128, CV_8UC1, shrinklena);
			cout << "目前顯示No blur shrinklena" << endl;
			imshow("lenaFile", shrinklenaMat);
			waitKey();
			destroyWindow("lenaFile");
			imwrite("1.2_Noblur.png", shrinklenaMat);

			char ori3_File[] = "lena_256_blur.raw";
			FILE* lena_256_blurFile;
			unsigned char* lena256blur = new unsigned char[size];
			unsigned char* blurshrinklena = new unsigned char[128 * 128];
			fopen_s(&lena_256_blurFile, ori3_File, "rb");
			fread(lena256blur, sizeof(unsigned char), size, lena_256_blurFile);
			neighborshrink(256, 256, 0.5, lena256blur, blurshrinklena);
			Mat blurshrinklenaMat = Mat(128, 128, CV_8UC1, blurshrinklena);
			cout << "目前顯示blurshrinklena" << endl;
			imshow("blurshrinklenaFile", blurshrinklenaMat);
			waitKey();
			destroyWindow("blurshrinklenaFile");
			
			imwrite("1.2_blurshrink.png", blurshrinklenaMat);
			break;
		}
		case '3':
		{
			//1.3
			//use neighbor  
			//↑2.5↓2
			double time_neighborlena_bs=0, time_neighborlena_sb=0, time_neighborlena_b=0;
			cout << "目前第1大題中第3小題 zoomin zoomout" << endl;
			cout << "目前顯示neighbor↑2.5↓2" << endl;
			cout << "按任意鍵下一張...." << endl;
			unsigned char* neighbor_640 = new unsigned char[640 * 640];
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數


			neighborbig(256, 256, 2.5, lena, neighbor_640);
			unsigned char* neighbor_320 = new unsigned char[320 * 320];
			neighborshrink(640, 640, 0.5, neighbor_640, neighbor_320);
			QueryPerformanceCounter(&nEndTime);
			time_neighborlena_bs=(double)(nEndTime.QuadPart - m_nBeginTime.QuadPart)  / m_nFreq.QuadPart ;
			//cout << time_neighborlena_bs << endl;
			Mat neighborlena_bsMat = Mat(320, 320, CV_8UC1, neighbor_320);
			imshow("neighborlenaFile", neighborlena_bsMat);
			waitKey();
			destroyWindow("neighborlenaFile");
			imwrite("1.3_neighbor_bigshrink.png", neighborlena_bsMat);
			//↓2↑2.5

			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示neighbor↓2↑2.5" << endl;
			cout << "按任意鍵下一張...." << endl;
			unsigned char* neighbor_128 = new unsigned char[128 * 128];
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			neighborshrink(256, 256, 0.5, lena, neighbor_128);
			neighborbig(128, 128, 2.5, neighbor_128, neighbor_320);
			QueryPerformanceCounter(&nEndTime);
			time_neighborlena_sb = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
			//cout << time_neighborlena_sb << endl;
			
			Mat neighborlena_sbMat = Mat(320, 320, CV_8UC1, neighbor_320);
			imshow("neighborlenaFile", neighborlena_sbMat);
			waitKey();
			destroyWindow("neighborlenaFile");
			imwrite("1.3_neighbor_shrinkbig.png", neighborlena_sbMat);
			//↑1.25
			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示neighbor↑1.25" << endl;
			cout << "按任意鍵下一張...." << endl;
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			neighborbig(256, 256, 1.25, lena, neighbor_320);
			QueryPerformanceCounter(&nEndTime);
			time_neighborlena_b = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
			//cout << time_neighborlena_b << endl;
			Mat neighborlena_bMat = Mat(320, 320, CV_8UC1, neighbor_320);
			imshow("neighborlenaFile", neighborlena_bMat);
			waitKey();
			destroyWindow("neighborlenaFile");
			imwrite("1.3_neighbor_big.png", neighborlena_bMat);
			//use bilinear
			//↑2.5↓2
			double time_bilinearlena_bs = 0, time_bilinearlena_sb = 0, time_bilinearlena_b = 0;
			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示bilinear↑2.5↓2" << endl;
			cout << "按任意鍵下一張...." << endl;
			unsigned char* bilinear_640 = new unsigned char[640 * 640];
			unsigned char* bilinear_320 = new unsigned char[320 * 320];
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			bilinear(256, 256, 2.5, lena, bilinear_640);			
			bilinear(640, 640, 0.5, bilinear_640, bilinear_320);			
			QueryPerformanceCounter(&nEndTime);
			time_bilinearlena_bs = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart)  / m_nFreq.QuadPart;
			//cout << time_bilinearlena_bs << endl;
			Mat bilinearlena_bsMat = Mat(320, 320, CV_8UC1, bilinear_320);
			imshow("bilinearlena_bsFile", bilinearlena_bsMat);
			waitKey();
			destroyWindow("bilinearlena_bsFile");
			imwrite("1.3_bilinear_bigshrink.png", bilinearlena_bsMat);
			//↓2↑2.5
			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示bilinear↓2↑2.5" << endl;
			cout << "按任意鍵下一張...." << endl;
			unsigned char* bilinear_128 = new unsigned char[128 * 128];
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			bilinear(256, 256, 0.5, lena, bilinear_128);
			bilinear(128, 128, 2.5, bilinear_128, bilinear_320);
			QueryPerformanceCounter(&nEndTime);
			time_bilinearlena_sb = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart)  / m_nFreq.QuadPart;
			//cout << time_bilinearlena_sb << endl;
			Mat bilinearlena_sbMat = Mat(320, 320, CV_8UC1, bilinear_320);
			imshow("bilinearlena_sbFile", bilinearlena_sbMat);
			waitKey();
			destroyWindow("bilinearlena_sbFile");
			imwrite("1.3_bilinear_shrinkbig.png", bilinearlena_sbMat);
			//↑1.25
			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示bilinear↑1.25" << endl;
			cout << "按任意鍵下一張...." << endl;
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			bilinear(256, 256, 1.25, lena, bilinear_320);
			QueryPerformanceCounter(&nEndTime);
			time_bilinearlena_b = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart)  / m_nFreq.QuadPart;
			//cout << time_bilinearlena_b << endl;
			Mat bilinearlena_bMat = Mat(320, 320, CV_8UC1, bilinear_320);
			imshow("bilinearlena_bFile", bilinearlena_bMat);
			waitKey();
			destroyWindow("bilinearlena_bFile");
			imwrite("1.3_bilinear_big.png", bilinearlena_bMat);
			//use bilcubic
			//↑2.5↓2
			double time_bilcubiclena_bs = 0, time_bilcubiclena_sb = 0, time_bilcubiclena_b = 0;
			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示bilcubic↑2.5↓2" << endl;
			cout << "按任意鍵下一張...." << endl;
			unsigned char* bilcubic_640 = new unsigned char[640 * 640];
			unsigned char* bilcubic_320 = new unsigned char[320 * 320];
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			bilcubic(256, 256, 2.5, lena, bilcubic_640);
			bilcubic(640, 640, 0.5, bilcubic_640, bilcubic_320);
			QueryPerformanceCounter(&nEndTime);
			time_bilcubiclena_bs = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart)  / m_nFreq.QuadPart;
			//cout << time_bilcubiclena_bs << endl;
			Mat bilcubiclena_bsMat = Mat(320, 320, CV_8UC1, bilcubic_320);
			imshow("bilcubiclena_bsFile", bilcubiclena_bsMat);
			waitKey();
			destroyWindow("bilcubiclena_bsFile");
			imwrite("1.3_bilcubic_bigshrink.png", bilinearlena_bMat);
			//↓2↑2.5
			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示bilcubic↓2↑2.5" << endl;
			cout << "按任意鍵下一張...." << endl;
			unsigned char* bilcubic_128 = new unsigned char[128 * 128];
			
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			bilcubic(256, 256, 0.5, lena, bilcubic_128);
			bilcubic(128, 128, 2.5, bilcubic_128, bilcubic_320);
			QueryPerformanceCounter(&nEndTime);
			time_bilcubiclena_sb = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart)  / m_nFreq.QuadPart;
			//cout << time_bilcubiclena_sb << endl;
			Mat bilcubiclena_sbMat = Mat(320, 320, CV_8UC1, bilcubic_320);
			imshow("bilcubiclena_sbFile", bilcubiclena_sbMat);
			waitKey();
			destroyWindow("bilcubiclena_sbFile");
			imwrite("1.3_bilcubic_shrinkbig.png", bilinearlena_bMat);
			//↑1.25
			cout << "目前第1大題中第3小題" << endl;
			cout << "目前顯示bilcubic↑1.25" << endl;
			cout << "按任意鍵重新選擇題目...." << endl;
			QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
			QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
			bilcubic(256, 256, 1.25, lena, bilcubic_320);
			QueryPerformanceCounter(&nEndTime);
			time_bilcubiclena_b = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart)  / m_nFreq.QuadPart;
			//cout << time_bilcubiclena_b << endl;
			
			Mat bilcubiclena_bMat = Mat(320, 320, CV_8UC1, bilcubic_320);
			imshow("bilcubiclena_bFile", bilcubiclena_bMat);
			waitKey();
			destroyWindow("bilcubiclena_bFile");
			imwrite("1.3_bilcubic_big.png", bilinearlena_bMat);
			cout << "nearest neighbor 2.5 Zooming -> 2 Shrinking Use :"<< time_neighborlena_bs<<" s" << endl;
			cout << "nearest neighbor 2 Shrinking -> 2.5 Zooming Use :" << time_neighborlena_sb << " s" << endl;
			cout << "nearest neighbor 1.5 Zooming  Use :" << time_neighborlena_b << " s"<<endl;
			cout << "bilinear 2.5 Zooming -> 2 Shrinking Use :" << time_bilinearlena_bs << " s" << endl;
			cout << "bilinear 2 Shrinking -> 2.5 Zooming Use :" << time_bilinearlena_sb << " s" << endl;
			cout << "bilinear 1.5 Zooming  Use :" << time_bilinearlena_b << " s" << endl;
			cout << "bicubic 2.5 Zooming -> 2 Shrinking Use :" << time_bilcubiclena_bs << " s" << endl;
			cout << "bicubic 2 Shrinking -> 2.5 Zooming Use :" << time_bilcubiclena_sb << " s" << endl;
			cout << "bicubic 1.5 Zooming  Use :" << time_bilcubiclena_b << " s" << endl;
			break;
		}
		
		}//1-小switch
		break;
	}
	case'2':
	{
		//2
		cout << "目前第2題 Gray level" << endl;
		double mse7_lena, mse6_lena, mse5_lena, mse4_lena, mse3_lena, mse2_lena, mse1_lena;
		double psnr7_lena, psnr6_lena, psnr5_lena, psnr4_lena, psnr3_lena, psnr2_lena, psnr1_lena;
		Mat tempbitlenaMat = Mat(height, width, CV_8UC1, lena);
		cout << "eightbit_lena" << endl;
		imshow("eightbitFile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("eightbitFile_lena");
		imwrite("8bit_lena.png", tempbitlenaMat);
		unsigned char* tempbitlena = new unsigned char[size];
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitlena[i * width + j] = lena[i * width + j] / 2 * 2;
				j++;
			}
			i++;
			j = 0;
		}

		tempbitlenaMat = Mat(height, width, CV_8UC1, tempbitlena);
		mse7_lena = msecul(256, 256, lena, tempbitlena);
		psnr7_lena = psnrcul(256, 256, lena, tempbitlena);
		cout << "sevenbit_lena" << endl;
		imshow("sevenbitFile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("sevenbitFile_lena");
		imwrite("7bit_lena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitlena[i * width + j] = lena[i * width + j] / 2 / 2 * 4.04;
				j++;
			}
			i++;
			j = 0;
		}
		mse6_lena = msecul(256, 256,lena, tempbitlena);
		psnr6_lena = psnrcul(256, 256, lena, tempbitlena);
		cout << "sixbit_lena" << endl;
		imshow("sixbitFile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("sixbitFile_lena");
		imwrite("6bit_lena.png", tempbitlenaMat);


		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitlena[i * width + j] = lena[i * width + j] / 2 / 2 / 2 * 8.22;
				j++;
			}
			i++;
			j = 0;
		}

		mse5_lena = msecul(256, 256, lena, tempbitlena);
		psnr5_lena = psnrcul(256, 256, lena, tempbitlena);
		cout << "fivebit_lena" << endl;
		imshow("fivebitFile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("fivebitFile_lena");
		imwrite("5bit_lena.png", tempbitlenaMat);

		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitlena[i * width + j] = lena[i * width + j] / 2 / 2 / 2 / 2 * 17;
				j++;
			}
			i++;
			j = 0;
		}
		mse4_lena = msecul(256, 256, lena, tempbitlena);
		psnr4_lena = psnrcul(256, 256, lena, tempbitlena);
		cout << "fourbit_lena" << endl;
		imshow("fourbitFile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("fourbitFile_lena");
		imwrite("4bit_lena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitlena[i * width + j] = lena[i * width + j] / 2 / 2 / 2 / 2 / 2 * 36.4;
				j++;
			}
			i++;
			j = 0;
		}
		mse3_lena = msecul(256, 256, lena, tempbitlena);
		psnr3_lena = psnrcul(256, 256, lena, tempbitlena);
		cout << "threebit_lena" << endl;
		imshow("threebitFile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("threebitFile_lena");
		imwrite("3bit_lena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitlena[i * width + j] = lena[i * width + j] / 2 / 2 / 2 / 2 / 2 / 2 * 85;
				j++;
			}
			i++;
			j = 0;
		}
		mse2_lena = msecul(256, 256, lena, tempbitlena);
		psnr2_lena = psnrcul(256, 256, lena, tempbitlena);
		cout << "twobit_lena" << endl;
		imshow("twobitfile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("twobitfile_lena");
		imwrite("2bit_lena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitlena[i * width + j] = lena[i * width + j] / 2 / 2 / 2 / 2 / 2 / 2 / 2 * 255;
				j++;
			}
			i++;
			j = 0;
		}
		mse1_lena = msecul(256, 256, lena, tempbitlena);
		psnr1_lena = psnrcul(256, 256, lena, tempbitlena);
		cout << "onebit_lena" << endl;
		imshow("onebitfile_lena", tempbitlenaMat);
		waitKey();
		destroyWindow("onebitfile_lena");
		imwrite("1bit_lena.png", tempbitlenaMat);
		
		//baboon_256
		char ori2_File[] = "baboon_256.raw";
		FILE* baboonFile;
		unsigned char* baboon = new unsigned char[size];
		fopen_s(&baboonFile, ori2_File, "rb");
		fread(baboon, sizeof(unsigned char), size, baboonFile);
		Mat baboonMat = Mat(height, width, CV_8UC1, baboon);
		cout << "eightbit_baboon" << endl;
		imshow("8bitfile_baboon", baboonMat);
		waitKey();
		destroyWindow("8bitfile_baboon");
		imwrite("8bit_baboon.png", baboonMat);

		double mse7_baboon, mse6_baboon, mse5_baboon, mse4_baboon, mse3_baboon, mse2_baboon, mse1_baboon;
		double psnr7_baboon, psnr6_baboon, psnr5_baboon, psnr4_baboon, psnr3_baboon, psnr2_baboon, psnr1_baboon;
		unsigned char* tempbitbaboon = new unsigned char[size];
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitbaboon[i * width + j] = baboon[i * width + j] / 2 * 2;
				j++;
			}
			i++;
			j = 0;
		}
		mse7_baboon = msecul(256, 256, baboon, tempbitbaboon);
		psnr7_baboon = psnrcul(256, 256, baboon, tempbitbaboon);
		Mat tempbitbaboonMat = Mat(height, width, CV_8UC1, tempbitbaboon);
		cout << "sevenbit_baboon" << endl;
		imshow("sevenbitFile_baboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("sevenbitFile_baboon");
		imwrite("7bit_baboon.png", tempbitbaboonMat);

		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitbaboon[i * width + j] = baboon[i * width + j] / 2 / 2 * 4.04;
				j++;
			}
			i++;
			j = 0;
		}
		mse6_baboon = msecul(256, 256, baboon, tempbitbaboon);
		psnr6_baboon = psnrcul(256, 256, baboon, tempbitbaboon);
		tempbitbaboonMat = Mat(height, width, CV_8UC1, tempbitbaboon);
		cout << "sixbit_baboon" << endl;
		imshow("sixbitFile_baboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("sixbitFile_baboon");
		imwrite("6bit_baboon.png", tempbitbaboonMat);



		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitbaboon[i * width + j] = baboon[i * width + j] / 2 / 2 / 2 * 8.22;
				j++;
			}
			i++;
			j = 0;
		}
		mse5_baboon = msecul(256, 256, baboon, tempbitbaboon);
		psnr5_baboon = psnrcul(256, 256, baboon, tempbitbaboon);
		cout << "fivebit_baboon" << endl;
		imshow("fivebitFile_baboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("fivebitFile_baboon");
		imwrite("5bit_baboon.png", tempbitbaboonMat);

		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitbaboon[i * width + j] = baboon[i * width + j] / 2 / 2 / 2 / 2 * 17;
				j++;
			}
			i++;
			j = 0;
		}
		mse4_baboon = msecul(256, 256, baboon, tempbitbaboon);
		psnr4_baboon = psnrcul(256, 256, baboon, tempbitbaboon);
		cout << "fourbit_baboon" << endl;
		imshow("fourbitFile_baboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("fourbitFile_baboon");
		imwrite("4bit_baboon.png", tempbitbaboonMat);
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitbaboon[i * width + j] = baboon[i * width + j] / 2 / 2 / 2 / 2 / 2 * 36.4;
				j++;
			}
			i++;
			j = 0;
		}
		mse3_baboon = msecul(256, 256, baboon, tempbitbaboon);
		psnr3_baboon = psnrcul(256, 256, baboon, tempbitbaboon);
		cout << "threebit_baboon" << endl;
		imshow("threebitFile_baboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("threebitFile_baboon");
		imwrite("3bit_baboon.png", tempbitbaboonMat);
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitbaboon[i * width + j] = baboon[i * width + j] / 2 / 2 / 2 / 2 / 2 / 2 * 85;
				j++;
			}
			i++;
			j = 0;
		}
		mse2_baboon = msecul(256, 256, baboon, tempbitbaboon);
		psnr2_baboon = psnrcul(256, 256, baboon, tempbitbaboon);
		cout << "twobit_baboon" << endl;
		imshow("twobitFile_baboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("twobitFile_baboon");
		imwrite("2bit_baboon.png", tempbitbaboonMat);
		i = 0;
		j = 0;
		while (i < width) {
			while (j < height) {
				tempbitbaboon[i * width + j] = baboon[i * width + j] / 2 / 2 / 2 / 2 / 2 / 2 / 2 * 255;
				j++;
			}
			i++;
			j = 0;
		}
		mse1_baboon = msecul(256, 256, baboon, tempbitbaboon);
		psnr1_baboon = psnrcul(256, 256, baboon, tempbitbaboon);
		cout << "onebit_baboon" << endl;
		imshow("onebitFile_baboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("onebitFile_baboon");
		imwrite("1bit_baboon.png", tempbitbaboonMat);
		cout << "lena MSE + PSNR"<<endl;
		cout << "MSE1 : " << mse1_lena << endl;
		cout << "MSE2 : " << mse2_lena << endl;        
		cout << "MSE3 : " << mse3_lena << endl;
		cout << "MSE4 : " << mse4_lena << endl;
		cout << "MSE5 : " << mse5_lena << endl;
		cout << "MSE6 : " << mse6_lena << endl;
		cout << "MSE7 :"  << mse7_lena << endl;
		cout << "1bit PSNR : " << psnr1_lena << " db"<<endl;
		cout << "2bit PSNR : " << psnr2_lena << " db" << endl;
		cout << "3bit PSNR : " << psnr3_lena << " db" << endl;
		cout << "4bit PSNR : " << psnr4_lena << " db" << endl;
		cout << "5bit PSNR : " << psnr5_lena << " db" << endl;
		cout << "6bit PSNR : " << psnr6_lena << " db" << endl;
		cout << "7bit PSNR : " << psnr7_lena << " db" << endl;
		cout << "baboon MSE + PSNR" << endl;
		cout << "MSE1 : " << mse1_baboon << endl;
		cout << "MSE2 : " << mse2_baboon << endl;
		cout << "MSE3 : " << mse3_baboon << endl;
		cout << "MSE4 : " << mse4_baboon << endl;
		cout << "MSE5 : " << mse5_baboon << endl;
		cout << "MSE6 : " << mse6_baboon << endl;
		cout << "MSE7 :"  << mse7_baboon << endl;
		cout << "1bit PSNR : " << psnr1_baboon << " db" << endl;
		cout << "2bit PSNR : " << psnr2_baboon << " db" << endl;
		cout << "3bit PSNR : " << psnr3_baboon << " db" << endl;
		cout << "4bit PSNR : " << psnr4_baboon << " db" << endl;
		cout << "5bit PSNR : " << psnr5_baboon << " db" << endl;
		cout << "6bit PSNR : " << psnr6_baboon << " db" << endl;
		cout << "7bit PSNR : " << psnr7_baboon << " db" << endl;
		break;
		
	}//case2的
	case '3':
	{	
		
		cout << "目前顯示第三題 Isopreference test" << endl;
		unsigned char* tempbitlena = new unsigned char[128*128];
		unsigned char* shrinklena = new unsigned char[128*128];
		bilcubic(256, 256, 0.5, lena, shrinklena);
		Mat shrinklenaMat = Mat(128, 128, CV_8UC1, shrinklena);
		imwrite("8bit_shrinklena.png", shrinklenaMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitlena[i * 128 + j] = shrinklena[i * 128 + j] / 2 * 2;
				j++;
			}
			i++;
			j = 0;
		}

		Mat tempbitlenaMat = Mat(128, 128, CV_8UC1, tempbitlena);
		
		imshow("sevenbitFile_shrinklena", tempbitlenaMat);
		waitKey();
		destroyWindow("sevenbitFile_shrinklena");
		imwrite("7bit_shrinklena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitlena[i * 128 + j] = shrinklena[i * 128 + j] / 2 / 2 * 4.04;
				j++;
			}
			i++;
			j = 0;
		}

		imshow("sixbitFile_shrinklena", tempbitlenaMat);
		waitKey();
		destroyWindow("sixbitFile_shrinklena");
		imwrite("6bit_shrinklena.png", tempbitlenaMat);


		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitlena[i * 128 + j] = shrinklena[i * 128 + j] / 2 / 2 / 2 * 8.22;
				j++;
			}
			i++;
			j = 0;
		}

		
		imshow("fivebitFile_shrinklena", tempbitlenaMat);
		waitKey();
		destroyWindow("fivebitFile_shrinklena");
		imwrite("5bit_shrinklena.png", tempbitlenaMat);

		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitlena[i * 128 + j] = shrinklena[i * 128 + j] / 2 / 2 / 2 / 2 * 17;
				j++;
			}
			i++;
			j = 0;
		}
		imshow("fourbitFile_shrinklena", tempbitlenaMat);
		waitKey();
		destroyWindow("fourbitFile_shrinklena");
		imwrite("4bit_shrinklena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitlena[i * 128 + j] = shrinklena[i * 128 + j] / 2 / 2 / 2 / 2 / 2 * 36.4;
				j++;
			}
			i++;
			j = 0;
		}
		imshow("threebitFile_shrinklena", tempbitlenaMat);
		waitKey();
		destroyWindow("threebitFile_shrinklena");
		imwrite("3bit_shrinklena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitlena[i * 128 + j] = shrinklena[i * 128 + j] / 2 / 2 / 2 / 2 / 2 / 2 * 85;
				j++;
			}
			i++;
			j = 0;
		}
		imshow("twobitfile_shrinklena", tempbitlenaMat);
		waitKey();
		destroyWindow("twobitfile_shrinklena");
		imwrite("2bit_shrinklena.png", tempbitlenaMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitlena[i * 128 + j] = shrinklena[i * 128 + j] / 2 / 2 / 2 / 2 / 2 / 2 / 2 * 255;
				j++;
			}
			i++;
			j = 0;
		}
	
		imshow("onebitfile_shrinklena", tempbitlenaMat);
		waitKey();
		destroyWindow("onebitfile_shrinklena");
		imwrite("1bit_shrinklena.png", tempbitlenaMat);

		char ori4_File[] = "baboon_256.raw";
		FILE* baboon;
		unsigned char* baboon256 = new unsigned char[size];
		unsigned char* shrinkbaboon = new unsigned char[128 * 128];
		fopen_s(&baboon, ori4_File, "rb");
		fread(baboon256, sizeof(unsigned char), size, baboon);
		bilcubic(256, 256, 0.5, baboon256, shrinkbaboon);
		Mat blurshrinkbaboonMat = Mat(128, 128, CV_8UC1, shrinkbaboon);
		imshow("3.shrinkbaboonFile", blurshrinkbaboonMat);
		waitKey();
		destroyWindow("3.shrinkbaboonFile");
		imwrite("1.2_blurshrink.png", blurshrinkbaboonMat);


		unsigned char* tempbitbaboon = new unsigned char[128*128];
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitbaboon[i * 128 + j] = shrinkbaboon[i * 128 + j] / 2 * 2;
				j++;
			}
			i++;
			j = 0;
		}
		
		Mat tempbitbaboonMat = Mat(128, 128, CV_8UC1, tempbitbaboon);
		imshow("sevenbitFile_shrinkbaboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("sevenbitFile_shrinkbaboon");
		imwrite("7bit_shrinkbaboon.png", tempbitbaboonMat);

		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitbaboon[i * 128 + j] = shrinkbaboon[i * 128 + j] / 2 / 2 * 4.04;
				j++;
			}
			i++;
			j = 0;
		}
		tempbitbaboonMat = Mat(128, 128, CV_8UC1, tempbitbaboon);
		imshow("sixbitFile_shrinkbaboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("sixbitFile_shrinkbaboon");
		imwrite("6bit_shrinkbaboon.png", tempbitbaboonMat);

		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitbaboon[i * 128 + j] = shrinkbaboon[i * 128 + j] / 2 / 2 / 2 * 8.22;
				j++;
			}
			i++;
			j = 0;
		}

		imshow("fivebitFile_shrinkbaboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("fivebitFile_shrinkbaboon");
		imwrite("5bit_shrinkbaboon.png", tempbitbaboonMat);

		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitbaboon[i * 128 + j] = shrinkbaboon[i * 128 + j] / 2 / 2 / 2 / 2 * 17;
				j++;
			}
			i++;
			j = 0;
		}
		
		imshow("fourbitFile_shrinkbaboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("fourbitFile_shrinkbaboon");
		imwrite("4bit_shrinkbaboon.png", tempbitbaboonMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitbaboon[i * 128 + j] = shrinkbaboon[i * 128 + j] / 2 / 2 / 2 / 2 / 2 * 36.4;
				j++;
			}
			i++;
			j = 0;
		}
		
		imshow("threebitFile_shrinkbaboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("threebitFile_shrinkbaboon");
		imwrite("3bit_shrinkbaboon.png", tempbitbaboonMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitbaboon[i * 128 + j] = shrinkbaboon[i * 128 + j] / 2 / 2 / 2 / 2 / 2 / 2 * 85;
				j++;
			}
			i++;
			j = 0;
		}

		imshow("twobitFile_shrinkbaboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("twobitFile_shrinkbaboon");
		imwrite("2bit_shrinkbaboon.png", tempbitbaboonMat);
		i = 0;
		j = 0;
		while (i < 128) {
			while (j < 128) {
				tempbitbaboon[i * 128 + j] = shrinkbaboon[i * 128 + j] / 2 / 2 / 2 / 2 / 2 / 2 / 2 * 255;
				j++;
			}
			i++;
			j = 0;
		}

		imshow("onebitFile_shrinkbaboon", tempbitbaboonMat);
		waitKey();
		destroyWindow("onebitFile_shrinkbaboon");
		imwrite("1bit_shrinkbaboon.png", tempbitbaboonMat);
		
		break;
	}
	case '5':
	{	
	
		char ori3_File[] = "map.raw";
		FILE* mapFile;
		unsigned char* map = new unsigned char[20*20];
		fopen_s(&mapFile, ori3_File, "rb");
		fread(map, sizeof(unsigned char),400, mapFile);
		int dis = 0;
		int width = 20;
		int height = 20;
		Mat mapMat = Mat(20, 20, CV_8UC1, map);
		
		unsigned char* tempmap = new unsigned char[400];
		int i = 0;
		int j = 0;
		while (i < width) {
			while (j < height) {
				tempmap[i * width + j] = map[i * width + j];
				j++;
			}
			i++;
			j = 0;
		}
		unsigned char* mapanswer = new unsigned char[400];
		Mat mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
		Mat tempmapMat = Mat(20, 20, CV_8UC1, tempmap);
		/*imshow("map", mapMat);
		waitKey();
		destroyWindow("map");*/
		cout << "目前在第五大題，請輸入第幾小題:" << endl;
		cout << "此次有1 2 3" << endl;
		cin >> No;
		switch (No) {
		case '1':
		{
			cout << "目前第5大題中第1題" << endl;
			//d4 only 80
			dis = 0;
			int k = 0;
			int l = 0;
			int i = 0;
			int j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i != 19 || j != 19) {
				if (tempmap[i * width + j] == 80)
				{
					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if (tempmap[(i + 1) * width + j] == 80 && ((i + 1) != k || (j != l))) {
						k = i; l = j;
						i = i + 1;
						dis++;
					}
					else if (tempmap[i * width + j + 1] == 80 && ((i != k) || ((j + 1) != l))) {
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					else if (tempmap[i * width + j - 1] == 80 && ((i != k) != k || ((j - 1) != l))) {
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else {
						tempmap[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000" << endl;
						i = k;
						j = l;
						dis--;
					}
					//cout << "i: " << i << " j: " << j << endl;
					
				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "d4_80 distance: " << dis << endl;
			Mat mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);

			imwrite("d4_only80.png", mapanswerMat);
			//d8 only80
			dis = 0;
			k = 0;
			l = 0;
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i != 19 || j != 19) {
				if (tempmap[i * width + j] == 80)
				{
					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if (tempmap[(i+1) * width + j + 1] == 80 && (((i+1) != k) || ((j + 1) != l))) {//右下
						k = i; l = j;
						j = j + 1;
						i++;
						dis++;
					}
					else if (tempmap[(i + 1) * width + j] == 80 && ((i + 1) != k || (j != l))) {//下
						k = i; l = j;
						i = i + 1;
						dis++;
					}
					else if (tempmap[(i + 1) * width + j - 1] == 80 && (((i + 1) != k) != k || ((j - 1) != l))) {//左下
						k = i; l = j;
						j = j - 1;
						i++;
						dis++;
					}					
					else if (tempmap[i * width + j + 1] == 80 && ((i != k) || ((j + 1) != l))) {//右
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					else if (tempmap[i * width + j - 1] == 80 && ((i != k) != k || ((j - 1) != l))) {//左
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else {
						tempmap[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000" << endl;
						i = k;
						j = l;
						dis--;
					}
					//cout << "i: " << i << " j: " << j << endl;

				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "d8_80 distance: " << dis << endl;
			//Mat mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("d8_only80.png", mapanswerMat);
			//dm only80
			dis = 0;
			k = 0;
			l = 0;
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i != 19 || j != 19) {
				if (tempmap[i * width + j] == 80)
				{
					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if (tempmap[i * width + j + 1] == 80 && ((i != k) || ((j + 1) != l))) {//右
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					else if (tempmap[(i + 1) * width + j] == 80 && ((i + 1) != k || (j != l))) {//下
						k = i; l = j;
						i = i + 1;
						dis++;
					}
					else if (tempmap[i * width + j - 1] == 80 && ((i != k) != k || ((j - 1) != l))) {//左
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else if (tempmap[(i + 1) * width + j - 1] == 80 && (((i + 1) != k) != k || ((j - 1) != l))) {//左下
						k = i; l = j;
						j = j - 1;
						i++;
						dis++;
					}					
					else if (tempmap[(i + 1) * width + j + 1] == 80 && (((i + 1) != k) || ((j + 1) != l))) {//右下
						k = i; l = j;
						j = j + 1;
						i++;
						dis++;
					}					
					else {
						tempmap[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000" << endl;
						i = k;
						j = l;
						dis--;
					}
					//cout << "i: " << i << " j: " << j << endl;

				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "dm_80 distance: " << dis << endl;
			//Mat mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("dm_only80.png", mapanswerMat);
			break;
		}
			// only 80,160
		case '2':
		{
			cout << "目前第5大題中第2小題" << endl;
			//d4
			int k1 = 0;
			int l1 = 0;
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			k = 0;
			l = 0;
			dis = 0;
			while (i != 19 || j != 19) {
				if (tempmap[i * width + j] == 80 || tempmap[i * width + j] == 160)
				{

					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if ((tempmap[(i + 1) * width + j] == 160 || tempmap[(i + 1) * width + j] == 80) && ((i + 1) != k || (j != l))) {
						
						k = i; l = j;
						i = i + 1;
						dis++;
					}
					else if ((tempmap[i * width + j + 1] == 160 || tempmap[i * width + j + 1] == 80) && ((i != k) || ((j + 1) != l))) {
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					else if ((tempmap[i * width + j - 1] == 160 || tempmap[i * width + j - 1] == 80) && ((i != k) != k || ((j - 1) != l))) {
						//k1 = k; l1 = l;
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else {
						tempmap[i * width + j] = 0;
						mapanswer[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000"<< endl;
						i = k;
						j = l;
						dis--;
					}
					//cout << "dis:"<< dis <<"i: " << i << " j: " << j << endl;
					//cout << "k: " << k << " l: " << l << endl;
					//cout << "k before: " << k1 << " l before: " << l1 << endl;
				}

			}
		
			mapanswer[i * width + j] = 255;
			cout << "d4_only80_160 distance: " << dis << endl;
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("d4_only80_160.png", mapanswerMat);
			//d8 80 160
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			k = 0;
			l = 0;
			dis = 0;
			while (i != 19 || j != 19) {
				if (tempmap[i * width + j] == 80 || tempmap[i * width + j] == 160)
				{

					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if ((tempmap[(i + 1) * width + j+1] == 160 || tempmap[(i + 1) * width + j+1] == 80) && ((i + 1) != k || ((j+1) != l))) {//右下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j = j + 1;
						dis++;
					}
					else if ((tempmap[(i + 1) * width + j - 1] == 160 || tempmap[(i + 1) * width + j - 1] == 80) && ((i + 1) != k || ((j - 1) != l))) {//左下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j = j - 1;
						dis++;
					}
					else if ((tempmap[i * width + j + 1] == 160 || tempmap[i * width + j + 1] == 80) && ((i != k) || ((j + 1) != l))) {//右
						k1 = k; l1 = l;
						k = i; l = j;
						j = j + 1;
						dis++;
					} 
					else if ((tempmap[(i + 1) * width + j] == 160 || tempmap[(i + 1) * width + j] == 80) && ((i + 1) != k || (j  != l))) {//下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						dis++;
					}					
					else if ((tempmap[(i-1) * width + j +1] == 160 || tempmap[(i-1) * width + j + 1] == 80) && ((i - 1) != k || ((j+1) != l))) {//右上///////
						//k1 = k; l1 = l;
						k1 = k; l1 = l;
						k = i; l = j;
						i = i - 1;
						j = j + 1;
						dis++;
					}					
					else if ((tempmap[i * width + j - 1] == 160 || tempmap[i * width + j - 1] == 80) && ((i != k) || ((j - 1) != l))) {//左
						k1 = k; l1 = l;
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else {
						tempmap[i * width + j] = 0;
						mapanswer[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000"<< endl;
						i = k;
						j = l;
						k = k1; l = l1;
						dis = dis - 1;
					}
					//cout << "dis:" << dis << "i: " << i << " j: " << j << endl;
					//cout << "k: " << k << " l: " << l << endl;
					//cout << "k before: " << k1 << " l before: " << l1 << endl;
				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "d8_only80_160 distance: " << dis << endl;
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("d8_only80_160.png", mapanswerMat);
			//dm 80 160
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			k = 0;
			l = 0;
			dis = 0;
			while (i != 19 || j != 19) {
				if (tempmap[i * width + j] == 80 || tempmap[i * width + j] == 160)
				{

					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if ((tempmap[(i + 1) * width + j] == 160 || tempmap[(i + 1) * width + j] == 80) && ((i + 1) != k || (j != l))) {//下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						dis++;
					}					
					else if ((tempmap[i * width + j + 1] == 160 || tempmap[i * width + j + 1] == 80) && ((i != k) || ((j + 1) != l))) {//右
						k1 = k; l1 = l;
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					else if ((tempmap[i * width + j - 1] == 160 || tempmap[i * width + j - 1] == 80) && ((i != k) || ((j - 1) != l))) {//左
						k1 = k; l1 = l;
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else if ((tempmap[(i + 1) * width + j + 1] == 160 || tempmap[(i + 1) * width + j + 1] == 80) && ((i + 1) != k || ((j + 1) != l))) {//右下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j = j + 1;
						dis++;
					}					
					else if ((tempmap[(i + 1) * width + j - 1] == 160 || tempmap[(i + 1) * width + j - 1] == 80) && ((i + 1) != k || ((j - 1) != l))) {//左下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j = j - 1;
						dis++;
					}
					else if ((tempmap[(i - 1) * width + j + 1] == 160 || tempmap[(i - 1) * width + j + 1] == 80) && ((i - 1) != k || ((j + 1) != l))) {//右上///////
						//k1 = k; l1 = l;
						k1 = k; l1 = l;
						k = i; l = j;
						i = i - 1;
						j = j + 1;
						dis++;
					}
					
					else {
						tempmap[i * width + j] = 0;
						mapanswer[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000"<< endl;
						i = k;
						j = l;
						k = k1; l = l1;
						dis = dis - 1;
					}
					//cout << "dis:" << dis << "i: " << i << " j: " << j << endl;
					//cout << "k: " << k << " l: " << l << endl;
					//cout << "k before: " << k1 << " l before: " << l1 << endl;
				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "dm_only80_160 distance: " << dis << endl;
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("dm_only80_160.png", mapanswerMat);
			break;
		}
			
		case '3':
		{
			//d4 only 80,160,255
			cout << "目前第5大題中第3小題" << endl;
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			k = 0;
			l = 0;
			int k1 = 0;
			int l1 = 0;
			dis = 0;
			while (i != 19 || j != 19) {
				if ((tempmap[i * width + j] == 255) || (tempmap[i * width + j] == 160) || (tempmap[(i)*width + j] == 80))
				{
					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if ((tempmap[(i + 1) * width + j] == 160 || tempmap[(i + 1) * width + j] == 80 || tempmap[(i + 1) * width + j] == 255) && ((i + 1) != k || (j != l))) {//下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						dis++;
					}
					else if ((tempmap[i * width + j + 1] == 160 || tempmap[i * width + j + 1] == 80 || tempmap[i * width + j + 1] == 255) && ((i != k) || ((j + 1) != l))) {//右
						k1 = k; l1 = l;
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					else if ((tempmap[i * width + j - 1] == 160 || tempmap[i * width + j - 1] == 80 || tempmap[i * width + j - 1] == 255) && ((i != k) != k || ((j - 1) != l))) {//左
						k1 = k; l1 = l;
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else {
						tempmap[i * width + j] = 0;
						mapanswer[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000" << endl;
						i = k;
						j = l;
						k = k1; l = l1;
						dis=dis-1;
					}
					//cout<<"dis"<<dis << "i: " << i << " j: " << j << endl;
					//cout << "k: " << k << " l: " << l << endl;
					//cout << "k before: " << k1 << " l before: " << l1 << endl;
				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "d4_only80_160_255 distance: " << dis << endl;
			tempmapMat = Mat(20, 20, CV_8UC1, tempmap);
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("d4_only80_160_255.png", mapanswerMat);
			//d8-80.160.255
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			k = 0;
			l = 0;
			k1 = 0;
			l1 = 0;
			dis = 0;
			while (i != 19 || j != 19) {
				if ((tempmap[i * width + j] == 255) || (tempmap[i * width + j] == 160) || (tempmap[(i)*width + j] == 80))
				{
					mapanswer[i * width + j] = 255;
					//tempmap[i * width + j] = 0;
					if ((tempmap[(i+1) * width + j + 1] == 160 || tempmap[(i+1) * width + j + 1] == 80 || tempmap[(i+1) * width + j + 1] == 255) && (((i+1) != k) || ((j + 1) != l))) {//右下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j = j + 1;
						dis++;
					}
					else if ((tempmap[(i + 1) * width + j - 1] == 160 || tempmap[(i + 1) * width + j - 1] == 80 || tempmap[(i + 1) * width + j - 1] == 255) && ((i + 1) != k || ((j - 1) != l))) {//左下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j--;
						dis++;
					}
					else if ((tempmap[i * width + j + 1] == 160 || tempmap[i * width + j + 1] == 80 || tempmap[i * width + j + 1] == 255) && ((i != k) || ((j + 1) != l))) {//右
						k1 = k; l1 = l;
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					
					else if ((tempmap[(i + 1) * width + j] == 160 || tempmap[(i + 1) * width + j] == 80 || tempmap[(i + 1) * width + j] == 255) && ((i + 1) != k || (j != l))) {//下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						dis++;
					}					
					else if ((tempmap[i * width + j - 1] == 160 || tempmap[i * width + j - 1] == 80 || tempmap[i * width + j - 1] == 255) && ((i != k) != k || ((j - 1) != l))) {//左
						k1 = k; l1 = l;
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else {
						tempmap[i * width + j] = 0;
						mapanswer[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000" << endl;
						i = k;
						j = l;
						k = k1; l = l1;
						dis = dis - 1;
					}
					//cout << "dis" << dis << "i: " << i << " j: " << j << endl;
					//cout << "k: " << k << " l: " << l << endl;
					//cout << "k before: " << k1 << " l before: " << l1 << endl;
				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "d8_only80_160_255 distance: " << dis << endl;
			tempmapMat = Mat(20, 20, CV_8UC1, tempmap);
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("d8_only80_160_255.png", mapanswerMat);
			//dm-80.160.255
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					mapanswer[i * width + j] = 0;
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			while (i < width) {
				while (j < height) {
					tempmap[i * width + j] = map[i * width + j];
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
			j = 0;
			k = 0;
			l = 0;
			k1 = 0;
			l1 = 0;
			dis = 0;
			while (i != 19 || j != 19) {
				if ((tempmap[i * width + j] == 255) || (tempmap[i * width + j] == 160) || (tempmap[(i)*width + j] == 80))
				{
					if ((i < 20) && (j < 20)) {
						mapanswer[i * width + j] = 255;
					}
					else {
						tempmap[i * width + j] = 0;
						mapanswer[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000" << endl;
						//cout << "上一步";
						i = k;
						j = l;
						k = k1; l = l1;
						dis = dis - 1;
					}
					//tempmap[i * width + j] = 0;
					if ((tempmap[i * width + j + 1] == 160 || tempmap[i * width + j + 1] == 80 || tempmap[i * width + j + 1] == 255) && (((i != k) || (((j + 1) != l))))) {//右
						k1 = k; l1 = l;
						k = i; l = j;
						j = j + 1;
						dis++;
					}
					else if ((tempmap[(i + 1) * width + j] == 160 || tempmap[(i + 1) * width + j] == 80 || tempmap[(i + 1) * width + j] == 255) && ((i + 1) != k || (j != l))) {//下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						dis++;
					}
					else if ((tempmap[i * width + j - 1] == 160 || tempmap[i * width + j - 1] == 80 || tempmap[i * width + j - 1] == 255) && ((i != k) != k || ((j - 1) != l))) {//左
						k1 = k; l1 = l;
						k = i; l = j;
						j = j - 1;
						dis++;
					}
					else if ((tempmap[(i + 1) * width + j + 1] == 160 || tempmap[(i + 1) * width + j + 1] == 80 || tempmap[(i + 1) * width + j + 1] == 255) && (((i + 1) != k) || ((j + 1) != l))) {//右下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j = j + 1;
						dis++;
					}
					else if ((tempmap[(i + 1) * width + j - 1] == 160 || tempmap[(i + 1) * width + j - 1] == 80 || tempmap[(i + 1) * width + j - 1] == 255) && ((i + 1) != k || ((j - 1) != l))) {//左下
						k1 = k; l1 = l;
						k = i; l = j;
						i = i + 1;
						j--;
						dis++;
					}						
					else {
						tempmap[i * width + j] = 0;
						mapanswer[i * width + j] = 0;
						//cout << "i: " << i << " j: " << j << "000000000000" << endl;
						//cout << "上一步";
						i = k;
						j = l;
						k = k1; l = l1;
						dis = dis - 1;
					}
					//cout << "dis" << dis << "i: " << i << " j: " << j << endl;
					//cout << "k: " << k << " l: " << l << endl;
					//cout << "k before: " << k1 << " l before: " << l1 << endl;
				}

			}
			
			mapanswer[i * width + j] = 255;
			cout << "dm_only80_160_255 distance: " << dis << endl;
			tempmapMat = Mat(20, 20, CV_8UC1, tempmap);
			mapanswerMat = Mat(20, 20, CV_8UC1, mapanswer);
			imwrite("dm_only80_160_255.png", mapanswerMat);
			break;			
		}
		break;
		}
		break;
	}
	}//大題switch
	return  main();	
}//main

