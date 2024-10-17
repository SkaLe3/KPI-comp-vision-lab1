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
		cv::Sobel(image, scharrX, CV_64F, 1, 0);
		cv::Sobel(image, scharrY, CV_64F, 0, 1);
		cv::magnitude(scharrX, scharrY, scharr);
		cv::imshow("Scharr Built In", scharr);

		cv::Mat laplace;
		cv::Laplacian(image, laplace, CV_64F);
		cv::imshow("Laplacian Built In", laplace);

		cv::Mat canny;
		double lower_threshold = 50;
		double uppper_threshold = 150;
		cv::Canny(image, canny, lower_threshold, uppper_threshold);
		cv::imshow("Canny Built In", canny);

		cv::waitKey(0);
	}

}

