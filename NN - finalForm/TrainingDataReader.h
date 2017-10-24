
#pragma once

#include "NeuralNetworkTrainer.h"
#include <string>


namespace BPN
{
    class TrainingDataReader
    {
    public:

        TrainingDataReader( std::string const& filename, int numInputs, int numOutputs );

        bool ReadData(const int typeOfDataReader);

        inline int GetNumInputs() const { return m_numInputs; }
        inline int GetNumOutputs() const { return m_numOutputs; }

        inline int GetNumTrainingSets() const { return 0; }
        TrainingData const& GetTrainingData() const { return m_data; }

    private:

        void CreateTrainingData();

    private:

        std::string                     m_filename;
        int                         m_numInputs;
        int                         m_numOutputs;

        std::vector<TrainingEntry>      m_entries;
        TrainingData                    m_data;
    };
}