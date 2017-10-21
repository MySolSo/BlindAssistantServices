#pragma once
#include <unordered_map>
#include <fstream>

namespace cv {
	class Mat;
}

class NNTrainer
{
private:
	const int step = 3;             /////      needs to be >= as filter rows and colls
	const int filterRows = 3;       ////    TODO: fix => needs to be equal or sablons wont work
	const int filterCols = 3;

	int norm_sizeX = 15;
	int norm_sizeY = 15;

	double numberOfImages = 0;

	std::vector<cv::Mat> generateFilters(const char* pathToSetOfPics);
	void modifyFilters(cv::Mat image, std::vector<cv::Mat> &filters);
	std::vector<cv::Mat> createFilters(cv::Mat image);
	void writeFilter(cv::Mat filter);

	std::vector<std::vector<double>> generateRatioVectorFromMat(cv::Mat letterImage);
	std::fstream fout;
public:
	NNTrainer();
	~NNTrainer();

	void startGeneratingFilters(const char* pathToSetOfPics);
	std::vector<int> generatingFunctionActivationVector(const std::vector<std::vector<double>>& activationOfFilters, const char* itIsThisLetter, const char* nameOfOutput);
	void createTemplates(std::unordered_map<char, std::vector<int>> templates, const char* letterTested, const std::vector<std::vector<double>>& activationOfFilters, const char* nameOfOutput);
	void initActivationTablesGenerateFile(const char* nameOfFile);
	void initFilterTablesGenerateFile(const char* nameOfFile);
};

