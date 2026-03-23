#include "BitCoinExchange.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

static void	dateCheck(std::stringstream& date);
static void dateAccept(std::stringstream &stream, char c);
static void	db_error(std::ifstream &db, const std::string& msg,
		int line_nr);
static bool	isLeapYear(int year);

bool	BitCoinExchange::initDatabase()
{
	std::ifstream	database;
	std::string		line;
	int				line_nr;

	// TODO : we might want to factor db name out of the class
	database.open(_database_name.c_str());
	if (database.fail())
	{
		std::cerr << "BCE : Missing or invalid database!" << std::endl;
		return (false);
	}
	line_nr = 1;
	std::getline(database, line);
	if (line != "date,exchange_rate")
		return (
			db_error(database, "Unexpected beginning of database", line_nr),
			false
		);
	// TODO : refactor while into readEntireFile or similar (also used later)
	while (std::getline(database, line))
	{
		line_nr++;
		std::stringstream	inner(line);
		double			 	price;
		try
        {
			dateCheck(inner);
			dateAccept(inner, ',');
		}
		catch (InvalidDateException &e) {
			return (
				db_error(database, "Invalid date format in database!", line_nr),
				false
			);
		}

		inner >> price;
		if (price < 0)
			return (
				db_error(database, "Database prices cannot be negative", line_nr),
				false
			);

		// This is because dates can be valid but different sizes
		// 2002-9-3 instead of 2002-09-03
		int comma_pos = line.find(",");
		line = line.substr(comma_pos);
		_hash.insert(std::pair<std::string, int>(line, price));
	}
	database.close();
	return (true);
}

static bool	isLeapYear(int year)
{
	return ((year % 4 == 0)
			&& (!(year % 100 == 0) || (year % 400 == 0)));
}

static void dateAccept(std::stringstream &stream, char c)
{
	char	i;
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

static void	db_error(std::ifstream &db, const std::string& msg,
		int line_nr)
{
		db.close();
		std::cerr << "BCE :" << msg << " line " << line_nr << std::endl;
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
	_hash(other._hash),
	_input_name(other._input_name),
	 _database_name(other._database_name)
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
