#include "Engine/common.hpp"


int mainasd(void)
{
	int i = 0;
	double aspectratio = 1.7778;
	glm::dmat4 mat = glm::perspective(45.0, aspectratio, 0.01, 1000.0);
	while (i <100)
	{
		++i; aspectratio += 0.1;
		mat = glm::perspective(45.0, aspectratio, 0.01, 1000.0);
	}


	return 0;
}
