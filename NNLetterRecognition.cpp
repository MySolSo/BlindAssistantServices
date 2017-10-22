#include "stdafx.h"
#include "NNLetterRecognition.h"
#include <fstream>
#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>
#include <opencv2/core/mat.hpp>
#include <opencv2/videostab/ring_buffer.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "NNLayer.h"

double NNLetterRecognition::getCurrentPixelRaport(double imagePixel, double filterPixel)
{
	double biggerOne;
	double smallerOne;

	if (imagePixel > filterPixel)
	{
		biggerOne = imagePixel;
		smallerOne = filterPixel;
	}
	else
	{
		biggerOne = filterPixel;
		smallerOne = imagePixel;
	}

	//              1- for you know it
	double difference = 1 - ((biggerOne - smallerOne));

	//if (imagePixel > filterPixel)
	//{
	//	double aux = imagePixel;
	//	imagePixel = filterPixel;
	//	filterPixel = aux;
	//}

	//if(imagePixel == 0)
	//{
	//	//imagePixel == 0.0001;
	//}

	//return imagePixel / filterPixel;

	return difference;
}
double NNLetterRecognition::rezultatSablon(std::vector<double> Litera, std::vector<int> Sablon)
{
	double Suma_Vector = 0;
	for (int i = 0; i < Litera.size(); i++)
	{
		if (Sablon[i] == 0)
		{
			Suma_Vector -= Litera[i];
		}
		else
		{
			Suma_Vector += Litera[i];
		}

	}
	return Suma_Vector;
}
double NNLetterRecognition::calculateFilterRatioAKANeuronActivity(std::vector<double> activities)
{
	//std::cout<<"length" << std::accumulate(activities.begin(), activities.end(), 0.0) << " --";
	return (std::accumulate(activities.begin(), activities.end(), 0.0) / activities.size());
}

int NNLetterRecognition::getIndexOfMaximum(std::vector<double> probabilityes)
{
	auto max = std::max_element(probabilityes.begin(), probabilityes.end());

	return std::distance(probabilityes.begin(), max);
}

NNLetterRecognition::NNLetterRecognition(const char* nameOfTraininedFilters, const char* nameOfTrainedAvtivationTables)
{

	std::ifstream trainingData(nameOfTraininedFilters);

	char bufferForIdentifier[20];

	trainingData >> bufferForIdentifier;

	if (std::strcmp(bufferForIdentifier, "<dimensionsFilter>") == 0)
	{
		std::cin.sync();
		trainingData >> _filterX >> _filterY;
	}
	else
	{
		std::cout << "Yo nigga training code haves identifier problems at" << bufferForIdentifier;
	}

	trainingData >> bufferForIdentifier;
	trainingData >> bufferForIdentifier;

	double** filter;

	while (std::strcmp(bufferForIdentifier, "<filter>") == 0) {

		filter = new double*[_filterX];
		for (auto i = 0; i < _filterX; ++i)
			filter[i] = new double[_filterY];

		for (auto i = 0; i < _filterX; ++i)
		{
			for (auto j = 0; j < _filterY; ++j)
			{
				trainingData >> filter[i][j];

			}
		}
		_filters.push_back(filter);

		trainingData >> bufferForIdentifier;
		trainingData >> bufferForIdentifier;
	}

	trainingData.close();
	trainingData.open(nameOfTrainedAvtivationTables);

	trainingData >> bufferForIdentifier;
	if (std::strcmp(bufferForIdentifier, "<dimensionsLetter>") == 0)
	{
		trainingData >> _letterX >> _letterY;
	}
	else
	{
		//throw bufferForIdentifier;
		std::cout << "Yo nigga training code haves identifier problems." << bufferForIdentifier;
	}

	trainingData >> bufferForIdentifier;
	trainingData >> bufferForIdentifier;

	if (std::strcmp(bufferForIdentifier, "<pixelStep>") == 0)
	{
		trainingData >> _stepSize;
	}
	else
	{
		//throw bufferForIdentifier;
		std::cout << "Yo nigga training code haves identifier problems." << bufferForIdentifier;
	}

	std::vector<int> sablon;
	auto sizeOfSablon = _filters.size() * (_letterX / _stepSize) * (_letterY / _stepSize);

	trainingData >> bufferForIdentifier;
	trainingData >> bufferForIdentifier;

	char letterIdentifyer;
	while (std::strcmp(bufferForIdentifier, "<sablon>") == 0) {
		trainingData >> letterIdentifyer;
		_sablonIdentifyer.push_back(letterIdentifyer);

		int bufferValue;
		for (auto i = 0; i < sizeOfSablon; ++i)
		{
			trainingData >> bufferValue;
			sablon.push_back(bufferValue);
		}
		_sablons.push_back(sablon);
		sablon.clear();

		trainingData >> bufferForIdentifier;
		if (!trainingData.eof())
			trainingData >> bufferForIdentifier;
		else
		{
			break;
		}
	}
	trainingData.close();
}

