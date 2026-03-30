#ifndef PMERGEME_HPP
# define PMERGEME_HPP
# include <deque>
# include <vector>

class PmergeMe
{
private :
	static std::deque<int>		deque;
	static std::vector<int>		vec;
	// <OCF>
	PmergeMe();
	~PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe	operator=(const PmergeMe &other);
	// </OCF>
public :
	class	StackEmptyException : public std::exception {
		const char* what() const throw() { return ("Stack empty!"); };
	};
	static bool	vecIngestInput(int argc, char *argv[]);
	static bool	dequeIngestInput(int argc, char *argv[]);
	static void	printVector();
	static void	printDeque();
	static void	sortVector();
	static void	sortDeque();

private :
	static void	appendToDeque(int a);
	static void	appendToVec(int a);
};

#endif // PMERGEME_HPP
