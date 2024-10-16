#pragma once
#include <opencv2/opencv.hpp>

namespace CompVision
{
	void EqualizeGrayscaleHistogram(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale);
}