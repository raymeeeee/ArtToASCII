#include <Windows.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;


const int nwidth = 1000; // width // resolution
const float aspect = 11.0 / 24.0; // aspect pixel to ascii
const char* gradient = " .:!/r(lZ4H9W8$@"; //shadowing

char** convertToASCII(Mat image, int scaledRows, int scaledCols, int scale=1) {

	char** ascii = new char* [scaledRows] {};
	for (int i = 0; i < scaledRows; i++) {
		ascii[i] = new char[scaledCols] {};
	}

	for (int i = 0, x = 0; x < scaledRows; i += scale, x++) {
		for (int j = 0, y = 0; y < scaledCols; j += scale, y++) {

			//taking pixels
			int colorSum = 0;
			for (int m = 0; m < scale; m++) {
				int color = 0;
				for (int n = 0; n < scale; n++) {
					if (i + m >= image.size().height - 1 || j + n >= image.size().width - 1) {
						color = 0;
					}
					else {
						Vec3b pixel = image.at<Vec3b>(i + m, j + n);
						color = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);
					}

				}
				colorSum += color;
			}
			//avg color
			colorSum /= (scale * scale);

			//cout << i << " " << j << " " << x << " " << y << " " << endl;

			ascii[x][y] = gradient[colorSum / 16];
		}
	}

	return ascii;
}

void printMatrix(char** matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << matrix[i][j] << "";
		}
		cout << endl;
	}
}

int main(void)
{
	 
	string path = "C:\\Users\\valer\\Desktop\\Files\\Pictures\\photo_13_2023-11-30_01-05-22.jpg"; // photo directory
	Mat image = imread(path, IMREAD_COLOR);
	
	Size sz = image.size();

	//rescaling
	int rows = sz.width, cols = sz.height;
	float imasp = (float)rows / nwidth;
	int nheight = round(cols * aspect / imasp);
	resize(image, image, { nwidth, nheight }, 0, 0, INTER_NEAREST);

	sz = image.size();
	
	rows = sz.height;
	cols = sz.width;

	cout << rows << " " << cols << endl;

	int scale = 2; //SCALExSCALE pixel square // reduction size

	int scaledRows = rows / scale;
	int scaledCols = cols / scale;

	cout << scaledRows << " " << scaledCols << endl;
	
	Sleep(2000); // TIME TO RESCALE CONSOLE TO MAXIMUM // IF NOT DONE THE IMAGE WILL BE TEARED
	cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
	printMatrix(convertToASCII(image, scaledRows, scaledCols, scale), scaledRows, scaledCols);
	
	return 0;
}