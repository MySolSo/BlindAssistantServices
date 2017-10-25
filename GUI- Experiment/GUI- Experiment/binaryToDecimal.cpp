#include <iostream>
#include <vector>
#include <conio.h>



int binaryToLetterIndex(const std::vector<int> binaryIndex)
{

	const int maxPower = binaryIndex.size() - 1;
	unsigned int currentPower;
	unsigned int indexToBeReturned = 0;
	for (auto i = 0; i <= maxPower; ++i)
	{
		if (binaryIndex[i] == 1)
		{
			currentPower = maxPower - i;
			indexToBeReturned += pow(2, currentPower);
		}
	}

	return indexToBeReturned;
}