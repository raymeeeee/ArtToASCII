#include "ImageToASCIIconvertor.h"



int main(void)
{
	string path;
	cout << "Input path to the file: " << endl;
	cin >> path;
	//string path = R"(C:\Users\valer\Desktop\Files\Pictures\photo_10_2025-04-21_03-49-02.jpg)"; // photo directory

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

	//cout << rows << " " << cols << endl;

	int scale = 1; //SCALExSCALE pixel square // reduction size

	int scaledRows = rows / scale;
	int scaledCols = cols / scale;

	//cout << scaledRows << " " << scaledCols << endl;
	
	// IMPORTANT
	Sleep(2000); // TIME TO RESCALE CONSOLE TO MAXIMUM // IF NOT DONE THE IMAGE WILL BE TEARED //
	// IMPORTANT

#if MODE != 1
	cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
	convertToASCII(image, scaledRows, scaledCols, scale);
#endif

#if MODE == 1
	printMatrix(convertToASCII(image, scaledRows, scaledCols, scale));
#endif
	
	return 0;
}