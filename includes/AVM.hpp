/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVM.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdzhanaz <vdzhanaz@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 17:06:45 by akupriia          #+#    #+#             */
/*   Updated: 2018/12/03 02:50:43 by vdzhanaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_HPP
# define AVM_HPP

# include "IOperand.hpp"
# include "OperandFactory.hpp"
#define RESET				"\033[0m"
#define B_CYAN				"\033[1;36m"
#define CYAN				"\033[0;36m"
#define B_YELLOW			"\033[1;33m"
#define YELLOW				"\033[0;33m"
#define BLUE				"\033[0;34m"
#define B_BLUE				"\033[1;34m"
#define GREEN				"\033[0;32m"
#define B_GREEN				"\033[1;32m"
#define PURPLE				"\033[0;35m"
#define B_PURPLE			"\033[1;35m"
#define RED					"\033[0;31m"
#define B_RED				"\033[1;31m"

class AVM
{
public:
	AVM(void);
	AVM(AVM const &vm);
	AVM &operator=(AVM const &vm);
	~AVM(void);

	void			push(eOperandType, std::string const&);
	void			bring_to_equality(std::string &, std::string &, eOperandType, eOperandType);
	void			assert(eOperandType, std::string const&);

	void			sub(void);
	void			div(void);
	void			add(void);
	void			mod(void);
	void			mul(void);
	void			type(void);
	void			sort(void);
	void			print(void);
	void			pmin(void);
	void			pop(void);
	void			pmax(void);
	void			exit(void);
	void			pmiddle(void);
	void			dump(void);
	void			ppop(void);
	void			pback(void);
	void			psum(void);
	bool			checkEoSI(std::string const&);

	void			zeroIter(void);
	void			execute(std::string const&);
	void			validate(std::string const&);
	eOperandType	getType(std::string const&);
	bool			getExit(void) const;
	bool			getExitExec(void) const;
	bool			gotExit(void) const;
	void			setExit(bool);
	void			setExitExec(bool);
	bool			isInvalid(void) const;
	void			clear_opcont(void);
private:
	OperandFactory																		factory;
	std::vector<const IOperand *>														oper_cont;
	bool																				to_quit;
	bool																				is_invalid;
	bool																				has_exit;
	bool																				exit_exec;
	unsigned int																		line_num;
	std::smatch 																		smatch;
	std::regex																			oper;
	std::regex																			operfd;
	std::regex																			cmd;
	std::regex																			comment;
	std::map <std::string, void (AVM::*)(eOperandType, std::string const&)>	opr_exec;
	std::map <std::string, void (AVM::*)()>												cmd_exec;
	std::map <std::string, eOperandType>												types;
	std::map <eOperandType, std::string>												rev_types;

	class AExitException: public std::exception
	{
		private:
		public:
			AExitException();
			AExitException(AExitException const &ae);
			AExitException &operator=(AExitException const &ae);
			~AExitException() throw();
			virtual const char *what() const throw();
	};

	class AssertException: public std::exception
	{
		private:
			std::string	s1;
			std::string s2;
		public:
			AssertException();
			AssertException(std::string, std::string);
			AssertException(AssertException const &ae);
			AssertException &operator=(AssertException const &ae);
			~AssertException() throw();
			virtual const char *what() const throw();
	};

	class AfterExitException: public std::exception
	{
		private:
			std::string str;
		public:
			AfterExitException();
			AfterExitException(std::string);
			AfterExitException(AfterExitException const &dee);
			AfterExitException &operator=(AfterExitException const &ae);
			~AfterExitException() throw();
			virtual const char *what() const throw();
	};

	class DExitException: public std::exception
	{
		private:
			std::string str;
		public:
			DExitException();
			DExitException(std::string);
			DExitException(DExitException const &dee);
			DExitException &operator=(DExitException const &ae);
			~DExitException() throw();
			virtual const char *what() const throw();
	};

	class EmptyStackException: public std::exception
	{
		private:
			std::string str;
		public:
			EmptyStackException();
			EmptyStackException(std::string);
			EmptyStackException(EmptyStackException const &dee);
			EmptyStackException &operator=(EmptyStackException const &ae);
			~EmptyStackException() throw();
			virtual const char *what() const throw();
	};

	class LexicalException: public std::exception
	{
		private:
			std::string str;
		public:
			LexicalException();
			LexicalException(std::string, std::string);
			LexicalException(LexicalException const &dee);
			LexicalException &operator=(LexicalException const &ae);
			~LexicalException() throw();
			virtual const char *what() const throw();
	};

	class OperandsException: public std::exception
	{
		private:
			std::string str;
		public:
			OperandsException();
			OperandsException(std::string);
			OperandsException(OperandsException const &dee);
			OperandsException &operator=(OperandsException const &ae);
			~OperandsException() throw();
			virtual const char *what() const throw();
	};

	class PrintException: public std::exception
	{
		private:
			const char *s1;
		public:
			PrintException();
			PrintException(const char *);
			PrintException(PrintException const &ae);
			PrintException &operator=(PrintException const &ae);
			~PrintException() throw();
			virtual const char *what() const throw();
	};
};

#endif