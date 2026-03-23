#include "BitCoinExchange.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

static int	dateCheck(std::stringstream& date);
static void dateAccept(std::stringstream &stream, char c);
static void	db_error(std::ifstream &db, const std::string& msg,
		int line_nr);
static void	input_error(std::string& line, const std::string& msg,
		int line_nr);
static bool	isLeapYear(int year);
static void	printHoldings(std::map<int, float>& map, int date, float btc);

void	BitCoinExchange::processInput()
{
	std::ifstream	input;
	std::string		line;
	int				line_nr;

	// expected format :
	// YYYY-MM-DD | value
	input.open(_input_name.c_str());
	if (input.fail())
	{
		std::cerr << "BCE : Missing or invalid input!" << std::endl;
		return ;
	}
	line_nr = 1;
	std::getline(input, line);
	if (line != "date | value")
	{
		std::cerr << "Error : Invalid first line in input" << std::endl;
		input.close();
		return ;
	}
	while (std::getline(input, line))
	{
		line_nr++;
		std::stringstream	inner(line);
		int					converted_date;
		double				amount;
		try
        {
			converted_date = dateCheck(inner);
			// disgusTANG
			dateAccept(inner, ' ');
			dateAccept(inner, '|');
			dateAccept(inner, ' ');
		}
		catch (InvalidDateException &e)
		{
			input_error(line, "Invalid input date", line_nr);
			continue ;
		}
		inner >> amount;
		if (!inner.eof() || inner.fail())
		{
			input_error(line, "Invalid input numeric", line_nr);
			continue ;
		}
		if (amount > 1000.f)
		{
			input_error(line, "Input value too large!", line_nr);
			continue ;
		}
		if (amount < 0.f)
		{
			input_error(line, "Input value can't be negative", line_nr);
			continue ;
		}
		// TODO : for now, printHoldings checks whether date is in DB,
		// might want to change that behaviour
		printHoldings(this->_hash, converted_date, amount);
	}
	input.close();
	return ;
}

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
	// NOTE : we could refactor while into readEntireFile or similar
	// (also used in input)
	while (std::getline(database, line))
	{
		line_nr++;
		std::stringstream	inner(line);
		int					converted_date;
		double			 	price;
		try
        {
			converted_date = dateCheck(inner);
			dateAccept(inner, ',');
		}
		catch (InvalidDateException &e) {
			return (
				db_error(database, "Invalid date format in database!", line_nr),
				false
			);
		}

		inner >> price;
		if (price < 0 || inner.fail())
			return (
				db_error(database, "Invalid database price", line_nr),
				false
			);
		_hash.insert(std::pair<int, float>(converted_date, price));
	}
	database.close();
	return (true);
}

// HELPER FUNCTIONS

static bool	isLeapYear(int year)
{
	return ((year % 4 == 0)
			&& (!(year % 100 == 0) || (year % 400 == 0)));
}

static void dateAccept(std::stringstream &stream, char c)
{
	char	i;
	if (stream.peek() == c)
		stream.read(&i, 1);
	else
		throw BitCoinExchange::InvalidDateException();
}

static int	dateCheck(std::stringstream& date)
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
	return (day + month * 100 + year * 100 * 100);
}

static void	printHoldings(std::map<int, float>& map, int date, float btc)
{
	std::map<int, float>::iterator it = map.lower_bound(date);
	if (it == map.end()
		|| (it == map.begin() && it->first != date))
	{
		std::cerr << "Error : Input date not in database" << std::endl;
		return ;
	}

	if (it->first != date)
		--it;
	int day = date % 100;
	int month = (date % 10000 - day) / 100;
	int year = (date - day - month) / 10000;
	
	std::cout << year << "-"
		<< month << "-"
		<< day << " => "
		<< btc << " = "
		<< btc * it->second << std::endl;
}

static void	input_error(std::string &line, const std::string& msg,
		int line_nr)
{
		std::cerr << "Error, line " << line_nr << ": " << msg << std::endl
			<< "[\"" << line << "\"]"
			<< std::endl;
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
