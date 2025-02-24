#include "ImageToASCIIconvertor.h"

int main(void)
{
	 
	string path = "C:\\Users\\valer\\Desktop\\Files\\Pictures\\260px-OP_C016_rei.jpg"; // photo directory
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
	
	Sleep(2000); // TIME TO RESCALE CONSOLE TO MAXIMUM // IF NOT DONE THE IMAGE WILL BE TEARED
	cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
	printMatrix(convertToASCII(image, scaledRows, scaledCols, scale));
	
	return 0;
}