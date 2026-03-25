#include "RPN.hpp"
#include <sstream>
#include <cctype>

static void	printError(int index, const char* message, std::string& symbol)
{
	std::cerr << "RPN Error at position " << index
		<< " [ " << symbol <<  " ]: " << message << std::endl;
}

static void stackCheckPop2(std::stack<int>& stack, int& a, int& b)
{
	if (stack.size() < 2)
		throw ReversePolish::StackEmptyException();
	a = stack.top();
	stack.pop();
	b = stack.top();
	stack.pop();
}

static void	stackAdd(std::stack<int>& stack)
{
	int a, b;
	stackCheckPop2(stack, a, b);
	stack.push(a + b);
}

static void	stackMult(std::stack<int>& stack)
{
	int a, b;
	stackCheckPop2(stack, a, b);
	stack.push(a * b);
}

static void	stackSub(std::stack<int>& stack)
{
	int a, b;
	stackCheckPop2(stack, a, b);
	stack.push(b - a);
}

static void	stackDiv(std::stack<int>& stack)
{
	int a, b;
	stackCheckPop2(stack, a, b);
	stack.push(b / a);
}

bool	ReversePolish::processString(const std::string& input)
{
	// operator>> on stringstream takes care of ws for us
	std::stringstream	ss(input);
	std::string			symbol;
	int					index = 1;
	char				symbol_val;


	// we will do the error management in this function
	while (!ss.eof())
	{
		index++;
		ss >> symbol;
		if (symbol.size() != 1)
		{
			printError(index, "Invalid syntax", symbol);
			return (false);
		}
		symbol_val = symbol[0];
		try {
			switch (symbol_val)
			{
				case '+':
					stackAdd(_stack);
					break ;
				case '-':
					stackSub(_stack);
					break ;
				case '*':
					stackMult(_stack);
					break ;
				case '/':
					stackDiv(_stack);
					break ;
				default :
					if (!std::isdigit(symbol_val))
					{
						printError(index, "Invalid token", symbol);
						return (false);
					}
					_stack.push(symbol_val - '0');
			}
		}
		catch (StackEmptyException& e) {
			printError(index, "Unexpected empty stack", symbol);
			return (false);
		}
	}
	if (_stack.size() != 1)
	{
		std::cerr << "RPN Error : stack != 1 elem at EOF" << std::endl;
		return (false);
	}
	std::cout << "Result : " << _stack.top() << std::endl;
	return (true);
}

// ----------BEGIN ORTHODOX CANONICAL FORM----------
ReversePolish::ReversePolish(): _stack() {}

ReversePolish::~ReversePolish() {}

ReversePolish::ReversePolish(const ReversePolish& other): _stack(other._stack) {}

ReversePolish	ReversePolish::operator=(const ReversePolish &other)
{
	if (this != &other)
	{
		this->_stack = other._stack;
	}
	return (*this);
}
// ----------END ORTHODOX CANONICAL FORM----------
