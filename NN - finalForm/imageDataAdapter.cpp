#include "imageDataAdapter.h"



imageDataAdapter::imageDataAdapter()
{
}


imageDataAdapter::~imageDataAdapter()
{
}

int imageDataAdapter::remapIntValueToUsedFormat(const double input,const int minInRange,const int maxInRange)
{
	const int atrMinRange = 0, atrMaxRange = 80;

	int result = (input - minInRange)/(maxInRange - minInRange) * (atrMaxRange - atrMinRange) + atrMinRange;

	return result;
}

std::vector<double> imageDataAdapter::adaptToHeatMap(const cv::Mat & image)
{
	cv::Mat originalImage = image;

	cv::Mat resized, binaryImg;

	cv::Size size(5, 5);
	cv::resize(originalImage, resized, size);

	cvtColor(resized, resized, CV_BGR2GRAY);
	
	//cv::threshold(resized, resized, 100, 255, CV_THRESH_BINARY_INV);
	
	std::vector<double> result;
	double currentValue;
	double minValue = std::numeric_limits<double>::max();
	double maxValue = std::numeric_limits<double>::min();

	double normalizedPixelValue, invertedValue;
	for (auto i = 0; i < size.height; ++i)
	{
		for (auto j = 0; j < size.width; ++j)
		{
			currentValue = static_cast<int>(resized.at<uchar>(i, j));
			invertedValue =  255 - currentValue;
			resized.at<uchar>(i, j) = invertedValue;
			if (invertedValue < minValue)
			{
				minValue = currentValue;
			}
			else if (invertedValue > maxValue)
			{
				maxValue = currentValue;
			}
		}
	}
	/*cv::imshow("display", resized);
	cv::waitKey(0);*/
	double finalValue;
	for (auto i = 0; i < size.height; ++i)
	{
		for (auto j = 0; j < size.width; ++j)
		{
			currentValue = static_cast<double>(resized.at<uchar>(i, j));
			//invertedValue = 255 - currentValue;
			/*if (currentValue < 100)
				result.push_back(0);
			else
				result.push_back(1);*/
			normalizedPixelValue = (currentValue - minValue) / (maxValue - minValue);
			finalValue = (255 * normalizedPixelValue) + 0;
			result.push_back(finalValue);
			
		}
	}
	

	return result;
}

