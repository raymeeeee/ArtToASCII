#include "ImageToASCIIconvertor.h"



int main(void)
{

	int MODE;
	cout << "Mode: (1 - default ascii), (2 - red coloring only), (3 - full color)\n";
	cin >> MODE;

	int nwidth = 200; // width // resolution
	cout << "Width of ascii representation of your image:\n";
	cin >> nwidth;


	int scale = 1; //SCALExSCALE pixel square // reduction size
	cout << "How many pixels in n*n area should be taken as 1 ascii character (resolution)\n";
	cin >> scale;

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

	int scaledRows = rows / scale;
	int scaledCols = cols / scale;

	//cout << scaledRows << " " << scaledCols << endl;
	

	// IMPORTANT
	cout << "ZOOM OUT TO SEE THE RESULT\n";
	Sleep(2000); // TIME TO RESCALE CONSOLE TO MAXIMUM // IF NOT DONE THE IMAGE WILL BE TEARED //
	// IMPORTANT

	cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
	
	if (MODE == 1) {
		printMatrix(convertToASCII(image, scaledRows, scaledCols, scale));
	}
	else {
		convertToASCII(image, scaledRows, scaledCols, scale, MODE);
	}

	cout << "\x1b[38;2;255;255;255m"; //reset colors
	return 0;
}