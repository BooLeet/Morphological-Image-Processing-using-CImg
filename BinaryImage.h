#pragma once
#include "pch.h"
#include <vector>
#include "CImg.h"

using namespace cimg_library;
using std::vector;

class BinaryImage
{
private:
	vector<vector<unsigned char>> image;
	int width;
	int height;

public:
	BinaryImage(const char* fileName,unsigned char whiteThreshold = 127)
	{
		CImg<unsigned char> sourceImage(fileName);
		width = sourceImage.width();
		height = sourceImage.height();

		for (int i = 0; i < width; ++i)
		{
			vector<unsigned char> column;
			for (int j = 0; j < height; ++j)
			{
				unsigned char red = sourceImage(i, j, 0, 0);
				unsigned char green = sourceImage(i, j, 0, 0);
				unsigned char blue = sourceImage(i, j, 0, 0);

				unsigned char greyScaleColor = red * 0.3 + green * 0.59 + blue * 0.11;

				greyScaleColor = greyScaleColor > whiteThreshold ? 1 : 0;

				column.push_back(greyScaleColor);
			}
			image.push_back(column);
		}
	}

	void Display()
	{
		CImg<unsigned char> imageForDisplay(width, height, 1, 3);

		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				imageForDisplay(i, j, 0, 0) = 255 * image[i][j];
				imageForDisplay(i, j, 0, 1) = 255 * image[i][j];
				imageForDisplay(i, j, 0, 2) = 255 * image[i][j];
			}
		}

		CImgDisplay mainDisplay(imageForDisplay, "Image");
		while (!mainDisplay.is_closed()) 
		{
			mainDisplay.wait();
		}
	}

	vector<vector<unsigned char>> GetErosion(int baseElementWidth)
	{
		vector<vector<unsigned char>> result = image;

		for (int i = baseElementWidth / 2; i < width - baseElementWidth / 2; ++i)
		{
			for (int j = baseElementWidth / 2; j < height - baseElementWidth / 2; ++j)
			{
				bool breakInnerLoops = false;
				for (int u = i - baseElementWidth / 2; u <= i + baseElementWidth / 2 && !breakInnerLoops; ++u)
				{
					for (int v = j - baseElementWidth / 2; v <= j + baseElementWidth / 2 && !breakInnerLoops; ++v)
					{
						if (image[u][v] == 0)
						{
							result[i][j] = 0;
							breakInnerLoops = true;
						}
					}
				}
			}
		}

		return result;
	}

	vector<vector<unsigned char>> GetDilatation(int baseElementWidth)
	{
		vector<vector<unsigned char>> result = image;

		for (int i = baseElementWidth / 2; i < width - baseElementWidth / 2; ++i)
		{
			for (int j = baseElementWidth / 2; j < height - baseElementWidth / 2; ++j)
			{
				bool breakInnerLoops = false;
				for (int u = i - baseElementWidth / 2; u <= i + baseElementWidth / 2 && !breakInnerLoops; ++u)
				{
					for (int v = j - baseElementWidth / 2; v <= j + baseElementWidth / 2 && !breakInnerLoops; ++v)
					{
						if (image[u][v] == 1)
						{
							result[i][j] = 1;
							breakInnerLoops = true;
						}
					}
				}
			}
		}

		return result;
	}

	void ErodeImage(int baseElementWidth = 3)
	{
		image = GetErosion(baseElementWidth);
	}

	void DilatateImage(int baseElementWidth = 3)
	{
		image = GetDilatation(baseElementWidth);
	}

	void OpenImage(int baseElementWidth = 3)
	{
		ErodeImage(baseElementWidth);
		DilatateImage(baseElementWidth);
	}

	void CloseImage(int baseElementWidth = 3)
	{
		DilatateImage(baseElementWidth);
		ErodeImage(baseElementWidth);
	}

	void HighlightBorders(int baseElementWidth = 3)
	{
		vector<vector<unsigned char>> result(width,vector<unsigned char>(height,0));
		vector<vector<unsigned char>> erosion = GetErosion(baseElementWidth);

		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				result[i][j] = (erosion[i][j] + image[i][j]) % 2;
			}
		}

		image.swap(result);
	}
};