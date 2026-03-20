#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP
# include <map>
// # include <cstddef> // NULL
# include <string>
class BitCoinExchange
{
	private :
		std::map<std::string, int>	_hash;
		const std::string			_input_name;
		const std::string			_database_name;

	public :
		// BEGIN ORTHODOX CANNONICAL FORM
		BitCoinExchange();
		~BitCoinExchange();
		BitCoinExchange(const BitCoinExchange& other);
		BitCoinExchange& operator=(const BitCoinExchange& other);
		// END ORTHODOX CANNONICAL FORM

		class InvalidPriceException : public std::exception {
			const char*	what() const throw(){return ("BCE: Bad Price!");};
		};
		class InvalidDateException : public std::exception {
			const char*	what() const throw(){return ("BCE: Bad Date!");};
		};
		BitCoinExchange(std::string _input_name, std::string _database_name);
		bool	initDatabase();
};
#endif // BITCOINEXCHANGE_HPP
