#pragma once
#include <opencv2/core/mat.hpp>

namespace cv {
	class Mat;
}

class ImagePreprocessor
{
	cv::Mat _image;
public:
	ImagePreprocessor();
	ImagePreprocessor(cv::Mat image):_image(image){};
	~ImagePreprocessor();

	std::vector<cv::Point2f> getRelativeCornersFromImage();
	cv::Mat perspectiveCorrection(cv::Mat image,const std::vector<cv::Point2f>& cornerPoints);
	std::vector<cv::Mat> getColorRefinedImageInLines(cv::Mat image);
};

