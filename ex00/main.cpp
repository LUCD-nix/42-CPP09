#include <iostream>
#include "BitCoinExchange.hpp"

int	main(void)
{
	BitCoinExchange bce("input.txt", "data.csv");
	bce.initDatabase();
	bce.processInput();
	std::cout << "Success!";
}

