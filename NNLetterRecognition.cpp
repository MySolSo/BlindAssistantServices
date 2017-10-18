#include "stdafx.h"
#include "NNLetterRecognition.h"
#include <fstream>
#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>
#include <opencv2/core/mat.hpp>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unordered_map>

const int step = 5;
const int filterRows = 10;
const int filterCols = 10;

double numberOfImages = 0;
std::ofstream fout("text.out");

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

	double difference = 1 - (biggerOne - smallerOne);

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

double NNLetterRecognition::calculateFilterRatioAKANeuronActivity(std::vector<double> activities)
{
	//std::cout<<"length" << std::accumulate(activities.begin(), activities.end(), 0.0) << " --";
	return (std::accumulate(activities.begin(), activities.end(), 0.0) / activities.size());
}

double NNLetterRecognition::rezultatSablon(std::vector<double> Litera, std::vector<int> Sablon)
{
	double Suma_Vector = 0;
	for (int i = 0; i < Litera.size(); i++)
	{
		if (Sablon[i] == 0)
		{
			Litera[i] = 0.0;
		}
		else
		{
			Litera[i] = 1.0;
			Suma_Vector++;
		}

	}
	return Suma_Vector;
}


void NNLetterRecognition::writeFilter(cv::Mat filter)
{
	for (int iFilter = 0; iFilter < filter.rows; iFilter++)
	{
		for (int jFilter = 0; jFilter < filter.cols; jFilter++)
		{
			fout << filter.at<double>(iFilter, jFilter) << ' ';
		}
		fout << std::endl;
	}
	fout << std::endl;
	fout << std::endl;
}
std::vector<cv::Mat> NNLetterRecognition::createFilters(cv::Mat image)
{
	cvtColor(image, image, cv::COLOR_BGR2GRAY);
	image.convertTo(image, CV_64FC1);

	cv::Mat filter;
	filter = cv::Mat::zeros(filterRows, filterCols, CV_64FC1);

	std::vector<cv::Mat>filters;
	for (int iMat = 0; iMat < image.rows - filterRows; iMat += step)
	{
		for (int jMat = 0; jMat < image.cols - filterCols; jMat += step)
		{

			for (int iFilter = 0; iFilter < filter.rows; iFilter++)
			{
				for (int jFilter = 0; jFilter < filter.cols; jFilter++)
				{
					filter.at<double>(iFilter, jFilter) = image.at<double>(iFilter + iMat, jFilter + jMat) / 255.0;
				}
			}
			filters.push_back(filter.clone());
			filter = cv::Mat::zeros(filterRows, filterCols, CV_64FC1);
		}
	}
	return filters;
}
void NNLetterRecognition::modifyFilters(cv::Mat image, std::vector<cv::Mat> &filters)
{
	cvtColor(image, image, cv::COLOR_BGR2GRAY);
	image.convertTo(image, CV_64FC1);

	cv::Mat filter;
	filter = cv::Mat::zeros(filterRows, filterCols, CV_64FC1);
	int currentFilter = 0;
	for (int iMat = 0; iMat < image.rows - filterRows; iMat += step)
	{
		for (int jMat = 0; jMat < image.cols - filterCols; jMat += step, currentFilter++)
		{
			for (int iFilter = 0; iFilter < filterRows; iFilter++)
			{
				for (int jFilter = 0; jFilter < filterCols; jFilter++)
				{
					filters[currentFilter].at<double>(iFilter, jFilter) += image.at<double>(iFilter + iMat, jFilter + jMat) / 255.0;
				}

			}
		}
	}
}
std::vector<cv::Mat> NNLetterRecognition::generateFilters()
{
	int norm_sizeX = 30;
	int norm_sizeY = 30;

	cv::VideoCapture images("./letter.jpg");
	std::vector<cv::Mat> filters;
	cv::Mat image, norm_image(norm_sizeX, norm_sizeY, CV_64F);
	cv::imshow("asd", images.get(0));
	cvWaitKey(0);
	if (images.isOpened())
	{
		if (images.read(image))
		{
			numberOfImages++;
			resize(image, norm_image, cv::Size(norm_sizeX, norm_sizeY));
			filters = createFilters(norm_image);
		}
	}
	while (images.isOpened())
	{
		if (images.read(image))
		{
			numberOfImages++;
			resize(image, norm_image, cv::Size(norm_sizeX, norm_sizeY));
			modifyFilters(norm_image, filters);
		}
		else
			images.release();
	}

	return filters;
}