std::vector<double> imageDataAdapter::adaptImageToDataset(/*const std::string& imagePath*/const cv::Mat& image)
{
	//const auto image = cv::imread(imagePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat originalImage = image;

	cv::Mat gray, binaryImg;
	cvtColor(originalImage, gray, cv::COLOR_BGR2GRAY);

	cv::threshold(gray, binaryImg, 100, 255, cv::THRESH_BINARY);
	////////////////////
	auto binaryValue = 255;
	////////////////////

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
			if (currentPixel == binaryValue)
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
					/*cv::imshow("letter test", binaryImg);
					cv::waitKey(0);*/
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
	const int boxCenterX = minimumXPosition + ((maximumXPosition - minimumXPosition)/2);
	const int boxCenterY = minimumYPosition + ((maximumYPosition - minimumYPosition)/2);
	const int numberOfPixelsInBox = boxWidth * boxHeight;
	const double oldxBar = onPixelsXCoordinateSum / numberOfPixelsInBox;
	const double oldyBar = onPixelsYCoordinateSum / numberOfPixelsInBox;
	unsigned int varianceOfXCoordBeforeDiv = 0;
	unsigned int varianceOfYCoordBeforeDiv = 0;

	unsigned int lastOffPixelXCoord = 0;
	unsigned int lastOffPixelYCoord = 0;

	double sumOfEdgeXCoord = 0;
	double sumOfEdgeYCoord = 0;

	double sumOfXrelativeToCenter = 0;
	double sumOfYrelativeToCenter = 0;

	double sumOfXSqRelativeToCenter = 0;
	double sumOfYSqRelativeToCenter = 0;

	unsigned int numberOfEdgesX = 0;
	unsigned int numberOfEdgesY = 0;

	double sumOfXYProductRelativeToCenter = 0;

	double maximumPossibleMeanForAtr8 = 0;
	double maximumPossibleMeanForAtr9 = 0;
	double maximumPossibleMeanForAtr10 = 0;
	double maximumPossibleMeanForAtr11 = 0;
	double maximumPossibleMeanForAtr12 = 0;
	double maximumPossibleRangeForAtr14 = 0;
	double maximumPossibleRangeForAtr16 = 0;
	double maximumPossibleMeanForAtr6 = 0;
	double maximumPossibleMeanForAtr7 = 0;

	double sumOfX2Y = 0;
	double sumOfXY2 = 0;

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
			if (static_cast<int>(binaryImg.at<uchar>(i, j)) == binaryValue)
			{
				varianceOfXCoordBeforeDiv += ((j - oldxBar)*(j - oldxBar));
				varianceOfYCoordBeforeDiv += ((i - oldyBar)*(i - oldyBar));

				sumOfXrelativeToCenter += (j - boxCenterX);
				sumOfYrelativeToCenter += (i - boxCenterY);

				sumOfXSqRelativeToCenter += ((j - boxCenterX) *(j - boxCenterX));
				sumOfYSqRelativeToCenter += ((i - boxCenterY) *(j - boxCenterY));

				sumOfXYProductRelativeToCenter += (j - boxCenterX) *(i - boxCenterY);

				sumOfX2Y += ((j - boxCenterX)*(j - boxCenterX))*(i - boxCenterY);
				sumOfXY2 += (i - boxCenterY)*(i - boxCenterY)*(j - boxCenterX);
			}
			auto currentPixel = static_cast<int>(binaryImg.at<uchar>(i, j)) == binaryValue;
			if (currentPixel && lastOffPixelXCoord == (j - 1) && lastOffPixelYCoord == i)
			{
				++numberOfEdgesX;
				sumOfEdgeYCoord += i;
			}
			if (currentPixel && /*lastOffPixelYCoord == (i - 1)*/ static_cast<int>(binaryImg.at<uchar>(i + 1, j)) != binaryValue /*&& lastOffPixelXCoord == j*/)
			{
				++numberOfEdgesY;
				sumOfEdgeXCoord += j;
			}
			if(!currentPixel)
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

	//const double varianceOfYCoord = sumOfYSquaredProduct / numberOfPixelsInBox;

	/*const int correlationOfXY = ((countOfOnPixels*sumOfXYProduct) - (onPixelsXCoordinateSum * onPixelsYCoordinateSum)) /
	(sqrt(((countOfOnPixels*sumOfXSquaredProduct) - (onPixelsXCoordinateSum*onPixelsXCoordinateSum)) *
	((countOfOnPixels*sumOfYSquaredProduct) - (onPixelsYCoordinateSum*onPixelsYCoordinateSum))));*/

	//const double correlationOfXY = sumOfXYProduct / numberOfPixelsInBox;

	const double varianceOfXCoord = ((sumOfXSqRelativeToCenter / static_cast<double>(countOfOnPixels)) /*/ static_cast<double>(boxWidth)*/);
	const double varianceOfYCoord = ((sumOfYSqRelativeToCenter / static_cast<double>(countOfOnPixels))/* / static_cast<double>(boxHeight)*/);

	const double correlationOfXY = sumOfXYProductRelativeToCenter / static_cast<double>(countOfOnPixels);
	/*const double x2ybr = sumX2YBar / numberOfPixelsInBox;
	const double xy2br = sumXY2Bar / numberOfPixelsInBox;*/
	const double x2ybr = sumOfX2Y / static_cast<double>(countOfOnPixels);
	const double xy2br = sumOfXY2 / static_cast<double>(countOfOnPixels);

	const double xBar = (sumOfXrelativeToCenter / static_cast<double>(boxWidth)) / static_cast<double>(countOfOnPixels);
	const double yBar = (sumOfYrelativeToCenter / static_cast<double>(boxHeight)) / static_cast<double>(countOfOnPixels);

	const double verticalEdgesMean = numberOfEdgesY / ((boxHeight / 2.) * boxWidth);
	const double horizontalEdgesMean = numberOfEdgesX / ((boxWidth / 2.) * boxHeight);

	const int atrMinRange = 0, atrMaxRange = 15;

	const double atr6MinRange = ((minimumXPosition - boxCenterX) + (maximumXPosition - boxCenterX)) / 2.;
	const double atr6MaxRange = ((minimumXPosition + maximumXPosition * boxHeight) / static_cast<double>(boxHeight + 1));

	const double atr7MinRange = ((minimumYPosition - boxCenterY) + (maximumYPosition - boxCenterY)) / 2.;
	const double atr7MaxRange = ((minimumYPosition + maximumYPosition * boxWidth) / static_cast<double>(boxWidth + 1));

	const double atr8MinRange = (pow(maximumXPosition - boxCenterX, 2) + pow(minimumXPosition - boxCenterX, 2)) / 2.;
	const double atr8MaxRange = (pow(maximumXPosition - boxCenterX, boxHeight)+pow(minimumXPosition - boxCenterX,2)) / static_cast<double>(boxHeight + 1);

	const double atr9MinRange = (pow(maximumYPosition - boxCenterY, 2) + pow(minimumYPosition - boxCenterY, 2)) / 2.;
	const double atr9MaxRange = (pow(maximumYPosition - boxCenterY, boxWidth) + pow(minimumYPosition - boxCenterY, 2)) / static_cast<double>(boxWidth + 1);

	const double atr10MaxRange = ((maximumXPosition - boxCenterX)*(maximumYPosition - boxCenterY) + (minimumXPosition - boxCenterX)*(minimumYPosition - boxCenterY)) / 2.;

	const double atr11MinRange = (pow(maximumXPosition - boxCenterX, 2)*(minimumYPosition - boxCenterY) + (pow(minimumXPosition - boxCenterX, 2)*(minimumYPosition - boxCenterY)) + pow(minimumXPosition - boxCenterX, 2) *(maximumYPosition - boxCenterY)) / 3.;
	const double atr12MinRange = (pow(maximumYPosition - boxCenterY, 2)*(minimumXPosition - boxCenterX) + (pow(minimumYPosition - boxCenterY, 2)*(minimumXPosition - boxCenterX)) + pow(minimumYPosition - boxCenterY, 2) *(maximumXPosition - boxCenterX)) / 3.;

	const double atr11MaxRange = (pow(maximumXPosition - boxCenterX, 2)*(maximumYPosition - boxCenterY) + (pow(minimumXPosition - boxCenterX, 2)*(minimumYPosition - boxCenterY)) + pow(minimumXPosition - boxCenterX, 2) *(maximumYPosition - boxCenterY)) / 3.;
	const double atr12MaxRange = (pow(maximumYPosition - boxCenterY, 2)*(maximumXPosition - boxCenterX) + (pow(minimumYPosition - boxCenterY, 2)*(minimumXPosition - boxCenterX)) + pow(minimumYPosition - boxCenterY, 2) *(maximumXPosition - boxCenterX)) / 3.;

	int atr_1 = remapIntValueToUsedFormat(boxCenterX, 0, binaryImg.cols);
	int atr_2 = remapIntValueToUsedFormat(boxCenterY, 0, binaryImg.rows);
	int atr_3 = remapIntValueToUsedFormat(boxWidth, 0, binaryImg.cols);
	int atr_4 = remapIntValueToUsedFormat(boxHeight, 0, binaryImg.rows);
	int atr_5 = remapIntValueToUsedFormat(countOfOnPixels, 0, binaryImg.cols*binaryImg.rows);

	int atr_6 = remapIntValueToUsedFormat(xBar, atr6MinRange, atr6MaxRange);
	int atr_7 = remapIntValueToUsedFormat(yBar, atr7MinRange, atr7MaxRange);

	int atr_8 = remapIntValueToUsedFormat(varianceOfXCoord, atr8MinRange, atr8MaxRange);
	int atr_9 = remapIntValueToUsedFormat(varianceOfYCoord, atr9MinRange, atr9MaxRange);
	int atr_10 = remapIntValueToUsedFormat(correlationOfXY, 0,atr10MaxRange);

	int atr_11 = remapIntValueToUsedFormat(x2ybr, atr11MinRange, atr11MaxRange);
	int atr_12 = remapIntValueToUsedFormat(xy2br, atr12MinRange, atr12MaxRange);
	
	int atr_13 = remapIntValueToUsedFormat(horizontalEdgesMean, 0, boxWidth / 2.);
	int atr_14 = remapIntValueToUsedFormat(sumOfEdgeYCoord, minimumYPosition, maximumPossibleRangeForAtr14);
	int atr_15 = remapIntValueToUsedFormat(verticalEdgesMean, 0, boxHeight / 2.);
	int atr_16 = remapIntValueToUsedFormat(sumOfEdgeXCoord, minimumXPosition, maximumPossibleRangeForAtr16);

	std::vector<double> imageAtr;
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

	/*for (auto value : imageAtr)
	{
		assert(value < 0 || value > 15);
	}*/
	
	return imageAtr;

}
