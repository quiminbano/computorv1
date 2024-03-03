/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:49:01 by corellan          #+#    #+#             */
/*   Updated: 2024/03/03 18:14:27 by corellan         ###   ########.fr       */
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
	void			p_solveCuadratic();
	void			p_solveGradeCero();
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