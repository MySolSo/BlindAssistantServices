#include "stdafx.h"
#include "ParseLetters.h"
#include <iostream>


int ParseLetters::getAverageBlackValue()
{

	int MedianOne = static_cast<int>(_line.at<uchar>(cv::Point(0, 0)));
	int MedianTwo = MedianOne;
	int color;

	for (auto cols = 0; cols < _line.cols; ++cols)
	{
		for (auto rows = 0; rows < _line.rows; ++rows)
		{
			color = static_cast<int>(_line.at<uchar>(cv::Point(cols, rows)));
			if (color < (MedianOne + MedianTwo) / 2)
			{
				MedianOne = (MedianOne + color) / 2;
			}
			else
			{
				MedianTwo = (MedianTwo + color) / 2;
			}
		}
	}

	return (MedianOne - 55);
}

std::vector<cv::Mat> ParseLetters::getVectorOfLettersFromLine(std::vector<cv::Mat> lines,const int lowerLimit)
{

	/// TODO: if needed for refinering work on image with undersampling

	/*
	cv::Mat blackAndWhite;
	cv::transform(_line, blackAndWhite, cv::Matx13f(1, 1, 1));

	cv::imshow("plm", blackAndWhite);
	cv::waitKey(0);
	*/
	std::vector<cv::Mat> returnLetters;

	for (auto line : lines) {

		cv::Vec3b color;
		bool isThereTrueBlack;
		for (auto cols = 0; cols < line.cols; ++cols)
		{
			isThereTrueBlack = false;
			for (auto rows = 0; rows < line.rows; ++rows)
			{
				color = line.at<cv::Vec3b>(cv::Point(cols, rows));

				if (static_cast<int>(color[0]) < lowerLimit && static_cast<int>(color[1]) < lowerLimit && static_cast<int>(color[2]) < lowerLimit)
				{
					isThereTrueBlack = true;
				}

				line.at<cv::Vec3b>(cv::Point(cols, rows)) = color;
			}

			if (!isThereTrueBlack)
			{
				int rows = 0;

				// For visual feedback

				for (auto rows = 0; rows < line.rows; ++rows)
				{
					color = line.at<cv::Vec3b>(cv::Point(cols, rows));

						color[0] = 0;
					color[1] = 0;
					color[2] = 255;

					line.at<cv::Vec3b>(cv::Point(cols, rows)) = color;
				}

				//

				color[0] = 0;
				color[1] = 0;
				color[2] = 255;

				line.at<cv::Vec3b>(cv::Point(cols, rows)) = color;
			}
		}

		//

		cv::imshow("plm", line);
		cvWaitKey(0);

		//

		bool neededPart_curent = false;
		bool neededPart_pervous = false;
		int startWidth;
		int endWidth;

		for (auto cols = 0; cols < line.cols; ++cols)
		{
			color = line.at<cv::Vec3b>(cv::Point(cols, 0));
			if (color[0] == 0 && color[1] == 0 && color[2] == 255)
			{
				neededPart_curent = false;
			}
			else
			{
				neededPart_curent = true;
			}

			if (neededPart_curent != neededPart_pervous) {
				if (neededPart_curent == true)
				{
					startWidth = cols;
				}
				else
				{
					endWidth = cols;
					cv::Mat letter = cv::Mat(line, cv::Rect(startWidth, 1, (endWidth - startWidth), line.rows - 1));

					returnLetters.push_back(letter);
				}
			}
			neededPart_pervous = neededPart_curent;
		}
	}
	return returnLetters;
}