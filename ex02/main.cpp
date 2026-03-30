#include "PmergeMe.hpp"
#include <iostream>

int	main(int argc, char* argv[])
{
	if (!PmergeMe::vecIngestInput(argc, argv))
	{
		return (1);
	}
	if (!PmergeMe::sortVector())
	{
		std::cerr << "Error, vec not sorted" << std::endl;
		return (1);
	}
	PmergeMe::printVector();
	std::cout << "Success!" << std::endl;
}
