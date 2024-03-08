/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:48:57 by corellan          #+#    #+#             */
/*   Updated: 2024/03/08 16:50:15 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PolynomConverter.hpp"

PolynomConverter::PolynomConverter() : p_hasInput(false), p_hasInputOverflow(false), p_hasFractionalExponent(false), p_grade(0), p_minExp(0), p_maxExp(0)
{
	
}

PolynomConverter::PolynomConverter(string_vector input) : p_input(input), p_hasInput(true), p_hasInputOverflow(false), p_hasFractionalExponent(false), p_grade(0), p_minExp(0), p_maxExp(0)
{
	p_polynom.clear();
	p_storeInMap();
	p_findExponents();
}

PolynomConverter::~PolynomConverter()
{
	
}

void	PolynomConverter::initializeInput(string_vector input)
{
	p_input = input;
	p_hasInput = true;
	p_hasInputOverflow = false;
	p_hasFractionalExponent = false;
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
				std::cout << (p_polynom.find(model)->second * -1) << " * ";
			else
				std::cout << (p_polynom.find(model)->second) << " * ";
		}
		else
		{
			if (idx > p_minExp)
				std::cout << "+ ";
			std::cout << 0 << " * ";
		}
		std::cout << model;
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
		return ;
	}
	if (p_maxExp > 2)
	{
		std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl;
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

void	PolynomConverter::clear()
{
	p_input.clear();
	p_polynom.clear();
	p_grade = 0;
	p_hasInput = false;
	p_hasInputOverflow = false;
	p_hasFractionalExponent = false;
	p_minExp = 0;
	p_maxExp = 0;
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
		return ;
	}
	std::cout << "The solution is:" << std::endl;
	if (result != p_floor(static_cast<double>(0)))
		std::cout << result << std::endl;
	else
		std::cout << static_cast<double>(0) << std::endl;
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
		return ;
	}
	if (discriminant < static_cast<double>(0))
	{
		std::cout << "Discriminant is strictly negative, the two solutions are:" << std::endl;
		p_calculateImaginary(a, b, discriminant);
		return ;
	}
	solution1 = (((b * -1) - (p_sqrt(discriminant))) / (static_cast<double>(2) * a));
	solution2 = (((b * -1) + (p_sqrt(discriminant))) / (static_cast<double>(2) * a));
	if (p_isOverflowed((b * -1), ((p_sqrt(discriminant))), "-") == true || p_isOverflowed((b * -1), ((p_sqrt(discriminant))), "+") == true || \
		p_isOverflowed(2, a, "*") == true || p_isOverflowed(((b * -1) - (p_sqrt(discriminant))), (2 * a), "/") == true || \
		p_isOverflowed(((b * -1) + (p_sqrt(discriminant))), (2 * a), "/") == true)
	{
		std::cout << "Some numbers overflowed trying to solve the polynom, I can't solve." << std::endl;
		return ;
	}
	std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
	if (solution1 != p_floor(static_cast<double>(0)))
		std::cout << solution1 << std::endl;
	else
		std::cout << static_cast<double>(0) << std::endl;
	if (solution2 != p_floor(static_cast<double>(0)))
		std::cout << solution2 << std::endl;
	else
		std::cout << static_cast<double>(0) << std::endl;
}

void	PolynomConverter::p_calculateImaginary(double a, double b, double discriminant)
{
	double	realSolution;
	double	imaginarySolution;

	realSolution = (b *-1)/(2 * a);
	imaginarySolution = p_sqrt((discriminant * -1))/(2 * a);
	if (p_isOverflowed((b *-1), (2 * a), "/") == true || p_isOverflowed(p_sqrt(discriminant * -1), (2 * a), "/") == true)
	{
		std::cout << "Some numbers overflowed trying to solve the polynom, I can't solve." << std::endl;
		return ;
	}
	if (realSolution == p_floor(static_cast<double>(0)))
		realSolution = p_floor(static_cast<double>(0));
	if (imaginarySolution == p_floor(static_cast<double>(0)))
		imaginarySolution = p_floor(static_cast<double>(0));
	std::cout << realSolution << " - " << imaginarySolution << " * i" << std::endl;
	std::cout << realSolution << " + " << imaginarySolution << " * i" << std::endl;	
}

void	PolynomConverter::p_solveGradeCero()
{
	double	a;

	a = p_polynom.find("X^0")->second;
	if (a == static_cast<double>(0))
		std::cout << "The solution is: Each real number." << std::endl;
	else
		std::cout << "I have reached an impossible solution for X, I can't solve." << std::endl;
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

const char	*PolynomConverter::EmptyInput::what() const throw()
{
	return ("No input provided, I can't solve it.");
}

const char	*PolynomConverter::FloorError::what() const throw()
{
	return ("Error trying to floor the number");
}
