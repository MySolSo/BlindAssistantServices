
#include "TrainingDataReader.h"
#include <assert.h>
#include <iosfwd>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <opencv2/videoio.hpp>
#include "imageDataAdapter.h"

namespace BPN
{
	TrainingDataReader::TrainingDataReader(std::string const& filename, int numInputs, int numOutputs)
		: m_filename(filename)
		, m_numInputs(numInputs)
		, m_numOutputs(numOutputs)
	{
		assert(!filename.empty() && m_numInputs > 0 && m_numOutputs > 0);
	}

	bool TrainingDataReader::ReadData(const int typeOfDataReader)
	{
		assert(!m_filename.empty());
		std::unordered_map<char, std::vector<int>> teacherValues = { { 'A', {0,0,0,0,1} },{ 'B', {0,0,0,1,0} },{ 'C', {0,0,0,1,1} },{'D', {0,0,1,0,0 } },{ 'E', {0,0,1,0,1} },{'F', {0,0,1,1,0} }, { 'G', {0,0,1,1,1} }, { 'H', {0,1,0,0,0 }}, { 'I', {0,1,0,0,1} },
																									  {'J',{0,1,0,1,0}},{'K',{0,1,0,1,1}},{'L',{0,1,1,0,0}},{'M',{0,1,1,0,1}},{'N',{0,1,1,1,0}},{'O',{0,1,1,1,1}},{'P',{1,0,0,0,0}},{'Q',{1,0,0,0,1}},{'R',{1,0,0,1,0}},{'S',{1,0,0,1,1}},{'T',{1,0,1,0,0}},{'U',{1,0,1,0,1}},{'V',{1,0,1,1,0}},{'W',{1,0,1,1,1}},
																									  {'X',{1,1,0,0,0}},{'Y',{1,1,0,0,1}},{'Z',{1,1,0,1,0}} };
		const int inputNeuronsNumber = 16;
		if (typeOfDataReader == 1)
		{

			std::fstream inputFile;
			std::vector<double> *Valori_Litera;
			char Litera;
			TrainingEntry* entry;
			inputFile.open(m_filename, std::ios::in);
			

			if (inputFile.is_open())
			{
				std::string line;

				//READING DATA

				while (!inputFile.eof() && inputFile >> Litera)
				{
					/*m_entries.push_back(TrainingEntry());
					TrainingEntry& entry = m_entries.back();*/
					entry = new TrainingEntry();


					int  auxiliar;
					char Virgula;

					Valori_Litera = new std::vector<double>(inputNeuronsNumber);

					for (int i = 0; i < 16; i++)
					{
						inputFile >> Virgula;
						inputFile >> auxiliar;
						(*Valori_Litera)[i] = (auxiliar);
					}
					if (true/*Litera == 'A' || Litera == 'B' || Litera == 'C' || Litera == 'D' || Litera == 'E'*/)
					{

						entry->m_inputs = *Valori_Litera;
						//Aici introduci tu codu 

					//	Valori_Litera.erase(Valori_Litera->begin(), Valori_Litera->end());
						//inputFile.close();

						entry->m_expectedOutputs = teacherValues[Litera];
						m_entries.push_back(*entry);
					}
					//Valori_Litera->erase(Valori_Litera->begin(), Valori_Litera->end());
				}
			}
		}
		else if (typeOfDataReader == 0)
		{
			const int totalNumberOfDataPerLetter = 199;
			std::string path = "./training/upper/";
			std::string extension = ".gif";
			TrainingEntry* entry;
			std::vector<double> *Valori_Litera;
			imageDataAdapter imageAdapter;
			
			for (auto letter : teacherValues)
			{
				for (auto i = 0; i <= totalNumberOfDataPerLetter; ++i)
				{
					std::string pathToTheLetter((path + letter.first + "/" + std::to_string(i) + extension).c_str());
					cv::VideoCapture images(pathToTheLetter);
					cv::Mat image;
					if (images.isOpened())
					{
						images.read(image);
						entry = new TrainingEntry();
						Valori_Litera = new std::vector<double>(inputNeuronsNumber);
						*Valori_Litera = imageAdapter.adaptToHeatMap(image);

						entry->m_inputs = *Valori_Litera;
						entry->m_expectedOutputs = teacherValues[letter.first];

						m_entries.push_back(*entry);
					}
				}

			}
		}
		if (!m_entries.empty())
		{
			CreateTrainingData();
			std::cout << "Input file: " << m_filename << "\nRead complete: " << m_entries.size() << " inputs loaded" << std::endl;
			return true;
		}
		else
		{
			std::cout << "Error Opening Input File: " << m_filename << std::endl;
			return false;
		}
	}

	void TrainingDataReader::CreateTrainingData()
	{
		assert(!m_entries.empty());

		std::random_shuffle(m_entries.begin(), m_entries.end());

		// Training set is set to 60% out of all data
		int const numEntries = (int)m_entries.size();
		int const numTrainingEntries = (int)(0.6 * numEntries);
		int const numGeneralizationEntries = (int)(ceil(0.2* numEntries));

		int entryIdx = 0;
		for (; entryIdx < numTrainingEntries; entryIdx++)
		{
			m_data.m_trainingSet.push_back(m_entries[entryIdx]);
		}

		// Generalization set is set to 20% out of all data
		
		for (; entryIdx < numTrainingEntries + numGeneralizationEntries; entryIdx++)
		{
			m_data.m_generalizationSet.push_back(m_entries[entryIdx]);
		}
		
		// Validation set is set to 20% out of all data
		for (; entryIdx < numEntries; entryIdx++)
		{
			m_data.m_validationSet.push_back(m_entries[entryIdx]);
		}
	}
}