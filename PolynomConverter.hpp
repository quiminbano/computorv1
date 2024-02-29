/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:49:01 by corellan          #+#    #+#             */
/*   Updated: 2024/02/29 12:13:32 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLYNOMCONVERTER_HPP
# define POLYNOMCONVERTER_HPP

# include <vector>
# include <map>
# include <string>
# include <stdexcept>
# include <iostream>

typedef std::vector<std::string>	string_vector;
class PolynomConverter
{
private:
	string_vector					p_input;
	bool							p_hasInput;
	std::map<std::string, double>	p_polynom;
	long							p_grade;

	PolynomConverter(PolynomConverter const &other);

	PolynomConverter	&operator=(PolynomConverter const &other);

	string_vector	p_split(std::string const &input, std::string const &needle);

public:
	class EmptyInput : public std::exception
	{
	public:
		virtual const char	*what() const throw() override;
	};
	
	PolynomConverter();
	PolynomConverter(string_vector input);
	~PolynomConverter();

	void	initializeInput(string_vector input);
	void	clear();
};

#endif