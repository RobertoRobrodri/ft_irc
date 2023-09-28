/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:22:20 by crisfern          #+#    #+#             */
/*   Updated: 2023/09/28 11:22:23 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>

bool	is_good_host(std::string host);
bool	is_good_port(std::string port);
bool	check_data_correct(char **argv);

std::vector<std::string> ft_split(std::string str, char separator);
#endif
