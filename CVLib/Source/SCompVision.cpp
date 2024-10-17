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


	cv::Mat ApplyFilter(const cv::Mat& image, const std::vector<std::vector<int32_t>>& kernel)
	{
		int32_t rows = image.rows;
		int32_t cols = image.cols;
		int32_t kernel_size = 3;
		int32_t k = kernel_size / 2;

		cv::Mat result = cv::Mat::zeros(rows, cols, CV_64F);

		for (int32_t i = k; i < rows - k; i++)
		{
			for (int32_t j = k; j < cols - k; j++)
			{
				double sum = 0.0;
				for (int32_t u = -k; u <= k; u++)
				{
					for (int32_t v = -k; v <= k; v++)
					{
						sum += image.at<uchar>(i + u, j + v) * kernel[u + k][v + k];
					}
				}
				result.at<double>(i, j) = sum;
			}
		}

		return result;
	}


	void EdgeDetectionSobel(const cv::Mat& graysclaleImage, cv::Mat& outGrayscale)
	{
		std::vector<std::vector<int32_t>> sobelX = {
			{-1, 0, 1},
			{-2, 0, 2},
			{-1, 0, 1}
		};

		std::vector<std::vector<int32_t>> sobelY = {
			{-1, -2, -1},
			{ 0,  0,  0},
			{ 1,  2,  1}
		};

		cv::Mat gradX = ApplyFilter(graysclaleImage, sobelX);
		cv::Mat gradY = ApplyFilter(graysclaleImage, sobelY);

		cv::Mat sobelMagnitude = cv::Mat::zeros(graysclaleImage.size(), CV_64F);
		for (int32_t i = 0; i < gradX.rows; i++)
		{
			for (int32_t j = 0; j < gradX.cols; j++)
			{
				double gx = gradX.at<double>(i, j);
				double gy = gradY.at<double>(i, j);
				sobelMagnitude.at<double>(i, j) = std::sqrt(gx * gx + gy * gy);
			}
		}

		cv::normalize(sobelMagnitude, sobelMagnitude, 0, 255, cv::NORM_MINMAX);
		sobelMagnitude.convertTo(sobelMagnitude, CV_8U);
		outGrayscale = sobelMagnitude;
	}
}
