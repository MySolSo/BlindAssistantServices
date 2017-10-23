#include "stdafx.h"
#include "NNLayer.h"
#include <ostream>
#include <vector>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>


std::vector<double> NNLayer::startGeneratingFiltersFromLayerData(std::vector<std::vector<double> > layerDatas, const char* outputName, int length)
{
	std::vector<double> summOfAll(layerDatas[0].size());
	for (auto asd : summOfAll)
	{
		asd = 0;
	}

	auto nrOfSets = 0;
	for (auto layer : layerDatas)
	{
		for (auto i = 0; i < layer.size(); ++i)
			summOfAll[i] += layer[i];

		++nrOfSets;
	}

	for (int i = 0; i<summOfAll.size();++i)
	{
		summOfAll[i] = summOfAll [i]/ nrOfSets;
	}

	std::ofstream fout(outputName, std::ios::app);

	for (auto i = 0; i < summOfAll.size(); ++i) {
		if (i%length == 0)
			fout << "<filter>" << std::endl;

		fout << summOfAll[i] << " ";

		if (i%length == length - 1)
			fout << std::endl << "</filter>" << std::endl;
	}
	return summOfAll;
}

std::vector<cv::Mat> NNLayer::convertLayerOutputToMat(std::vector<std::vector<double>> layerOutput, int imgX, int imgY)
{
	std::vector<cv::Mat> returnData;

	for (auto layer : layerOutput)
	{
		cv::Mat image(cv::Size(imgX, imgY), CV_64FC3);
		for (auto x = 0; x < imgX; ++x)
		{
			for (auto y = 0; y < imgY; ++y)
			{
				double valuer = layer.front();
				if (valuer < 0)
					valuer = valuer * (-1);
				cv::Vec3d color;
				color[0] = valuer;
				color[1] = valuer;
				color[2] = valuer;

				image.at<cv::Vec3d>(cv::Point(x, y)) = color;
				layer.erase(layer.begin());
			}
		}
		cv::imshow("rezultate", image);
		cvWaitKey(0);
		returnData.push_back(image.clone());
	}

	return returnData;
}

