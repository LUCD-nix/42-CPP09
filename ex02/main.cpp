#include "PmergeMe.hpp"
#include <iostream>

int	main(int argc, char* argv[])
{
	if (!PmergeMe::vecIngestInput(argc, argv))
	{
		return (1);
	}
	PmergeMe::sortVector();
	PmergeMe::printVector();
	std::cout << "Success!";
}

