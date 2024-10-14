#pragma once
#include <opencv2/opencv.hpp>

namespace CompVision
{
	cv::Mat EqualizeGrayscaleHistogram(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale);
}