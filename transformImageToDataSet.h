#pragma once
#include <vector>

namespace cv {
	class Mat;
}

class transformImageToDataSet
{
	int remapIntValueToUsedFormat(const double input, int minInRange, int maxInRange);
public:
	transformImageToDataSet();
	~transformImageToDataSet();
	std::vector<int> adaptingImageToDataset(const cv::Mat& inputImage);
};

