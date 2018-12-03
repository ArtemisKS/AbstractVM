/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVM.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdzhanaz <vdzhanaz@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 17:06:40 by akupriia          #+#    #+#             */
/*   Updated: 2018/12/03 02:58:54 by vdzhanaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AVM.hpp"

AVM::AVM(void) : to_quit(false), is_invalid(false), has_exit(false), exit_exec(false), line_num(0), 
oper("[ \t]*((push)|(assert))[ \t]+?((int8)|(int16)|(int32))[ \t]*?\\(([-]?[0-9]+)\\)[ \t]*([;].*)?"),
operfd("[ \t]*((push)|(assert))[ \t]+?((float)|(double))[ \t]*?\\(([-]?[0-9]+[.][0-9]+)\\)[ \t]*([;].*)?"),
cmd("[ \t]*((add)|(sub)|(mul)|(div)|(mod)|(pop)|(dump)|(print)|(exit)|(pmiddle)|(type)|(psum)|(sort)|(pmin)|(pmax)|(ppop)|(pback))[ \t]*([;].*)?"),
comment("[ \t]*([;].*)?") {
	opr_exec = {
		{"push", &AVM::push},
		{"assert", &AVM::assert},
	};
	cmd_exec = {
		{"add", &AVM::add},
		{"sub", &AVM::sub},
		{"mul", &AVM::mul},
		{"div", &AVM::div},
		{"mod", &AVM::mod},
		{"pop", &AVM::pop},
		{"dump", &AVM::dump},
		{"print", &AVM::print},
		{"exit", &AVM::exit},
		{"pmiddle", &AVM::pmiddle},
		{"psum", &AVM::psum},
		{"sort", &AVM::sort},
		{"type", &AVM::type},
		{"pmin", &AVM::pmin},
		{"pmax", &AVM::pmax},
		{"pback", &AVM::pback},
		{"ppop", &AVM::ppop}
	};
	types = {
		{"int8", Int8},
		{"int16", Int16},
		{"int32", Int32},
		{"float", Float},
		{"double", Double}
	};
	rev_types = {
		{Int8, "int8"},
		{Int16, "int16"},
		{Int32, "int32",},
		{Float, "float",},
		{Double, "double"}
	};
}

AVM::~AVM(void){}

AVM::AVM(AVM const &vm)
{
	if (this != &vm)
		*this = vm;
}

AVM &AVM::operator=(AVM const &vm)
{
	this->has_exit = vm.gotExit();
	this->to_quit = vm.getExit();
	this->is_invalid = vm.isInvalid();
	return (*this);
}

bool			AVM::getExit(void) const
{
	return (this->to_quit);
}

void			AVM::exit(void)
{
	to_quit = true;
}

bool			AVM::isInvalid(void) const
{
	if (!this->has_exit)
		throw AExitException();
	return (this->is_invalid);
}

/* =================================== EXCEPTIONS =================================== */

/************ AExitException ************/
AVM::AExitException::AExitException(){}
AVM::AExitException::AExitException(AExitException const &e){*this = e;}
AVM::AExitException &AVM::AExitException::operator=(AExitException const &e)
{
	exception::operator=(e);
	return *this;
}
AVM::AExitException::~AExitException() throw() {}
const char *AVM::AExitException::what() const throw()
{
	return ("ERROR: exit command absent");
}

/************ AssertException ************/
AVM::AssertException::AssertException(){}
AVM::AssertException::AssertException(std::string s1, std::string s2) : s1(s1), s2(s2) {}
AVM::AssertException::AssertException(AssertException const &e){*this = e;}
AVM::AssertException &AVM::AssertException::operator=(AssertException const &e)
{
	exception::operator=(e);
	return *this;
}
AVM::AssertException::~AssertException() throw() {}
const char *AVM::AssertException::what() const throw()
{
	std::string res = "ERROR: assertion failed (" + s1 + " != " + s2 + ")";
	return (res.c_str());
}

/************ DExitException ************/
AVM::DExitException::DExitException(){}
AVM::DExitException::DExitException(std::string s1)
{
	str = "ERROR: double exit on line #" + s1;
}
AVM::DExitException::DExitException(DExitException const &dee){*this = dee;}
AVM::DExitException &AVM::DExitException::operator=(DExitException const &dee)
{
	exception::operator=(dee);
	return *this;
}
AVM::DExitException::~DExitException() throw(){};
const char *AVM::DExitException::what() const throw()
{
	return (str.c_str());
}

