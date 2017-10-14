#pragma once
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ParseLetters
{
private:
	cv::Mat _line;
public:
	ParseLetters(cv::Mat line): _line(line){};
	int getAverageBlackValue();
	std::vector<cv::Mat> getVectorOfLettersFromLine(std::vector<cv::Mat> lines, const int lowerLimit);
};
