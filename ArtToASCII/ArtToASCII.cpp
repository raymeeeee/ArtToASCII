#include <Windows.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



using namespace cv;
using namespace std;

//void SetWindow(int Width, int Height)
//{
//	_COORD coord;
//	coord.X = Width;
//	coord.Y = Height;
//	_SMALL_RECT Rect;
//	Rect.Top = 0;
//	Rect.Left = 0;
//	Rect.Bottom = Height - 1;
//	Rect.Right = Width - 1;
//	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleScreenBufferSize(Handle, coord);
//	SetConsoleWindowInfo(Handle, TRUE, &Rect);
//	string str = "mode con cols=" + to_string(Width) + " lines=" + to_string(Height);
//	system(str.c_str());
//}

int main(void)
{
	int nwidth = 340; // width // resolution
	float aspect = 11.0 / 24.0; // aspect pixel to ascii
	string path = "C:\\Users\\valer\\Desktop\\Files\\Pictures\\photo_13_2023-11-30_01-05-22.jpg";
	// Вставьте сюда путь к фото
	Mat image = imread(path, IMREAD_COLOR);
	
	

	Size sz = image.size();
	
	int im_width = sz.width, im_height = sz.height;
	float imasp = (float)im_width / nwidth;
	int nheight = round(im_height * aspect / imasp);
	resize(image, image, { nwidth, nheight }, 0, 0, INTER_NEAREST);

	sz = image.size();

	//SetWindow(nwidth, nheight);

	//wchar_t* screen = new wchar_t[nwidth * nheight + 1];
	
	int rows = sz.height;
	int cols = sz.width;
	cout << rows << " " << cols << endl;
	char** imageASCII = new char* [rows] {};
	for (int i = 0; i < rows; ++i) {
		imageASCII[i] = new char[cols] {};
	}

	const char* gradient = " .:!/r(lZ4H9W8$@";

	Sleep(2000); // TIME TO RESCALE CONSOLE TO MAXIMUM // IF NOT DONE THE IMAGE WILL BE TEARED

	int scale = 1;

	int scaledRows = rows / scale;
	int scaledCols = cols / scale;

	cout << scaledRows << " " << scaledCols << endl;

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
					if (i+m >= rows-1 || j + n >= cols-1) {
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

	for (int i = 0; i < scaledRows; i++) {
		for (int j = 0; j < scaledCols; j++) {
			cout << ascii[i][j] << "";
		}
		cout << endl;
	}

	//for (int i = 0; i < sz.area(); i++) imageASCII[i] = ' ';
	//HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//SetConsoleActiveScreenBuffer(hConsole);
	//DWORD dwBytesWritten = 0;

	

	/*for (int x = 0; x < image.rows; x++) {
		for (int y = 0; y < image.cols; y++) {
			Vec3b pixel = image.at<Vec3b>(x, y);
			int color = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);
			imageASCII[x * nwidth + y] = gradient[color / 16];
		}
	}*/



	//while (1) {

	//	WriteConsoleOutputCharacter(hConsole, screen, nwidth * nheight, { 0, 0 }, &dwBytesWritten);
	//	//std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
	//	Sleep(1000000000);
	//};

	return 0;
}