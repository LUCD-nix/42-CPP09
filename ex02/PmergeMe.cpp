#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> PmergeMe::vec;
std::deque<int> PmergeMe::deque;

static void	printError(int index, const char* message, char* symbol)
{
	std::cerr << "PmergeMe: Error at position " << index
	<< " [ " << symbol <<  " ]: " << message << std::endl;
}
static int jacobsthal[] = {0, 1, 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203, 5592405, 11184811, 22369621, 44739243, 89478485, 178956971, 357913941, 715827883, 1431655765 };

// Adapted from https://en.cppreference.com/w/cpp/algorithm/lower_bound.html
template<typename It>
It stepBinarySearch(It beg, It end, std::size_t batch_size, int value)
{
	It	iter;
	std::size_t count = std::distance(beg, end) / batch_size;
	std::size_t step;
	while (count > 0)
	{
		iter = beg;
		step = count / 2;
		std::advance(iter, step * batch_size);
		if (*iter < value)
		{
			beg = iter + batch_size;
			count -= step + 1;
		}
		else
			count = step;
	}
	return beg;
}

std::vector<int> jacobsthalBatchingIndices(int size)
{
	int 				index= 1;
	int 				current = 2;
	int					res_i = -1;
	std::vector<int>	res(size);
	
	// the first number of each batch is jacobsthal + 1
	size += 2;
	while (1)
	{
		if (1 + jacobsthal[index] > size)
		{
			for (int i = size; i > current; i--)
				res[++res_i] = i;
			break ;
		}
		for (int i = 1 + jacobsthal[index]; i > current; i--)
			res[++res_i] = i;
		current = 1 + jacobsthal[index];
		index++;
	}
	return (res);
}

typedef std::vector<int>::iterator VecIt;
void sortVecRecursive(std::size_t batch_size, std::vector<int>& cont)
{
	VecIt first = cont.begin();
	VecIt last = cont.end();
	std::size_t dist = std::distance(first, last);
	std::size_t	count = dist / batch_size;
	std::size_t	n_pairs = count / 2;
	std::size_t pair_size = batch_size * 2;
	std::size_t i;

	// Base case
	if (count == 0 || count == 1)
		return ;
	if (count == 2 && *(last - batch_size) < *first)
	{
		std::swap_ranges(first, first + batch_size, first + batch_size);
		return ;
	}

	// Form pairs
	for (i = 0; i < n_pairs; i++)
	{
		VecIt left = first + i * pair_size;
		VecIt right = left + batch_size;
		if (*left < *right)
		{
			std::swap_ranges(left, left + batch_size, right);
		}
	}
	std::vector<int> leftover(batch_size);
	leftover.clear();
	if (count % 2)
	{
		leftover.insert(leftover.end(), last - batch_size, last);
		cont.erase(last - batch_size, last);
	}

	sortVecRecursive(pair_size, cont);

	/// we can safely put y1 back
	std::swap_ranges(first + batch_size, first + pair_size, first);

    std::vector<int> indices = jacobsthalBatchingIndices(n_pairs - 1);
	std::vector<int> losers(n_pairs * batch_size);

    losers.clear();

	// separate losers from winners
    for (i = 0; i < n_pairs - 1; i++)
	{
		VecIt winner = first + pair_size + batch_size * i;
		VecIt loser = winner + batch_size;
		losers.insert(losers.end(), loser, loser + batch_size);
		cont.erase(loser, loser + batch_size);
	}
    for (i = 0; i < n_pairs - 1; i++)
	{
		VecIt search_to = cont.begin() + batch_size * (indices[i] + i);
		VecIt to_insert = losers.begin() + (indices[i] - 3) * batch_size;
		VecIt pos = stepBinarySearch(cont.begin(),
						 search_to, batch_size,
						 *to_insert);
		cont.insert(pos, to_insert, to_insert + batch_size);
	}
	if (count % 2)
	{
		VecIt pos = stepBinarySearch(cont.begin(), cont.end(), batch_size,
									 *leftover.begin());
		cont.insert(pos, leftover.begin(), leftover.end());
	}
}