NNLetterRecognition::~NNLetterRecognition()
{
	for (auto filter : _filters)
	{
		for (auto i = 0; i < _filterX; i++)
			delete filter[i];
		delete filter;
	}
}

std::vector<std::vector<double>> NNLetterRecognition::convertMatToRatios(const cv::Mat letterImage)
{
	std::vector<std::vector<double>> convertedImage(letterImage.rows);

	for (int iteratorX = 0; iteratorX < letterImage.rows; ++iteratorX)
	{
		for (int iteratorY = 0; iteratorY < letterImage.cols; ++iteratorY)
		{
			auto valuesOfSelectedPosition = letterImage.at<cv::Vec3d>(cv::Point(iteratorX, iteratorY));
			double valueInGrey = (valuesOfSelectedPosition[0] + valuesOfSelectedPosition[1] + valuesOfSelectedPosition[2]) / 3;
			double valueInRatio = 1 - (valueInGrey / 255);

			convertedImage[iteratorX].push_back(valueInRatio);
		}
	}

	return convertedImage;
}
std::vector<std::vector<double>> NNLetterRecognition::generateRatioVectorFromMat(cv::Mat letterImage)
{
	cv::imshow("asd", letterImage);
	//cvWaitKey(0);

	std::vector<std::vector<double>> transformedImage(letterImage.rows);

	for (int iMat = 0; iMat < letterImage.rows; ++iMat)
	{
		for (int jMat = 0; jMat < letterImage.cols; ++jMat)
		{
			auto colors = letterImage.at<cv::Vec3b>(iMat, jMat);
			//                               add 1 -
			transformedImage[iMat].push_back(((colors[0] + colors[1] + colors[2]) / 3) / 255.0);
		}
	}
	return transformedImage;
}


std::vector<double> NNLetterRecognition::generateFirstLayerOutpuToActivationVector(cv::Mat letter)
{
	cv::resize(letter, letter, cv::Size(_letterX, _letterY));

	std::vector<double> pixelRatios;
	std::vector<double> NNactivities;
	std::vector<std::vector<double>> letterInRatios = generateRatioVectorFromMat(letter);

	for (auto filter : _filters) {
		for (auto rowIterator = 0; rowIterator < _letterX - _filterX + 1; rowIterator += _stepSize)
		{
			for (auto collumnIterator = 0; collumnIterator < _letterY - _filterY + 1; collumnIterator += _stepSize)
			{
				pixelRatios.clear();
				for (auto filterIteratorX = rowIterator; filterIteratorX < rowIterator + _filterX; ++filterIteratorX)
				{
					for (auto filterIteratorY = collumnIterator; filterIteratorY < collumnIterator + _filterY; ++filterIteratorY)
					{
						//std::cout << "x-" << filterIteratorX << " y-" << filterIteratorY << " | ";
						pixelRatios.push_back(getCurrentPixelRaport(letterInRatios[filterIteratorX][filterIteratorY], filter[filterIteratorX - rowIterator][filterIteratorY - collumnIterator]));
						//std::cout << pixelRatios[pixelRatios.size() - 1] << "   " ;
					}
				}
				NNactivities.push_back(calculateFilterRatioAKANeuronActivity(pixelRatios));
				// cout 
					//std::cout << "|" <<  calculateFilterRatioAKANeuronActivity(pixelRatios) << std::endl;
			}
		}
		// cout
		//std::cout << std::endl;
	}
	return NNactivities;
}

std::vector<double> NNLetterRecognition::generateFirstLayerOutpuToActivationVector(std::vector<double> letter)
{
	std::vector<double> NNactivities;
	std::vector<double> pixelRatios;

	for (auto filter : _filters) {
		pixelRatios.clear();
		for (int letterIterator = 0; letterIterator < letter.size() - _filterY - _stepSize + 1; letterIterator += _stepSize)
		{
			for (int filterIterator = 0; filterIterator < _filterY; filterIterator++)
				pixelRatios.push_back(getCurrentPixelRaport(letter[letterIterator + filterIterator], filter[0][filterIterator]));
		}
		NNactivities.push_back(calculateFilterRatioAKANeuronActivity(pixelRatios));
	}

	return NNactivities;
}

char NNLetterRecognition::recognizeThisNigga(cv::Mat letter)
{
	auto activationTable = generateFirstLayerOutpuToActivationVector(letter);

	std::vector<double> probabilityes;
	for (auto sablon : _sablons)
	{
		probabilityes.push_back(rezultatSablon(activationTable, sablon));
	}

	for (auto prob : probabilityes)
	{
		std::cout << prob << " ";
	}
	std::cout << "----" << probabilityes[getIndexOfMaximum(probabilityes)] << std::endl;

	return _sablonIdentifyer[getIndexOfMaximum(probabilityes)];
}
