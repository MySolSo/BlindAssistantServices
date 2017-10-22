#pragma once
#include <vector>
#include <opencv2/core/mat.hpp>

class NNLayer
{
	//const unsigned _filterX;
	//const unsigned _filterY;
public:
	//NNLayer();
	std::vector<double> startGeneratingFiltersFromLayerData(std::vector < std::vector<double> >layerDatas, const char* outputName, int length);
	std::vector<cv::Mat> convertLayerOutputToMat(std::vector<std::vector<double>> layerOutput, int imgX, int imgY);
};
