#pragma once

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <vector>
#include <string>

class imageDataAdapter
{

	int remapIntValueToUsedFormat(const double input, const int minInRange, const int maxInRange);
public:
	imageDataAdapter();
	~imageDataAdapter();

	std::vector<double> adaptToHeatMap(const cv::Mat& image);
	std::vector<double> adaptImageToDataset(/*const std::string& image*/const cv::Mat& image);
};

