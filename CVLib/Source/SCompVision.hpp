#pragma once
#include "opencv2/opencv.hpp"

namespace CompVision
{
	// Task 1
	void EqualizeGrayscaleHistogram(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale);

	// Task5
	void EdgeDetectionSobel(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale);
	void EdgeDetectionScharr(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale);
	void EdgeDetectionLaplacian(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale);
	void EdgeDetectionCanny(const cv::Mat& grayscaleImage, cv::Mat& outEdges, double lowThreshold, double highThreshold);
}