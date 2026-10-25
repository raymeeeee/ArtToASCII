#pragma once
#include <Windows.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <stdexcept>

#include <filesystem>
using namespace std::filesystem;

using namespace cv;
using namespace std;

//#define MODE 1

//1 - default ascii
//2 - red + default ascii
//3 - colored ascii


//DEFAULT ASCII
//int nwidth = 200; // width // resolution
const float aspect = 11.0 / 24.0; // aspect pixel to ascii;
const string gradient = " .:!/r(l24H9W8$@"; //shadowing // "     .:!/r(lZ4H9W8$@" adding spaces causes shadow increasing


//JAPANESE
//const float aspect = 1; // aspect pixel to ascii;
//const vector<string> gradient = {
//	u8"〾", u8"あ", u8"い", u8"う",
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
	// convert RGB to HSV
	auto [h, s, v] = RGBtoHSV(red, green, blue);

	// red color is between 0° and 360° in HSV
	// check to areas with given range
	bool is_red_hue = (h <= hue_range) || (h >= 360.0f - hue_range);

	// check minimal saturation and brightness
	bool is_saturated = s >= min_saturation;
	bool is_bright = v >= min_value;

	// check that red is dominant
	bool red_dominant = (red > green * 1.5f) && (red > blue * 1.5f);

	return is_red_hue && is_saturated && is_bright && red_dominant;
}


vector<vector<string>> convertToASCII(const Mat& image, int scaledRows, int scaledCols, int scale = 1, int mode = 1) {

	if (mode != 1 && mode != 2 && mode != 3) { throw invalid_argument("Mode must be 1, 2 or 3"); }

	//SetConsoleOutputCP(65001); //for japanese char

	vector<vector<string>> ascii(scaledRows, vector<string>(scaledCols, " "));

	if (mode == 2 || mode == 3) { // allows console to be colored
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD consoleMode = 0;
		GetConsoleMode(hConsole, &consoleMode);
		SetConsoleMode(hConsole, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}

	for (int i = 0, x = 0; x < scaledRows; i += scale, x++) {
		for (int j = 0, y = 0; y < scaledCols; j += scale, y++) {

			// --- avg color of block scale x scale ---
			int r = 0, g = 0, b = 0;
			int pixelCount = 0;

			for (int m = 0; m < scale; m++) {
				for (int n = 0; n < scale; n++) {
					if (i + m >= image.rows || j + n >= image.cols) {
						continue; // check if pixel is out of bounds
					}
					Vec3b pixel = image.at<Vec3b>(i + m, j + n);
					b += pixel[0];
					g += pixel[1];
					r += pixel[2];
					pixelCount++;
				}
			}

			// if whole block is out of bounds
			if (pixelCount == 0) {
				ascii[x][y] = " ";
				if (mode == 2 || mode == 3) cout << ' ';
				continue;
			}

			// avg value of r g b of pixels
			r /= pixelCount;
			g /= pixelCount;
			b /= pixelCount;

			int grayscale = (r + g + b) / 3; // 0..255

			// --- choose of symbol ---
			ascii[x][y] = gradient[grayscale * (gradient.size() - 1) / 255];

			// --- immediate colored output ---
			if (mode == 2) {
				if (isRedRGB(r, g, b)) {
					cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
				}
				else {
					cout << "\x1b[38;2;255;255;255m";
				}
				cout << ascii[x][y];
			}
			else if (mode == 3) {
				cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
				cout << ascii[x][y];
			}
		}
		if (mode == 2 || mode == 3) {
			cout << '\n';
		}
	}

	if (mode == 2 || mode == 3) {
		cout << "\x1b[0m"; // console color reset
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