// Deque has cont.{begin/end}() spam because the iterators get invalidated if
// they move, which is not the case with std::vector as the memory is contiguous
typedef std::deque<int>::iterator DequeIt;
void sortDequeRecursive(std::size_t batch_size, std::deque<int>& cont)
{
	std::size_t dist = std::distance(cont.begin(), cont.end());
	std::size_t	count = dist / batch_size;
	std::size_t	n_pairs = count / 2;
	std::size_t pair_size = batch_size * 2;
	std::size_t i;
	if (count == 0 || count == 1)
		return ;
	if (count == 2 && *(cont.end() - batch_size) < *cont.begin())
	{
		std::swap_ranges(cont.begin(), cont.begin() + batch_size, cont.begin() + batch_size);
		return ;
	}
	for (i = 0; i < n_pairs; i++)
	{
		DequeIt left = cont.begin() + i * pair_size;
		DequeIt right = left + batch_size;
		if (*left < *right)
		{
			std::swap_ranges(left, left + batch_size, right);
		}
	}
	std::deque<int> leftover(batch_size);
	leftover.clear();
	if (count % 2)
	{
		leftover.insert(leftover.end(), cont.end() - batch_size, cont.end());
		cont.erase(cont.end() - batch_size, cont.end());
	}
	sortDequeRecursive(pair_size, cont);
	std::swap_ranges(cont.begin() + batch_size, cont.begin() + pair_size, cont.begin());
	std::vector<int> indices = jacobsthalBatchingIndices(n_pairs - 1);
	std::deque<int> losers(n_pairs * batch_size);
    losers.clear();
    for (i = 0; i < n_pairs - 1; i++)
	{
		DequeIt winner = cont.begin() + pair_size + batch_size * i;
		DequeIt loser = winner + batch_size;
		losers.insert(losers.end(), loser, loser + batch_size);
		cont.erase(loser, loser + batch_size);
	}
    for (i = 0; i < n_pairs - 1; i++)
	{
		DequeIt search_to = cont.begin() + batch_size * (indices[i] + i);
		DequeIt to_insert = losers.begin() + (indices[i] - 3) * batch_size;
		DequeIt pos = stepBinarySearch(cont.begin(),
						 search_to, batch_size,
						 *to_insert);
		cont.insert(pos, to_insert, to_insert + batch_size);
	}
	if (count % 2)
	{
		DequeIt pos = stepBinarySearch(cont.begin(), cont.end(), batch_size,
									 *leftover.begin());
		cont.insert(pos, leftover.begin(), leftover.end());
	}
}

template <typename It>
bool	is_sorted(It begin, It end)
{
	while (++begin != end)
	{
		// allow same
		if (*begin < *(begin - 1))
			return (false);
	}
	return (true);
}

bool PmergeMe::sortVector()
{
	sortVecRecursive(1, vec);
	return (is_sorted(vec.begin(), vec.end()));
}

bool PmergeMe::sortDeque()
{
	sortDequeRecursive(1, deque);
	return (is_sorted(deque.begin(), deque.end()));
}


// Common input checking to both
static bool ingestInput(int argc, char *argv[], void (*append)(int))
{
	int	i, temp;
	
	for (i = 1; i < argc; i++)
	{
		// Input format: One positive integer per char* in argv
		std::stringstream	ss(argv[i]);
		ss >> temp;
		if (ss.fail() || !ss.eof() || temp <= 0)
		{
			printError(i, "Invalid argument", argv[i]);
			return (false);
		}
		append(temp);
	}
	return (true);
}

void PmergeMe::appendToDeque(int a) { deque.push_back(a); }
void PmergeMe::appendToVec(int a) { vec.push_back(a); }
// We're just assuming we have enough memory
bool	PmergeMe::vecIngestInput(int argc, char *argv[])
{
	return (ingestInput(argc, argv, appendToVec));
}

bool	PmergeMe::dequeIngestInput(int argc, char *argv[])
{
	return (ingestInput(argc, argv, appendToDeque));
}

void PmergeMe::printVector()
{
	VecIt it = vec.begin();

	std::cout << "{ ";
	while (it != vec.end())
	{
		std::cout << *it << " ";
		it++;
	}
	std::cout << "}" << std::endl;
}

void PmergeMe::printDeque()
{
	DequeIt it = deque.begin();

	std::cout << "{ ";
	while (it != deque.end())
	{
		std::cout << *it << " ";
		it++;
	}
	std::cout << "}" << std::endl;
}

// ----------BEGIN ORTHODOX CANONICAL FORM----------
PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) {(void) other;}
PmergeMe::~PmergeMe() {}

PmergeMe	PmergeMe::operator=(const PmergeMe &other)
{ (void) other; return (*this); }
// ----------END ORTHODOX CANONICAL FORM----------

