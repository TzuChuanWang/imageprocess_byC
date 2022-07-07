#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <ctime>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI acos(-1)
using namespace std;
using namespace cv;

int const width = 512;
int const height = 512;
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
void histogram_match(int ori_width, int ori_height, unsigned char* lena1, int* histogramlena1, unsigned char* lena2, int* histogramlena2, unsigned char* newlena2, int* newlena2histogram, Mat newlena2histogramat, int percent = 100, int bit = 8) {//2映射到1
	int i = 0;
	int max_numbit = pow(2, bit);
	int size = ori_width * ori_height;
	while (i < max_numbit)
	{
		histogramlena1[i] = 0;
		histogramlena2[i] = 0;
		i++;
	}
	i = 0;
	int numbit1, numbit2;
	while (i < size) {
		numbit1 = (int)lena1[i];
		histogramlena1[numbit1] = (int)histogramlena1[numbit1] + 1;
		numbit2 = (int)lena2[i];
		histogramlena2[numbit2] = (int)histogramlena2[numbit2] + 1;
		//cout << "i: " << i << "histogram[" << numbit << "] num: " << histogram[numbit]<<endl;
		i++;
	}
	double all1 = 0, all2 = 0;
	double percent1[256], percent2[256];
	for (int i = 0; i < 256; i++)
	{

		//cout << " histogramlena1[ "<<i<<" ] = " << histogramlena1[i] << endl;
		//cout << " histogramlena2[ " << i << " ] = " << histogramlena2[i] << endl;
		all1 = histogramlena1[i] + all1;
		all2 = histogramlena2[i] + all2;
		percent1[i] = all1 / size;
		percent2[i] = all2 / size;
		//cout << " percent1[ " << i << " ] = " << (double)percent1[i] << endl;
		//cout << " percent2[ " << i << " ] = " << (double)percent2[i] << endl;
	}
	//cout << all1<<endl;
	//cout << all1 << endl;
	double temp_percent1 = 0;
	int bit2tobit1[256];
	for (int i = 0; i < 256; i++)
	{
		if (percent1[i] > temp_percent1) {
			//cout << i << endl;
			for (int j = 0; j < 256; j++)
			{
				if ((percent2[j] <= percent1[i]) && (percent2[j] > temp_percent1)) {
					//cout << i << endl;
					bit2tobit1[j] = i;
				}
			}
			temp_percent1 = percent1[i];
		}

	}
	//for (int i = 0; i < 256; i++)//對照表
	//{
	//	cout << " bit2tobit1[ " << i << " ] = " << bit2tobit1[i]<< endl;
	//}
	for (int i = 0; i < size; i++)
	{
		int k = 0;
		k = lena2[i];
		newlena2[i] = bit2tobit1[k];
	}

	//newhistogram
	histogramplot(512, 512, newlena2, newlena2histogram, newlena2histogramat, percent, 8);
}

void filtering_3x3(int new_width, int new_height, unsigned char* ori_img, unsigned char* newimg, int filter[3][3],int padding){
	int ori_width = new_width + padding * 2;
	int ori_height = new_height + padding * 2;
	int temp = 0;
	for (int i = 1; i < new_height+1; i++)
	{
		for (int j = 1; j < new_width+1; j++)
		{
			temp = 0;
			temp=temp+ori_img[(j -1) + (i-1)  * ori_width] * filter[0][0];
			temp =temp+ori_img[(j) + (i - 1) * ori_width] * filter[0][1];
			temp = temp + ori_img[(j+1)+(i - 1) * ori_width] * filter[0][2];
			temp = temp + ori_img[(j - 1) + (i) * ori_width] * filter[1][0];
			temp = temp + ori_img[(j)+(i) * ori_width] * filter[1][1];
			temp = temp + ori_img[(j + 1) + (i) * ori_width] * filter[1][2];
			temp = temp + ori_img[(j - 1) + (i+1)*ori_width] * filter[2][0];
			temp = temp + ori_img[(j)+(i+1)*ori_width] * filter[2][1];
			temp = temp + ori_img[(j + 1) + (i + 1) * ori_width] * filter[2][2];
			if (temp > 255)
			{
				temp = 255;
			}
			if (temp < 0)
			{
				temp = 0;
			}
			newimg[(j-1) + (i-1) * new_width] = temp;
		}
	}
}

