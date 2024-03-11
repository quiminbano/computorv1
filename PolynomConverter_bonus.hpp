/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolynomConverter_bonus.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:49:01 by corellan          #+#    #+#             */
/*   Updated: 2024/03/11 17:09:03 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLYNOMCONVERTER_BONUS_HPP
# define POLYNOMCONVERTER_BONUS_HPP

# include <vector>
# include <map>
# include <string>
# include <stdexcept>
# include <iostream>

typedef std::vector<std::string>					string_vector;
typedef std::pair<std::string, double>				pair_t;
typedef	std::vector<std::pair<std::string, double>>	vector_pair;
typedef enum e_sign
{
	PLUS,
	MINUS

}	t_sign;

class PolynomConverter
{
private:
	string_vector					p_input;
	string_vector					p_before;
	string_vector					p_after;
	bool							p_hasInput;
	bool							p_hasInputOverflow;
	bool							p_hasFractionalExponent;
	bool							p_hasFailedSolving;
	std::map<std::string, double>	p_polynom;
	long							p_grade;
	long							p_minExp;
	long							p_maxExp;
	double							p_solution1;
	double							p_solution2;
	double							p_discriminant;
	double							p_rootDiscriminant;
	double							p_realPart;
	double							p_imaginaryPart;

	PolynomConverter(PolynomConverter const &other);

	PolynomConverter	&operator=(PolynomConverter const &other);

	string_vector	p_split(std::string const &input, std::string const &needle);
	void			p_storeInMap();
	void			p_findExponents();
	void			p_solveLinear();
	void			p_solveQuadratic();
	void			p_calculateImaginary(double a, double b, double discriminant);
	void			p_solveGradeCero();
	void			p_printIrreductible(double number, bool toPrint, int grade, double numerator, double denominator);
	void			p_printIrreductibleImaginary(bool toPrint, double a, double b, double rootValue, t_sign sign);
	double			p_sqrt(double number);
	double			p_floor(double number);
	long long		p_pow(long long number, long long exponent);
	long long		p_gcd(long long numerator, long long denominator);
	vector_pair		p_getVectorPair(string_vector input);
	bool			p_isOverflowed(double number1, double number2, std::string sign);
	bool			p_isOverflowedInIrreductible(double number);
	void			p_printPairs(vector_pair paired);
	void			p_printMap();
	void			p_printFirstStep();
	void			p_printSecondStep();
	void			p_printGradeOneSteps();
	void			p_printGradeTwoSteps();
	bool			p_calculateUpAndDown(double number, double numerator, double denominator, long long &up, long long &down);
	double			p_correctZero(double number);

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
	PolynomConverter(string_vector input, string_vector before, string_vector after);
	~PolynomConverter();

	void	initializeInput(string_vector input, string_vector before, string_vector after);
	void	printPolynom();
	bool	getFractionalExponentFlag() const;
	bool	getHasSolutionFailed() const;
	void	printPolynomGrade();
	void	solvePolynom();
	void	printProcess();
	void	clear();
};

#endif