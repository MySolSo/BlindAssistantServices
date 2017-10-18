#pragma once
#include <vector>
#include <unordered_map>

namespace cv {
	class Mat;
}

class NNLetterRecognition
{
private:
	int _filterX, _filterY;
	int _letterX, _letterY;
	int _stepSize;
	std::vector<double**> _filters;
	std::vector<std::vector<int>> _sablons;

	std::vector<std::vector<double>> convertMatToRatios(const cv::Mat letterImage);
	double getCurrentPixelRaport(double imagePixel, double filterPixel);
	double calculateFilterRatioAKANeuronActivity(std::vector<double> activities);
	double rezultatSablon(std::vector<double> Litera, std::vector<int> Sablon);

	std::vector<cv::Mat> generateFilters();
	void modifyFilters(cv::Mat image, std::vector<cv::Mat> &filters);
	std::vector<cv::Mat> createFilters(cv::Mat image);
	void writeFilter(cv::Mat filter);
public:
	NNLetterRecognition(const char* nameOfTrainingResultFile);
	~NNLetterRecognition();

	std::vector<double> generateFirstLayerOutpuToActivationVector(const cv::Mat letter);
	void startGeneratingFilters();
	std::vector<int> generatingFunctionActivationVector(const std::vector<std::vector<double>>& activationOfFilters);
	void createTemplates(std::unordered_map<char, std::vector<int>> templates, const char letterTested, const std::vector<std::vector<double>>& activationOfFilters);
};

