// BlindAssistantServices.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParseLetters.h"
#include "ImagePreprocessor.h"
#include "NNLetterRecognition.h"
#include <iostream>

int main()
{
	const cv::String name_of_files = "test.jpg";

	const auto image = cv::imread(name_of_files, CV_LOAD_IMAGE_COLOR);
	assert(image.data);

	cv::Mat copyOfImage = image;

	ImagePreprocessor preprocessor(image);


	auto corners = preprocessor.getRelativeCornersFromImage();
	auto corneredImage = preprocessor.perspectiveCorrection(copyOfImage, corners);
	//
	auto preprocessedLines = preprocessor.getColorRefinedImageInLines(corneredImage);//corneredImage);
	//
	/*ParseLetters parser(image);
	int blackLimit = parser.getAverageBlackValue();
	auto lineLetters = parser.getVectorOfLettersFromLine(preprocessedLines,blackLimit);

	for(auto letter : lineLetters)
	{
		cv::destroyWindow("BlackNiggaWindowForYouFella");
		cv::imshow("BlackNiggaWindowForYouFella", letter);
		cvWaitKey(0);
	}
*/
	std::cout << "--------------------------------------" << std::endl;

	/*try {
		NNLetterRecognition theDamnNN("trainingResults.txt");
		theDamnNN.generateFirstLayerOutpuToActivationVector(preprocessedLines[0]);
	}
	catch (char* error)
	{
		std::cout << error << std::endl;
	}*/

	NNLetterRecognition coolStuff("trainingResults.txt");

	coolStuff.startGeneratingFilters();
	std::vector<std::vector<double>> bigIDK;
	auto idk = coolStuff.generateFirstLayerOutpuToActivationVector(preprocessedLines[0]);
	bigIDK.push_back(idk);
	auto cindarella = coolStuff.generatingFunctionActivationVector(bigIDK);

	for(auto cindy : cindarella)
	{
		std::cout << cindy;
	}

    return 0;
}



