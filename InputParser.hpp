/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:19:09 by corellan          #+#    #+#             */
/*   Updated: 2024/02/28 00:40:27 by corellan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTPARSER_HPP
# define INPUTPARSER_HPP

# include <string>
# include <vector>
# include <stdexcept>
# include <sstream>

typedef std::vector<std::string>				string_vector;
typedef std::pair<string_vector, string_vector>	pair_parser;
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

public:
	class ParserError : public std::exception
	{
	public:
		virtual const char	*what() const throw() override;
	};
	InputParser();
	InputParser(int argc, char **argv);
	~InputParser();

	void		input(int argc, char **argv);
	bool		fail() const;
	pair_parser	getInput();
};

#endif