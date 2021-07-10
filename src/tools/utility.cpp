#include <iostream>
#include <glm/mat4x4.hpp>

void printMatrix(glm::mat4 &MVP)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << MVP[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}