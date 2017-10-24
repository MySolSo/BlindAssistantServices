

#pragma once
#include <stdint.h>
#include <vector>


namespace BPN
{
    enum class ActivationFunctionType
    {
        Sigmoid
    };


    class Network
    {
        friend class NetworkTrainer;

        inline static double SigmoidActivationFunction( double x )
        {
            return 1.0 / ( 1.0 + std::exp( -x ) );
        }

        inline static int getResultedOutput (double x )
        {
            if ( x < 0.1 ) return 0;
            else if ( x > 0.9 ) return 1;
            else return -1;
        }

    public:

        struct Settings
        {
            unsigned int                        m_numInputs;
            unsigned int                        m_numHidden;
            unsigned int                        m_numOutputs;
        };

    public:

        Network( Settings const& settings );
        Network( Settings const& settings, std::vector<double> const& weights );

        std::vector<int> const& Evaluate( std::vector<double> const& input );

        std::vector<double> const& GetInputHiddenWeights() const { return m_weightsInputHidden; }
        std::vector<double> const& GetHiddenOutputWeights() const { return m_weightsHiddenOutput; }

    private:

        void InitializeNetwork();
        void InitializeWeights();
        void LoadWeights( std::vector<double> const& weights );

        int GetInputHiddenWeightIndex( int inputIdx, int hiddenIdx ) const { return inputIdx * m_numHidden + hiddenIdx; }
        int GetHiddenOutputWeightIndex( int hiddenIdx, int outputIdx ) const { return hiddenIdx * m_numOutputs + outputIdx; }

    private:

        int                 m_numInputs;
        int                 m_numHidden;
        int                 m_numOutputs;

        std::vector<double>     m_inputNeurons;
        std::vector<double>     m_hiddenNeurons;
        std::vector<double>     m_outputNeurons;

        std::vector<int>    m_result;

        std::vector<double>     m_weightsInputHidden;
        std::vector<double>     m_weightsHiddenOutput;
    };
}