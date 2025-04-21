#pragma once
#include <Windows.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#include <filesystem>
using namespace std::filesystem;

using namespace cv;
using namespace std;

#define MODE 3

//1 - default ascii
//2 - red + default ascii
//3 - colored ascii


//DEFAULT ASCII
int nwidth = 600; // width // resolution
const float aspect = 11.0 / 24.0; // aspect pixel to ascii;
const string gradient = " .:!/r(l24H9W8$@"; //shadowing // "     .:!/r(lZ4H9W8$@" adding spaces causes shadow increasing


//JAPANESE
//const float aspect = 1; // aspect pixel to ascii;
//const vector<string> gradient = {
//	u8"・", u8"。", u8"、", u8"〃", u8"〄",
//	u8"々", u8"〆", u8"〇", u8"〒", u8"〓",
//	u8"〠", u8"〰", u8"〱", u8"〲", u8"〳",
//	u8"〴", u8"〵", u8"〶", u8"〷", u8"〸",
//	u8"〹", u8"〺", u8"〻", u8"〼", u8"〽",
//	u8"〾", u8"〿", u8"あ", u8"い", u8"う",
//	u8"え", u8"お", u8"か", u8"き", u8"く",
//	u8"け", u8"こ", u8"漢", u8"字", u8"読",
//	u8"書", u8"語", u8"文", u8"章", u8"学"
//};

// Function to convert RGB to HSV
tuple<double, double, double> RGBtoHSV(int red, int green, int blue) {
	double r = red / 255.0;
	double g = green / 255.0;
	double b = blue / 255.0;

	double maxVal = max({ r, g, b });
	double minVal = min({ r, g, b });
	double delta = maxVal - minVal;

	double h = 0.0, s = 0.0, v = maxVal;

	if (delta != 0.0) {
		if (maxVal == r) {
			h = 60 * fmod(((g - b) / delta), 6.0f);
		}
		else if (maxVal == g) {
			h = 60 * (((b - r) / delta) + 2.0f);
		}
		else if (maxVal == b) {
			h = 60 * (((r - g) / delta) + 4.0f);
		}
	}
	if (h < 0.0f) h += 360.0f;

	s = (maxVal == 0.0f) ? 0.0f : (delta / maxVal);

	return make_tuple(h, s, v);
}

bool isRedRGB(int red, int green, int blue, float min_saturation = 0.4f,
	float min_value = 0.3f,
	float hue_range = 20.0f) {
	// Преобразуем RGB в HSV
	auto [h, s, v] = RGBtoHSV(red, green, blue);

	// Красный цвет находится около 0° и 360° в HSV
	// Мы проверяем два диапазона с учетом заданного диапазона оттенков
	bool is_red_hue = (h <= hue_range) || (h >= 360.0f - hue_range);

	// Проверяем минимальную насыщенность и яркость
	bool is_saturated = s >= min_saturation;
	bool is_bright = v >= min_value;

	// Также можно добавить проверку, что красный компонент преобладает
	bool red_dominant = (red > green * 1.5f) && (red > blue * 1.5f);

	return is_red_hue && is_saturated && is_bright && red_dominant;
}


vector<vector<string>> convertToASCII(Mat image, int scaledRows, int scaledCols, int scale = 1) {
	SetConsoleOutputCP(65001); //for japanese chars
	vector<vector<string>> ascii(scaledRows, vector<string>(scaledCols, " "));
#if MODE != 1
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hConsole, &mode);
	SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

	if (scale == 1) {
		for (int i = 0, x = 0; x < scaledRows; i += scale, x++) {
			for (int j = 0, y = 0; y < scaledCols; j += scale, y++) {

				//if scale == 1
				Vec3b pixel = image.at<Vec3b>(i, j);
				int r = pixel[2];
				int g = pixel[1];
				int b = pixel[0];

				int grayscale = round((r + g + b) / 3.0);

#if MODE == 1
				
				ascii[x][y] = gradient[grayscale / gradient.size()];
#endif

#if MODE == 2
				int colorRed = isRedRGB(r, g, b);
				ascii[x][y] = gradient[grayscale / gradient.size()];
				if (colorRed) {
					string colorConsole = "\x1b[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
					cout << colorConsole;
					cout << ascii[x][y];
				}
				else {
					cout << "\x1b[38;2;255;255;255m";
					cout << ascii[x][y];
				}
#endif

#if MODE == 3

				string colorConsole = "\x1b[38;2;" + to_string(pixel[2]) + ";" + to_string(pixel[1]) + ";" + to_string(pixel[0]) + "m";
				ascii[x][y] = gradient[grayscale / gradient.size()];
				cout << colorConsole;
				cout << ascii[x][y];
#endif

			}
#if MODE != 1
			cout << endl;
#endif
		}
		return ascii;
	}

	//else
	for (int i = 0, x = 0; x < scaledRows; i += scale, x++) {
		for (int j = 0, y = 0; y < scaledCols; j += scale, y++) {

			//taking pixels
			//if scale > 1

			int grayscale = 0;

			int r = 0;
			int g = 0;
			int b = 0;

			int pixelCount = 0;
			for (int m = 0; m < scale; m++) {
				int color = 0;
				for (int n = 0; n < scale; n++) {
					if (i + m >= image.size().height - 1 || j + n >= image.size().width - 1) {
						color = 0;
					}
					else {
						pixelCount++;
						Vec3b pixel = image.at<Vec3b>(i + m, j + n);
						color = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);

						r += pixel[2];
						g += pixel[1];
						b += pixel[0];


					}

				}
				grayscale += color;
			}

			//avg color if scale > 1
			grayscale /= pixelCount;
			r /= pixelCount;
			g /= pixelCount;
			b /= pixelCount;

#if MODE == 1
			ascii[x][y] = gradient[grayscale / gradient.size()];
#endif

#if MODE == 2
			int colorRed = isRedRGB(r,g,b);
			ascii[x][y] = gradient[grayscale / gradient.size()];
			if (colorRed) {
				string colorConsole = "\x1b[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
				cout << colorConsole;
				cout << ascii[x][y];
			}
			else {
				cout << "\x1b[38;2;255;255;255m";
				cout << ascii[x][y];
			}


#endif

#if MODE == 3
			string colorConsole = "\x1b[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
			ascii[x][y] = gradient[grayscale / gradient.size()];
			cout << colorConsole;
			cout << ascii[x][y];
#endif
			
		}
#if MODE != 1
		cout << endl;
#endif
	}

	return ascii;
}


void printMatrix(vector<vector<string>> matrix) {
	string image = "";
	for (auto row : matrix) {
		for (auto c : row) {
			image += c;
		}
		image += '\n'; // Add newline at the end of each row
	}
	cout << image;
}