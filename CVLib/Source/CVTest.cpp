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

		cv::imwrite(("equalized" + filePath).c_str(), equalizedImage);

		cv::waitKey(0);
	}

}

