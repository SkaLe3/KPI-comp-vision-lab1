#include "CVTest.hpp"
#include "SCompVision.hpp"
#include <iostream>


namespace CVTest
{

	void RunEqualizeGrayscaleHistogram(const std::string& filePath)
	{
		cv::Mat image = cv::imread(filePath.c_str(), cv::IMREAD_GRAYSCALE);

		if (image.empty())
		{
			std::cout << "Unable to open image\n";
			return;
		}

		cv::Mat equalizedImage;
		cv::Mat equalizedImageBuiltIn;
		CompVision::EqualizeGrayscaleHistogram(image, equalizedImage);
		cv:equalizeHist(image, equalizedImageBuiltIn);


		cv::imshow("Original Image", image);
		cv::imshow("Equalized Image", equalizedImage);
		cv::imshow("Equalized Image Built In", equalizedImageBuiltIn);

		cv::imwrite(("equalized_" + filePath).c_str(), equalizedImage);

		cv::waitKey(0);
	}

	void RunEdgeDetector(const std::string& filePath)
	{
		cv::Mat image = cv::imread(filePath.c_str(), cv::IMREAD_GRAYSCALE);
		if (image.empty())
		{
			std::cout << "Unable to open image\n";
			return;
		}
		cv::imshow("Original Image", image);

		cv::Mat sobelImage;
		CompVision::EdgeDetectionSobel(image, sobelImage);
		cv::imshow("Sobel Image", sobelImage);
		cv::imwrite(("sobel_" + filePath).c_str(), sobelImage);

		cv::Mat scharrImage;
		CompVision::EdgeDetectionScharr(image, scharrImage);
		cv::imshow("Scharr Image", scharrImage);
		cv::imwrite(("scharr_" + filePath).c_str(), scharrImage);

		cv::Mat laplacianImage;
		CompVision::EdgeDetectionLaplacian(image, laplacianImage);
		cv::imshow("Laplacian Image", laplacianImage);
		cv::imwrite(("laplacian_" + filePath).c_str(), laplacianImage);

		cv::Mat cannyImage;
		CompVision::EdgeDetectionCanny(image, cannyImage, 50, 150);
		cv::imshow("Canny Image", cannyImage);
		cv::imwrite(("canny_" + filePath).c_str(), cannyImage);
		

		cv::Mat imageB;
		GaussianBlur(image, imageB, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
		cv::Mat sobelX, sobelY, sobel;
		cv::Mat absSobelX, absSobelY;	
		cv::Sobel(imageB, sobelX, CV_64F, 1, 0, 3);
		cv::Sobel(imageB, sobelY, CV_64F, 0, 1, 3);
		cv::convertScaleAbs(sobelX, absSobelX);
		cv::convertScaleAbs(sobelY, absSobelY);
		cv::addWeighted(absSobelX, 0.5, absSobelY, 0.5, 0, sobel);
		cv::imshow("Sobel Built In", sobel);

		cv::Mat scharrX, scharrY, scharr;
		cv::Mat absScharrX, absScharrY;
		cv::Scharr(image, scharrX, CV_64F, 1, 0);
		cv::Scharr(image, scharrY, CV_64F, 0, 1);
		cv::convertScaleAbs(scharrX, absScharrX);
		cv::convertScaleAbs(scharrY, absScharrY);
		cv::addWeighted(absScharrX, 0.5, absScharrY, 0.5, 0, scharr);
		cv::imshow("Scharr Built In", scharr);

		cv::Mat laplace;
		cv::Laplacian(image, laplace, CV_64F);
		cv::normalize(laplace, laplace, 0, 255, cv::NORM_MINMAX);
		laplace.convertTo(laplace, CV_8U);
		cv::imshow("Laplacian Built In", laplace);

		cv::Mat canny;
		double lower_threshold = 50;
		double uppper_threshold = 150;
		cv::Canny(image, canny, lower_threshold, uppper_threshold);
		cv::imshow("Canny Built In", canny);

		cv::waitKey(0);
	}

}

