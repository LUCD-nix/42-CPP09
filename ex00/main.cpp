#include <iostream>

static bool	isLeapYear(int year)
{
	return ((year % 4 == 0)
			&& (!(year % 100 == 0) || (year % 400 == 0)));
}
int	main(void)
{
	std::cout << "testing 1700 is leap? " << isLeapYear(1700) << std::endl;
	std::cout << "testing 1600 is leap? " << isLeapYear(1600) << std::endl;
	std::cout << "testing 1604 is leap? " << isLeapYear(1604) << std::endl;
}

