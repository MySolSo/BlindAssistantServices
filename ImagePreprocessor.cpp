#include "stdafx.h"
#include "ImagePreprocessor.h"

#include <opencv2/opencv.hpp> 
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

const int blockSize = 11;//27

ImagePreprocessor::ImagePreprocessor()
{
}
ImagePreprocessor::~ImagePreprocessor()
{
}

std::vector<cv::Point2f> ImagePreprocessor::getRelativeCornersFromImage()
{
	cv::namedWindow("Original image", CV_WINDOW_AUTOSIZE);
	imshow("Original image", _image);

	auto src1 = _image;

	cv::Mat gray, edge, draw;
	cvtColor(src1, gray, CV_BGR2GRAY);

	Canny(gray, edge, 50, 150, 3);
	cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(13, 13));
	//morphologyEx(edge, edge, MORPH_CLOSE, element);
	dilate(edge, edge, element);
	std::vector<std::vector<cv::Point>> contours;
	findContours(edge, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	double max_area = 0;
	std::vector<cv::Point> max_contour;
	std::vector<std::vector<cv::Point>> cnt;
	for (auto contour : contours)
	{
		if (contourArea(contour) > max_area)
			max_contour = contour, max_area = contourArea(contour);
	}

	cv::RotatedRect enclosing_rect = minAreaRect(max_contour);

	cv::Point2f* corners = new cv::Point2f[4];
	enclosing_rect.points(corners);
	for (int i = 0; i < 4; i++)
	{
		circle(src1, corners[i], 5, cv::Scalar(255, 0, 0), 2);
		std::cout << corners[i].x << " " << corners[i].y << std::endl;
	}
	cv::imshow("paper", src1);
	cv::waitKey(0);
	cv::destroyAllWindows();
	edge.convertTo(draw, CV_8U);
	cv::namedWindow("image", CV_WINDOW_AUTOSIZE);
	cv::imshow("image", draw);
	cv::waitKey(0);

	std::vector<cv::Point2f> returns;
	for (int i = 3; i > -1; --i)
		returns.push_back(corners[i]);

	return returns;
}
cv::Mat ImagePreprocessor::perspectiveCorrection(const std::vector<cv::Point2f>& cornerPoints)
{

	//the display window was resized for better fit on screen 
	cv::namedWindow("Display window", cv::WINDOW_NORMAL);
	cv::resizeWindow("Display window", 800, 800);

	cv::imshow("Display window", _image); //original image

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
	cv::warpPerspective(_image, output, homography, size);

	cv::imshow("Warped source image", output);

	return output;
}
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

	std::vector<cv::Mat> responses;
	for (auto contour : contours)
	{
		cv::Rect r = boundingRect(contour);
		if (r.height < 40 && r.height > 20)
		{
			responses.insert(responses.begin(),temp(r));
			std::cout << r.x << " " << r.y << " " << r.width << " " << r.height << std::endl;
			/// TODO: sort that vector ASAP sdfs 
		}
	}
	
	/*for (auto disp : responses)
	{
		cv::imshow("rect", disp);
		cv::waitKey(0);
	}
*/
	drawContours(temp, contours2, -1, cv::Scalar(0, 0, 255), 2);
	imshow("cnts", temp);
	cv::namedWindow("Display final window", cv::WINDOW_AUTOSIZE);
	imshow("Display final window", image);
	cv::waitKey(0);
	return responses;
}
