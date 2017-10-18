#pragma once
#include <vector>

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
public:
	NNLetterRecognition(const char* nameOfTrainingResultFile);
	~NNLetterRecognition();

	std::vector<double> generateFirstLayerOutpuToActivationVector(const cv::Mat letter);
};

