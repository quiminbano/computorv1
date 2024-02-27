/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 00:22:40 by corellan          #+#    #+#             */
/*   Updated: 2024/02/28 00:40:57 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputParser.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	InputParser								test(argc, argv);
	std::pair<string_vector, string_vector>	pair;

	if (test.fail() == true)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}
	try
	{
		pair = test.getInput();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	for (std::string &i : pair.first)
		std::cout << i << std::endl;
	for (std::string &j : pair.second)
		std::cout << j << std::endl;
	return (0);
}
