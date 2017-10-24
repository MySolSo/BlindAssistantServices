

#include "NeuralNetwork/neuralNetworkTrainer.h"
#include "NeuralNetwork/TrainingDataReader.h"
#include <iostream>
#include <conio.h>
#include "imageDataAdapter.h"

std::vector<double> aquireWeightData(const std::string& fileName)
{
	std::vector<double> weights;
	std::ifstream weightData(fileName);

	double currentValue;
	if (weightData.is_open())
	{
		int inputWeightsNumber, hiddenWeightNumber;
		weightData >> inputWeightsNumber;
		weightData >> hiddenWeightNumber;
		while (!weightData.eof())
		{
			weightData >> currentValue;
			if (currentValue != 73577357)
			{
				weights.push_back(currentValue);

			}
		}
	}
	return weights;
}
int main()
{
	std::string trainingDataPath, weightTrainingDataResultsPath;
	weightTrainingDataResultsPath = "NN_WEIGHTS_INPUT_HIDDEN_OUTPUT.txt";
	trainingDataPath = "letterDataset.txt";


	//Adapts data to neural network dataset
	imageDataAdapter dataAdapter;


	

	int numInputs = 25;
	//numOutputs -> binary representation of the letterIndex
	int numOutputs = 5;
	int numHidden = 50;

	/////////PHOTOS dataReader(0) / 16-ATR dataReader(1)
	BPN::TrainingDataReader dataReader(trainingDataPath, numInputs, numOutputs);
	if (!dataReader.ReadData(0))
	{
		return 1;
	}

	//Creating the neural network
	BPN::Network::Settings networkSettings{ numInputs, numHidden, numOutputs };
	BPN::Network nn(networkSettings/*, aquireWeightData(weightTrainingDataResultsPath)*/); //While training, comment the aquireWeightData argument

	//// Create the neural network trainer
	BPN::NetworkTrainer::Settings trainerSettings;
	trainerSettings.m_learningRate = 0.04;
	trainerSettings.m_momentum = 0.015;
	trainerSettings.m_maxEpochs = 10000;
	trainerSettings.m_desiredAccuracy = 90;

	//How to train your dragon
	BPN::NetworkTrainer trainer(trainerSettings, &nn);

	//Training this annoying thing
	trainer.Train(dataReader.GetTrainingData());


	//////////////////TEST PURPOSES
	std::vector<double> letterA1 = { 8,15,7,8,4,8,2,3,2,8,5,12,5,4,5,5 };
	std::vector<double> letterA2 = { 3,6,5,4,1,7,5,3,1,6,1,8,2,7,2,7 };
	std::vector<double> letterB1 = { 4,6,6,4,5,9,8,3,6,7,6,8,6,8,6,9 };
	std::vector<double> letterB2 = { 5,9,7,7,5,8,8,4,7,10,5,5,2,8,6,10 };
	std::vector<double> letterL1 = { 3,6,5,4,5,8,8,3,5,5,7,9,6,11,6,5 };
	std::vector<double> letterL2 = { 10,15,8,8,4,9,2,4,5,12,4,13,2,7,6,8 };
	std::vector<double> letterD1 = { 4,11,6,8,7,8,7,5,7,7,6,6,6,8,3,7 };
	std::vector<double> letterD2 = { 5,5,5,7,3,6,7,10,10,7,7,6,3,8,4,8 };

	std::vector<std::vector<double>> solutions;
	solutions.push_back(letterA1);
	solutions.push_back(letterA2);
	solutions.push_back(letterB1);
	solutions.push_back(letterB2);
	solutions.push_back(letterL1);
	solutions.push_back(letterL2);
	solutions.push_back(letterD1);
	solutions.push_back(letterD2);


	auto image = cv::imread("blackA.jpg", CV_LOAD_IMAGE_COLOR);
	//std::vector<int> output = nn.Evaluate(letterA);
	/*for (auto letter : solutions)
	{
		std::vector<int> output = nn.Evaluate(letter);
		for (auto value : output)
		{
			std::cout << value << "  ";
		}
		std::cout << std::endl;
	}*/
	/////////////////////////
	std::vector<int> output = nn.Evaluate(dataAdapter.adaptImageToDataset(image));
	for (auto value : output)
	{
		std::cout << value << "   ";
	}

	_getch();
	return 0;
}
