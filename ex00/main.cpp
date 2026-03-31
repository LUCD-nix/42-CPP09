#include <iostream>
#include "BitCoinExchange.hpp"

int	main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error, must provide input file!" << std::endl;
		return (1);
	}
	std::string input_file_name(argv[1]);
	BitCoinExchange	bce(input_file_name, "data.csv");
	if (!bce.initDatabase())
	{
		return (1);
	}
	bce.processInput();
	std::cout << "Success!" << std::endl;
}
