// BlindAssistantServices.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParseLetters.h"
#include "ImagePreprocessor.h"
#include "NNLetterRecognition.h"
#include <iostream>
#include "NNTrainer.h"

int main()
{
	const cv::String name_of_files = "testLine.jpg";

	const auto image = cv::imread(name_of_files, CV_LOAD_IMAGE_COLOR);
	assert(image.data);

	cv::Mat copyOfImage = image;

	ImagePreprocessor preprocessor(image);


	//auto corners = preprocessor.getRelativeCornersFromImage();
	//auto corneredImage = preprocessor.perspectiveCorrection(copyOfImage, corners);
	//
	auto preprocessedLines = preprocessor.getColorRefinedImageInLines(image);//corneredImage);
	//
	/*ParseLetters parser(image);
	int blackLimit = parser.getAverageBlackValue();
	auto lineLetters = parser.getVectorOfLettersFromLine(preprocessedLines,blackLimit);
*/
/*for(auto letter : preprocessedLines)
{
	cv::destroyWindow("BlackNiggaWindowForYouFella");
	cv::imshow("BlackNiggaWindowForYouFella", letter);
	cvWaitKey(0);
}*/

	std::cout << "--------------------------------------" << std::endl;

	/*try {
		NNLetterRecognition theDamnNN("trainingResults.txt");
		theDamnNN.generateFirstLayerOutpuToActivationVector(preprocessedLines[0]);
	}
	catch (char* error)
	{
		std::cout << error << std::endl;
	}*/


	// Train that buffalo

	NNTrainer trainer;

	trainer.initFilterTablesGenerateFile("filters.txt");															   //  delete to add another filter set
	trainer.startGeneratingFilters("letter.jpg");                                                                      // generate filters
	trainer.startGeneratingFilters("letterD.jpg");
	trainer.startGeneratingFilters("letterI.jpg");
		
	std::vector<std::string> letters = { "A","B","C" };

	std::string path = "./training/lower/";
	std::string extension = ".gif";
	
	for (auto letter : letters) {
		for (auto i = 0; i < 200; ++i) {
			system("cls");
			std::cout << letter << " => " << (100 * (i + 1)) / 200 << "%";
			trainer.startGeneratingFilters((path + letter + "/" + std::to_string(i) + extension).c_str());
		}
	}	

	NNLetterRecognition getAvtivationTablesFromGeneratedFilters("filters.txt", "activationTables.txt");


	for(auto i = 0; i < 200; ++i)
	{
		std::vector<std::vector<double>> bigIDK;
		bigIDK.push_back(getAvtivationTablesFromGeneratedFilters.generateFirstLayerOutpuToActivationVector(preprocessedLines[23]));

		trainer.initActivationTablesGenerateFile("activationTables.txt");                                                  //  delete to add another activity table
		auto cindarella = trainer.generatingFunctionActivationVector(bigIDK, 'a', "activationTables.txt");                 //  generate activation tables
		cv::imshow("current window", preprocessedLines[0]);
		cvWaitKey(0);


	}

	{


		std::vector<std::vector<double>> bigIDK;
		bigIDK.push_back(getAvtivationTablesFromGeneratedFilters.generateFirstLayerOutpuToActivationVector(preprocessedLines[26]));

		auto cindarella = trainer.generatingFunctionActivationVector(bigIDK, 'c', "activationTables.txt");                 //  generate activation tables
		cv::imshow("current window", preprocessedLines[0]);
		cvWaitKey(0);
	}

	std::cout << "-----------------------" << std::endl;



	NNLetterRecognition recognizerizator("filters.txt", "activationTables.txt");
	std::cout << recognizerizator.recognizeThisNigga(preprocessedLines[0]) << std::endl;
	std::cout << recognizerizator.recognizeThisNigga(preprocessedLines[26]) << std::endl;
	std::cout << recognizerizator.recognizeThisNigga(preprocessedLines[23]) << std::endl;

	return 0;
}



