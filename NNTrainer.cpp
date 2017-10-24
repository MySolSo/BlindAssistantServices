#include "stdafx.h"
#include "NNTrainer.h"
#include <unordered_map>
#include <opencv2/core/mat.hpp>
#include <opencv2/shape/hist_cost.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


NNTrainer::NNTrainer()
{
}


NNTrainer::~NNTrainer()
{
}


void NNTrainer::writeFilter(cv::Mat filter)
{
	fout.close();
	fout.open("filters.txt", std::fstream::app);
	fout << "<filter>" << std::endl;
	for (int iFilter = 0; iFilter < filter.rows; iFilter++)
	{
		for (int jFilter = 0; jFilter < filter.cols; jFilter++)
		{
			fout << filter.at<double>(iFilter, jFilter)/numberOfImages << ' ';
		}
		fout << std::endl;
	}
	fout << "</filter>" << std::endl;
	fout.close();
}
std::vector<cv::Mat> NNTrainer::createFilters(cv::Mat image)
{
	cvtColor(image, image, cv::COLOR_BGR2GRAY);
	image.convertTo(image, CV_64FC1);

	cv::Mat filter;
	filter = cv::Mat::zeros(filterRows, filterCols, CV_64FC1);

	std::vector<cv::Mat>filters;
	for (int iMat = 0; iMat < image.rows - filterRows + 1; iMat += step)
	{
		for (int jMat = 0; jMat < image.cols - filterCols + 1; jMat += step)
		{
			for (int iFilter = 0; iFilter < filter.rows; iFilter++)
			{
				for (int jFilter = 0; jFilter < filter.cols; jFilter++)
				{
					//          add 1 - etc for images with white background 

					filter.at<double>(iFilter, jFilter) = (image.at<double>(iFilter + iMat, jFilter + jMat) / 255.0);
				}
			}
			filters.push_back(filter.clone());
			filter = cv::Mat::zeros(filterRows, filterCols, CV_64FC1);
		}
	}
	return filters;
}
void NNTrainer::modifyFilters(cv::Mat image, std::vector<cv::Mat> &filters)
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
std::vector<cv::Mat> NNTrainer::generateFilters(const char* pathToSetOfPics)
{

	cv::VideoCapture images(pathToSetOfPics);
	std::vector<cv::Mat> filters;
	cv::Mat originalImageOfLetter, norm_image(norm_sizeX, norm_sizeY, CV_64F);
	//cv::resize(image, norm_image, cv::Size(norm_sizeX, norm_sizeY));
	//cvWaitKey(0);
	images.read(originalImageOfLetter);
	if (images.isOpened())
	{
		if (images.read(originalImageOfLetter))
		{

			//////////
			//test purposes only
			cv::imwrite("../letterImage.jpg", originalImageOfLetter);
			//////////

			numberOfImages++;
			//ADD WHITE BACKGROUND
			cv::Mat whiteBackground(originalImageOfLetter.rows, originalImageOfLetter.cols + 3, CV_8UC3, cv::Scalar(255, 255, 255));
			originalImageOfLetter.copyTo(whiteBackground(cv::Rect((whiteBackground.cols - originalImageOfLetter.cols)
				/ 2, (whiteBackground.rows - originalImageOfLetter.rows) / 2, originalImageOfLetter.cols, originalImageOfLetter.rows)));
			//that double line though.

			//RESIZE TO NEEDED SIZE
			resize(whiteBackground, norm_image, cv::Size(norm_sizeX, norm_sizeY));
			cv::imshow("WHITE BACKGROUND", whiteBackground);
			cv::imshow("asd", norm_image);
			filters = createFilters(norm_image);
		}
	}

	while (images.isOpened())
	{
		if (images.read(originalImageOfLetter))
		{

			//////////
			//test purposes only
			cv::imwrite("../letterImage.jpg", originalImageOfLetter);
			//////////

			numberOfImages++;
			cv::Mat whiteBackground(originalImageOfLetter.rows, originalImageOfLetter.cols + 3, CV_8UC3, cv::Scalar(255, 255, 255));
			originalImageOfLetter.copyTo(whiteBackground(cv::Rect((whiteBackground.cols - originalImageOfLetter.cols)
				/ 2, (whiteBackground.rows - originalImageOfLetter.rows) / 2, originalImageOfLetter.cols, originalImageOfLetter.rows)));

			resize(whiteBackground, norm_image, cv::Size(norm_sizeX, norm_sizeY));
			modifyFilters(norm_image, filters);
		}
		else
			images.release();
	}

	for (auto filter : filters)
		writeFilter(filter);

	return filters;
}

