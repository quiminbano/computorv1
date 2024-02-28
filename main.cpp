/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 00:22:40 by corellan          #+#    #+#             */
/*   Updated: 2024/02/28 17:45:04 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputParser.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	InputParser		test(argc, argv);
	string_vector	input;

	if (test.fail() == true)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}
	try
	{
		input = test.getInput();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	
	return (0);
}
