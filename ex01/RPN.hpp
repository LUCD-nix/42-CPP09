#ifndef RPN_HPP
# define RPN_HPP
# include <stack>
# include <iostream>

class ReversePolish
{
private :
	std::stack<int>	_stack;

public :
	// <OCF>
	ReversePolish();
	~ReversePolish();
	ReversePolish(const ReversePolish& other);
	ReversePolish	operator=(const ReversePolish &other);
	// </OCF>
	class	StackEmptyException : public std::exception {
		const char* what() const throw() { return ("Stack empty!"); };
	};
	bool	processString(const std::string& input);
};
std::ostream&	operator<<(std::ostream &out, const ReversePolish& span);
#endif // RPN_HPP
