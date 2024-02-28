/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:49:01 by corellan          #+#    #+#             */
/*   Updated: 2024/02/28 18:06:09 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLYNOMCONVERTER_HPP
# define POLYNOMCONVERTER_HPP

# include <vector>
# include <string>

typedef std::vector<std::string>	string_vector;
class PolynomConverter
{
private:
	PolynomConverter(PolynomConverter const &other);

	PolynomConverter	&operator=(PolynomConverter const &other);

public:
	PolynomConverter();
	PolynomConverter(string_vector input);
	~PolynomConverter();
};

#endif