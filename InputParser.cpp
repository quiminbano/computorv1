/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:20:30 by corellan          #+#    #+#             */
/*   Updated: 2024/02/28 00:48:42 by corellan         ###   ########.fr       */
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

pair_parser	InputParser::getInput()
{
	std::string								beforeEqualString;
	std::string								afterEqualString;
	std::stringstream						parserObject;
	string_vector							beforeEqualVector;
	string_vector							afterEqualVector;
	std::pair<string_vector, string_vector>	pairToReturn;
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
	beforeEqualVector = p_getSplitted(beforeEqualString);
	afterEqualVector = p_getSplitted(afterEqualString);
	pairToReturn = std::make_pair(beforeEqualVector, afterEqualVector);
	return (pairToReturn);
}

string_vector	InputParser::p_getSplitted(std::string input)
{
	string_vector	split;
	std::string		delimiters;
	std::string		temp;
	size_t			possition;


	split.clear();
	delimiters = "+-";
	temp.clear();
	possition = 0;
	while ((possition = input.find_first_of(delimiters)) != std::string::npos)
	{
		temp = input.substr(0, possition);
		split.push_back(temp);
		input.erase(0, possition + 1);
	}
	if (input.size() > 0)
		split.push_back(input);
	return (split);
}

const char	*InputParser::ParserError::what() const throw()
{
	return ("Error trying to return the parsed input");
}