void highboost_filtering_3x3(int new_width, int new_height, unsigned char* ori_img, unsigned char* newimg, int filter[3][3], int padding, int a) {
	int ori_width = new_width + padding * 2;
	int ori_height = new_height + padding * 2;
	filter[1][1] = filter[1][1] + a;
	int temp = 0;
	for (int i = 1; i < new_height + 1; i++)
	{
		for (int j = 1; j < new_width + 1; j++)
		{
			temp = 0;
			temp = temp + ori_img[(j - 1) + (i - 1) * ori_width] * filter[0][0];
			temp = temp + ori_img[(j)+(i - 1) * ori_width] * filter[0][1];
			temp = temp + ori_img[(j + 1) + (i - 1) * ori_width] * filter[0][2];
			temp = temp + ori_img[(j - 1) + (i)*ori_width] * filter[1][0];
			temp = temp + ori_img[(j)+(i)*ori_width] * filter[1][1];
			temp = temp + ori_img[(j + 1) + (i)*ori_width] * filter[1][2];
			temp = temp + ori_img[(j - 1) + (i + 1) * ori_width] * filter[2][0];
			temp = temp + ori_img[(j)+(i + 1) * ori_width] * filter[2][1];
			temp = temp + ori_img[(j + 1) + (i + 1) * ori_width] * filter[2][2];
			if (a != 0)
			{
				temp = temp / a;
			}
			if (temp > 255)
			{
				temp = 255;
			}
			if (temp < 0)
			{
				temp = 0;
			}
			
			
			newimg[(j - 1) + (i - 1) * new_width] = temp;
		}
	}
	filter[1][1] = filter[1][1] - a;
}

