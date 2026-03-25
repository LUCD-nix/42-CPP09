#include "RPN.hpp"
#include <iostream>

int	main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error, must provide input string" << std::endl;
		return (1);
	}
	std::string input(argv[1]);
	ReversePolish	rpn;
	if (!rpn.processString(input))
		return (1);
	std::cout << "Success!";
}

