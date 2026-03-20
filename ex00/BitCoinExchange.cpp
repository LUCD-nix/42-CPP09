#include "BitCoinExchange.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

static bool	isLeapYear(int year)
{
	return ((year % 4 == 0)
			&& (!(year % 100 == 0) || (year % 400 == 0)));
}

static void dateAccept(std::stringstream &stream, char c)
{
	int	i;
	if (stream.peek() == c)
		stream >> i;
	else
		throw BitCoinExchange::InvalidDateException();
}

static void	dateCheck(std::stringstream& date)
{
	static int valid_months_normal[12] =
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static int valid_months_leap[12] =
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	int		year = 0;
	date >> year;
	// We're gonna allow years above 2022, for now
	// TODO : we could check if it's greater than current day
	if (year < 2009)
		throw BitCoinExchange::InvalidDateException();
	dateAccept(date, '-');

	int		month = 0;
	date >> month;
	if (month < 1 || month > 12)
		throw BitCoinExchange::InvalidDateException();
	dateAccept(date, '-');

	int		day = 0;
	date >> day;
	int*	check;
	if (isLeapYear(year))
		check = valid_months_leap;
	else
		check = valid_months_normal;
	if (day < 1 || day > check[month - 1])
		throw BitCoinExchange::InvalidDateException();
}

bool	BitCoinExchange::initDatabase()
{
	std::ifstream	database;
	std::string		line;

	// TODO : we might want to factor db name out of the class
	database.open(_database_name.c_str());
	if (database.fail())
	{
		std::cerr << "BCE : Missing or invalid database!" << std::endl;
		return (false);
	}
	// TODO : refactor while into readEntireFile or similar (also used later)
	// TODO : skip/process first line
	while (std::getline(database, line))
	{
		std::stringstream	inner(line);
		double			 	price;
		try {
			dateCheck(inner);
			dateAccept(inner, ',');
		}
		catch (InvalidDateException &e) {
			database.close();
			std::cerr << "BCE : Invalid date format in database!" << std::endl;
			return (false);
		}

		inner >> price;
		if (price < 0)
		{
			database.close();
			std::cerr << "BCE : Database price cannot be negative" << std::endl;
			return (false);
		}

		// TODO : this can fail if date is valid but of different size: 
		// 2002-9-3 instead of 2002-09-03, consider fixing
		line = line.substr(10);
		_hash.insert(std::pair(line, price));
	}
	database.close();
	return (true);
}

BitCoinExchange::BitCoinExchange(std::string input, std::string data):
	_hash(), _input_name(input), _database_name(data)
{ }

// ------- BEGIN ORTHODOX CANNONICAL FORM -------
BitCoinExchange::BitCoinExchange()
{ }
BitCoinExchange::~BitCoinExchange()
{ }
BitCoinExchange::BitCoinExchange(const BitCoinExchange& other):
	_hash(other._hash), _database_name(other._database_name),
	_input_name(other._input_name)
{ }

BitCoinExchange& BitCoinExchange::operator=(const BitCoinExchange& other)
{
	if (this != &other)
	{
		this->_hash = other._hash;
	}
	return (*this);
}
// ------- END ORTHODOX CANNONICAL FORM -------
