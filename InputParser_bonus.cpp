/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputParser_bonus.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:20:30 by corellan          #+#    #+#             */
/*   Updated: 2024/03/06 10:33:47 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputParser_bonus.hpp"
#include <iostream>

InputParser::InputParser() : p_hasInput(false), p_hasFailed(true)
{
	
}

InputParser::~InputParser()
{
	
}

InputParser::InputParser(int argc, char **argv) : p_hasInput(false), p_hasFailed(true)
{
	if (!argv || argc == 1)
		return ;
	p_rawData.clear();
	for (size_t i = 1; argv[i]; i++)
	{
		p_rawData.append(argv[i]);
		p_rawData.append(" ");
	}
	p_hasInput = true;
	p_hasFailed = false;
	return ;
}

void	InputParser::input(int argc, char **argv)
{
	if (!argv || argc == 1)
		return ;
	p_rawData.clear();
	for (size_t i = 1; argv[i]; i++)
	{
		p_rawData.append(argv[i]);
		p_rawData.append(" ");
	}
	p_hasInput = true;
	p_hasFailed = false;
	return ;
}

bool	InputParser::fail() const
{
	return (p_hasFailed);
}

string_vector	InputParser::getInput()
{
	std::string			beforeEqualString;
	std::string			afterEqualString;
	std::stringstream	parserObject;
	string_vector		beforeEqualVector;
	string_vector		afterEqualVector;
	int					i;

	i = 0;
	beforeEqualString.clear();
	afterEqualString.clear();
	if (p_hasInput == false || p_hasFailed == true)
		throw ParserError();
	if (p_rawData.find("=") == std::string::npos)
		throw ParserError();
	parserObject.clear();
	parserObject.str(p_rawData);
	if (parserObject.fail() == true)
		throw ParserError();
	while (i < 2)
	{
		if (i == 0)
			std::getline(parserObject, beforeEqualString, '=');
		else
			std::getline(parserObject, afterEqualString, '=');
		i++;
	}
	if (i < 2 || parserObject.eof() == false)
		throw ParserError();
	try
	{
		if (p_justspaces(afterEqualString))
			throw ParserError();
		beforeEqualVector = p_getSplitted(beforeEqualString);
		afterEqualVector = p_getSplitted(afterEqualString);
		p_validateVectors(beforeEqualVector, afterEqualVector);
	}
	catch(const std::exception& e)
	{
		throw ParserError();
	}
	return (p_mergeVectors(beforeEqualVector, afterEqualVector));
}

string_vector	InputParser::p_getSplitted(std::string input)
{
	string_vector				split;
	std::regex					pattern1("[+-]?[0-9]+(\\.[0-9]+)?\\*?(:?[xX]\\^[+-]?[0-9]+(:?\\.[0-9]+)?)?");
	std::regex					pattern2("^[+-][0-9]+(\\.[0-9]+)?\\*[xX]\\^[+-]?[0-9]+(:?\\.[0-9]+)?");
	std::smatch					match1;
	std::smatch					match2;
	std::string					copy;
	std::string					temp;
	
	copy = input;
	copy.erase((std::remove_if(copy.begin(), copy.end(), [](unsigned char c){return (std::isspace(c));})), copy.end());
	if (std::isdigit(copy[0]))
		copy.insert(0, "+");
	try
	{
		p_fixSyntax(copy);
	}
	catch(const std::exception& e)
	{
		throw ParserError();
	}
	while (std::regex_search(copy, match1, pattern1))
	{
		if (match1.position() != 0)
			throw ParserError();
		temp = copy.substr(0, match1.length());
		if (!temp.compare("0"))
			temp.append("*X^0");
		if (std::regex_match(temp, match2, pattern2) == false)
			throw ParserError();
		split.push_back(temp);
		copy = match1.suffix().str();
		try
		{
			p_fixSyntax(copy);
		}
		catch(const std::exception& e)
		{
			throw ParserError();
		}
	}
	if (copy.size() != 0)
		throw ParserError();
	return (split);
}

void	InputParser::p_fixSyntax(std::string &copy)
{
	std::regex	pattern1("^(\\+X|\\+x|\\-X|\\-x)");
	std::regex	pattern2("^(\\+\\.|\\-\\.|\\.)");
	std::regex	pattern3("^[+-]?[0-9]+(:?\\.[0-9]+)?");
	std::regex	pattern4("^[+-]?[0-9]+(:?\\.[0-9]+)?\\*[xX]");
	std::smatch	match1;
	std::smatch	match2;
	std::smatch	match3;
	std::smatch	match4;

	if (copy[0] == 'X' || copy[0] == 'x')
		copy.insert(0, "+1*");
	else if (std::regex_search(copy, match1, pattern1) == true)
		copy.insert(1, "1*");
	if (std::regex_search(copy, match2, pattern2) == true)
	{
		if (copy[0] == '+' || copy[0] == '-')
			copy.insert(1, "0");
		else
			copy.insert(0, "+0");
	}
	if (std::regex_search(copy, match3, pattern3) == true)
	{
		if (copy[match3.length()] == '.')
			return ;
		if (copy[match3.length()] == 'x' || copy[match3.length()] == 'X')
			copy.insert(match3.length(), "*");
		if (copy[match3.length()] != '*')
			copy.insert(match3.length(), "*X^0");
	}
	if (std::regex_search(copy, match4, pattern4) == true)
		p_checkNext(copy, match4.length());
}

void	InputParser::p_checkNext(std::string &copy, ptrdiff_t position)
{
	std::regex	pattern1("^[+-]?[0-9]*(:?\\.[0-9]+)?\\*?[xX]\\^?[+-]?[0-9]?(:?\\.[0-9]+)?");
	std::regex	pattern2("^[+-]?\\.[0-9]+");
	std::regex	pattern3("^[+-]?[0-9]+(\\.[0-9]+)?\\*?(:?[xX]\\^[+-]?[0-9]+(:?\\.[0-9]+)?)?");
	std::string	sub;
	std::smatch	match1;
	std::smatch	match2;
	std::smatch	match3;

	if (copy.size() == 0)
		return ;
	sub = copy.substr(static_cast<size_t>(position));
	if ((copy.size() != 0 && sub.size() == 0) || std::regex_search(sub, match1, pattern1) == true)
	{
		copy.insert(static_cast<size_t>(position), "^1");
		return ;
	}
	sub.clear();
	sub = copy.substr(position + 1);
	if (copy[position] != '^' && (std::isdigit(copy[position]) || copy[position] == '+' || copy[position] == '-' || copy[position] == '.'))
	{
		if (copy[position] == '+' || copy[position] == '-')
			copy.insert(position, "^");
		else
			copy.insert(position, "^+");
	}
	sub.clear();
	sub = copy.substr(position + 1);
	if ((copy.size() != 0 && sub.size() != 0) && std::regex_search(sub, match2, pattern2) == true)
	{
		if ((copy[position + 1] == '+' || copy[position + 1] == '-'))
			copy.insert((position + 2), "0");
		else
			copy.insert((position + 1), "0");
		return ;
	}
	if (std::regex_search(copy, match3, pattern3) == true)
		return ;
	throw ParserError();
}

int	InputParser::p_justspaces(std::string temp)
{
	size_t	i;

	i = 0;
	while (temp[i])
	{
		if (!std::isspace(temp[i]))
			break ;
		i++;
	}
	if (i == temp.size())
		return (1);
	return (0);
}

void	InputParser::p_validateVectors(string_vector beforeEqual, string_vector afterEqual)
{
	for (std::string &snipet : beforeEqual)
	{
		if (p_validateInput(snipet) == false)
			throw ParserError();
	}
	for (std::string &snipet2 : afterEqual)
	{
		if (p_validateInput(snipet2) == false)
			throw ParserError();
	}
}

bool	InputParser::p_validateInput(std::string input)
{
	std::regex	pattern("^[-+]?[0-9]+\\.?[0-9]*\\*[xX]\\^[-+]?[0-9]+(:?\\.[0-9]+)?$");

	return (std::regex_match(input, pattern));
}

string_vector	InputParser::p_mergeVectors(string_vector beforeEqual, string_vector afterEqual)
{
	std::string	temp;

	for (std::string &content : afterEqual)
	{
		temp.clear();
		if (content[0] == '+' || content[0] == '-')
		{
			if (content[0] == '+')
				temp.push_back('-');
			else
				temp.push_back('+');
			temp.append(content.substr(1));
		}
		else
		{
			temp.push_back('-');
			temp.append(content);
		}
		beforeEqual.push_back(temp);
	}
	return (beforeEqual);
}

const char	*InputParser::ParserError::what() const throw()
{
	return ("The input provided is not valid, I can't solve it.");
}