void NNTrainer::startGeneratingFilters(const char* pathToSetOfPics)
{
	fout.open("filters.txt", std::fstream::out | std::fstream::app);

	std::vector<cv::Mat> filters = generateFilters(pathToSetOfPics);
	//std::cout << filters.size();
	cv::Mat img(filterRows, filterCols, CV_64F);
	//namedWindow("brack", WINDOW_AUTOSIZE);
	//int nr = 0;
	for (int i = 0; i < filters.size(); i += 1)
	{
		resize(filters[i], img, img.size(), 0, 0, cv::INTER_NEAREST);
		for (int k = 0; k < img.rows; k++)
			for (int j = 0; j < img.cols; j++)
				img.at<double>(k, j) /= numberOfImages;
		//imshow("brack", img);

		//cv::waitKey(0);
		//nr = i;
	}
	//std::cout << std::endl << nr << std::endl;
}

std::vector<int> NNTrainer::generatingFunctionActivationVector(const std::vector<std::vector<double>>& activationOfFilters, const char* itIsThisLetter, const char* nameOfOutput)
{
	fout.close();
	fout.open(nameOfOutput, std::fstream::in | std::fstream::out | std::fstream::app);

	const unsigned numberOfFilters = activationOfFilters[0].size();
	const unsigned numberOfActivationVectors = activationOfFilters.size();
	const double threshold = 0.85; //selected randomly; 

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

	fout << "<sablon>" << std::endl;
	fout << itIsThisLetter << " ";
	for (auto activation : generatedTemplate)
	{
		fout << activation << " ";
	}
	fout << std::endl << "</sablon>" << std::endl;

	fout.close();

	return generatedTemplate;
}

void NNTrainer::createTemplates(std::unordered_map<char, std::vector<int>> templates, const char* letterTested, const std::vector<std::vector<double>>& activationOfFilters, const char* nameOfOutput)
{

	//templates.emplace(letterTested, generatingFunctionActivationVector(activationOfFilters, letterTested, nameOfOutput)); //the implementation for the function can be changed eventually
}

void NNTrainer::initActivationTablesGenerateFile(const char* nameOfFile)
{
	std::fstream fileOut(nameOfFile, std::fstream::out);

	fileOut << "<dimensionsLetter>" << std::endl;
	fileOut << "15 15" << std::endl;
	fileOut << "</dimensionsLetter>" << std::endl;
	fileOut << "<pixelStep>" << std::endl;
	fileOut << step << std::endl;
	fileOut << "</pixelStep>" << std::endl;

	fileOut.close();
}

void NNTrainer::initFilterTablesGenerateFile(const char* nameOfFile)
{
	std::fstream fileOut(nameOfFile, std::fstream::out);

	fileOut << "<dimensionsFilter>" << std::endl;
	fileOut << filterRows << " " << filterCols << std::endl;
	fileOut << "</dimensionsFilter>" << std::endl;

	fileOut.close();
}

void NNTrainer::initFilterTablesGenerateFile(const char* nameOfFile, int length, int height)
{
	std::fstream fileOut(nameOfFile, std::fstream::out);

	fileOut << "<dimensionsFilter>" << std::endl;
	fileOut << length << " " << height << std::endl;
	fileOut << "</dimensionsFilter>" << std::endl;

	fileOut.close();
}
