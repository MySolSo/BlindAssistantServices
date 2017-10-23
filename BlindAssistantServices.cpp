// BlindAssistantServices.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParseLetters.h"
#include "ImagePreprocessor.h"
#include "NNLetterRecognition.h"
#include <iostream>
#include "NNTrainer.h"
#include <opencv2/videostab/ring_buffer.hpp>
#include "NNLayer.h"
#include "transformImageToDataSet.h"

int main()
{
	const cv::String name_of_files = "test.png";

	const auto image = cv::imread(name_of_files, CV_LOAD_IMAGE_COLOR);
	assert(image.data);

	cv::Mat copyOfImage = image;

	ImagePreprocessor preprocessor(image);

	//auto corners = preprocessor.getRelativeCornersFromImage();
	//auto corneredImage = preprocessor.perspectiveCorrection(copyOfImage, corners);
	//
	//

	std::vector<cv::Mat> toProcess;

	toProcess.push_back(image);

	ParseLetters parser(image);
	int blackLimit = parser.getAverageBlackValue();
	auto lineLetters = parser.getVectorOfLettersFromLine(toProcess,100);

	/*for (auto letter : lineLetters)
	{
		cv::destroyWindow("BlackNiggaWindowForYouFella");
		cv::imshow("BlackNiggaWindowForYouFella", letter);
		cvWaitKey(0);
	}*/

     	auto preprocessedLines = preprocessor.getColorRefinedImageInLines(lineLetters[2]);//corneredImage);

for(auto letter : preprocessedLines)
{
	cv::destroyWindow("BlackNiggaWindowForYouFella");
	cv::imshow("BlackNiggaWindowForYouFella", letter);
	cvWaitKey(0); 
}


	transformImageToDataSet transformer;

	auto data = transformer.adaptingImageToDataset(preprocessedLines[1]);



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

	std::vector<std::string> letters = { //"d", "p", "b", "h" };
	"a","b","c","d","e","f","g","h"//,"i","j"
	,"k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" };

	std::string path = "C:/Users/gyula/source/repos/BlindAssistantServices/BlindAssistantServices/training/upper/";
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

	//trainer.initFilterTablesGenerateFile("filtersLayer1.txt", 1, 150);
	//trainer.initFilterTablesGenerateFile("filtersLayer2.txt", 3, 3);
	//trainer.initFilterTablesGenerateFile("filtersLayer3.txt", 1, 1);

	//trainer.initActivationTablesGenerateFile("activationTables.txt");

	//std::vector<std::vector<double>> bigIDK;
	//std::vector<std::vector<double>> biggestIDK;

	//for (auto letter : letters) {
	//	bigIDK.clear();
	//	for (auto i = 0; i < 10; ++i) {

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

	//	// one layer of data

		//NNLayer layer;

		//layer.startGeneratingFiltersFromLayerData(bigIDK,"filtersLayer1.txt", 150);
		////auto Layer2InputToNN = layer.convertLayerOutputToMat(bigIDK,50,  50);

		//NNLetterRecognition recognizeSecondLayer("filtersLayer1.txt", "activationTables.txt");

		//biggestIDK.clear();
		//for (auto i = 0; i < 10; ++i)
		//{
		//	biggestIDK.push_back(
		//	recognizeSecondLayer.generateFirstLayerOutpuToActivationVector(bigIDK[i])
		//	);
		//}
		//
	//	// replace to biggestIDK

	//	auto cindarella = trainer.generatingFunctionActivationVector(bigIDK, letter.c_str(), "activationTables.txt");                 //  generate activation tables
	//}


	/////////////////


	std::cout << "-----------------------" << std::endl;

	extension = ".gif";

	NNLetterRecognition recognizerizator("filters.txt", "activationTables.txt");
	//std::cout << recognizerizator.recognizeThisNigga(preprocessedLines[0]) << std::endl;

	int pro = 0,contra=0;

	for (auto letter : letters) {
		for (auto i = 0; i < 10; ++i) {
			cv::VideoCapture imagesForSablonTraining((path + letter + "/" + std::to_string(i) + extension).c_str());
			cv::Mat test21;
			imagesForSablonTraining.read(test21);

			cv::imshow("asd", test21);
			//cvWaitKey(1);

			char result = recognizerizator.recognizeThisNigga(test21);

			char* asd = new char[1];

			*asd = result;

			std::cout << letter.c_str() << "-";

			if(strcmp(letter.c_str(), asd) == 0)
			{
				pro++;
			}
			else
			{
				contra++;
			}
			delete asd;
			std::cout << result << std::endl;
		}
	}

	std::cout << "---   " << pro << " ---- " << contra << std::endl;

	return 0;
}



