#include "CVTest.hpp"
#include "SCompVision.hpp"
#include <iostream>


namespace CVTest
{

	void RunEqualizeGrayscaleHistogram(const std::string& filePath)
	{
		cv::Mat image = cv::imread(filePath.c_str());
		cv::Mat grayscaleImage;

		if (image.empty())
		{
			std::cout << "unable to open image\n";
			return;
		}

		cv::Mat equalizedImage = CompVision::EqualizeGrayscaleHistogram(image, grayscaleImage);

		cv::imshow("Original Image", image);
		cv::imshow("Grayscale image", grayscaleImage);
		cv::imshow("Equalized Image", equalizedImage);

		cv::imwrite("equalized_image.jpg", equalizedImage);

		cv::waitKey(0);
	}

}