NNLetterRecognition::NNLetterRecognition(const char* nameOfTrainingResultFile)
{
	std::ifstream trainingData(nameOfTrainingResultFile);

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

	while (std::strcmp(bufferForIdentifier, "<sablon>") == 0) {

		for (auto i = 0; i < sizeOfSablon; ++i)
		{
			trainingData >> sablon[i];
		}
		_sablons.push_back(sablon);

		trainingData >> bufferForIdentifier;
		if (!trainingData.eof())
			trainingData >> bufferForIdentifier;
		else
		{
			break;
		}
	}

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

	for(int iteratorX = 0; iteratorX < letterImage.rows; ++iteratorX)
	{
		for (int iteratorY = 0; iteratorY < letterImage.cols; ++iteratorY)
		{
			auto valuesOfSelectedPosition = letterImage.at<cv::Vec3d>(cv::Point(iteratorX, iteratorY));
			double valueInGrey = (valuesOfSelectedPosition[0] + valuesOfSelectedPosition[1] + valuesOfSelectedPosition[2]) / 3;
			double valueInRatio = 1 - (valueInGrey/255);

			convertedImage[iteratorX].push_back(valueInRatio);
		}
	}

	return convertedImage;
}

std::vector<double> NNLetterRecognition::generateFirstLayerOutpuToActivationVector(const cv::Mat letter)
{
	std::vector<double> pixelRatios;
	std::vector<double> NNactivities;
	std::vector<std::vector<double>> letterInRatios = { {0.1, 0.1, 0.5 , 0.5 }, {0.1, 0.1, 0.5, 0.5},{ 0.1, 0.1, 0.5 , 0.5 },{ 0.1, 0.1, 0.5, 0.5 } };//= convertMatToRatios(letter);

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
					std::cout << "|" <<  calculateFilterRatioAKANeuronActivity(pixelRatios) << std::endl;
			}
		}
		// cout
		std::cout << std::endl;
	}
	return NNactivities;
}

void NNLetterRecognition::startGeneratingFilters()
{

	std::vector<cv::Mat> filters = generateFilters();
	std::cout << filters.size();
	cv::Mat img(filterRows, filterCols, CV_64F);
	//namedWindow("brack", WINDOW_AUTOSIZE);
	for (int i = 0; i<filters.size(); i += 1)
	{
		resize(filters[i], img, img.size(), 0, 0, cv::INTER_NEAREST);
		for (int k = 0; k < img.rows; k++)
			for (int j = 0; j < img.cols; j++)
				img.at<double>(k, j) /= numberOfImages;
		imshow("brack", img);
		writeFilter(img);
		cv::waitKey(0);
	}
}

std::vector<int> NNLetterRecognition::generatingFunctionActivationVector(const std::vector<std::vector<double>>& activationOfFilters)
{
	const unsigned numberOfFilters = activationOfFilters[0].size();
	const unsigned numberOfActivationVectors = activationOfFilters.size();
	const double threshold = 85; //selected randomly; 


	std::vector<double> result(numberOfFilters, 0.0);
	for (auto &&vector : activationOfFilters)
	{
		std::transform(result.begin(), result.end(), vector.begin(), result.begin(), std::plus<double>());
	}
	std::vector<int> generatedTemplate(numberOfFilters, 0);
	for (auto indexOfResult = 0; indexOfResult < numberOfFilters; ++indexOfResult)
	{
		result[indexOfResult] /= numberOfActivationVectors;
		if (result[indexOfResult] >= threshold)
		{
			generatedTemplate[indexOfResult] = 1;
		}
	}

	return generatedTemplate;
}

void NNLetterRecognition::createTemplates(std::unordered_map<char, std::vector<int>> templates, const char letterTested, const std::vector<std::vector<double>>& activationOfFilters)
{
	templates.emplace(letterTested, generatingFunctionActivationVector(activationOfFilters)); //the implementation for the function can be changed eventually
}