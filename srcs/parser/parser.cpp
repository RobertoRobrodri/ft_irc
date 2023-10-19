/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:22:03 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/19 19:03:08 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser/parser.hpp"

std::vector<std::string> ft_split(std::string str, char separator)
{
	bool						flag = false;
	size_t						index, last_char;
	std::string 				segment;
	std::vector <std::string>	seglist;

	if ((index = str.find_first_not_of(separator)) != std::string::npos)
		str = str.substr(index);
	if ((index = str.find_last_not_of(separator) + 1) != std::string::npos)
		str = str.substr(0, index);
	while (!flag)
	{
		if ((index = str.find(separator)) != std::string::npos)
		{
			segment = str.substr(0, index);
			str = str.substr(index);
		}
		else
		{
			segment = str;
			flag = true;
		}
		if ((last_char = segment.find_last_of('\r') ) != std::string::npos)
			segment.erase(last_char);
 		seglist.push_back(segment);
		if ((index = str.find_first_not_of(separator)) != std::string::npos)
			str = str.substr(index);
	}
	return seglist;
}

bool	is_good_host(std::string host)
{
	std::stringstream test(host);
	std::string segment;
	std::vector <int>seglist;

	while (std::getline(test,segment,'.'))
	{
		if (segment.c_str() != std::to_string(atoi(segment.c_str())))
			seglist.push_back(-1);
		else
			seglist.push_back(atoi(segment.c_str()));
	}
	if (seglist.size() != 4)
		return (0);
	for (int i = 0; i < 4; i++)
		if (seglist[i] < 0 || seglist[i] > 255)
			return (0);
	return (1);
}

bool	is_good_port(std::string port)
{
	int port_i;

	port_i = atoi(port.c_str());
	if (port != std::to_string(port_i))
		return(0);
	if (port_i < 6000 || port_i > 7000)
		return (0);
	return (1);
}

bool	check_data_correct(char **argv)
{
	std::vector <std::string>seglist = ft_split(argv[1], ':');
	std::string arg_2(argv[2]), arg_3(argv[3]);

	if (seglist.size() != 3)
		return 0;
	if (seglist[0].empty() || !is_good_host(seglist[0]))
		return (0);
	if (seglist[1].empty() || !is_good_port(seglist[1]))
		return (0);
	if (seglist[2].empty())
		return (0);
	if (arg_2.empty() ||  !is_good_port(arg_2))
		return (0);
	if (arg_3.empty())
		return (0);
	return (1);
}