void filtering_5x5(int new_width, int new_height, unsigned char* ori_img, unsigned char* newimg, double filter[5][5], int padding){
	int ori_width = new_width + padding * 2;
	int ori_height = new_height + padding * 2;
	double temp = 0;
	for (int i = 2; i < new_height+2; i++)
	{
		for (int j = 2; j < new_width+2; j++)
		{
			temp = 0;
			temp = temp + ori_img[(j - 2) + (i - 2) * ori_width] * filter[0][0];
			temp = temp + ori_img[(j - 1) + (i - 2) * ori_width] * filter[0][1];
			temp = temp + ori_img[(j ) + (i - 2) * ori_width] * filter[0][2];
			temp = temp + ori_img[(j +1) + (i - 2) * ori_width] * filter[0][3];
			temp = temp + ori_img[(j + 2) + (i - 2) * ori_width] * filter[0][4];

			temp = temp + ori_img[(j - 2) + (i - 1) * ori_width] * filter[1][0];
			temp = temp + ori_img[(j - 1) + (i - 1) * ori_width] * filter[1][1];
			temp = temp + ori_img[(j)+(i - 1) * ori_width] * filter[1][2];
			temp = temp + ori_img[(j + 1) + (i - 1) * ori_width] * filter[1][3];
			temp = temp + ori_img[(j + 2) + (i - 1) * ori_width] * filter[1][4];

			temp = temp + ori_img[(j - 2) + (i) * ori_width] * filter[2][0];
			temp = temp + ori_img[(j - 1) + (i) * ori_width] * filter[2][1];
			temp = temp + ori_img[(j)+(i) * ori_width] * filter[2][2];
			temp = temp + ori_img[(j + 1) + (i) * ori_width] * filter[2][3];
			temp = temp + ori_img[(j + 2) + (i) * ori_width] * filter[2][4];

			temp = temp + ori_img[(j - 2) + (i+1)*ori_width] * filter[3][0];
			temp = temp + ori_img[(j - 1) + (i+1)*ori_width] * filter[3][1];
			temp = temp + ori_img[(j)+(i+1)*ori_width] * filter[3][2];
			temp = temp + ori_img[(j + 1) + (i+1)*ori_width] * filter[3][3];
			temp = temp + ori_img[(j + 2) + (i+1)*ori_width] * filter[3][4];

			temp = temp + ori_img[(j - 2) + (i + 2) * ori_width] * filter[4][0];
			temp = temp + ori_img[(j - 1) + (i + 2) * ori_width] * filter[4][1];
			temp = temp + ori_img[(j)+(i + 2) * ori_width] * filter[4][2];
			temp = temp + ori_img[(j + 1) + (i + 2) * ori_width] * filter[4][3];
			temp = temp + ori_img[(j + 2) + (i + 2) * ori_width] * filter[4][4];
			if (temp > 255)
			{
				temp = 255;
			}
			if (temp < 0)
			{
				temp = 0;
			}
			newimg[(j - 2) + (i - 2) * new_width] = (int)temp;
		}
	}
}
void read_file(int width, int height, const char filename[], unsigned char* dst){
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
void zero_padding(int ori_width, int ori_height, unsigned char* orilena, unsigned char* zeroout, int padding){
	int new_width = ori_width + padding * 2;
	int new_height = ori_height + padding * 2;
	for (int i = 0; i < new_width; i++)
	{
		zeroout[i] = 0;//上
		zeroout[i + (new_height - 1) * new_width] = 0;//下
	}
	for (int i = 0; i < new_height; i++)
	{
		zeroout[i * new_width] = 0;//左
		zeroout[(new_width - 1) + i * new_width] = 0;//右
	}
	for (int i = 0; i < ori_height; i++)
	{
		for (int j = 0; j < ori_width; j++)
		{
			zeroout[(j + padding) + (i + padding) * new_width] = orilena[j + i * ori_width];
		}
	}
	
}
void mirror_padding(int ori_width, int ori_height, unsigned char* orilena, unsigned char* mirrorout, int padding){
	int new_width = ori_width + padding * 2;
	int new_height = ori_height + padding * 2;

	for (int i = 0; i < ori_height; i++)//填值
	{
		for (int j = 0; j < ori_width; j++)
		{
			mirrorout[(j + padding) + (i + padding) * new_width] = orilena[j + i * ori_width];
		}
	}
	for (int j = padding - 1; j >= 0; j--)//上下
	{
		for (int i = j + 1; i < new_width - (j + 1); i++)
		{
			mirrorout[i + j * new_width] = mirrorout[i + (padding * 2 - 1 - j) * new_width];
			mirrorout[i + (new_height - j - 1) * new_width] = mirrorout[i + (new_height - 1 - (padding * 2 - 1 - j)) * new_width];
		}
		for (int i = j + 1; i < new_height - (j + 1); i++)
		{
			mirrorout[j + i * new_width] = mirrorout[(padding * 2 - 1 - j) + i * new_width];
			mirrorout[(new_width - j - 1) + i * new_width] = mirrorout[(new_width - 1 - (padding * 2 - 1 - j)) + i * new_width];
		}
		mirrorout[j + j * new_width] = mirrorout[(padding * 2 - 1 - j) + (padding * 2 - 1 - j) * new_width];
		mirrorout[(new_width - 1 - j) + j * new_width] = mirrorout[(new_width - 1 - (padding * 2 - 1 - j)) + (padding * 2 - 1 - j) * new_width];
		mirrorout[j + (new_height - 1 - j) * new_width] = mirrorout[(padding * 2 - 1 - j) + (new_height - 1 - (padding * 2 - 1 - j)) * new_width];
		mirrorout[(new_width - 1 - j) + (new_height - 1 - j) * new_width] = mirrorout[(new_width - 1 - (padding * 2 - 1 - j)) + (new_height - 1 - (padding * 2 - 1 - j)) * new_width];
	}
}
void histogram_equalization(int ori_width, int ori_height, unsigned char* orilena, int* histogram, int* newhistogram, Mat histogramat, unsigned char* newlena, Mat newhistogramat, int percent = 100, int bit = 8) {
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
		rectangle(histogramat, Point(i, percent), Point((i + 1), (max_num - (int)histogram[i]) * percent / max_num + 2), Scalar(0));
		i++;
	}
	//equlization
	double P[256];
	double cdf[256] = { 0 };
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
		newhistogram[i] = ((cdf[i] - cdf_min) * size / (size - (cdf_min * size)) * (max_numbit - 1));
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
void local_histogram_equalization(int ori_width, int ori_height, unsigned char* orilena, int* histogram, int* newhistogram, Mat histogramat, unsigned char* newlena, Mat newhistogramat, int mask = 256 ,int percent = 100, int bit = 8) {
	int m = 0;
	int k = mask;
	int i = 0;
	int j = 0;
	int l = mask;
	int m1 = 0;
	int j1 = 0;
	int max_numbit = pow(2, bit);
	int b = ori_width / mask;
	int size = ori_width * ori_height / b / b;
	while (m1 < ori_width) {
		while (j1 < ori_height) {
			i = 0;
			j = 0;
			while (i < max_numbit)
			{
				histogram[i] = 0;
				i++;
			}
			i = 0;
			int numbit;
			m = m1;
			while (m < k && k <= ori_width) {
				while (j < l && l <= ori_height) {
					numbit = (int)orilena[m * ori_width + j];
					histogram[numbit] = (int)histogram[numbit] + 1;
					j = j++;
				}
				m = m++;
				j = j1;
			}
			/////////
			/*int all=0;
			for (int hh = 0; hh < 256; hh++) {
				all = histogram[hh] + all;
				cout << "histogram[" << hh << "] num: " << histogram[hh]<<endl;
			}
			cout << all;*/
			/////////
			//while (i < size) {
			//	numbit = (int)orilena[i];
			//	histogram[numbit] = (int)histogram[numbit] + 1;
			//	//cout << "i: " << i << "histogram[" << numbit << "] num: " << histogram[numbit]<<endl;
			//	i++;
			//}
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

			/*i = 0;
			while (i < max_numbit)
			{
				rectangle(histogramat, Point(i, percent), Point((i + 1), (max_num - (int)histogram[i]) * percent / max_num + 2), Scalar(0));
				i++;
			}*/
			//equlization
			double P[256];
			double cdf[256] = { 0 };
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
			int kk = 0;
			for (i = 0; i < 256; i++) {
				newhistogram[i] = ((cdf[i] - cdf_min) * size / (size - (cdf_min * size)) * (max_numbit - 1));
				//int a = ((cdf[i] - cdf_min) * size / (size - (cdf_min * size)) * (max_numbit - 1));
				//int a = ((cdf_min -cdf[i]) * size / (size - (cdf_min * size)) * (max_numbit - 1));
				/*if (a < 0) { newhistogram[i] = 0; }
				else { newhistogram[i] = a; }*/
				//newhistogram[i] = a;
				//cout << "newhistogram[" << i << "] = " << newhistogram[i] << endl;
				
				/*if (kk > newhistogram[i]) {
					kk = newhistogram[i];
				}*/
			}
			for (i = 0; i < 256; i++) {
				newhistogram[i] = newhistogram[i]+kk;
				
			}
			//對照
			m = m1;
			j = j1;
			while (m < k && k <= ori_width) {
				while (j < l && l <= ori_height) {
					for (int bitt = 0; bitt < 256; bitt++) {
						if ((int)orilena[m * ori_width + j] == bitt) { newlena[m * ori_width + j] = newhistogram[bitt]; }
					}
					j++;
				}
				m = m++;
				j = j1;
			}
			j1 = j1 + mask;
			l = l + mask;
		}
			//對照
			/*for (i = m; i < k; i++) {
				for (int j = 0; j < l; j++) {
					for (int bitt = 0; bitt < 256; bitt++) {
						if ((int)orilena[i * ori_width + j] == bitt) { newlena[i * ori_width + j] = newhistogram[bitt]; }
					}
				}
			}*/
		j1 = 0;
		l = mask;
		m1 = m1 + mask;
		k = mask + k;
	}
	//新的histogram

	histogramplot(512, 512, newlena, newhistogram, newhistogramat, percent, 8);
}
int img_average( unsigned char* average,int frames_num) {

	VideoCapture cap("street.avi");
	if (!cap.isOpened()) {
		cout << "ERROR opening video!!" << endl;
		return -1;
	}
	int num = 0;
	int* temp = new int[960 * 540]();
	while (1) {
		Mat frame;
		//下方程式碼會依序獲得每一帧
		//請根據上課所學修改此段
		cap >> frame;

		//若影片結束跳出迴圈
		if (frame.empty())
			break;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		unsigned char* framedata = frame.data;
		/*unsigned char** temp = new unsigned char*[477];

		for (int i = 0; i < 477; i++) {
			temp[i] = new unsigned char[960 * 540];
		}
		for (int l = 0; l < 477; l++) {
			
			for (int i = 0; i < 960; i++) {
				for (int j = 0; j < 540; j++) {
					temp[l][i + j * 960] = (int)framedata[i + j * 960];
				}		
			}
		}
	*/
		if (num < frames_num) {
			for (int i = 0; i < 960; i++) {
				for (int j = 0; j < 540; j++) {
					temp[i + j * 960] = temp[i + j * 960] + (int)framedata[i + j * 960];
				}
			}
		}
		num++;
		/*char c = (char)waitKey(1);
		if (c == 27)
			break;*/

		
	}
	for (int i = 0; i < 960; i++) {
		for (int j = 0; j < 540; j++) {
			temp[i + j * 960] = temp[i + j * 960] / (frames_num);
			average[i + j * 960] = temp[i + j * 960];
		}
	}
	
	cap.release();
	return 0;

}
void gaussian_filter5x5_set(double sigma, double filter[5][5]) {
	
	double r ;
	double q = sigma * sigma;
	double s = 2.0 * q* PI;

	double sum = 0.0;
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			r = (x * x + y * y);
			filter[x + 2][y + 2] = (exp(-(r) / 2/q)) / s;
			sum =sum+ filter[x + 2][y + 2];
		}
	}

	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			filter[x + 2][y + 2] = filter[x + 2][y + 2] / sum;
		}
	}
}
void different_of_gaussian_filter5x5_set(double sigma1, double sigma2, double filter[5][5]) {
	double tempfilter1[5][5]{};
	double tempfilter2[5][5]{};
	gaussian_filter5x5_set(sigma1, tempfilter1);
	gaussian_filter5x5_set(sigma2, tempfilter2);
	double sum = 0.0;
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			filter[x + 2][y + 2] = tempfilter1[x + 2][y + 2]- tempfilter2[x + 2][y + 2];
			sum = sum + filter[x + 2][y + 2];
		}
	}
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			filter[x + 2][y + 2] = filter[x + 2][y + 2] / sum;
		}
	}
}
int main() {

	int const size = width * height;
	//2	

	

	

	
	int i = 0;
	int j = 0;
	int l = 0;
	int k = 0;
	
	///////////////////////////////////////////////////////////////////


	char No;
	cout << "請先輸入第幾大題:" << endl;
	cout << "此次有1 2 3 4 " << endl;
	cin >> No;
	switch (No)
	{
		///////////////////
	case '1':
	{
		cout << "目前在第一大題，請輸入第幾小題:" << endl;
		cout << "此次有a b " << endl;
		cin >> No;
		switch (No) {
		case'a':
		{
			cout << "目前第1大題中第1小題" << endl;
			unsigned char* lena512 = new unsigned char[size];
			read_file(512, 512, "lena512.raw", lena512);
			Mat lena512Mat = Mat(512, 512, CV_8UC1, lena512);
			imshow("lena512File", lena512Mat);
			waitKey();
			destroyWindow("lena512File");
			imwrite("lena512.png", lena512Mat);
			int lena512_histogram[256];
			int percent = 100;
			Mat lena512_histogramat(percent, 256, CV_8U, Scalar(127));
			histogramplot(width, height, lena512, lena512_histogram, lena512_histogramat, percent);
			imshow("lena512_histogramFile", lena512_histogramat);
			waitKey();
			destroyWindow("lena512_histogramFile");
			imwrite("lena512_histogram.png", lena512_histogramat);

			unsigned char* house512 = new unsigned char[size];
			unsigned char* newhouse512 = new unsigned char[size];
			read_file(512, 512, "house512.raw", house512);
			Mat house512Mat = Mat(512, 512, CV_8UC1, house512);
			imshow("house512File", house512Mat);
			waitKey();
			destroyWindow("house512File");
			imwrite("house512.png", house512Mat);
			int house512_histogram[256];
			Mat house512_histogramat(percent, 256, CV_8U, Scalar(127));
			histogramplot(width, height, house512, house512_histogram, house512_histogramat, percent);
			imshow("house512_histogramFile", house512_histogramat);
			waitKey();
			destroyWindow("house512_histogramFile");
			imwrite("house512_histogram.png", house512_histogramat);

			Mat newhouse512Mat = Mat(512, 512, CV_8UC1, newhouse512);
			int newhouse512_histogram[256];
			Mat newhouse512_histogramat(percent, 256, CV_8U, Scalar(127));
			histogram_match(width, height, lena512, lena512_histogram, house512, house512_histogram, newhouse512, newhouse512_histogram, newhouse512_histogramat, percent);
			imshow("newhouse512File", newhouse512Mat);
			waitKey();
			destroyWindow("newhouse512File");
			imwrite("newhouse512.png", newhouse512Mat);

			imshow("newhouse512_histogramFile", newhouse512_histogramat);
			waitKey();
			destroyWindow("newhouse512_histogramFile");
			imwrite("newhouse512_histogram.png", newhouse512_histogramat);
			break;

		}
		case 'b':
		{

			cout << "目前第1大題中第2小題" << endl;
			unsigned char* lena512 = new unsigned char[size];
			read_file(512, 512, "lena512.raw", lena512);
			Mat lena512Mat = Mat(512, 512, CV_8UC1, lena512);
			int lena512_histogram[256];
			int percent = 100;
			Mat lena512_histogramat(percent, 256, CV_8U, Scalar(127));
			histogramplot(width, height, lena512, lena512_histogram, lena512_histogramat, percent);
			

			unsigned char* house512 = new unsigned char[size];
			unsigned char* newhouse512 = new unsigned char[size];
			read_file(512, 512, "house512.raw", house512);
			Mat house512Mat = Mat(512, 512, CV_8UC1, house512);
			int house512_histogram[256];
			Mat house512_histogramat(percent, 256, CV_8U, Scalar(127));
			histogramplot(width, height, house512, house512_histogram, house512_histogramat, percent);


			Mat newhouse512Mat = Mat(512, 512, CV_8UC1, newhouse512);
			int newhouse512_histogram[256];
			Mat newhouse512_histogramat(percent, 256, CV_8U, Scalar(127));
			histogram_equalization(width, height, house512, house512_histogram, newhouse512_histogram, house512_histogramat, newhouse512, newhouse512_histogramat, percent = 100);
			int mask;
			local_histogram_equalization(width, height, house512, house512_histogram, newhouse512_histogram, house512_histogramat, newhouse512, newhouse512_histogramat, mask = 256);
			imshow("newhouse512_local256", newhouse512Mat);
			waitKey();
			destroyWindow("newhouse512_local256");
			imwrite("newhouse512_local256.png", newhouse512Mat);

			imshow("newhouse512_histogramat_local256", newhouse512_histogramat);
			waitKey();
			destroyWindow("newhouse512_histogramat_local256");
			imwrite("newhouse512_histogram_local256.png", newhouse512_histogramat);

			local_histogram_equalization(width, height, house512, house512_histogram, newhouse512_histogram, house512_histogramat, newhouse512, newhouse512_histogramat, mask = 128);
			imshow("newhouse512_local128", newhouse512Mat);
			waitKey();
			destroyWindow("newhouse512_local128");
			imwrite("newhouse512_local128.png", newhouse512Mat);

			imshow("newhouse512_histogramat_local128", newhouse512_histogramat);
			waitKey();
			destroyWindow("newhouse512_histogramat_local128");
			imwrite("newhouse512_histogram_local128.png", newhouse512_histogramat);

			local_histogram_equalization(width, height, house512, house512_histogram, newhouse512_histogram, house512_histogramat, newhouse512, newhouse512_histogramat, mask = 64);
			imshow("newhouse512_local64", newhouse512Mat);
			waitKey();
			destroyWindow("newhouse512_local64");
			imwrite("newhouse512_local64.png", newhouse512Mat);

			imshow("newhouse512_histogramat_local64", newhouse512_histogramat);
			waitKey();
			destroyWindow("newhouse512_histogramat_local64");
			imwrite("newhouse512_histogram_local64.png", newhouse512_histogramat);

			local_histogram_equalization(width, height, house512, house512_histogram, newhouse512_histogram, house512_histogramat, newhouse512, newhouse512_histogramat, mask = 32);
			imshow("newhouse512_local32", newhouse512Mat);
			waitKey();
			destroyWindow("newhouse512_local32");
			imwrite("newhouse512_local32.png", newhouse512Mat);

			imshow("newhouse512_histogramat_local32", newhouse512_histogramat);
			waitKey();
			destroyWindow("newhouse512_histogramat_local32");
			imwrite("newhouse512_histogram_local32.png", newhouse512_histogramat);
			break;
		}

		}//1-小switch
		break;
	}
	case'2':
	{
		//2
		cout << "目前第2題" << endl;
		unsigned char* average10 = new unsigned char[960 * 540];
		unsigned char* average200 = new unsigned char[960 * 540];
		unsigned char* average477 = new unsigned char[960 * 540];
		img_average(average10, 10);
		Mat aver10Mat = Mat(540, 960, CV_8UC1, average10);
		
		

		img_average(average200, 200);
		Mat aver200Mat = Mat(540, 960, CV_8UC1, average200);
		

		img_average(average477, 477);
		Mat aver477Mat = Mat(540, 960, CV_8UC1, average477);
		imshow("aver10MatFile", aver10Mat);
		waitKey();
		destroyWindow("aver10MatFile");
		imwrite("aver10.png", aver10Mat);

		imshow("aver200MatFile", aver200Mat);
		waitKey();
		destroyWindow("aver200MatFile");
		imwrite("aver200.png", aver200Mat);

		imshow("aver477MatFile", aver477Mat);
		waitKey();
		destroyWindow("aver477MatFile");
		imwrite("aver477.png", aver477Mat);
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
			int padding = 1;
			int laplace_filter1[3][3] = { { 0, -1,  0},
										  {-1,  4, -1},
										  { 0, -1,  0} };
			int laplace_filter2[3][3] = { { -1, -1,  -1},
										  {-1,  8, -1},
										  { -1, -1,  -1} };
			unsigned char* walkbridge = new unsigned char[size];
			read_file(512, 512, "walkbridge.raw", walkbridge);
			Mat walkbridgemat = Mat(512, 512, CV_8UC1, walkbridge);
			unsigned char* walkbridge_zero_padding = new unsigned char[(width + 2 * padding) * (height + 2 * padding)];
			zero_padding(width, height, walkbridge, walkbridge_zero_padding, 1);
			Mat walkbridg_zero_paddingemat = Mat(width + 2 * padding, width + 2 * padding, CV_8UC1, walkbridge_zero_padding);
			unsigned char* walkbridge_afterfilter1 = new unsigned char[size];
			filtering_3x3(width, height, walkbridge_zero_padding, walkbridge_afterfilter1, laplace_filter1,1);//514;514
			Mat walkbridge_afterfilter1mat = Mat(512, 512, CV_8UC1, walkbridge_afterfilter1);
			imshow("walkbridge_afterfilter1mat", walkbridge_afterfilter1mat);
			waitKey();
			destroyWindow("walkbridge_afterfilter1mat");
			imwrite("walkbridge_afterfilter1mat.png", walkbridge_afterfilter1mat);

			unsigned char* walkbridge_afterfilter2 = new unsigned char[size];
			filtering_3x3(width, height, walkbridge_zero_padding, walkbridge_afterfilter2, laplace_filter2,1);//514;514
			Mat walkbridge_afterfilter2mat = Mat(512, 512, CV_8UC1, walkbridge_afterfilter2);
			imshow("walkbridge_afterfilter2mat", walkbridge_afterfilter2mat);
			waitKey();
			destroyWindow("walkbridge_afterfilter2mat");
			imwrite("walkbridge_afterfilter2mat.png", walkbridge_afterfilter2mat);
			break;
		}
		case 'b':
		{
			//1.2
			cout << "目前第三大題中第b小題" << endl;
			int padding = 1;
			int laplace_filter1[3][3] = { { 0, -1,  0},
										  {-1,  4, -1},
										  { 0, -1,  0} };
			int laplace_filter2[3][3] = { { -1, -1,  -1},
										  {-1,  8, -1},
										  { -1, -1,  -1} };
			unsigned char* walkbridge = new unsigned char[size];
			read_file(512, 512, "walkbridge.raw", walkbridge);
			Mat walkbridgemat = Mat(512, 512, CV_8UC1, walkbridge);
			unsigned char* walkbridge_mirror_padding = new unsigned char[(width + 2 * padding) * (height + 2 * padding)];
			mirror_padding(width, height, walkbridge, walkbridge_mirror_padding, 1);
			Mat walkbridg_mirror_paddingemat = Mat(width + 2 * padding, width + 2 * padding, CV_8UC1, walkbridge_mirror_padding);
			unsigned char* walkbridge_mirror_afterfilter1 = new unsigned char[size];
			highboost_filtering_3x3(width, height, walkbridge_mirror_padding, walkbridge_mirror_afterfilter1, laplace_filter1,1,1);//514;514,a=1
			Mat walkbridge_mirror_afterfilter1mat = Mat(512, 512, CV_8UC1, walkbridge_mirror_afterfilter1);
			
			imshow("walkbridge_mirror_afterfilter1mat_a1", walkbridge_mirror_afterfilter1mat);
			waitKey();
			destroyWindow("walkbridge_mirror_afterfilter1mat_a1");
			imwrite("walkbridge_mirror_afterfilter1mat_a1.png", walkbridge_mirror_afterfilter1mat);

			unsigned char* walkbridge_mirror_afterfilter2 = new unsigned char[size];
			highboost_filtering_3x3(width, height, walkbridge_mirror_padding, walkbridge_mirror_afterfilter2, laplace_filter1,1,2);//514;514,a=2
			Mat walkbridge_mirror_afterfilter2mat = Mat(512, 512, CV_8UC1, walkbridge_mirror_afterfilter2);
			imshow("walkbridge_mirror_afterfilter1mat_a2", walkbridge_mirror_afterfilter2mat);
			waitKey();
			destroyWindow("walkbridge_mirror_afterfilter1mat_a2");
			imwrite("walkbridge_mirror_afterfilter1mat_a2.png", walkbridge_mirror_afterfilter2mat);

			unsigned char* walkbridge_mirror_afterfilter3 = new unsigned char[size];
			highboost_filtering_3x3(width, height, walkbridge_mirror_padding, walkbridge_mirror_afterfilter3, laplace_filter1, 1, 4);//514;514,a=4
			Mat walkbridge_mirror_afterfilter3mat = Mat(512, 512, CV_8UC1, walkbridge_mirror_afterfilter3);
			imshow("walkbridge_mirror_afterfilter1mat_a4", walkbridge_mirror_afterfilter3mat);
			waitKey();
			destroyWindow("walkbridge_mirror_afterfilter1mat_a4");
			imwrite("walkbridge_mirror_afterfilter1mat_a4.png", walkbridge_mirror_afterfilter3mat);

			//fliter2
			unsigned char* walkbridge_mirror_afterfilter4 = new unsigned char[size];
			highboost_filtering_3x3(width, height, walkbridge_mirror_padding, walkbridge_mirror_afterfilter4, laplace_filter2, 1, 1);//514;514,a=1
			Mat walkbridge_mirror_afterfilter4mat = Mat(512, 512, CV_8UC1, walkbridge_mirror_afterfilter4);
			imshow("walkbridge_mirror_afterfilter2mat_a1", walkbridge_mirror_afterfilter4mat);
			waitKey();
			destroyWindow("walkbridge_mirror_afterfilter2mat_a1");
			imwrite("walkbridge_mirror_afterfilter2mat_a1.png", walkbridge_mirror_afterfilter4mat);

			unsigned char* walkbridge_mirror_afterfilter5 = new unsigned char[size];
			highboost_filtering_3x3(width, height, walkbridge_mirror_padding, walkbridge_mirror_afterfilter5, laplace_filter2, 1, 2);//514;514,a=2
			Mat walkbridge_mirror_afterfilter5mat = Mat(512, 512, CV_8UC1, walkbridge_mirror_afterfilter5);
			imshow("walkbridge_mirror_afterfilter2mat_a2", walkbridge_mirror_afterfilter5mat);
			waitKey();
			destroyWindow("walkbridge_mirror_afterfilter2mat_a2");
			imwrite("walkbridge_mirror_afterfilter2mat_a2.png", walkbridge_mirror_afterfilter5mat);

			unsigned char* walkbridge_mirror_afterfilter6 = new unsigned char[size];
			highboost_filtering_3x3(width, height, walkbridge_mirror_padding, walkbridge_mirror_afterfilter6, laplace_filter2, 1, 4);//514;514,a=4
			Mat walkbridge_mirror_afterfilter6mat = Mat(512, 512, CV_8UC1, walkbridge_mirror_afterfilter6);
			imshow("walkbridge_mirror_afterfilter2mat_a4", walkbridge_mirror_afterfilter6mat);
			waitKey();
			destroyWindow("walkbridge_mirror_afterfilter2mat_a4");
			imwrite("walkbridge_mirror_afterfilter2mat_a4.png", walkbridge_mirror_afterfilter6mat);
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
			int padding = 2;
			double gaussian_filter1[5][5]{};
			double gaussian_filter2[5][5]{};
			double gaussian_filter3[5][5]{};
			unsigned char* turtle512 = new unsigned char[size];
			read_file(512, 512, "turtle512.raw", turtle512);
			Mat turtle512mat = Mat(512, 512, CV_8UC1, turtle512);
			unsigned char* turtle512_mirror_padding = new unsigned char[(width + 2 * padding) * (height + 2 * padding)];
			mirror_padding(width, height, turtle512, turtle512_mirror_padding, 2);
			Mat turtle512_mirror_paddingemat = Mat(width + 2 * padding, width + 2 * padding, CV_8UC1, turtle512_mirror_padding);
			unsigned char* turtle512_mirror_afterfilter1 = new unsigned char[size];
			gaussian_filter5x5_set(0.8, gaussian_filter1);
			double sum = 0;
			for (int x = -2; x <= 2; x++) {
				for (int y = -2; y <= 2; y++) {
					cout << gaussian_filter1[x + 2][y + 2] << " ";
					sum = sum + gaussian_filter1[x + 2][y + 2];
				}
				cout << endl;
			}
			cout << sum;
			filtering_5x5(width, height, turtle512_mirror_padding, turtle512_mirror_afterfilter1, gaussian_filter1, 2);//516
			Mat turtle512_mirror_afterfilter1mat = Mat(512, 512, CV_8UC1, turtle512_mirror_afterfilter1);
			imshow("turtle512_mirror_afterfilter8", turtle512_mirror_afterfilter1mat);
			waitKey();
			destroyWindow("turtle512_mirror_afterfilter8");
			imwrite("turtle512_mirror_afterfilter8.png", turtle512_mirror_afterfilter1mat);

			unsigned char* turtle512_mirror_afterfilter2 = new unsigned char[size];
			gaussian_filter5x5_set(1.3, gaussian_filter2);
			sum = 0;
			for (int x = -2; x <= 2; x++) {
				for (int y = -2; y <= 2; y++) {
					cout << gaussian_filter2[x + 2][y + 2] << " ";
					sum = sum + gaussian_filter2[x + 2][y + 2];
				}
				cout << endl;
			}
			cout << sum;
			filtering_5x5(width, height, turtle512_mirror_padding, turtle512_mirror_afterfilter2, gaussian_filter2, 2);//516
			Mat turtle512_mirror_afterfilter2mat = Mat(512, 512, CV_8UC1, turtle512_mirror_afterfilter2);
			imshow("turtle512_mirror_afterfilter13", turtle512_mirror_afterfilter2mat);
			waitKey();
			destroyWindow("turtle512_mirror_afterfilter13");
			imwrite("turtle512_mirror_afterfilter13.png", turtle512_mirror_afterfilter2mat);

			unsigned char* turtle512_mirror_afterfilter3 = new unsigned char[size];
			gaussian_filter5x5_set(2.0, gaussian_filter3);
			sum = 0;
			for (int x = -2; x <= 2; x++) {
				for (int y = -2; y <= 2; y++) {
					cout << gaussian_filter3[x + 2][y + 2] << " ";
					sum = sum + gaussian_filter3[x + 2][y + 2];
				}
				cout << endl;
			}
			cout << sum;
			filtering_5x5(width, height, turtle512_mirror_padding, turtle512_mirror_afterfilter3, gaussian_filter3, 2);//516
			Mat turtle512_mirror_afterfilter3mat = Mat(512, 512, CV_8UC1, turtle512_mirror_afterfilter3);
			imshow("turtle512_mirror_afterfilter20", turtle512_mirror_afterfilter3mat);
			waitKey();
			destroyWindow("turtle512_mirror_afterfilter20");
			imwrite("turtle512_mirror_afterfilter20.png", turtle512_mirror_afterfilter3mat);
			break;
		}
		case 'b':
		{
			//1.2
			cout << "目前第四大題中第b小題" << endl;
			int padding = 2;
			double different_of_gaussian_filter[5][5]{};

			unsigned char* turtle512 = new unsigned char[size];
			read_file(512, 512, "turtle512.raw", turtle512);
			Mat turtle512mat = Mat(512, 512, CV_8UC1, turtle512);
			unsigned char* turtle512_mirror_padding = new unsigned char[(width + 2 * padding) * (height + 2 * padding)];
			mirror_padding(width, height, turtle512, turtle512_mirror_padding, 2);
			Mat turtle512_mirror_paddingemat = Mat(width + 2 * padding, width + 2 * padding, CV_8UC1, turtle512_mirror_padding);
			unsigned char* turtle512_mirror_afterfilter1 = new unsigned char[size];
			different_of_gaussian_filter5x5_set(0.5, 1.5, different_of_gaussian_filter);
			/*double sum = 0;
			for (int x = -2; x <= 2; x++) {
				for (int y = -2; y <= 2; y++) {
					cout << different_of_gaussian_filter[x + 2][y + 2] << " ";
					sum = sum + different_of_gaussian_filter[x + 2][y + 2];
				}
				cout << endl;
			}
			cout << sum;*/
			filtering_5x5(width, height, turtle512_mirror_padding, turtle512_mirror_afterfilter1, different_of_gaussian_filter, 2);//516
			Mat turtle512_mirror_afterfilter1mat = Mat(512, 512, CV_8UC1, turtle512_mirror_afterfilter1);
			imshow("turtle512_DoG", turtle512_mirror_afterfilter1mat);
			waitKey();
			destroyWindow("turtle512_DoG");
			imwrite("turtle512_DoG.png", turtle512_mirror_afterfilter1mat);
			break;
		}

		}//4-小switch
		break;
	}
	}//大題switch
	return  main();
}//main

