#include "stdafx.h"
#include "ImagePreprocessor.h"

#include <opencv2/opencv.hpp> 
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include <map>


const int blockSize = 11;//27

ImagePreprocessor::ImagePreprocessor()
{
}
ImagePreprocessor::~ImagePreprocessor()
{
}
static std::vector<cv::Point2f> extremePoints(std::vector<cv::Point>pts)
{
	int  xmin, ymin, xmax, ymax, i;
	cv::Point ptxmin, ptymin, ptxmax, ptymax;

	cv::Point pt = pts[0];

	ptxmin = ptymin = ptxmax = ptymax = pt;
	xmax = ymax = -std::numeric_limits<int>::max();
	xmin = ymin = std::numeric_limits<int>::max();

	const double thresholdForFindingExtremeties = 2.; //////THIS NEEDS SOME REVIEW (IT'S WAY TOO LATE FOR THIS)

	for (size_t i = 1; i < pts.size(); i++)
	{
		pt = pts[i];
		if (pt.x > 0.01)
		{

			if (xmin > pt.x + thresholdForFindingExtremeties)
			{
				if (pt != ptxmin)
				{
					xmin = pt.x;
					ptxmin = pt;
				}
			}


			if (xmax < pt.x + thresholdForFindingExtremeties)
			{
				if (pt != ptxmax)
				{
					xmax = pt.x;
					ptxmax = pt;
				}
			}

			if (ymin > pt.y + thresholdForFindingExtremeties)
			{
				if (pt != ptymin)
				{
					ymin = pt.y;
					ptymin = pt;
				}
			}

			if (ymax < pt.y + thresholdForFindingExtremeties)
			{
				if (pt != ptymax)
				{
					ymax = pt.y;
					ptymax = pt;
				}
			}
		}
	}
	std::vector<cv::Point2f> res;
	res.push_back(ptxmin);
	res.push_back(ptxmax);
	res.push_back(ptymin);
	res.push_back(ptymax);


	return res;
}
void cornerIndexingCorrection(std::vector<cv::Point2f>& corners)
{
	double minSum = 99999999999, maxSum = 0;
	std::vector<cv::Point2f> correctIndexing(4);
	int numberOfIterations = 3;
	while (numberOfIterations >= 0)
	{


		for (auto point : corners)
		{
			double currentSum = point.x + point.y;
			if (currentSum < minSum)
			{
				minSum = currentSum;
				correctIndexing[0] = point;
			}
			if (currentSum > maxSum)
			{
				maxSum = currentSum;
				correctIndexing[2] = point;
			}
			if (currentSum < maxSum && currentSum > minSum && point.x > point.y)
			{
				correctIndexing[1] = point;
			}
			if (currentSum < maxSum && currentSum > minSum && point.x < point.y)
			{
				correctIndexing[3] = point;
			}
		}
		--numberOfIterations;
	}
	for (auto point : correctIndexing)
	{
		if (point.x < 0.001)
		{
			std::cout << std::endl;
		}
	}
	corners = correctIndexing;
}
std::vector<cv::Point2f> ImagePreprocessor::getRelativeCornersFromImage()
{
	cv::namedWindow("Original image", CV_WINDOW_AUTOSIZE);
	imshow("Original image", _image);

	cv::Mat src1 = _image;
	cv::Mat imageCopy = src1;

	cvtColor(imageCopy, imageCopy, CV_BGR2GRAY);

	cv::Mat afterBlur, afterThresh, afterBitwiseNot, cannyResult, afterErosion, dilated;

	cv::Size size(3, 3);
	cv::GaussianBlur(imageCopy, afterBlur, size, 0);
	cv::threshold(afterBlur, afterThresh, 110, 255, cv::THRESH_TOZERO);

	cv::namedWindow("AFTER THRESHOLD", cv::WINDOW_NORMAL);
	cv::imshow("AFTER THRESHOLD", afterThresh);

	cv::erode(afterThresh, afterErosion, cv::Mat());
	cv::dilate(afterErosion, dilated, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);

	cv::Mat element5(7, 35, CV_8U, cv::Scalar(4));
	cv::Mat closed, dilatedOneMoreTime;
	cv::morphologyEx(dilated, closed, cv::MORPH_CLOSE, element5);

	cv::dilate(closed, dilatedOneMoreTime, cv::Mat(), cv::Point(-1, -1), 2, 1, 100);

	Canny(closed, cannyResult, 350, 100, 5);

	cv::namedWindow("CANNY", cv::WINDOW_NORMAL);
	cv::imshow("CANNY", cannyResult);

	cv::bitwise_not(cannyResult, afterBitwiseNot);
	cv::namedWindow("BITWISE", cv::WINDOW_NORMAL);
	cv::imshow("BITWISE", afterBitwiseNot);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(cannyResult, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	auto max_area = 0.;
	std::vector<cv::Point> max_contour;

	for (auto contour : contours)
	{
		if (cv::contourArea(contour) > max_area)
		{
			max_area = cv::contourArea(contour);
			max_contour = contour;
		}
	}

	std::vector<std::vector<cv::Point>> paperContour;
	paperContour.push_back(max_contour);
	cv::drawContours(imageCopy, paperContour, -1, cv::Scalar(0, 200, 120), 5, 8);

	std::vector<cv::Point2f> corners;
	corners = extremePoints(max_contour);

	for (auto corner : corners)
	{
		cv::circle(imageCopy, corner, 5, cv::Scalar(255, 0, 255), 2);
	}
	cornerIndexingCorrection(corners);

	
	cv::namedWindow("DISPLAY", cv::WINDOW_NORMAL);
	cv::imshow("DISPLAY", imageCopy);
	return corners;
	//return returns;
}
/// TODO: Review code
cv::Mat ImagePreprocessor::perspectiveCorrection(cv::Mat copy, const std::vector<cv::Point2f>& cornerPoints)
{

	//target image is loaded
//	cv::Mat image = cv::imread("book.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Mat image = copy;
	//the display window was resized for better fit on screen 
	cv::namedWindow("Display window", cv::WINDOW_NORMAL);
	cv::resizeWindow("Display window", 800, 800);

	cv::imshow("Display window", image); //original image

										 //corner pixels (coordinates)
	std::vector<cv::Point2f> sourcePoints;
	sourcePoints.push_back(cornerPoints[0]);
	sourcePoints.push_back(cornerPoints[1]);
	sourcePoints.push_back(cornerPoints[2]);
	sourcePoints.push_back(cornerPoints[3]);

	cv::Size size(300, 400);//setting the destination image size
							//most books and sheets of paper are 3:4, but we should discuss this

							//the destination points are set
	std::vector<cv::Point2f> destinationPoints;
	destinationPoints.push_back(cv::Point2f(0, 0));
	destinationPoints.push_back(cv::Point2f(size.width - 1, 0));
	destinationPoints.push_back(cv::Point2f(size.width - 1, size.height - 1));
	destinationPoints.push_back(cv::Point2f(0, size.height - 1));

	//creating a blank image where the new image will be saved
	cv::Mat output = cv::Mat::zeros(size, CV_8UC3);

	//homography calculation
	cv::Mat homography = cv::findHomography(sourcePoints, destinationPoints);

	//perspective correction
	cv::warpPerspective(image, output, homography, size);

	cv::imshow("Warped source image", output);

	return output; // should you need the cv::Mat

////the display window was resized for better fit on screen 
//cv::namedWindow("Display window", cv::WINDOW_NORMAL);
//cv::resizeWindow("Display window", 800, 800);

//cv::imshow("Display window", _image); //original image

//									 //corner pixels (coordinates)
//std::vector<cv::Point2f> sourcePoints;
//sourcePoints.push_back(cornerPoints[0]);
//sourcePoints.push_back(cornerPoints[1]);
//sourcePoints.push_back(cornerPoints[2]);
//sourcePoints.push_back(cornerPoints[3]);

//cv::Size size(300, 400);//setting the destination image size
//						//most books and sheets of paper are 3:4, but we should discuss this

//						//the destination points are set
//std::vector<cv::Point2f> destinationPoints;
//destinationPoints.push_back(cv::Point2f(0, 0));
//destinationPoints.push_back(cv::Point2f(size.width - 1, 0));
//destinationPoints.push_back(cv::Point2f(size.width - 1, size.height - 1));
//destinationPoints.push_back(cv::Point2f(0, size.height - 1));

////creating a blank image where the new image will be saved
//cv::Mat output = cv::Mat::zeros(size, CV_8UC3);

////homography calculation
//cv::Mat homography = cv::findHomography(sourcePoints, destinationPoints);

////perspective correction
//cv::warpPerspective(_image, output, homography, size);

//cv::imshow("Warped source image", output);

//return output;
}
struct sortingComparatorStruct
{
	bool operator()(double x1, double x2)
	{
		return x1 < x2;
	}
};

std::vector<cv::Mat> ImagePreprocessor::getColorRefinedImageInLines(cv::Mat image)
{
	cv::Mat temp = cv::Mat(image);
	//_image = image;

	//cvtColor(image, image, CV_8UC1);
	cvtColor(image, image, CV_BGR2GRAY);
	adaptiveThreshold(image, image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, 10);//20

	cv::namedWindow("Display gray window", cv::WINDOW_AUTOSIZE);

	//medianBlur(image, image, 5); pt imagini zgomotoase

	imshow("Display gray window", image);
	//int imSize = int((image.cols + image.rows) / 2);
	//int blockSize = int(imSize / 19 / 2) * 2 - 1; // /2 *2 pentru a avea un numar IMPAR

	cv::imshow("th", image);
	cv::waitKey(0);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Rect> contours2;
	findContours(image, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	std::map<double, cv::Mat, sortingComparatorStruct> responses;
	for (auto contour : contours)
	{
		cv::Rect r = boundingRect(contour);
		if (r.height < 40 && r.height > 20)
		{
			//responses.insert(responses.begin(), temp(r));
			responses.emplace(r.x, temp(r));
			std::cout << r.x << " " << r.y << " " << r.width << " " << r.height << std::endl;

			/// TODO: sort that vector ASAP sdfs 
		}
	}

	/*for (auto disp : responses)
	{
		cv::imshow("rect", disp);
		cv::waitKey(0);
	}*/

	drawContours(temp, contours2, -1, cv::Scalar(0, 0, 255), 2);
	imshow("cnts", temp);
	cv::namedWindow("Display final window", cv::WINDOW_AUTOSIZE);
	imshow("Display final window", image);
	cv::waitKey(0);
	
	std::vector<cv::Mat> sortedLetters;
	for (auto element : responses)
	{
		sortedLetters.push_back(element.second);
	}

	return sortedLetters;
}

//////////////////////////////////////////////////