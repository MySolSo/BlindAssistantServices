#pragma once
#include <vector>
#include <unordered_map>
#include <fstream>

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
	std::vector<char> _sablonIdentifyer;

	double rezultatSablon(std::vector<double> Litera, std::vector<int> Sablon);

	std::vector<std::vector<double>> convertMatToRatios(const cv::Mat letterImage);
	double getCurrentPixelRaport(double imagePixel, double filterPixel);
	double calculateFilterRatioAKANeuronActivity(std::vector<double> activities);
	int getIndexOfMaximum(std::vector<double> probabilityes);

	std::vector<std::vector<double>> generateRatioVectorFromMat(cv::Mat letterImage);

public:
	NNLetterRecognition(const char* nameOfTrainingResultFile, const char* nameOfTraininedActivationTables);
	~NNLetterRecognition();

	std::vector<double> NNLetterRecognition::generateFirstLayerOutpuToActivationVector(std::vector<double> letter);
	std::vector<double> generateFirstLayerOutpuToActivationVector(cv::Mat letter);
	char recognizeThisNigga(cv::Mat letter);
};

