#pragma once
#pragma once
#include <Windows.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;


const int nwidth = 1200; // width // resolution
const float aspect = 11.0 / 24.0; // aspect pixel to ascii
string gradient = " .:!/r(lZ4H9W8$@"; //shadowing // "     .:!/r(lZ4H9W8$@" adding spaces causes shadow increasing

// Function to convert RGB to HSV
std::tuple<double, double, double> RGBtoHSV(int red, int green, int blue) {
	double r = red / 255.0;
	double g = green / 255.0;
	double b = blue / 255.0;

	double maxVal = std::max({ r, g, b });
	double minVal = std::min({ r, g, b });
	double delta = maxVal - minVal;

	double h = 0.0, s = 0.0, v = maxVal;

	if (delta == 0) {
		h = 0;
	}
	else if (maxVal == r) {
		h = 60 * ((g - b) / delta + (g < b ? 6 : 0));
	}
	else if (maxVal == g) {
		h = 60 * ((b - r) / delta + 2);
	}
	else if (maxVal == b) {
		h = 60 * ((r - g) / delta + 4);
	}

	if (maxVal == 0) {
		s = 0;
	}
	else {
		s = delta / maxVal;
	}

	return std::make_tuple(h, s, v);
}

bool isRedRGB(int red, int green, int blue, int rgbRedThreshold = 150, int rgbDifferenceThreshold = 50, double hueLower = 345, double hueUpper = 9, double saturationThreshold=0.3, double valueThreshold = 0.3) {

	//RGB checks
	if (red < rgbRedThreshold) {
		return false;
	}

	if ((red - green) < rgbDifferenceThreshold || (red - blue) < rgbDifferenceThreshold) {
		return false;
	}

	//HSV checks.
	double h, s, v;
	std::tie(h, s, v) = RGBtoHSV(red, green, blue);

	if (h < 0) {
		h += 360;
	}

	bool hueInRange = (h >= hueLower && h <= hueUpper) || (hueLower > hueUpper && (h >= hueLower || h <= hueUpper));
	bool saturationInRange = s >= saturationThreshold;
	bool valueInRange = v >= valueThreshold;

	return hueInRange && saturationInRange && valueInRange;
}

vector<vector<string>> convertToASCII(Mat image, int scaledRows, int scaledCols, int scale = 1) {

	vector<vector<string>> ascii(scaledRows, vector<string>(scaledCols, " "));

	if (scale == 1) {
		for (int i = 0, x = 0; x < scaledRows; i += scale, x++) {
			for (int j = 0, y = 0; y < scaledCols; j += scale, y++) {

				//if scale == 1
				Vec3b pixel = image.at<Vec3b>(i, j);
				//red mask
				//cout << isRedRGB(pixel[2], pixel[1], pixel[0]);

				int color = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);
				ascii[x][y] = gradient[color / gradient.size()] + (isRedRGB(pixel[2], pixel[1], pixel[0]) ? string("red") : string(""));
			}
			//cout << endl;
		}

		return ascii;
	}

	//else
	for (int i = 0, x = 0; x < scaledRows; i += scale, x++) {
		for (int j = 0, y = 0; y < scaledCols; j += scale, y++) {

			//taking pixels
			//if scale > 1
			int colorSum = 0;
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
				colorSum += color;
			}

			//cout << isRedRGB(r / pixelCount, g / pixelCount, b / pixelCount);

			//if scale == 1
			/*Vec3b pixel = image.at<Vec3b>(i, j);
			int color = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);*/

			//avg color if scale > 1
			colorSum /= pixelCount;		

			//cout << i << " " << j << " " << x << " " << y << " " << endl;
			int colorRed = isRedRGB(r/ pixelCount, g / pixelCount, b / pixelCount);
			//if scale > 1
			ascii[x][y] = gradient[colorSum / gradient.size()] + (colorRed ? string("red") : string(""));
			//ascii[x][y] = gradient[color / 16];
		}
		//cout << endl;
	}

	return ascii;
}


void printMatrix(vector<vector<string>> matrix) {

	//vector<char> buffer(matrix.size() * matrix[0].size() + matrix.size()); //area + (\n)

	//size_t bufferIndex = 0;
	//for (const auto& row : matrix) {
	//	for (char c : row) {
	//		buffer[bufferIndex++] = c;
	//	}
	//	buffer[bufferIndex++] = '\n'; // Add newline at the end of each row
	//}

	//cout.write(buffer.data(), buffer.size());

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j].size() > 1) { //contains red
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Set text color to red
				cout << matrix[i][j][0];
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //Default color (white)
			}
			else {
				cout << matrix[i][j][0];
			}
			
		}
		cout << endl;
	}
}