#include "SCompVision.hpp"
#include <iostream>


namespace CompVision
{
	cv::Mat EqualizeGrayscaleHistogram(const cv::Mat& inputImage, cv::Mat& outGrayscale)
	{
		if (inputImage.empty())
		{
			std::cout << "Input image is empty.\n";
		}
		cv::Mat grayImage;
		if (inputImage.channels() == 3)
		{
			cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
		}
		else if (inputImage.channels() == 4)
		{
			cv::cvtColor(inputImage, grayImage, cv::COLOR_BGRA2GRAY);
		}
		else
		{
			grayImage = inputImage;
		}
		outGrayscale = grayImage;
		cv::Mat equalizedImage;
		cv::equalizeHist(grayImage, equalizedImage);

		return equalizedImage;
	}

}
