#include "stdafx.h"
#include "TxtParser.h"
#include <fstream>
#include <iostream>

using namespace Willem;

void TxtParser::Parse(const std::string& path, std::vector < Willem::Vector2 >& positions)
{
	std::ifstream input;

	std::string line;

	input.open(path, std::ios::in | std::ios::binary);

	if (input.is_open())
	{
		while (std::getline(input, line, '\r'))
		{
			if (line.find('[') != std::string::npos)
			{
				std::string lineOnlyNumbers = line;

				int charsToSkipAtTheStart = 1;

				if (line[0] == '\n')
					charsToSkipAtTheStart++;
				
				lineOnlyNumbers.erase(lineOnlyNumbers.begin(), lineOnlyNumbers.begin() + charsToSkipAtTheStart);

				std::string FirstNumber = lineOnlyNumbers;
				size_t NextSpaceIndex = FirstNumber.find_first_of(',');
				FirstNumber.erase(FirstNumber.begin() + NextSpaceIndex, FirstNumber.end());

				std::string SecondNumber = lineOnlyNumbers;
				NextSpaceIndex = SecondNumber.find_first_of(',');
				SecondNumber.erase(SecondNumber.begin(), SecondNumber.begin() + NextSpaceIndex + 1);
				NextSpaceIndex = SecondNumber.find_first_of(']');
				SecondNumber.erase(SecondNumber.begin() + NextSpaceIndex, SecondNumber.end());

				Vector2 newPos = { std::stof(FirstNumber), std::stof(SecondNumber)};
				positions.push_back(newPos);

			}
		}
		input.close();
	}
}