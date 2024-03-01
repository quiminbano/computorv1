/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:19:09 by corellan          #+#    #+#             */
/*   Updated: 2024/03/01 17:12:11 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTPARSER_HPP
# define INPUTPARSER_HPP

# include <string>
# include <vector>
# include <stdexcept>
# include <sstream>
# include <algorithm>
# include <regex>

typedef std::vector<std::string>	string_vector;
class InputParser
{
private:

	std::string					p_rawData;
	std::vector<std::string>	p_input;
	bool						p_hasInput;
	bool						p_hasFailed;

	InputParser(InputParser const &other);

	InputParser	&operator=(InputParser const &other);

	string_vector	p_getSplitted(std::string input);
	int				p_justspaces(std::string temp);
	void			p_validateVectors(string_vector beforeEqual, string_vector afterEqual);
	bool			p_validateInput(std::string input);
	string_vector	p_mergeVectors(string_vector beforeEqual, string_vector afterEqual);

public:
	class ParserError : public std::exception
	{
	public:
		virtual const char	*what() const throw() override;
	};
	InputParser();
	InputParser(int argc, char **argv);
	~InputParser();

	void			input(int argc, char **argv);
	bool			fail() const;
	string_vector	getInput();
};

#endif