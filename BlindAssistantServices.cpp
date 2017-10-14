// BlindAssistantServices.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParseLetters.h"
#include "ImagePreprocessor.h"

int main()
{
	const cv::String name_of_files = "test.png";

	const auto image = cv::imread(name_of_files, CV_LOAD_IMAGE_COLOR);
	assert(image.data);

	ImagePreprocessor preprocessor(image);

	auto corners = preprocessor.getRelativeCornersFromImage();
	auto corneredImage = preprocessor.perspectiveCorrection(corners);
	auto preprocessedLines = preprocessor.getColorRefinedImageInLines(corneredImage);

	ParseLetters parser(image);
	int blackLimit = parser.getAverageBlackValue();
	auto lineLetters = parser.getVectorOfLettersFromLine(preprocessedLines,blackLimit);

	for(auto letter : lineLetters)
	{
		cv::destroyWindow("BlackNiggaWindowForYouFella");
		cv::imshow("BlackNiggaWindowForYouFella", letter);
		cvWaitKey(0);
	}

    return 0;
}

