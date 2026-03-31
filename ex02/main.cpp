#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <sys/time.h>

int	main(int argc, char* argv[])
{
	timeval start, end;
	double elapsed;

	// INGEST VECTOR
	gettimeofday(&start, NULL);
	if (!PmergeMe::vecIngestInput(argc, argv))
	{
		return (1);
	}
	gettimeofday(&end, NULL);
	elapsed = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)
												 / 1e6;
	std::cout << std::setprecision(6)
		<< "Time to ingest data into vector : " << elapsed << "s" << std::endl;

	// SORT VECTOR
	gettimeofday(&start, NULL);
	if (!PmergeMe::sortVector())
	{
		std::cerr << "Error, vec not sorted" << std::endl;
		return (1);
	}
	gettimeofday(&end, NULL);
	elapsed = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)
												 / 1e6;
	std::cout << std::setprecision(6)
		<< "Time to sort vector :" << elapsed << "s" << std::endl;

	PmergeMe::printVector();
	std::cout << "Success!" << std::endl;
}
