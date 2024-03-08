/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:49:01 by corellan          #+#    #+#             */
/*   Updated: 2024/03/08 16:24:50 by corellan         ###   ########.fr       */
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
	bool							p_hasInputOverflow;
	bool							p_hasFractionalExponent;
	bool							p_isImaginary;
	std::map<std::string, double>	p_polynom;
	long							p_grade;
	long							p_minExp;
	long							p_maxExp;

	PolynomConverter(PolynomConverter const &other);

	PolynomConverter	&operator=(PolynomConverter const &other);

	string_vector	p_split(std::string const &input, std::string const &needle);
	void			p_storeInMap();
	void			p_findExponents();
	void			p_solveLinear();
	void			p_solveQuadratic();
	void			p_calculateImaginary(double a, double b, double discriminant);
	void			p_solveGradeCero();
	bool			p_isOverflowed(double number1, double number2, std::string sign);
	double			p_sqrt(double number);
	double			p_floor(double number);

public:
	class EmptyInput : public std::exception
	{
	public:
		virtual const char	*what() const throw() override;
	};
	class FloorError : public std::exception
	{
	public:
		virtual const char	*what() const throw() override;
	};
	
	PolynomConverter();
	PolynomConverter(string_vector input);
	~PolynomConverter();

	void	initializeInput(string_vector input);
	void	printPolynom();
	void	printPolynomGrade();
	void	solvePolynom();
	void	clear();
};

#endif