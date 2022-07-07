#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int const width = 256;
int const height = 256;




int main() {
	int const size = width * height;

	FILE* lenaFile;
	lenaFile = fopen("lena_256.raw", "rb");
	FILE* saveFile;
	
	if (lenaFile == NULL)
	{
		cout << "Loading File Error!";
		system("PAUSE");
		return true;
	}
	unsigned char* lena = new unsigned char[size];
	fread(lena, sizeof(unsigned char), size, lenaFile);
	
	//1.2b
	//cout << size << endl;
	//cout << "1st row 1st col: " << (int)lena[0] << endl;
	cout << "1.2(a)" << endl;
	cout << "123th row 234th col: " << (int)lena[123 * width + 234] << endl;
	cout << "1.2(b)" << endl;
	cout << "5487th pixel: " << (int)lena[5486] << endl;
	cout << "5487th pixel is at: " << 5486/width <<" row " <<5486%height<<" col"<< endl;

	//1.2c
	Mat img = Mat(height, width, CV_8UC1, lena);
	imshow("1.2c", img);
	waitKey();
	destroyWindow("1.2c");
	saveFile = fopen("1.2c.raw", "wb");
	fwrite(lena, 1, size, saveFile);

	//1.2d
	unsigned char rotate[width][height];
	int i = 0;
	int j = 0;
	while (i < width) {
		while (j < height) {
			rotate[j][i] = lena[i * width + j];
			j++;
		}
		i++;
		j = 0;
	}

	Mat rotateimg = Mat(height, width, CV_8UC1, rotate);
	imshow("1.2d", rotateimg);
	waitKey();
	destroyWindow("1.2d");
	unsigned char* temp = new unsigned char[size];
	int row, column;
	i = 0;
	for (row = 0; row < 256; row++) {
		for (column = 0; column < 256; column++) {
			temp[i] = rotate[row][column];
			i++;
		}
	}
	saveFile = fopen("1.2d.raw", "wb");
	fwrite(temp, 1, size, saveFile);

	//1.2e

	unsigned char imgcut[16][4097];

	i = 0;
	j = 0;
	int k = 0;
	int l = 0;
	int o = 0;
	int jj = 0; int kk = 0; int ll = 0; int oo = 0;
	int jjj = 0; int kkk = 0; int lll = 0; int ooo = 0;
	int jjjj = 0; int kkkk = 0; int llll = 0; int oooo = 0;

	fread(lena, sizeof(unsigned char), size, lenaFile);
	while (i < 65536) {
		if (i < 16384) {
			if (i / 64 % 4 == 0) { imgcut[0][j] = lena[i]; j++; i++; continue; }
			if (i / 64 % 4 == 1) { imgcut[1][k] = lena[i]; k++; i++; continue; }
			if (i / 64 % 4 == 2) { imgcut[2][l] = lena[i]; l++; i++; continue; }
			if (i / 64 % 4 == 3) { imgcut[3][o] = lena[i]; o++; i++; continue; }
		}
		if (i < 32768 && i>16383) {
			if (i / 64 % 4 == 0) { imgcut[4][jj] = lena[i]; jj++; i++; continue; }
			if (i / 64 % 4 == 1) { imgcut[5][kk] = lena[i]; kk++; i++; continue; }
			if (i / 64 % 4 == 2) { imgcut[6][ll] = lena[i]; ll++; i++; continue; }
			if (i / 64 % 4 == 3) { imgcut[7][oo] = lena[i]; oo++; i++; continue; }
		}
		if (i < 49152 && i>32767) {
			if (i / 64 % 4 == 0) { imgcut[8][jjj] = lena[i]; jjj++; i++; continue; }
			if (i / 64 % 4 == 1) { imgcut[9][kkk] = lena[i]; kkk++; i++; continue; }
			if (i / 64 % 4 == 2) { imgcut[10][lll] = lena[i]; lll++; i++; continue; }
			if (i / 64 % 4 == 3) { imgcut[11][ooo] = lena[i]; ooo++; i++; continue; }
		}
		if (i < 65536 && i>49151) {
			if (i / 64 % 4 == 0) { imgcut[12][jjjj] = lena[i]; jjjj++; i++;  continue; }
			if (i / 64 % 4 == 1) { imgcut[13][kkkk] = lena[i]; kkkk++; i++;  continue; }
			if (i / 64 % 4 == 2) { imgcut[14][llll] = lena[i]; llll++; i++;  continue; }
			if (i / 64 % 4 == 3) { imgcut[15][oooo] = lena[i]; oooo++; i++;  continue; }
		}
	}


	int seg[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	srand(time(NULL));
	int container, a, b;
	for (int i = 0; i < 20; i++)
	{
		a = rand() % 16;
		b = rand() % 16;
		container = seg[a];
		seg[a] = seg[b];
		seg[b] = container;
	}
	i = 0;
	j = 0;
	l = 0;
	//存左1
	while (i < 64) {
		while (j < 64) {
			rotate[i][j] = imgcut[seg[0]][l];
			l++;
			j++;
		}
		i++;
		j = 0;
	}
	i = 0;
	j = 64;
	l = 0;
	//存左2
	while (i < 64) {
		while (j < 128) {
			rotate[i][j] = imgcut[seg[1]][l];
			l++;
			j++;
		}
		i++;
		j = 64;
	}
	i = 0;
	j = 128;
	l = 0;
	//存左3
	while (i < 64) {
		while (j < 192) {
			rotate[i][j] = imgcut[seg[2]][l];
			l++;
			j++;
		}
		i++;
		j = 128;
	}
	i = 0;
	j = 192;
	l = 0;
	//存左4
	while (i < 64) {
		while (j < 256) {
			rotate[i][j] = imgcut[seg[3]][l];
			l++;
			j++;
		}
		i++;
		j = 192;
	}
	i = 64;
	j = 0;
	l = 0;
	//存2-1
	while (i < 128) {
		while (j < 64) {
			rotate[i][j] = imgcut[seg[4]][l];
			l++;
			j++;
		}
		i++;
		j = 0;
	}
	i = 64;
	j = 64;
	l = 0;
	//存2-2
	while (i < 128) {
		while (j < 128) {
			rotate[i][j] = imgcut[seg[5]][l];
			l++;
			j++;
		}
		i++;
		j = 64;
	}
	i = 64;
	j = 128;
	k = 0;
	l = 0;
	//存2-3
	while (i < 128) {
		while (j < 192) {
			rotate[i][j] = imgcut[seg[6]][l];
			l++;
			j++;
		}
		i++;
		j = 128;
	}
	i = 64;
	j = 192;
	l = 0;
	//存2-4
	while (i < 128) {
		while (j < 256) {
			rotate[i][j] = imgcut[seg[7]][l];
			l++;
			j++;
		}
		i++;
		j = 192;
	}
	i = 128;
	j = 0;
	l = 0;
	//存3-1
	while (i < 192) {
		while (j < 64) {
			rotate[i][j] = imgcut[seg[8]][l];
			l++;
			j++;
		}
		i++;
		j = 0;
	}
	i = 128;
	j = 64;
	l = 0;
	//存3-2
	while (i < 192) {
		while (j < 128) {
			rotate[i][j] = imgcut[seg[9]][l];
			l++;
			j++;
		}
		i++;
		j = 64;
	}
	i = 128;
	j = 128;
	l = 0;
	//存3-3
	while (i < 192) {
		while (j < 192) {
			rotate[i][j] = imgcut[seg[10]][l];
			l++;
			j++;
		}
		i++;
		j = 128;
	}
	i = 128;
	j = 192;
	l = 0;
	//存3-4
	while (i < 192) {
		while (j < 256) {
			rotate[i][j] = imgcut[seg[11]][l];
			l++;
			j++;
		}
		i++;
		j = 192;
	}
	i = 192;
	j = 0;
	l = 0;
	//存4-1
	while (i < 256) {
		while (j < 64) {
			rotate[i][j] = imgcut[seg[12]][l];
			l++;
			j++;
		}
		i++;
		j = 0;
	}
	i = 192;
	j = 64;
	l = 0;
	//存4-2
	while (i < 256) {
		while (j < 128) {
			rotate[i][j] = imgcut[seg[13]][l];
			l++;
			j++;
		}
		i++;
		j = 64;
	}
	i = 192;
	j = 128;
	k = 0;
	l = 0;
	//存4-3
	while (i < 256) {
		while (j < 192) {
			rotate[i][j] = imgcut[seg[14]][l];
			l++;
			j++;
		}
		i++;
		j = 128;
	}
	i = 192;
	j = 192;
	k = 0;
	l = 0;
	//存4-4
	while (i < 256) {
		while (j < 256) {
			rotate[i][j] = imgcut[seg[15]][l];
			l++;
			j++;
		}
		i++;
		j = 192;
	}
	Mat cut = Mat(256, 256, CV_8UC1, rotate);
	imshow("1.2e", cut);
	waitKey();
	destroyWindow("1.2e");

	
	i = 0;
	for (row = 0; row < 256; row++) {
		for (column = 0; column < 256; column++) {
			temp[i] = rotate[row][column];
			i++;
		}
	}
	saveFile = fopen("1.2e.raw", "wb");
	fwrite(temp, 1, size, saveFile);





	//1.2f
	FILE* newFile;
	newFile = fopen("p03.raw", "rb");
	unsigned char* moon = new unsigned char[65536];
	fread(moon, sizeof(unsigned char), size, newFile);
	unsigned char newmoon[512][512];
	i = 0;
	j = 0;
	k = 0;
	l = 0;
	//存左上
	while (i < 256) {
		while (j < 256) {
			newmoon[i][j] = moon[k * width + l];
			l++;
			j++;
		}
		i++;
		k++;
		l = 0;
		j = 0;
	}
	newFile = fopen("p04.raw", "rb");
	fread(moon, sizeof(unsigned char), size, newFile);

	i = 0;
	j = 256;
	k = 0;
	l = 0;
	while (i < 256) {
		while (j < 512) {
			newmoon[i][j] = moon[k * width + l];
			j++;
			l++;
		}
		i++;
		k++;
		l = 0;
		j = 256;
	}
	newFile = fopen("p02.raw", "rb");

	fread(moon, sizeof(unsigned char), size, newFile);

	i = 256;
	j = 0;
	k = 0;
	l = 0;
	while (i < 512) {
		while (j < 256) {
			newmoon[i][j] = moon[k * width + l];
			j++;
			l++;
		}
		i++;
		k++;
		j = 0;
		l = 0;
	}
	newFile = fopen("p01.raw", "rb");
	fread(moon, sizeof(unsigned char), size, newFile);

	i = 256;
	j = 256;
	k = 0;
	l = 0;
	while (i < 512) {
		while (j < 512) {
			newmoon[i][j] = moon[k * width + l];
			j++;
			l++;
		}
		i++;
		k++;
		j = 256;
		l = 0;
	}
	Mat newmoonimg = Mat(512, 512, CV_8UC1, newmoon);
	imshow("1.2f", newmoonimg);
	waitKey();
	destroyWindow("1.2f");
	unsigned char* temp512 = new unsigned char[512 * 512];
	i = 0;
	
	for (row = 0; row < 512; row++) {
		for (column = 0; column < 512; column++) {
			temp512[i] = newmoon[row][column];
			i++;
		}
	}
	saveFile = fopen("1.2f.raw", "wb");
	fwrite(temp512, 1, 512*512, saveFile);

	//1.3a
	unsigned char bright[width][height];
	i = 0;
	j = 0;
	while (i < width) {
		while (j < height) {
			bright[i][j] = (lena[i * width + j] + 50 > 255) ? 255 : lena[i * width + j] + 50;
			j++;
		}
		i++;
		j = 0;
	}
	Mat brightimg = Mat(height, width, CV_8UC1, bright);

	imshow("brightFile", brightimg);
	waitKey();
	destroyWindow("brightFile");
	i = 0;
	for (row = 0; row < 256; row++) {
		for (column = 0; column < 256; column++) {
			temp[i] = bright[row][column];
			i++;
		}
	}
	saveFile = fopen("1.3a.raw", "wb");
	fwrite(temp, 1, size, saveFile);
	//1.3b
	char bright2[width][height];
	i = 0;
	j = 0;
	while (i < width) {
		while (j < height) {
			bright2[i][j] = lena[i * width + j] / 0.390625 - 50;
			j++;
		}
		i++;
		j = 0;
	}
	Mat brightimg2 = Mat(height, width, CV_8SC1, bright2);

	imshow("brightFile2", brightimg2);
	waitKey();
	destroyWindow("brightFile2");

	i = 0;
	for (row = 0; row < 256; row++) {
		for (column = 0; column < 256; column++) {
			temp[i] = bright2[row][column];
			i++;
		}
	}
	saveFile = fopen("1.3b.raw", "wb");
	fwrite(temp, 1, size, saveFile);
	
	//2
	unsigned char* haha = new unsigned char[512 * 512];
	FILE* hahaFile;
	hahaFile = fopen("haha.raw", "rb");
	fread(haha, sizeof(unsigned char), 512 * 512, hahaFile);
	Mat hahaimg = Mat(512, 512, CV_8UC1, haha);
	putText(hahaimg, "109318417", Point(168, 50), 2, 1, (0, 0, 0), 1, 1, 0);
	putText(hahaimg, "HW1finish~", Point(190, 403), 2, 1, (0, 0, 0), 1, 1, 0);
	imshow("haha", hahaimg);
	waitKey();
	destroyWindow("haha");
	

	imwrite("2.png", hahaimg);
}
	





