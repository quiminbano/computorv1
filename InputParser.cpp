/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:20:30 by corellan          #+#    #+#             */
/*   Updated: 2024/03/04 17:34:30 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputParser.hpp"
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
	std::string								beforeEqualString;
	std::string								afterEqualString;
	std::stringstream						parserObject;
	string_vector							beforeEqualVector;
	string_vector							afterEqualVector;
	int										i;

	i = 0;
	beforeEqualString.clear();
	afterEqualString.clear();
	if (p_hasInput == false || p_hasFailed == true)
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
	std::regex					pattern("[+-]?[0-9]+(\\.[0-9]+)?\\*?(:?[xX]\\^[+-]?[0-9]+(:?\\.[0-9]+)?)?");
	std::smatch					match;
	std::string					copy;
	std::string					temp;
	
	copy = input;
	copy.erase((std::remove_if(copy.begin(), copy.end(), [](unsigned char c){return (std::isspace(c));})), copy.end());
	while (std::regex_search(copy, match, pattern))
	{
		if (match.position() != 0)
			throw ParserError();
		temp = copy.substr(0, match.length());
		if (!temp.compare("0"))
			temp.append("*X^0");
		split.push_back(temp);
		copy = match.suffix().str();
	}
	if (copy.size() != 0)
		throw ParserError();
	return (split);
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
