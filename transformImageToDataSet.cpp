#include "stdafx.h"
#include "transformImageToDataSet.h"

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

transformImageToDataSet::transformImageToDataSet()
{
}


transformImageToDataSet::~transformImageToDataSet()
{
}

int transformImageToDataSet::remapIntValueToUsedFormat(const double input, int minInRange, int maxInRange)
{
	const int atrMinRange = 0, atrMaxRange = 15;

	double intermediateValue = (static_cast<double>(input)) / (maxInRange - minInRange);
	int result = atrMinRange + (atrMaxRange - atrMinRange) * intermediateValue;

	return result;
}

std::vector<int> transformImageToDataSet::adaptingImageToDataset(const cv::Mat& inputImage)
{
	cv::Mat originalImage = inputImage;

	cv::Mat gray, binaryImg;
	cvtColor(originalImage, gray, cv::COLOR_BGR2GRAY);

	cv::threshold(gray, binaryImg, 100, 255, cv::THRESH_BINARY);

	int minimumXPosition = std::numeric_limits<int>::max();
	int maximumXPosition = std::numeric_limits<int>::min();
	int minimumYPosition;
	int maximumYPosition = std::numeric_limits<int>::min();
	unsigned int countOfOnPixels = 0;
	bool wasYFound = false;
	double onPixelsXCoordinateSum = 0;
	double onPixelsYCoordinateSum = 0;
	double sumOfXYProduct = 0;
	double sumOfXSquaredProduct = 0;
	double sumOfYSquaredProduct = 0;
	double sumNeededForCorrelationXY = 0;
	double sumXY2Bar = 0;
	double sumX2YBar = 0;

	for (auto i = 0; i < binaryImg.rows; ++i)
	{
		for (auto j = 0; j < binaryImg.cols; ++j)
		{
			///cv::imshow("sdf", binaryImg);
			////cvWaitKey(0);
			//auto x = binaryImg.at<int>(i,j);
			//std::cout << static_cast<int>(x) << std::endl;
		//	auto y = binaryImg.at<cv::Point>(i).y;
			auto currentPixel = static_cast<int>(binaryImg.at<uchar>(i, j));
			/*if (currentPixel != 255)
			{
				cv::waitKey(0);
			}*/
			if (currentPixel == 0)
			{

				//sum of x coordinates ->needed for mean of x coordinates
				onPixelsXCoordinateSum += j;

				//sum of x coordinates ->needed for mean of x coordinates
				onPixelsYCoordinateSum += i;

				//correlation variables needed for calculation
				sumOfXYProduct += (i*j);
				sumOfXSquaredProduct += (j*j);
				sumOfYSquaredProduct += (i*i);

				sumXY2Bar += (i*i*j);
				sumX2YBar += (i*j*j);

				//counting ON pixels in image
				++countOfOnPixels;

				if (j < minimumXPosition)
				{
					minimumXPosition = j;
					cv::imshow("letter test", binaryImg);
					cv::waitKey(0);
				}
				else if (j > maximumXPosition)
				{
					maximumXPosition = j;
				}
				if (!wasYFound)
				{
					minimumYPosition = i;
					wasYFound = true;
				}
				if (i > maximumYPosition)
				{
					maximumYPosition = i;
				}
			}
		}
	}
	const int boxWidth = maximumXPosition - minimumXPosition;
	const int boxHeight = maximumYPosition - minimumYPosition;
	const int numberOfPixelsInBox = boxWidth * boxHeight;
	const double xBar = onPixelsXCoordinateSum / numberOfPixelsInBox;
	const double yBar = onPixelsYCoordinateSum / numberOfPixelsInBox;
	unsigned int varianceOfXCoordBeforeDiv = 0;
	unsigned int varianceOfYCoordBeforeDiv = 0;

	unsigned int lastOffPixelXCoord = 0;
	unsigned int lastOffPixelYCoord = 0;

	double sumOfEdgeXCoord = 0;
	double sumOfEdgeYCoord = 0;

	unsigned int numberOfEdgesX = 0;
	unsigned int numberOfEdgesY = 0;

	double maximumPossibleMeanForAtr8 = 0;
	double maximumPossibleMeanForAtr9 = 0;
	double maximumPossibleMeanForAtr10 = 0;
	double maximumPossibleMeanForAtr11 = 0;
	double maximumPossibleMeanForAtr12 = 0;
	double maximumPossibleRangeForAtr14 = 0;
	double maximumPossibleRangeForAtr16 = 0;
	double maximumPossibleMeanForAtr6 = 0;
	double maximumPossibleMeanForAtr7 = 0;

	bool isFirstIterationFinished = false;
	for (auto i = minimumYPosition; i < maximumYPosition; ++i)
	{
		for (auto j = minimumXPosition; j < maximumXPosition; ++j)
		{
			maximumPossibleMeanForAtr6 += j;
			maximumPossibleMeanForAtr7 += i;
			maximumPossibleMeanForAtr10 += i*j;
			maximumPossibleMeanForAtr8 += j * j;
			maximumPossibleMeanForAtr9 += i * i;
			maximumPossibleMeanForAtr11 += j*j*i;
			maximumPossibleMeanForAtr12 += i*i*j;
			if (static_cast<int>(binaryImg.at<uchar>(i, j)) == 0)
			{
				varianceOfXCoordBeforeDiv += ((j - xBar)*(j - xBar));
				varianceOfYCoordBeforeDiv += ((i - yBar)*(i - yBar));
			}
			auto currentPixel = static_cast<int>(binaryImg.at<uchar>(i, j)) == 0;
			if (currentPixel && lastOffPixelXCoord == (j - 1) && lastOffPixelYCoord == i)
			{
				++numberOfEdgesX;
				sumOfEdgeYCoord += i;
			}
			if (currentPixel && /*lastOffPixelYCoord == (i - 1)*/ static_cast<int>(binaryImg.at<uchar>(i + 1, j)) == 255 /*&& lastOffPixelXCoord == j*/)
			{
				++numberOfEdgesY;
				sumOfEdgeXCoord += j;
			}
			else
			{
				lastOffPixelXCoord = j;
				lastOffPixelYCoord = i;
			}
			if (!isFirstIterationFinished)
			{
				

			}
			if (j % 2 != 0)
			{
				maximumPossibleRangeForAtr16 += j;
			}

			if (i % 2 != 0)
			{
				maximumPossibleRangeForAtr14 += i;
			}
		}
		if (i == 1)
		{
			isFirstIterationFinished = true;
		}
	}
	//const int varianceOfXCoord = varianceOfXCoordBeforeDiv / countOfOnPixels;
	//const int varianceOfYCoord = varianceOfYCoordBeforeDiv / countOfOnPixels;

	const double varianceOfXCoord = sumOfXSquaredProduct / numberOfPixelsInBox;
	const double varianceOfYCoord = sumOfYSquaredProduct / numberOfPixelsInBox;

	/*const int correlationOfXY = ((countOfOnPixels*sumOfXYProduct) - (onPixelsXCoordinateSum * onPixelsYCoordinateSum)) /
	(sqrt(((countOfOnPixels*sumOfXSquaredProduct) - (onPixelsXCoordinateSum*onPixelsXCoordinateSum)) *
	((countOfOnPixels*sumOfYSquaredProduct) - (onPixelsYCoordinateSum*onPixelsYCoordinateSum))));*/

	const double correlationOfXY = sumOfXYProduct / numberOfPixelsInBox;
	const double x2ybr = sumX2YBar / numberOfPixelsInBox;
	const double xy2br = sumXY2Bar / numberOfPixelsInBox;
 

	const double verticalEdgesMean = numberOfEdgesY / ((boxHeight /2. ) * boxWidth);
	const double horizontalEdgesMean = numberOfEdgesX / ((boxWidth /2.) * boxHeight);

	const int atrMinRange = 0, atrMaxRange = 15;

	int atr_1 = remapIntValueToUsedFormat(minimumXPosition, 0, binaryImg.cols);
	int atr_2 = remapIntValueToUsedFormat(minimumYPosition, 0, binaryImg.rows);
	int atr_3 = remapIntValueToUsedFormat(boxWidth, 0, binaryImg.cols);
	int atr_4 = remapIntValueToUsedFormat(boxHeight, 0, binaryImg.rows);
	int atr_5 = remapIntValueToUsedFormat(countOfOnPixels, 0, boxWidth * boxHeight);

	
	int atr_6 = remapIntValueToUsedFormat(xBar, minimumXPosition, maximumPossibleMeanForAtr6 / static_cast<double>( numberOfPixelsInBox));

	int atr_7 = remapIntValueToUsedFormat(yBar, minimumYPosition, maximumPossibleMeanForAtr7 / static_cast<double>(numberOfPixelsInBox));

	int atr_8 = remapIntValueToUsedFormat(varianceOfXCoord, minimumXPosition*minimumXPosition, maximumPossibleMeanForAtr8 / static_cast<double>(numberOfPixelsInBox));
	int atr_9 = remapIntValueToUsedFormat(varianceOfYCoord, minimumYPosition*minimumYPosition, maximumPossibleMeanForAtr9 / static_cast<double>(numberOfPixelsInBox));
	int atr_10 = remapIntValueToUsedFormat(correlationOfXY, minimumXPosition*minimumYPosition, maximumPossibleMeanForAtr10 / static_cast<double>(numberOfPixelsInBox));
	int atr_11 = remapIntValueToUsedFormat(x2ybr, minimumXPosition*minimumXPosition*minimumYPosition, maximumPossibleMeanForAtr11 / static_cast<double>(numberOfPixelsInBox));

 
	int atr_12 = remapIntValueToUsedFormat(xy2br, minimumXPosition*minimumYPosition*minimumYPosition, maximumPossibleMeanForAtr12 / static_cast<double>(numberOfPixelsInBox));
	int atr_13 = remapIntValueToUsedFormat(horizontalEdgesMean, 0, boxWidth / 2.);
	int atr_14 = remapIntValueToUsedFormat(sumOfEdgeYCoord, minimumYPosition + 1, maximumPossibleRangeForAtr14);
	int atr_15 = remapIntValueToUsedFormat(verticalEdgesMean, 0, boxHeight / 2.);
	int atr_16 = remapIntValueToUsedFormat(sumOfEdgeXCoord, minimumXPosition + 1, maximumPossibleRangeForAtr16);

	std::vector<int> imageAtr;
	imageAtr.push_back(atr_1);
	imageAtr.push_back(atr_2);
	imageAtr.push_back(atr_3);
	imageAtr.push_back(atr_4);
	imageAtr.push_back(atr_5);
	imageAtr.push_back(atr_6);
	imageAtr.push_back(atr_7);
	imageAtr.push_back(atr_8);
	imageAtr.push_back(atr_9);
	imageAtr.push_back(atr_10);
	imageAtr.push_back(atr_11);
	imageAtr.push_back(atr_12);
	imageAtr.push_back(atr_13);
	imageAtr.push_back(atr_14);
	imageAtr.push_back(atr_15);
	imageAtr.push_back(atr_16);

	return imageAtr;

}