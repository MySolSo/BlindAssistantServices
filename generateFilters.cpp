#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
using namespace cv;

const int step = 5;
const int filterRows = 10;
const int filterCols = 10;

double numberOfImages = 0;
ofstream fout("text.out");

void writeFilter(Mat filter)
{
	for (int iFilter = 0; iFilter <  filter.rows; iFilter++)
	{
		for (int jFilter = 0; jFilter < filter.cols; jFilter++)
		{
			fout << filter.at<double>(iFilter, jFilter) << ' ';
		}
		fout << endl;
	}
	fout << endl;
	fout << endl;
}
vector<Mat> createFilters(Mat image)
{
	cvtColor(image, image, COLOR_BGR2GRAY);
	image.convertTo(image, CV_64FC1);
	
	Mat filter;
	filter = Mat::zeros(filterRows, filterCols, CV_64FC1);
	
	vector<Mat>filters;
	for (int iMat = 0; iMat < image.rows - filterRows; iMat += step)
	{
		for (int jMat = 0; jMat < image.cols - filterCols; jMat += step)
		{
			
			for (int iFilter = 0; iFilter < filter.rows; iFilter++)
			{
				for (int jFilter = 0; jFilter < filter.cols; jFilter++)
				{
					filter.at<double>(iFilter, jFilter) = image.at<double>(iFilter + iMat, jFilter + jMat)/255.0;
				}
			}
			filters.push_back(filter.clone());
			filter = Mat::zeros(filterRows, filterCols, CV_64FC1);
		}
	}
	return filters;
}
void modifyFilters(Mat image, vector<Mat> &filters)
{
	cvtColor(image, image, COLOR_BGR2GRAY);
	image.convertTo(image, CV_64FC1);

	Mat filter;
	filter = Mat::zeros(filterRows, filterCols, CV_64FC1);
	int currentFilter = 0;
	for (int iMat = 0; iMat < image.rows - filterRows; iMat += step)
	{
		for (int jMat = 0; jMat < image.cols - filterCols; jMat += step, currentFilter++)
		{
			for (int iFilter = 0; iFilter < filterRows; iFilter++)
			{
				for (int jFilter = 0; jFilter < filterCols; jFilter++)
				{
					filters[currentFilter].at<double>(iFilter, jFilter) += image.at<double>(iFilter + iMat, jFilter + jMat)/255.0;
				}

			}
		}
	}
}
vector<Mat> generateFilters()
{
	VideoCapture images("../data/text%d.jpg");
	vector<Mat> filters;
	int norm_size = 30;
	Mat image, norm_image(norm_size, norm_size, CV_64F);
	if (images.isOpened())
	{
		if (images.read(image))
		{
			numberOfImages++;
			resize(image, norm_image, Size(norm_size, norm_size));
			filters=createFilters(norm_image); 
		}
	}
	while (images.isOpened())
	{
		if (images.read(image))
		{
			numberOfImages++;
			resize(image, norm_image, Size(norm_size, norm_size));
			modifyFilters(norm_image,filters);
		}
		else
			images.release();
	}
	
	return filters;
}

int main()
{
	vector<Mat> filters = generateFilters();
	std::cout << filters.size();
	Mat img(filterRows,filterCols, CV_64F);
	//namedWindow("brack", WINDOW_AUTOSIZE);
	for (int i=0;i<filters.size();i+=1)
	{
		resize(filters[i], img, img.size(), 0, 0, INTER_NEAREST);
		for (int k = 0; k < img.rows; k++)
			for (int j = 0; j < img.cols; j++)
				img.at<double>(k, j) /= numberOfImages;
		//imshow("brack", img);
		writeFilter(img);
		waitKey(0);
	}
	return 0;
}