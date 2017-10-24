
#pragma once

#include "NeuralNetwork.h"
#include <fstream>

namespace BPN
{
    struct TrainingEntry
    {
        std::vector<double>         m_inputs;
        std::vector<int>        m_expectedOutputs;
    };

    typedef std::vector<TrainingEntry> TrainingSet;

    struct TrainingData
    {
        TrainingSet m_trainingSet;
        TrainingSet m_generalizationSet;
        TrainingSet m_validationSet;
    };


    class NetworkTrainer
    {
    public:

        struct Settings
        {
            double      m_learningRate = 0.001;
            double      m_momentum = 0.9;

            unsigned int    m_maxEpochs = 150;
            double      m_desiredAccuracy = 90;
        };

    public:

        NetworkTrainer( Settings const& settings, Network* pNetwork );

        void Train( TrainingData const& trainingData );

    private:

        inline double GetOutputErrorGradient( double desiredValue, double outputValue ) const { return outputValue * ( 1.0 - outputValue ) * ( desiredValue - outputValue ); }
        double GetHiddenErrorGradient( int hiddenIdx ) const;

        void RunEpoch( TrainingSet const& trainingSet );
        void Backpropagate( std::vector<int> const& expectedOutputs );
        void UpdateWeights();
		void writeWeightsToFile();

        void GetSetAccuracyAndMSE( TrainingSet const& trainingSet, double& accuracy, double& mse ) const;

    private:
        
        Network*                    m_pNetwork;                 // Network to train

        // Training settings
        double                      m_learningRate;             // Adjusts the step size of the weight update
        double                      m_momentum;                 // Improves performance of stochastic learning
        double                      m_desiredAccuracy;          // Target accuracy for training
        unsigned int                    m_maxEpochs;                // Max number of training epochs

        // Training data
        std::vector<double>         m_deltaInputHidden;         // Delta for input hidden layer
        std::vector<double>         m_deltaHiddenOutput;        // Delta for hidden output layer
        std::vector<double>         m_errorGradientsHidden;     // Error gradients for the hidden layer
        std::vector<double>         m_errorGradientsOutput;     // Error gradients for the outputs

        unsigned int                    m_currentEpoch;             // Epoch counter
        double                      m_trainingSetAccuracy;
        double                      m_validationSetAccuracy;
        double                      m_generalizationSetAccuracy;
        double                      m_trainingSetMSE;
        double                      m_validationSetMSE;
        double                      m_generalizationSetMSE;
    };
}