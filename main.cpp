#include "pch.h"
#include <iostream>
#include <string>
#include "CImg.h"

#include "BinaryImage.h"

using namespace cimg_library;
using std::string;

int main() {
	std::cout << "Enter the file name: ";
	string fileName;
	getline(std::cin, fileName);
	BinaryImage image(fileName.data());


	//image.ErodeImage();
	//image.DilatateImage();
	//image.OpenImage();
	//image.CloseImage();
	image.HighlightBorders();


	image.Display();

	return 0;
}
