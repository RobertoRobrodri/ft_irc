#include "../parser/parser.hpp"

// HACER TEMPLATE PARA ACEPTAR DEVOLVER CUALQUIER TIPO DE VECTOR
std::vector<std::string> ft_split(std::string str, char separator)
{
	std::stringstream 	test(str);
	std::string 		segment;
	std::vector <std::string>seglist;

	while (std::getline(test, segment, separator))
		seglist.push_back(segment);
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
	// TODO optimizar esta mierda
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