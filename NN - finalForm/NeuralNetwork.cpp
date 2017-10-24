
#include "NeuralNetwork.h"
#include <assert.h>
#include <stdlib.h>
#include <ctime>
#include <random>
#include <iostream>

namespace BPN
{
    Network::Network( Settings const& settings )
        : m_numInputs( settings.m_numInputs )
        , m_numHidden( settings.m_numHidden )
        , m_numOutputs( settings.m_numOutputs )
    {
        assert( settings.m_numInputs > 0 && settings.m_numOutputs > 0 && settings.m_numHidden > 0 );
        InitializeNetwork();
        InitializeWeights();
    }

    Network::Network( Settings const& settings, std::vector<double> const& weights )
        : m_numInputs( settings.m_numInputs )
        , m_numHidden( settings.m_numHidden )
        , m_numOutputs( settings.m_numOutputs )
    {
        assert( settings.m_numInputs > 0 && settings.m_numOutputs > 0 && settings.m_numHidden > 0 );
        InitializeNetwork();
        LoadWeights( weights );
    }

    void Network::InitializeNetwork()
    {
        // Create storage and initialize the neurons and the outputs
        //-------------------------------------------------------------------------

        // Add bias neurons or maybe not
		int const totalNumInputs = m_numInputs;// +1;
		int const totalNumHiddens = m_numHidden;// +1;

        m_inputNeurons.resize( totalNumInputs );
        m_hiddenNeurons.resize( totalNumHiddens );
        m_outputNeurons.resize( m_numOutputs );
        m_result.resize( m_numOutputs );

        memset( m_inputNeurons.data(), 0, m_inputNeurons.size() * sizeof( double ) );
        memset( m_hiddenNeurons.data(), 0, m_hiddenNeurons.size() * sizeof( double ) );
        memset( m_outputNeurons.data(), 0, m_outputNeurons.size() * sizeof( double ) );
        memset( m_result.data(), 0, m_result.size() * sizeof( int ) );

        // Set bias values or maybe let them be
        //m_inputNeurons.back() = -1.0;
        //m_hiddenNeurons.back() = -1.0;

        // Create storage and initialize and layer weights

		int const numInputHiddenWeights = totalNumInputs * totalNumHiddens;
        int const numHiddenOutputWeights = totalNumHiddens * m_numOutputs;
        m_weightsInputHidden.resize( numInputHiddenWeights );
        m_weightsHiddenOutput.resize( numHiddenOutputWeights );
    }

    void Network::InitializeWeights()
    {
        std::random_device rd;
        std::mt19937 generator( rd() );

        double const distributionRangeHalfWidth = ( 2.4 / m_numInputs );
        double const standardDeviation = distributionRangeHalfWidth * 2 / 6;
        std::normal_distribution<> normalDistribution( 0, standardDeviation );

        // Set weights to normally distributed random values between [-2.4 / numInputs, 2.4 / numInputs]
        for ( int inputIdx = 0; inputIdx < m_numInputs; inputIdx++ )
        {
            for ( int hiddenIdx = 0; hiddenIdx < m_numHidden; hiddenIdx++ )
            {
                int const weightIdx = GetInputHiddenWeightIndex( inputIdx, hiddenIdx );
                double const weight = normalDistribution( generator );
                m_weightsInputHidden[weightIdx] = weight;
            }
        }

        // Set weights to normally distributed random values between [-2.4 / numInputs, 2.4 / numInputs]
        for ( int hiddenIdx = 0; hiddenIdx < m_numHidden; hiddenIdx++ )
        {
            for ( int outputIdx = 0; outputIdx < m_numOutputs; outputIdx++ )
            {
                int const weightIdx = GetHiddenOutputWeightIndex( hiddenIdx, outputIdx );
                double const weight = normalDistribution( generator );
                m_weightsHiddenOutput[weightIdx] = weight;
            }
        }
    }

    void Network::LoadWeights( std::vector<double> const& weights )
    {
        int const numInputHiddenWeights = (m_numInputs) * (m_numHidden);
        int const numHiddenOutputWeights = (m_numHidden) * (m_numOutputs);
        assert( weights.size() == numInputHiddenWeights + numHiddenOutputWeights + 1);

        int weightIdx = 0;
        for ( auto InputHiddenIdx = 0; InputHiddenIdx < numInputHiddenWeights; InputHiddenIdx++ )
        {
            m_weightsInputHidden[InputHiddenIdx] = weights[weightIdx];
            weightIdx++;
        }

        for ( auto HiddenOutputIdx = 0; HiddenOutputIdx < numHiddenOutputWeights; HiddenOutputIdx++ )
        {
			if (weightIdx >= ((numInputHiddenWeights)+numHiddenOutputWeights))
				break;
            m_weightsHiddenOutput[HiddenOutputIdx] = weights[weightIdx];
            weightIdx++;
        }

		std::cout << "==============================================================" << std::endl
			<< "WEIGHTS LOADED" << std::endl << "==============================================================" << std::endl;
    }

    std::vector<int> const& Network::Evaluate( std::vector<double> const& input )
    {
        assert( input.size() == m_numInputs );
//        assert( m_inputNeurons.back() == -1.0 && m_hiddenNeurons.back() == -1.0 );

        // Set input values
        //-------------------------------------------------------------------------

        memcpy( m_inputNeurons.data(), input.data(), input.size() * sizeof( double ) );

        // Update hidden neurons
        //-------------------------------------------------------------------------

        for ( int hiddenIdx = 0; hiddenIdx < m_numHidden; hiddenIdx++ )
        {
            m_hiddenNeurons[hiddenIdx] = 0;

            // Get weighted sum
            for ( int inputIdx = 0; inputIdx < m_numInputs; inputIdx++ )
            {
                int const weightIdx = GetInputHiddenWeightIndex( inputIdx, hiddenIdx );
                m_hiddenNeurons[hiddenIdx] += m_inputNeurons[inputIdx] * m_weightsInputHidden[weightIdx];
            }

            // Apply activation function
            m_hiddenNeurons[hiddenIdx] = SigmoidActivationFunction( m_hiddenNeurons[hiddenIdx] );
        }

        // Calculate output values

        for ( int outputIdx = 0; outputIdx < m_numOutputs; outputIdx++ )
        {
            m_outputNeurons[outputIdx] = 0;

            // Get weighted sum
            for ( int hiddenIdx = 0; hiddenIdx < m_numHidden; hiddenIdx++ )
            {
                int const weightIdx = GetHiddenOutputWeightIndex( hiddenIdx, outputIdx );
                m_outputNeurons[outputIdx] += m_hiddenNeurons[hiddenIdx] * m_weightsHiddenOutput[weightIdx];
            }

            // Apply activation function and return result
            m_outputNeurons[outputIdx] = SigmoidActivationFunction( m_outputNeurons[outputIdx] );
            m_result[outputIdx] = getResultedOutput( m_outputNeurons[outputIdx] );
        }

        return m_result;
    }
}