/************ AfterExitException ************/
AVM::AfterExitException::AfterExitException(){}
AVM::AfterExitException::AfterExitException(std::string s1)
{
	str = "ERROR: command or operation after exit on line #" + s1;
}
AVM::AfterExitException::AfterExitException(AfterExitException const &aee){*this = aee;}
AVM::AfterExitException &AVM::AfterExitException::operator=(AfterExitException const &aee)
{
	exception::operator=(aee);
	return *this;
}
AVM::AfterExitException::~AfterExitException() throw(){};
const char *AVM::AfterExitException::what() const throw()
{
	return (str.c_str());
}

/************ EmptyStackException ************/
AVM::EmptyStackException::EmptyStackException(){}
AVM::EmptyStackException::EmptyStackException(std::string s1)
{
	str = "ERROR: empty stack on line #" + s1;
}
AVM::EmptyStackException::EmptyStackException(EmptyStackException const &ese){*this = ese;}
AVM::EmptyStackException &AVM::EmptyStackException::operator=(EmptyStackException const &ese)
{
	exception::operator=(ese);
	return *this;
}
AVM::EmptyStackException::~EmptyStackException() throw(){};
const char *AVM::EmptyStackException::what() const throw()
{
	return (str.c_str());
}

/************ LexicalException ************/
AVM::LexicalException::LexicalException(){}
AVM::LexicalException::LexicalException(std::string s1, std::string s2)
{
	str = "ERROR: lexical err on line #" + s1 + " --> " + s2;
}
AVM::LexicalException::LexicalException(LexicalException const &lex){*this = lex;}
AVM::LexicalException &AVM::LexicalException::operator=(LexicalException const &lex)
{
	exception::operator=(lex);
	return *this;
}
AVM::LexicalException::~LexicalException() throw(){};
const char *AVM::LexicalException::what() const throw()
{
	return (str.c_str());
}

/************ OperandsException ************/
AVM::OperandsException::OperandsException(){}
AVM::OperandsException::OperandsException(std::string s1)
{
	str = "ERROR: not enough operands for command exec on line #" + s1;
}
AVM::OperandsException::OperandsException(OperandsException const &ope){*this = ope;}
AVM::OperandsException &AVM::OperandsException::operator=(OperandsException const &ope)
{
	exception::operator=(ope);
	return *this;
}
AVM::OperandsException::~OperandsException() throw(){};
const char *AVM::OperandsException::what() const throw()
{
	return (str.c_str());
}

/************ PrintException ************/
AVM::PrintException::PrintException(){}
AVM::PrintException::PrintException(const char *s1) : s1(s1) {}
AVM::PrintException::PrintException(PrintException const &pe){*this = pe;}
AVM::PrintException &AVM::PrintException::operator=(PrintException const &pe)
{
	exception::operator=(pe);
	return *this;
}
AVM::PrintException::~PrintException() throw() {}
const char *AVM::PrintException::what() const throw()
{
	const char *strerr = "ERROR: print command failed --> ";
	const char *tmp = " is not Int8";
	int len = strlen(strerr) + strlen(s1) + strlen(tmp);
	char *res = new char[len];
	bzero(res, len);
	strcat(res, strerr) && strcat(res, s1) && strcat(res, tmp);
	return (res);
}

/* =================================== END_OF_EXCEPTIONS =================================== */

bool			AVM::gotExit(void) const
{
	return (this->has_exit);
}

void			AVM::setExit(bool val)
{
	this->to_quit = val;
}

void			AVM::setExitExec(bool val)
{
	this->exit_exec = val;
}

bool			AVM::getExitExec(void) const
{
	return (this->exit_exec);
}

void			AVM::zeroIter(void)
{
	this->line_num = 0;
}

void			AVM::clear_opcont(void)
{
	for(auto i = this->oper_cont.begin(); i != this->oper_cont.end(); i++)
		delete *i;
}

bool			AVM::checkEoSI(std::string const &line)
{
	if (regex_match(line, oper) || regex_match(line, operfd) || regex_match(line, cmd))
		return (false);
	else if (line.find(";;") != std::string::npos)
		return (true);
	return (false);
}

