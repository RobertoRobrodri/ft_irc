/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:22:20 by crisfern          #+#    #+#             */
/*   Updated: 2023/10/23 15:07:09 by mzomeno-         ###   ########.fr       */
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
bool	check_data_correct(char **argv, int argc);

std::vector<std::string> ft_split(std::string str, char separator);
#endif
