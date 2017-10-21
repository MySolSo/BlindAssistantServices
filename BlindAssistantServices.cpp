// BlindAssistantServices.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParseLetters.h"
#include "ImagePreprocessor.h"
#include "NNLetterRecognition.h"
#include <iostream>
#include "NNTrainer.h"
#include <opencv2/videostab/ring_buffer.hpp>

int main()
{
	const cv::String name_of_files = "test.jpg";

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
	////////////////

	std::vector<std::string> letters = { "d", "p", "b", "h" };
		//"a","b","c","d","e","f","g","h"//,"i","j"
		//,"k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" };

	std::string path = "C:/Users/gyula/source/repos/BlindAssistantServices/BlindAssistantServices/training/upper/" ;
	std::string extension = ".jpg";
	
	//trainer.initFilterTablesGenerateFile("filters.txt");															   //  delete to add another filter set
	//for (auto letter : letters) {
	//		trainer.startGeneratingFilters((path + letter + "/" + 
	//			//std::to_string(i) 
	//			"%1d"
	//			+ extension
	//			).c_str());
	//}

	NNLetterRecognition getAvtivationTablesFromGeneratedFilters("filters.txt", "activationTables.txt");


	/////////////////
	//
	extension = ".gif";
	
	//trainer.initActivationTablesGenerateFile("activationTables.txt");
	//std::vector<std::vector<double>> bigIDK;

	//for (auto letter : letters) {
	//	bigIDK.clear();
	//	for (auto i = 0; i < 200; ++i) {

	//		system("cls");
	//		std::cout << letter << " activation table" << " => " << (100 * (i + 1)) / 200 << "%";

	//		cv::VideoCapture imagesForSablonTraining((path + letter + "/" + std::to_string(i) + extension).c_str());
	//		cv::Mat test2;
	//		imagesForSablonTraining.read(test2);
	//		assert(test2.data);

	//		bigIDK.push_back(getAvtivationTablesFromGeneratedFilters.generateFirstLayerOutpuToActivationVector(test2));

	//		//cv::imshow("asdasd", test2);
	//		//cvWaitKey(0);
	//	}
	//	auto cindarella = trainer.generatingFunctionActivationVector(bigIDK, letter.c_str(), "activationTables.txt");                 //  generate activation tables
	//}
	

	/////////////////


	std::cout << "-----------------------" << std::endl;

	extension = ".gif";

	NNLetterRecognition recognizerizator("filters.txt", "activationTables.txt");
	//std::cout << recognizerizator.recognizeThisNigga(preprocessedLines[0]) << std::endl;

	for (auto letter : letters) {
		for (auto i = 0; i < 10; ++i) {
			cv::VideoCapture imagesForSablonTraining((path + letter + "/" + std::to_string(i) + extension).c_str());
			cv::Mat test21;
			imagesForSablonTraining.read(test21);

			cv::imshow("asd", test21);
			cvWaitKey(0);


			std::cout << recognizerizator.recognizeThisNigga(test21) << std::endl;
		}
	}

	return 0;
}



