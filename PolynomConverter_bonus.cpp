/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter_bonus.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:48:57 by corellan          #+#    #+#             */
/*   Updated: 2024/03/07 18:56:18 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PolynomConverter_bonus.hpp"
#include <iomanip>

PolynomConverter::PolynomConverter() : p_hasInput(false), p_hasInputOverflow(false), p_hasFractionalExponent(false), p_grade(0), p_minExp(0), p_maxExp(0)
{
	p_hasFailedSolving = false;
}

PolynomConverter::PolynomConverter(string_vector input, string_vector before, string_vector after) : p_input(input), p_hasInput(true), p_hasInputOverflow(false), p_hasFractionalExponent(false), p_grade(0), p_minExp(0), p_maxExp(0)
{
	p_polynom.clear();
	p_before = before;
	p_after = after;
	p_hasFailedSolving = false;
	p_solution1 = 0;
	p_solution2 = 0;
	p_discriminant = 0;
	p_rootDiscriminant = 0;
	p_storeInMap();
	p_findExponents();
}

PolynomConverter::~PolynomConverter()
{
	
}

void	PolynomConverter::initializeInput(string_vector input, string_vector before, string_vector after)
{
	p_input = input;
	p_before = before;
	p_after = after;
	p_solution1 = 0;
	p_solution2 = 0;
	p_discriminant = 0;
	p_rootDiscriminant = 0;
	p_hasInput = true;
	p_hasInputOverflow = false;
	p_hasFractionalExponent = false;
	p_hasFailedSolving = false;
	p_polynom.clear();
	p_storeInMap();
	p_findExponents();
}

void	PolynomConverter::printPolynom()
{
	long		idx;
	std::string	coefficient;
	std::string	model;

	idx = p_minExp;
	if (p_hasInput == false)
		throw EmptyInput();
	if (p_hasFractionalExponent == true)
	{
		std::cout << "The polynom has one or more non whole coefficients, I can't solve." << std::endl;
		p_hasFailedSolving = true;
		return ;
	}
	if (p_hasInputOverflow == true)
	{
		std::cout << "Some numbers overflowed during the conversion of the polymon, I can't solve." << std::endl;
		return ;
	}
	std::cout << "Reduced form: ";
	while (idx <= p_maxExp)
	{
		model.clear();
		model.append("X^");
		model.append(std::to_string(idx));
		if (p_polynom.find(model) != p_polynom.end())
		{
			coefficient = std::to_string(p_polynom.find(model)->second);
			if (coefficient[0] != '-' && idx != p_minExp)
				std::cout << "+";
			else if (coefficient[0] != '-' && idx == p_minExp)
				std::cout << "";
			else
				std::cout << "-";
			std::cout << " ";
			if (p_polynom.find(model)->second < 0)
				std::cout << (p_polynom.find(model)->second * -1);
			else
				std::cout << (p_polynom.find(model)->second);
		}
		else
		{
			if (idx > p_minExp)
				std::cout << "+ ";
			std::cout << 0;
		}
		if (model.compare("X^0"))
		{
			std::cout << " * ";
			if (!model.compare("X^1"))
				std::cout << "X";
			else
				std::cout << model;
		}
		std::cout << " ";
		idx++;
	}
	std::cout << "= 0" << std::endl;
}

void	PolynomConverter::printPolynomGrade()
{
	if (p_hasInput == false)
		throw EmptyInput();
	if (p_hasFractionalExponent == true)
		return ;
	if (p_hasInputOverflow == true)
		return ;
	std::cout << "Polynomial degree: " << p_grade << std::endl;
}

void	PolynomConverter::solvePolynom()
{
	if (p_hasInput == false)
		throw EmptyInput();
	if (p_hasFractionalExponent == true)
		return ;
	if (p_hasInputOverflow == true)
		return ;
	if (p_minExp < 0)
	{
		std::cout << "The polynom has negative exponents, I can't solve." << std::endl;
		p_hasFailedSolving = true;
		return ;
	}
	if (p_maxExp > 2)
	{
		std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl;
		p_hasFailedSolving = true;
		return ;
	}
	switch (p_grade)
	{
	case 1:
		p_solveLinear();
		break;
	case 2:
		p_solveQuadratic();
		break;
	default:
		p_solveGradeCero();
		break;
	}
}

bool	PolynomConverter::getFractionalExponentFlag() const
{
	return (p_hasFractionalExponent);
}

bool	PolynomConverter::getHasSolutionFailed() const
{
	return (p_hasFailedSolving);
}

void	PolynomConverter::clear()
{
	p_input.clear();
	p_before.clear();
	p_after.clear();
	p_polynom.clear();
	p_grade = 0;
	p_hasInput = false;
	p_hasInputOverflow = false;
	p_hasFractionalExponent = false;
	p_minExp = 0;
	p_maxExp = 0;
	p_solution1 = 0;
	p_solution2 = 0;
	p_discriminant = 0;
	p_rootDiscriminant = 0;
}

void	PolynomConverter::printProcess()
{
	if (p_hasInput == false)
		throw EmptyInput();
	if (p_hasFractionalExponent == true)
		return ;
	p_printFirstStep();
	p_printSecondStep();
	if (p_grade == 1)
		p_printGradeOneSteps();
	else if (p_grade == 2)
		p_printGradeTwoSteps();
}

void	PolynomConverter::p_storeInMap()
{
	double			tempNumber;
	double			tempExponent;
	double			rounded;
	string_vector	split;
	std::string		tempString;

	for (std::string &iter : p_input)
	{
		split.clear();
		split = p_split(iter, "*");
		if (split[1][0] == 'x')
			split[1][0] = 'X';
		if (!split[1].compare("X"))
			split[1].append("^0");
		if (split[1][2] == '+')
			split[1].erase(2, 1);
		try
		{
			tempNumber = std::stod(split[0]);
			if (tempNumber == p_floor(static_cast<double>(0)))
				tempNumber = 0;
		}
		catch(const std::exception &e)
		{
			p_hasInputOverflow = true;
			tempNumber = static_cast<double>(0);
		}
		try
		{
			tempExponent = std::stod(split[1].substr(2));
		}
		catch(const std::exception &e)
		{
			p_hasInputOverflow = true;
			tempExponent = static_cast<double>(0);
		}
		try
		{
			rounded = p_floor(tempExponent);
		}
		catch(const std::exception &e)
		{
			p_hasInputOverflow = true;
		}
		if (rounded != tempExponent)
		{
			p_hasFractionalExponent = true;
		}
		tempString.clear();
		tempString.append("X^" + std::to_string(static_cast<long long>(rounded)));
		if (p_polynom.find(tempString) != p_polynom.end())
		{
			if (p_isOverflowed(p_polynom.find(tempString)->second, tempNumber, "+") == false)
				p_polynom[tempString] = p_polynom[tempString] + tempNumber;
			else
				p_hasInputOverflow = true;
		}
		else
			p_polynom[tempString] = tempNumber;
	}
}

void	PolynomConverter::p_findExponents()
{
	long		tempExp;
	std::string	exponents;

	for (std::pair<std::string const, double> &temp : p_polynom)
	{
		exponents.clear();
		exponents = temp.first.substr(2);
		try
		{
			tempExp = std::stol(exponents);
		}
		catch(const std::exception &e)
		{
			p_hasInputOverflow = true;
			tempExp = 0;
		}
		if (tempExp < p_minExp && temp.second != static_cast<double>(0))
			p_minExp = tempExp;
		if (tempExp > p_maxExp && temp.second != static_cast<double>(0))
			p_maxExp = tempExp;
	}
	p_grade = p_maxExp;
}

static size_t	findPosString(std::string const &input, std::string const &needle, size_t n)
{
	size_t		i;
	std::string	temp;

	temp = input.substr(0, n);
	if (temp.find(needle) != std::string::npos)
		i = temp.find(needle);
	else
		i = temp.size();
	return (i);
}

static size_t	countInString(std::string const &input, std::string const &needle)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	k = 0;
	while (input[i])
	{
		j = 0;
		while (input[i] && needle[j] && input[i] == needle[j])
		{
			i++;
			j++;
		}
		if (j == needle.size())
			k = i;
		else
			break ;
	}
	return (k);
}

static bool	checkBlock(std::string const &input, std::string const &needle, size_t i)
{
	size_t	len_needle;
	size_t	start_point;

	len_needle = needle.size() - 1;
	start_point = i;
	while (len_needle > 0 && start_point > 0)
	{
		if (needle[len_needle] != input[start_point])
			break ;
		len_needle--;
		start_point--;
	}
	if (needle[len_needle] != input[start_point])
		return (false);
	return (true);
}

static size_t	countWords(std::string const &input, std::string const &needle)
{
	size_t	i;
	size_t	j;
	size_t	k;
	int		flag;
	size_t	words;

	words = 0;
	i = 0;
	while (input[i])
	{
		flag = 0;
		k = (i + 1);
		j = 0;
		while ((input[k]) && (needle[j]) && (input[k] == needle[j]))
		{
			k++;
			j++;
		}
		if (j == needle.size())
			flag = 1;
		if ((checkBlock(input, needle, i) == false) && (flag == 1 || input[i + 1] == '\0'))
			words++;
		i++;
	}
	return (words);
}

string_vector	PolynomConverter::p_split(std::string const &input, std::string const &needle)
{
	string_vector	split;
	std::string		temp;
	size_t			words;
	size_t			i;

	temp = input;
	if (needle.size() == 0 || input.size() < needle.size())
	{
		split.push_back(input);
		return (split);
	}
	else if (!input.compare(needle))
	{
		split.push_back("");
		return (split);
	}
	words = countWords(input, needle);
	i = 0;
	while (i < words)
	{
		temp = temp.substr(countInString(temp, needle));
		split.push_back(temp.substr(0, findPosString(temp, needle, temp.size())));
		if ((i + 1) < words)
			temp = temp.substr(findPosString(temp, needle, temp.size()) + countInString(temp.substr(findPosString(temp, needle, temp.size())), needle));
		i++;
	}
	return (split);
}

void	PolynomConverter::p_solveLinear()
{
	double									expZero;
	double									expOne;
	double									result;
	std::map<std::string, double>::iterator	iter;

	result = 0;
	iter = p_polynom.find("X^0");
	if (iter != p_polynom.end())
		expZero = (iter->second * -1);
	else
		expZero = 0;
	expOne = p_polynom.find("X^1")->second;
	result = expZero / expOne;
	if (p_isOverflowed(expZero, expOne, "/") == true)
	{
		std::cout << "Some numbers overflowed trying to solve the polynom, I can't solve." << std::endl;
		p_hasFailedSolving = true;
		return ;
	}
	std::cout << "The solution is:" << std::endl;
	if (result != p_floor(static_cast<double>(0)))
		std::cout << result << std::endl;
	else
		std::cout << static_cast<double>(0) << std::endl;
	p_printIrreductible(result, true, 1, expZero, expOne);
	p_solution1 = result;
}

void	PolynomConverter::p_solveQuadratic()
{
	double									a;
	double									b;
	double									c;
	double									discriminant;
	double									solution1;
	double									solution2;
	std::map<std::string, double>::iterator	iterB;
	std::map<std::string, double>::iterator	iterC;

	discriminant = 0;
	b = 0;
	c = 0;
	solution1 = 0;
	solution2 = 0;
	a = p_polynom.find("X^2")->second;
	iterB = p_polynom.find("X^1");
	if (iterB != p_polynom.end())
		b = p_polynom.find("X^1")->second;
	iterC = p_polynom.find("X^0");
	if (iterC != p_polynom.end())
		c = p_polynom.find("X^0")->second;
	discriminant = ((b * b) - (static_cast<double>(4) * a * c));
	if (p_isOverflowed(b, b, "*") == true || p_isOverflowed((b * b), (4 * a * c), "-") == true || \
		p_isOverflowed(4, (a * c), "*") == true)
	{
		std::cout << "Some numbers overflowed trying to solve the polynom, I can't solve." << std::endl;
		p_hasFailedSolving = true;
		return ;
	}
	if (discriminant == p_floor(static_cast<double>(0)))
		discriminant = 0;
	p_discriminant = discriminant;
	if (discriminant < static_cast<double>(0))
	{
		std::cout << "Discriminant is strictly negative, I can't solve." << std::endl;
		p_hasFailedSolving = true;
		return ;
	}
	p_rootDiscriminant = p_sqrt(discriminant);
	if (p_rootDiscriminant == p_floor(static_cast<double>(0)))
		p_rootDiscriminant = 0;
	solution1 = (((b * -1) - (p_sqrt(discriminant))) / (static_cast<double>(2) * a));
	solution2 = (((b * -1) + (p_sqrt(discriminant))) / (static_cast<double>(2) * a));
	if (p_isOverflowed((b * -1), ((p_sqrt(discriminant))), "-") == true || p_isOverflowed((b * -1), ((p_sqrt(discriminant))), "+") == true || \
		p_isOverflowed(2, a, "*") == true || p_isOverflowed(((b * -1) - (p_sqrt(discriminant))), (2 * a), "/") == true || \
		p_isOverflowed(((b * -1) + (p_sqrt(discriminant))), (2 * a), "/") == true)
	{
		std::cout << "Some numbers overflowed trying to solve the polynom, I can't solve." << std::endl;
		p_hasFailedSolving = true;
		return ;
	}
	std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
	if (solution1 == p_floor(static_cast<double>(0)))
		solution1 = static_cast<double>(0);
	if (solution2 == p_floor(static_cast<double>(0)))
		solution2 = static_cast<double>(0);
	std::cout << solution1 << std::endl;
	std::cout << solution2 << std::endl;
	p_printIrreductible(solution1, true, 2, ((b * -1) - (p_sqrt(discriminant))), (static_cast<double>(2) * a));
	p_printIrreductible(solution2, false, 2, ((b * -1) + (p_sqrt(discriminant))), (static_cast<double>(2) * a));
	p_solution1 = solution1;
	p_solution2 = solution2;
}

void	PolynomConverter::p_solveGradeCero()
{
	double	a;

	a = p_polynom.find("X^0")->second;
	if (a == static_cast<double>(0))
		std::cout << "The solution is: Each real number." << std::endl;
	else
	{
		std::cout << "I have reached an impossible solution for X, I can't solve." << std::endl;
		p_hasFailedSolving = true;
	}
}

void	PolynomConverter::p_printIrreductible(double number, bool toPrint, int grade, double numerator, double denominator)
{
	long long	down;
	long long	up;
	long long	scale;
	long long	commonDivisor;
	double		decimal;

	if (toPrint == true && grade == 1)
		std::cout << "The solution as irreductible fraction is:" << std::endl;
	else if (toPrint == true && grade == 2)
		std::cout << "The solution as irreductible fractions are:" << std::endl;
	if (p_floor(number) == number)
	{
		if (number != p_floor(static_cast<double>(0)))
			std::cout << number << std::endl;
		else
			std::cout << static_cast<double>(0) << std::endl;
		return ;
	}
	if (p_floor(numerator) == numerator && p_floor(denominator) == denominator)
	{
		up = static_cast<long long>(numerator);
		down = static_cast<long long>(denominator);
	}
	else
	{
		if (number > p_floor(number))
		{
			decimal = number - p_floor(number);
		}
		else
		{
			decimal = p_floor(number) - number;
			decimal = (1 + decimal);
		}
		scale = (std::to_string(decimal).size() - std::to_string(decimal).find(".") - 1);
		down = p_pow(10, scale);
		if (down >= 1000000)
			down = 1000000000000000;
		up = static_cast<long long>(number * static_cast<double>(down));
		if (static_cast<double>(up) < (number * static_cast<double>(down)))
		{
			std::cout << "An overflow happened trying to obtain the irreductible solution, I can't solve it." << std::endl;
			return ;
		}
	}
	commonDivisor = p_gcd(up, down);
	up /= commonDivisor;
	down /= commonDivisor;
	if (down < 0 && up > 0)
	{
		down *= -1;
		up *= -1;
	}
	std::cout << up << "/" << down << std::endl;
}

double	PolynomConverter::p_sqrt(double squared)
{
	double	current;
	double	next;
	int		counter;

	counter = 0;
	current = (squared / static_cast<double>(2));
	if (current == static_cast<double>(0))
	{
		next = static_cast<double>(0);
		return next;
	}
	while (1)
	{
		next = ((current + (squared / current)) / static_cast<double>(2));
		if (next == current)
			break ;
		counter++;
		if (counter == 20)
			break ;
		current = next;
	}
	return (next);
}

double	PolynomConverter::p_floor(double number)
{
	long long	n;
	double		toReturn;

	if (number >= (static_cast<double>((LLONG_MAX / 2) + 1) * static_cast<double>(2)) || \
		number <= static_cast<double>(LLONG_MIN))
		throw FloorError();
	n = static_cast<long long>(number);
	toReturn = static_cast<double>(n);
	if (toReturn == number || toReturn >= static_cast<double>(0))
		return (toReturn);
	return (toReturn - static_cast<double>(1));
}

long long	PolynomConverter::p_pow(long long number, long long exponent)
{
	double	toReturn;

	toReturn = number;
	for (long long index = 1; index < exponent; index++)
		toReturn *= number;
	return (toReturn);
}

long long	PolynomConverter::p_gcd(long long numerator, long long denominator)
{
	if (denominator == 0)
		return (numerator);
	else
		return (p_gcd(denominator, (numerator % denominator)));
}

vector_pair	PolynomConverter::p_getVectorPair(string_vector input)
{
	double			tempNumber;
	double			tempExponent;
	double			rounded;
	string_vector	split;
	std::string		tempString;
	pair_t			inputContainer;
	vector_pair		toReturn;

	for (std::string &iter : input)
	{
		split.clear();
		split = p_split(iter, "*");
		if (split[1][0] == 'x')
			split[1][0] = 'X';
		if (!split[1].compare("X"))
			split[1].append("^0");
		if (split[1][2] == '+')
			split[1].erase(2, 1);
		try
		{
			tempNumber = std::stod(split[0]);
			if (tempNumber == p_floor(static_cast<double>(0)))
				tempNumber = 0;
		}
		catch(const std::exception &e)
		{
			p_hasInputOverflow = true;
			tempNumber = static_cast<double>(0);
		}
		try
		{
			tempExponent = std::stod(split[1].substr(2));
		}
		catch(const std::exception &e)
		{
			p_hasInputOverflow = true;
			tempExponent = static_cast<double>(0);
		}
		try
		{
			rounded = p_floor(tempExponent);
		}
		catch(const std::exception &e)
		{
			p_hasInputOverflow = true;
		}
		if (rounded != tempExponent)
		{
			p_hasFractionalExponent = true;
		}
		tempString.clear();
		tempString.append("X^" + std::to_string(static_cast<long long>(rounded)));
		inputContainer = std::make_pair(tempString, tempNumber);
		toReturn.push_back(inputContainer);
	}
	return (toReturn);
}

void	PolynomConverter::p_printFirstStep()
{
	vector_pair	beforeEqual;
	vector_pair	afterEqual;
	vector_pair	afterProcess;

	std::cout << std::endl << "This is the equation:" << std::endl << std::endl;
	beforeEqual = p_getVectorPair(p_before);
	afterEqual = p_getVectorPair(p_after);
	p_printPairs(beforeEqual);
	std::cout << " = ";
	p_printPairs(afterEqual);
	std::cout << std::endl << std::endl << "After rearranging all the term in the left side of the equation, we get: " << std::endl << std::endl;
	afterProcess = p_getVectorPair(p_input);
	p_printPairs(afterProcess);
	std::cout << "= 0" << std::endl;
}

void	PolynomConverter::p_printSecondStep()
{
	std::cout << std::endl << "If we reduce the term of the equation, we get:" << std::endl << std::endl;
	p_printMap();
	std::cout << "= 0" << std::endl << std::endl;
}

void	PolynomConverter::p_printPairs(vector_pair paired)
{
	std::string	coefficient;
	size_t		idx;

	idx = 0;
	if (p_hasInput == false)
		throw EmptyInput();
	if (p_hasFractionalExponent == true)
	{
		std::cout << "The polynom has one or more non whole coefficients, I can't solve." << std::endl;
		return ;
	}
	for (pair_t &iter : paired)
	{
		coefficient = std::to_string(iter.second);
		if (coefficient[0] != '-' && idx != 0)
			std::cout << "+";
		else if (coefficient[0] != '-' && idx == 0)
			std::cout << "";
		else
			std::cout << "-";
		std::cout << " ";
		if (iter.second < 0)
			std::cout << (iter.second * -1);
		else
			std::cout << (iter.second);
		if (iter.first.compare("X^0"))
		{
			std::cout << " * ";
			if (!iter.first.compare("X^1"))
				std::cout << "X";
			else
				std::cout << iter.first;
		}
		std::cout << " ";
		idx++;
	}
}

void	PolynomConverter::p_printMap()
{
	std::string	coefficient;
	size_t		idx;

	idx = 0;
	if (p_hasInput == false)
		throw EmptyInput();
	if (p_hasFractionalExponent == true)
	{
		std::cout << "The polynom has one or more non whole coefficients, I can't solve." << std::endl;
		return ;
	}
	for (std::pair<const std::string, double> &iter : p_polynom)
	{
		coefficient = std::to_string(iter.second);
		if (coefficient[0] != '-' && idx != 0)
			std::cout << "+";
		else if (coefficient[0] != '-' && idx == 0)
			std::cout << "";
		else
			std::cout << "-";
		std::cout << " ";
		if (iter.second < 0)
			std::cout << (iter.second * -1);
		else
			std::cout << (iter.second);
		if (iter.first.compare("X^0"))
		{
			std::cout << " * ";
			if (!iter.first.compare("X^1"))
				std::cout << "X";
			else
				std::cout << iter.first;
		}
		std::cout << " ";
		idx++;
	}
}

void	PolynomConverter::p_printGradeOneSteps()
{
	double	numerator;
	double	denominator;
	
	std::cout << "We pass the constants to the right side:" << std::endl << std::endl;
	numerator = p_polynom.find("X^0")->second;
	denominator = p_polynom.find("X^1")->second;
	if (denominator < 0)
		std::cout << " - " << (denominator * -1) << " * X" << " = ";
	else
		std::cout << " " << denominator << " * X" << " = ";
	if (numerator < 0)
		std::cout << " " << (numerator * -1) << std::endl << std::endl;
	else
		std::cout << " - " << numerator << std::endl << std::endl;
	std::cout << "Isolating X, we get:" << std::endl << std::endl;
	std::cout << " X = ";
	if (numerator > 0 && denominator < 0)
		denominator *= static_cast<double>(-1);
	else if (numerator > 0 && denominator > 0)
		numerator *= static_cast<double>(-1);
	else if (numerator < 0 && denominator < 0)
	{
		numerator *= static_cast<double>(-1);
		denominator *= static_cast<double>(-1);
	}
	if (numerator == p_floor(static_cast<double>(0)))
		numerator = 0;
	if (numerator < 0)
	{
		std::cout << "- ";
		numerator *= -1;
	}
	std::cout << numerator << "/" << denominator << std::endl << std::endl;
	std::cout << "Then, the result is:" << std::endl << std::endl;
	std::cout << " " << p_solution1 << std::endl << std::endl;
}

void	PolynomConverter::p_printGradeTwoSteps()
{
	double	a;
	double	b;
	double	c;

	std::cout << std::endl << "A second grade ecuation has the following form:" << std::endl << std::endl;
	std::cout << " c + b * X + a * X^2 = 0" << std::endl << std::endl;
	a = p_polynom.find("X^2")->second;
	if (p_polynom.find("X^1") != p_polynom.end())
		b = p_polynom.find("X^1")->second;
	else
		b = 0;
	if (p_polynom.find("X^0") != p_polynom.end())
		c = p_polynom.find("X^0")->second;
	else
		c = 0;
	if (b == p_floor(static_cast<double>(0)))
		b = 0;
	if (c == p_floor(static_cast<double>(0)))
		c = 0;
	std::cout << "In this case, the equation can be written as follows:" << std::endl << std::endl << " ";
	if (c < 0)
		std::cout << " - " << (c * -1) << " ";
	else
		std::cout << " " << c << " ";
	if (b < 0)
		std::cout << "- " << (b * -1) << " * X ";
	else
		std::cout << "+ " << b << " * X ";
	if (a < 0)
		std::cout << "- " << (a * -1) << " * X^2 = 0" << std::endl << std::endl;
	else
		std::cout << "+ " << a << " * X^2 = 0" << std::endl;
	std::cout << "Where:" << std::endl << " c = " << c << std::endl << " b = " << b << std::endl << " a = " << a << std::endl << std::endl;
	std::cout << "The equations to solve the possible values for X in a quadratic equation are:" << std::endl << std::endl;
	std::cout << "X1 = (-b - sqrt(b^2 - (4 * a * c))/(2 * a)) (solution1)" << std::endl;
	std::cout << "X2 = (-b + sqrt(b^2 - (4 * a * c))/(2 * a)) (solution2)" << std::endl;
	std::cout << "Where: (4 * a * c) is the discriminant." << std::endl << std::endl;
	std::cout << "Replacing the values of a, b and c, we get:" << std::endl << std::endl;
	std::cout << "X1 = (" << p_correctZero((b * -1)) << " - sqrt(" << p_correctZero((b * b)) << " - (" << 4 << " * " << a << " * " << c << "))/(";
	std::cout << 2 << " * " << a << "))  (solution1)" << std::endl;
	std::cout << "X2 = (" << p_correctZero((b * -1)) << " + sqrt(" << p_correctZero((b * b)) << " - (" << 4 << " * " << a << " * " << c << "))/(";
	std::cout << 2 << " * " << a << "))  (solution2)" << std::endl << std::endl;
	std::cout << "If we continue resolving the equations we get:" << std::endl << std::endl;
	if (a < 0)
	{
		std::cout << "X1 = - (" << p_correctZero((b * -1)) << " - sqrt(" << p_discriminant << "))/(";
		std::cout << 2 << " * " << p_correctZero((a * -1)) << "))  (solution1)" << std::endl;
		std::cout << "X2 = - (" << p_correctZero((b * -1)) << " + sqrt(" << p_discriminant << "))/(";
		std::cout << 2 << " * " << p_correctZero((a * -1)) << "))  (solution2)" << std::endl << std::endl;
		std::cout << "X1 = - (" << p_correctZero((b * -1)) << " - " << p_rootDiscriminant << ")/(";
		std::cout << p_correctZero((2 * (a * -1))) << "))  (solution1)" << std::endl;
		std::cout << "X2 = - (" << p_correctZero((b * -1)) << " + " << p_rootDiscriminant << ")/(";
		std::cout << p_correctZero((2 * (a * -1))) << "))  (solution2)" << std::endl << std::endl;
		std::cout << "X1 = - (" << p_correctZero(((b * -1) - p_rootDiscriminant)) << ")/(";
		std::cout << p_correctZero((2 * (a * -1))) << "))  (solution1)" << std::endl;
		std::cout << "X2 = - (" << p_correctZero(((b * -1) - p_rootDiscriminant)) << ")/(";
		std::cout << p_correctZero((2 * (a * -1))) << "))  (solution2)" << std::endl << std::endl;
		if ((p_correctZero((b * -1) - p_rootDiscriminant) < 0))
			std::cout << "X1 = " << p_correctZero((((b * -1) - p_rootDiscriminant) * -1)) << "/" << p_correctZero((2 * (a * -1))) << "  (solution1)" << std::endl;
		else
			std::cout << "X1 = - " << p_correctZero((((b * -1) - p_rootDiscriminant)) * -1) << "/" << p_correctZero((2 * (a * -1))) << "  (solution1)" << std::endl;
		if (p_correctZero(((b * -1) + p_rootDiscriminant)) < 0)
			std::cout << "X2 = " << p_correctZero((((b * -1) + p_rootDiscriminant) * -1)) << "/" << p_correctZero((2 * (a * -1))) << "  (solution2)" << std::endl << std::endl;
		else
			std::cout << "X2 = - " << p_correctZero((((b * -1) + p_rootDiscriminant) * -1)) << "/" << p_correctZero((2 * (a * -1))) << "  (solution2)" << std::endl << std::endl;
	}
	else
	{
		std::cout << "X1 = (" << p_correctZero((b * -1)) << " - sqrt(" << p_discriminant << "))/(";
		std::cout << 2 << " * " << a << "))  (solution1)" << std::endl;
		std::cout << "X2 = (" << p_correctZero((b * -1)) << " + sqrt(" << p_discriminant << "))/(";
		std::cout << 2 << " * " << a << "))  (solution2)" << std::endl << std::endl;
		std::cout << "X1 = (" << p_correctZero((b * -1)) << " - " << p_rootDiscriminant << ")/(";
		std::cout << p_correctZero((2 * (a))) << "))  (solution1)" << std::endl;
		std::cout << "X2 = (" << p_correctZero((b * -1)) << " + " << p_rootDiscriminant << ")/(";
		std::cout << p_correctZero((2 * (a))) << "))  (solution2)" << std::endl << std::endl;
		std::cout << "X1 = (" << p_correctZero(((b * -1) - p_rootDiscriminant)) << ")/(";
		std::cout << p_correctZero((2 * (a))) << "))  (solution1)" << std::endl;
		std::cout << "X2 = (" << p_correctZero(((b * -1) + p_rootDiscriminant)) << ")/(";
		std::cout << p_correctZero((2 * (a))) << "))  (solution2)" << std::endl << std::endl;
		if (p_correctZero(((b * -1) - p_rootDiscriminant)) < 0)
			std::cout << "X1 = - " << p_correctZero((((b * -1) - p_rootDiscriminant) * -1)) << "/" << p_correctZero((2 * (a))) << "  (solution1)" << std::endl;
		else
			std::cout << "X1 = " << p_correctZero((((b * -1) - p_rootDiscriminant) * -1)) << "/" << p_correctZero((2 * (a))) << "  (solution1)" << std::endl;
		if (p_correctZero(((b * -1) + p_rootDiscriminant)) < 0)
			std::cout << "X2 = - " << p_correctZero((((b * -1) + p_rootDiscriminant) * -1)) << "/" << p_correctZero((2 * (a))) << "  (solution2)" << std::endl << std::endl;
		else
			std::cout << "X2 = " << p_correctZero((((b * -1) + p_rootDiscriminant) * -1)) << "/" << p_correctZero((2 * (a))) << "  (solution2)" << std::endl << std::endl;
	}
	std::cout << "X1 = " << p_solution1 << "  (solution1)" << std::endl;
	std::cout << "X2 = " << p_solution2 << "  (solution2)" << std::endl << std::endl;
}

bool	PolynomConverter::p_isOverflowed(double number1, double number2, std::string sign)
{
	double	result;

	if (!sign.compare("+"))
		result = number1 + number2;
	if (!sign.compare("-"))
		result = number1 - number2;
	if (!sign.compare("*"))
		result = number1 * number2;
	if (!sign.compare("/"))
		result = number1 / number2;
	if (result == std::numeric_limits<double>::infinity())
		return (true);
	else if (result == (std::numeric_limits<double>::infinity() * -1))
		return (true);
	return (false);
}

double	PolynomConverter::p_correctZero(double number)
{
	if (number != p_floor(static_cast<double>(0)))
		return (number);
	return (static_cast<double>(0));
}

const char	*PolynomConverter::EmptyInput::what() const throw()
{
	return ("No input provided, I can't solve it.");
}

const char	*PolynomConverter::FloorError::what() const throw()
{
	return ("Error trying to floor the number");
}
