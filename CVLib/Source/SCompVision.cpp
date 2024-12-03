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
		int32_t kernelSize = static_cast<int32_t>(kernel.size());
		int32_t pad = kernelSize / 2;

		cv::Mat result = cv::Mat::zeros(image.size(), CV_64F);

		for (int32_t i = pad; i < image.rows - pad; i++)
		{
			for (int32_t j = pad; j < image.cols - pad; j++)
			{
				double sum = 0.0;
				for (int32_t ki = -pad; ki <= pad; ki++)
				{
					for (int32_t kj = -pad; kj <= pad; kj++)
					{
						int32_t pixelVal = image.at<uchar>(i + ki, j + kj);
						sum += pixelVal * kernel[ki + pad][kj + pad];
					}
				}
				result.at<double>(i, j) = sum;
			}
		}

		return result;
	}

	cv::Mat GradientMagnitude(const cv::Mat& gradX, const cv::Mat& gradY)
	{
		cv::Mat magnitude = cv::Mat::zeros(gradX.size(), CV_64F);
		for (int32_t i = 0; i < gradX.rows; i++)
		{
			for (int32_t j = 0; j < gradX.cols; j++)
			{
				double gx = gradX.at<double>(i, j);
				double gy = gradY.at<double>(i, j);
				magnitude.at<double>(i, j) = std::sqrt(gx * gx + gy * gy);
			}
		}

		//cv::normalize(magnitude, magnitude, 0, 255, cv::NORM_MINMAX);
		magnitude.convertTo(magnitude, CV_8U);
		return magnitude;
	}


	void EdgeDetectionSobel(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale)
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

		cv::Mat gradX = ApplyFilter(grayscaleImage, sobelX);
		cv::Mat gradY = ApplyFilter(grayscaleImage, sobelY);


		outGrayscale = GradientMagnitude(gradX, gradY);
	}

	void EdgeDetectionScharr(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale)
	{
		std::vector<std::vector<int32_t>> scharrX = {
			{-3, 0, 3},
			{-10, 0, 10},
			{-3, 0, 3}
		};

		std::vector<std::vector<int32_t>> scharrY = {
			{-3, -10, -3},
			{ 0,  0,  0},
			{ 3,  10,  3}
		};

		cv::Mat gradX = ApplyFilter(grayscaleImage, scharrX);
		cv::Mat gradY = ApplyFilter(grayscaleImage, scharrY);


		outGrayscale = GradientMagnitude(gradX, gradY);
	}

	void EdgeDetectionLaplacian(const cv::Mat& grayscaleImage, cv::Mat& outGrayscale)
	{
		std::vector<std::vector<int32_t>> laplacianKernel = {
			{ 0,  1,  0},
			{ 1, -4,  1},
			{ 0,  1,  0}
		};

		outGrayscale = ApplyFilter(grayscaleImage, laplacianKernel);

		cv::normalize(outGrayscale, outGrayscale, 0, 255, cv::NORM_MINMAX);
		outGrayscale.convertTo(outGrayscale, CV_8U);
	}

	void EdgeDetectionCanny(const cv::Mat& grayscaleImage, cv::Mat& outEdges, double lowThreshold, double highThreshold)
	{

		// Step 2: Reduce noise with a Gaussian filter
		cv::Mat blurredImage;
		cv::GaussianBlur(grayscaleImage, blurredImage, cv::Size(5, 5), 1.4);

		// Step 3: Compute gradients using Sobel operator
		cv::Mat gradX, gradY;
		cv::Sobel(blurredImage, gradX, CV_32F, 1, 0, 3);
		cv::Sobel(blurredImage, gradY, CV_32F, 0, 1, 3);

		// Step 4: Calculate gradient magnitude and direction
		cv::Mat magnitude, direction;
		cv::cartToPolar(gradX, gradY, magnitude, direction, true); // Use true for degrees

		// Step 5: Non-maximum suppression
		cv::Mat nonMaxSuppressed = cv::Mat::zeros(magnitude.size(), CV_32F);
		for (int y = 1; y < magnitude.rows - 1; ++y)
		{
			for (int x = 1; x < magnitude.cols - 1; ++x)
			{
				float angle = direction.at<float>(y, x);
				float q = 255; // Quantized gradient magnitude
				float r = 255; // Right neighbor

				// Determine neighbors based on the angle
				if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <= 180))
				{
					q = magnitude.at<float>(y, x + 1);
					r = magnitude.at<float>(y, x - 1);
				}
				else if (angle >= 22.5 && angle < 67.5)
				{
					q = magnitude.at<float>(y - 1, x + 1);
					r = magnitude.at<float>(y + 1, x - 1);
				}
				else if (angle >= 67.5 && angle < 112.5)
				{
					q = magnitude.at<float>(y - 1, x);
					r = magnitude.at<float>(y + 1, x);
				}
				else if (angle >= 112.5 && angle < 157.5)
				{
					q = magnitude.at<float>(y - 1, x - 1);
					r = magnitude.at<float>(y + 1, x + 1);
				}

				// Suppress non-maxima
				if (magnitude.at<float>(y, x) >= q && magnitude.at<float>(y, x) >= r)
				{
					nonMaxSuppressed.at<float>(y, x) = magnitude.at<float>(y, x);
				}
				else
				{
					nonMaxSuppressed.at<float>(y, x) = 0;
				}
			}
		}

		// Step 6: Double thresholding
		cv::Mat thresholded;
		cv::threshold(nonMaxSuppressed, thresholded, lowThreshold, 255, cv::THRESH_BINARY);

		// Step 7: Edge tracking by hysteresis
		outEdges = cv::Mat::zeros(thresholded.size(), CV_8U);
		for (int y = 1; y < thresholded.rows - 1; ++y)
		{
			for (int x = 1; x < thresholded.cols - 1; ++x)
			{
				if (thresholded.at<float>(y, x) == 255)
				{
					outEdges.at<uchar>(y, x) = 255; // Strong edge
				}
				else if (thresholded.at<float>(y, x) > lowThreshold)
				{
					// Check surrounding pixels for strong edges
					bool hasStrongEdge = false;
					for (int dy = -1; dy <= 1; ++dy)
					{
						for (int dx = -1; dx <= 1; ++dx)
						{
							if (outEdges.at<uchar>(y + dy, x + dx) == 255)
							{
								hasStrongEdge = true;
								break;
							}
						}
						if (hasStrongEdge) break;
					}
					outEdges.at<uchar>(y, x) = hasStrongEdge ? 255 : 0; // Connect to strong edge or discard
				}
			}
		}

	}

}
