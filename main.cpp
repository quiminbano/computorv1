/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 00:22:40 by corellan          #+#    #+#             */
/*   Updated: 2024/02/29 11:24:27 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputParser.hpp"
#include "PolynomConverter.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	InputParser			parser(argc, argv);
	PolynomConverter	converter;
	string_vector		input;

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
	converter.initializeInput(input);
	return (0);
}