void			AVM::execute(std::string const &line)
{
	bool opfd = false;
	this->line_num++;
	try
	{
		if (this->getExitExec())
			throw AfterExitException(std::to_string(this->line_num));
		if (regex_match(line, oper) || (regex_match(line, operfd) && (opfd = true)))
		{
			int num_pos;
			(!opfd) ? std::regex_search(line.begin(), line.end(), smatch, oper)
			: std::regex_search(line.begin(), line.end(), smatch, operfd);
			num_pos = (!opfd) ? (8) : (7);
			// for (int i = 0; i < smatch.length(); i++)
			// 	std::cout << "smatch.str(" << i << ") :" << smatch.str(i) << '\n';
			(this->*opr_exec[smatch.str(1)])(getType(smatch.str(4)), smatch.str(num_pos));
		}
		else if (regex_match(line, cmd))
		{
			std::regex_search(line.begin(), line.end(), smatch, cmd);
			// for (int i = 0; i < smatch.length(); i++)
			// 	std::cout << "smatch.str(" << i << ") :" << smatch.str(i) << '\n';
			(this->*cmd_exec[smatch.str(1)])();
		}
		else if (regex_match(line, comment))
			return ;
		else if (line.find("exit") != std::string::npos)
		{
			this->setExitExec(true);
			this->setExit(true);
			return ;
		}
	}
	catch(PrintException& e)
	{
		const char *str = e.what();
		if (!this->getExit())
			this->setExit(true);
		// this->clear_opcont();
		std::cerr << RED << str << RESET <<  '\n';
		delete[] str;
	}
	catch(const std::exception& e)
	{
		if (!this->getExit())
			this->setExit(true);
		// this->clear_opcont();
		std::cerr << RED << e.what() << RESET <<  '\n';
	}
}

void			AVM::validate(std::string const &line)
{
	this->line_num++;
	if (!regex_match(line, oper) && !regex_match(line, operfd) && !regex_match(line, cmd)
	&& !regex_match(line, comment) && (is_invalid = true))
		throw LexicalException(std::to_string(this->line_num), line);
	else if (line.find("exit") != std::string::npos && regex_match(line, cmd))
	{
		if (this->has_exit && (is_invalid = true))
			throw DExitException(std::to_string(this->line_num));
		this->has_exit = true;
	}
}

eOperandType	AVM::getType(std::string const &type)
{
	return (types[type]);
}

void			AVM::bring_to_equality(std::string &back_v, std::string &ass_v,
	eOperandType otype, eOperandType type)
{
	if (type == Double || otype == Double)
	{
		ass_v = std::to_string(std::stod(ass_v));
		(otype != Double) ? back_v = std::to_string(std::stod(back_v)) : "";
	}
	else if (type == Float || otype == Float)
	{
		ass_v = std::to_string(std::stof(ass_v));
		(otype != Float) ? back_v = std::to_string(std::stof(back_v)) : "";
	}
	// std::cout << "back_v: " << back_v << "; ass_v: " << ass_v
	// << "; type: " << type << "; this->getType(back_v) :" << otype << std::endl;
}

void			AVM::assert(eOperandType type, std::string const &str)
{
	std::string back_v(this->oper_cont.back()->toString());
	std::string ass_v(str);
	eOperandType otype = this->oper_cont.back()->getType();
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num).c_str());
	const IOperand *op = factory.createOperand(type, str);
	delete op;
	this->bring_to_equality(back_v, ass_v, otype, type);
	if ((ass_v.compare(back_v)))
	{
		throw AssertException(ass_v, back_v);
	}
}

void			AVM::push(eOperandType type, std::string const &str)
{
	this->oper_cont.push_back(factory.createOperand(type, str));
}

void			AVM::add(void)
{
	if (this->oper_cont.size() < 2)
		throw OperandsException(std::to_string(this->line_num));
	const IOperand *x = this->oper_cont.back();
	this->oper_cont.pop_back();
	const IOperand *y = this->oper_cont.back();
	this->oper_cont.pop_back();
	try
	{
		const IOperand *z = *y + *x;
		this->push(z->getType(), z->toString());
		delete z;
	}
	catch (std::exception &e)
	{
		this->setExit(true);
		std::cout << RED << e.what() << ", line " << this->line_num << RESET << std::endl;
	}
	delete x;
	delete y;
}

void			AVM::sub(void)
{
	if (this->oper_cont.size() < 2)
		throw OperandsException(std::to_string(this->line_num));
	const IOperand *x = this->oper_cont.back();
	this->oper_cont.pop_back();
	const IOperand *y = this->oper_cont.back();
	this->oper_cont.pop_back();
	try
	{
		const IOperand *z = *y - *x;
		this->push(z->getType(), z->toString());
		delete z;
	}
	catch (std::exception &e)
	{
		this->setExit(true);
		std::cout << RED << e.what() << ", line " << this->line_num << RESET << std::endl;
	}
	delete x;
	delete y;
}

void			AVM::mul(void)
{
	if (this->oper_cont.size() < 2)
		throw OperandsException(std::to_string(this->line_num));
	const IOperand *x = this->oper_cont.back();
	this->oper_cont.pop_back();
	const IOperand *y = this->oper_cont.back();
	this->oper_cont.pop_back();
	try
	{
		const IOperand *z = *y * *x;
		this->push(z->getType(), z->toString());
		delete z;
	}
	catch (std::exception &e)
	{
		this->setExit(true);
		std::cout << RED << e.what() << ", line " << this->line_num << RESET << std::endl;
	}
	delete x;
	delete y;
}

