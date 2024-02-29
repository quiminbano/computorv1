/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:48:57 by corellan          #+#    #+#             */
/*   Updated: 2024/02/29 18:00:18 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PolynomConverter.hpp"

PolynomConverter::PolynomConverter() : p_hasInput(false), p_grade(0)
{
	
}

PolynomConverter::PolynomConverter(string_vector input) : p_input(input), p_hasInput(true), p_grade(0)
{
	string_vector	split;
	double			tempNumber;

	p_polynom.clear();
	for (std::string &iter : p_input)
	{
		split.clear();
		split = p_split(iter, "*");
		tempNumber = std::stod(split[0]);
		if (p_polynom.find(split[1]) != p_polynom.end())
			p_polynom[split[1]] = p_polynom[split[1]] + tempNumber;
		else
			p_polynom[split[1]] = tempNumber;
	}
}

PolynomConverter::~PolynomConverter()
{
	
}

void	PolynomConverter::initializeInput(string_vector input)
{
	string_vector	split;
	double			tempNumber;

	p_input = input;
	p_hasInput = true;
	p_polynom.clear();
	for (std::string &iter : p_input)
	{
		split.clear();
		split = p_split(iter, "*");
		if (split[1][0] == 'x')
			split[1][0] = 'X';
		if (!split[1].compare("X"))
			split[1].append("^0");
		tempNumber = std::stod(split[0]);
		if (p_polynom.find(split[1]) != p_polynom.end())
			p_polynom[split[1]] = p_polynom[split[1]] + tempNumber;
		else
			p_polynom[split[1]] = tempNumber;
	}
	for (std::pair<std::string const, double> &temp : p_polynom)
	{
		std::cout << "This is the key: " << temp.first << ". This is the value: " << temp.second << std::endl;
	}
}

void	PolynomConverter::clear()
{
	p_input.clear();
	p_polynom.clear();
	p_grade = 0;
	p_hasInput = false;
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

const char	*PolynomConverter::EmptyInput::what() const throw()
{
	return ("No input provided");
}
