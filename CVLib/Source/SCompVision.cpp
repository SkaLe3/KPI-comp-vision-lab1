#include "SCompVision.hpp"
#include <iostream>
#include <vector>

namespace CompVision
{
	void EqualizeGrayscaleHistogram(const cv::Mat& inImage, cv::Mat& outEqualized)
	{
		if (inImage.empty())
		{
			std::cout << "Input image is empty.\n";
		}
		
		// Count histogram
		std::vector<int32_t> histogram(256, 0);
		for (int32_t y = 0; y < inImage.rows; y++)
		{
			for (int32_t x = 0; x < inImage.cols; x++)
			{
				histogram[inImage.at<uchar>(y, x)]++;
			}
		}

		// Cumulative distribution function
		std::vector<int32_t> cdf(256, 0);
		cdf[0] = histogram[0];
		for (int32_t i = 1; i < 256; i++)
		{
			cdf[i] = cdf[i-1] + histogram[i];
		}

		// Normalize
		int32_t cdfMin = 0;
		while (cdfMin == 0)
		{
			cdfMin = *std::find_if(cdf.begin(), cdf.end(), [](int32_t x) { return x != 0; });
		}
		int32_t totalPixels = inImage.rows * inImage.cols;
		std::vector<uchar> equalizedLut(256, 0);
		for (int32_t i = 0; i < 256; i++)
		{
			equalizedLut[i] = cv::saturate_cast<uchar>(255.0 * (cdf[i] - cdfMin) / (totalPixels - cdfMin));
		}

		outEqualized = inImage.clone();
		for (int32_t y = 0; y < inImage.rows; y++)
		{
			for (int32_t x = 0; x < inImage.cols; x++)
			{
				outEqualized.at<uchar>(y, x) = equalizedLut[inImage.at<uchar>(y, x)];
			}
		}
	}
}