void			AVM::div(void)
{
	if (this->oper_cont.size() < 2)
		throw OperandsException(std::to_string(this->line_num));
	const IOperand *x = this->oper_cont.back();
	this->oper_cont.pop_back();
	const IOperand *y = this->oper_cont.back();
	this->oper_cont.pop_back();
	try
	{
		const IOperand *z = *y / *x;
		this->push(z->getType(), z->toString());
		delete z;
	}
	catch (std::exception &e)
	{
		this->setExit(true);
		std::cout << RED << e.what() << ", line " << this->line_num << RESET << std::endl;
	}
	delete x;
	delete y;
}

void			AVM::mod(void)
{
	if (this->oper_cont.size() < 2)
		throw OperandsException(std::to_string(this->line_num));
	const IOperand *x = this->oper_cont.back();
	this->oper_cont.pop_back();
	const IOperand *y = this->oper_cont.back();
	this->oper_cont.pop_back();
	try
	{
		const IOperand *z = *y % *x;
		this->push(z->getType(), z->toString());
		delete z;
	}
	catch (std::exception &e)
	{
		this->setExit(true);
		std::cout << RED << e.what() << ", line " << this->line_num << RESET << std::endl;
	}
	delete x;
	delete y;
}

void			AVM::pop(void)
{
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	const IOperand *y = this->oper_cont.back();
	this->oper_cont.pop_back();
	delete y;
}

void			AVM::ppop(void)
{
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	const IOperand *op = this->oper_cont.back();
	std::cout << op->toString() << std::endl;
	this->oper_cont.pop_back();
	delete op;
}

void			AVM::pback(void)
{
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	std::cout << this->oper_cont.back()->toString() << std::endl;
}

void			AVM::dump(void)
{
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	for (int i = this->oper_cont.size() - 1; i >= 0; i--)
		std::cout << this->oper_cont[i]->toString() << std::endl;
}

void			AVM::pmiddle(void)
{
	double	res;
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	const IOperand *op = factory.createOperand(Int32, std::to_string(0));
	for(size_t i = 0; i < this->oper_cont.size(); i++)
	{
		IOperand *psum = (IOperand *)(*op + *(this->oper_cont[i]));
		delete op;
		op = psum;
	}
	res = std::stod(op->toString());
	res /= (double)this->oper_cont.size();
	std::cout << std::to_string(std::stod(std::to_string(res))) << std::endl;
	delete op;
}

void			AVM::pmax(void)
{
	if (!this->oper_cont.empty())
	{
		const IOperand *op = this->oper_cont[0];
		for(size_t i = 1; i < this->oper_cont.size(); i++)
			if (*this->oper_cont[i] > *op)
				op = this->oper_cont[i];
		std::cout << op->toString() << std::endl;
	}
}

void			AVM::print(void)
{
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	const IOperand *op = this->oper_cont.back();
	if (op->getType() != Int8)
		throw PrintException(op->toString().c_str());
	std::cout << static_cast<char>(std::stoi(op->toString())) << std::endl;
}

void			AVM::pmin(void)
{
	if (!this->oper_cont.empty())
	{
		const IOperand *op = this->oper_cont[0];
		for(size_t i = 1; i < this->oper_cont.size(); i++)
			if (*this->oper_cont[i] < *op)
				op = this->oper_cont[i];
		std::cout << op->toString() << std::endl;
	}
}

void			AVM::psum(void)
{
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	const IOperand *op = factory.createOperand(Int32, std::to_string(0));
	for(size_t i = 0; i < this->oper_cont.size(); i++)
	{
		IOperand *psum = (IOperand *)(*op + *(this->oper_cont[i]));
		delete op;
		op = psum;
	}
	std::cout << op->toString() << std::endl;
	delete op;
}

void			AVM::type(void)
{
	if (this->oper_cont.empty())
		throw EmptyStackException(std::to_string(this->line_num));
	const IOperand *op = this->oper_cont.back();
	std::cout << rev_types.at(op->getType()) << std::endl;
}

void			swap(const IOperand **x, const IOperand **y) 
{
	const IOperand *t = *x; 
	*x = *y; 
	*y = t;
}

void			quickSort(std::vector<const IOperand *> *arr, int left, int right) {
	int i = left;
	int j = right;
	const IOperand *pivot = (*arr)[(left + right) / 2];
	while (i <= j)
	{
		while (*(*arr)[i] < *pivot)
				i++;
		while (*(*arr)[j] > *pivot)
				j--;
		if (i <= j)
		{
			swap(&(*arr)[i], &(*arr)[j]);
			i++;
			j--;
		}
	}
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

void			AVM::sort(void)
{
	quickSort(&this->oper_cont, 0, this->oper_cont.size() - 1);
}
