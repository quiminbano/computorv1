/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 00:22:40 by corellan          #+#    #+#             */
/*   Updated: 2024/03/06 12:08:51 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputParser_bonus.hpp"
#include "PolynomConverter_bonus.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	InputParser			parser(argc, argv);
	PolynomConverter	converter;
	string_vector		input;
	string_vector		before;
	string_vector		after;
	std::string			userInputOptions;

	if (parser.fail() == true)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}
	try
	{
		input = parser.getInput();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	before = parser.getBefore();
	after = parser.getAfter();
	converter.initializeInput(input, before, after);
	try
	{
		converter.printPolynom();
		converter.printPolynomGrade();
		converter.solvePolynom();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	if (converter.getFractionalExponentFlag() == true || converter.getHasSolutionFailed() == true)
		return (1);
	while (1)
	{
		std::cout << "Would you like to check the procedure to reach this result? (Y/N)" << std::endl;
		std::getline(std::cin, userInputOptions);
		if (std::cin.eof() == true)
			return (0);
		std::cin.sync();
		std::transform(userInputOptions.begin(), userInputOptions.end(), userInputOptions.begin(), ::toupper);
		if (!userInputOptions.compare("N"))
			break ;
		else if (!userInputOptions.compare("Y"))
		{
			try 
			{
				converter.printProcess();
			}
			catch (std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				return (1);
			}
			break ;
		}
		std::cout << "Option entered is not valid, please try again." << std::endl;
	}
	return (0);
}
