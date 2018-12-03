/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akupriia <akupriia@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 23:09:23 by akupriia          #+#    #+#             */
/*   Updated: 2018/12/03 03:21:37 by akupriia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operand.hpp"
#include "OperandFactory.hpp"
#include "AVM.hpp"

#define PRINTFD 			true

static size_t		calc_mllen(std::vector<std::string> instr)
{
	size_t		mllen = 0;

	for (size_t i = 0; i < instr.size(); i++)
		if (mllen < instr[i].size())
			mllen = instr[i].size();
	return (mllen);
}

void			print_instr(std::string title, std::vector<std::string> instr)
{
	int width = std::to_string(instr.size() + 1).size();
	size_t	mllen = calc_mllen(instr) + 5 + width;

	std::cout << '\n' << B_YELLOW << title << std::endl;
	std::cout << B_GREEN << "* <" << std::setfill('=') << std::setw(mllen) << "> *"  << CYAN << std::endl;
	for (size_t i = 0; i < instr.size(); i++)
		std::cout << "   " << std::setfill('0') << std::setw(width)
		<< i + 1 << " " << instr[i] << std::endl;
	std::cout << B_GREEN << "* <" << std::setfill('=') << std::setw(mllen) << "> *" << RESET <<std::endl;
}

void	validate_and_execute(AVM &vm, std::vector<std::string> instr)
{
	for (size_t i = 0; i < instr.size(); i++)
	{
		try
		{
			vm.validate(instr[i]);
		}
		catch (std::exception &e)
		{
			std::cerr << RED << e.what() << RESET << std::endl;
		}
	}
	try
	{
		if (!vm.isInvalid())
		{
			vm.zeroIter();
			for(size_t i = 0; i < instr.size() && !vm.getExit(); i++)
			{
				vm.execute(instr[i]);
				if (vm.getExit())
				{
					vm.clear_opcont();
					break ;
				}
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
}

void	read_files(const char *fname)
{
	AVM							vm;
	std::string					line;
	std::vector<std::string>	instr;
	std::ifstream				file(fname);

	if (!file || file.fail())
	{
		std::cout << RED << "Error: can't open file " << RESET << std::endl;
		file.close();
	}
	else
	{
		while (std::getline(file, line))
			instr.push_back(line);
		if (PRINTFD)
			print_instr("Instructions to exec from" + std::string(fname), instr);
		validate_and_execute(vm, instr);
		file.close();
	}
}

void	read_std(void)
{
	AVM							vm;
	std::string					line;
	std::vector<std::string>	instr;

	while (true)
	{
		std::getline(std::cin, line);
		instr.push_back(line);
		if (vm.checkEoSI(line) || !std::cin)
			break ;
	}
	if (PRINTFD)
		print_instr("Instructions to exec from std", instr);
	validate_and_execute(vm, instr);
}

int	main(int ac, char **av)
{	
	if (ac == 1)
		read_std();
	else
		for (int i = 1; i < ac; i++)
			read_files(av[i]);
	// system("leaks -q avm");
	return (0);
}