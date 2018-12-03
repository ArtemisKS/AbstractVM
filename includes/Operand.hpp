/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdzhanaz <vdzhanaz@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 23:16:10 by akupriia          #+#    #+#             */
/*   Updated: 2018/12/03 02:51:04 by vdzhanaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_CLASS_HPP
# define OPERAND_CLASS_HPP

# include "IOperand.hpp"
# include "AVM.hpp"

template <typename T>
class Operand: public IOperand
{
public:
	class DivByZeroException: public std::exception
	{
		const char *what() const throw()
		{
			return ("Error: division by zero");
		}
	};

	Operand(void) : str("0"), type(Int8){}
	Operand(T value) :	str(std::to_string(value)), type(identifyType()){}
	Operand(Operand const &src) : 	str(src.toString()), type(src.getType()){}
	Operand(const IOperand *op)
	{
		this->str = (*op).toString();
		this->type = (*op).getType();
	}
	Operand				&operator=(Operand const &op)
	{
		this->str = op.toString();
		this->type = op.getType();
		return (*this);
	}
	Operand				&operator=(const IOperand *op)
	{
		this->str = (*op).toString();
		this->type = (*op).getType();
		return (*this);
	}
	~Operand(void){}
	
	int					getPrecision(void) const { return (static_cast<int>(type)); }

	eOperandType		getType(void) const { return (this->type); }

	template <typename U>
	const IOperand		*calculate(U a, U b, eOperandType type, char c) const
	{
		const IOperand	*res;
		if (c == '-')
			res = factory.createOperand(type, std::to_string(a - b));
		else if (c == '/')
			(b == 0) ? throw DivByZeroException() : res = factory.createOperand(type, std::to_string(a / b));
		else
			res = (typeid(U) == typeid(double) || typeid(U) == typeid(float)) ? factory.createOperand(type, std::to_string(fmod(a, b)))
			: factory.createOperand(type, std::to_string(static_cast<int>(a) % static_cast<int>(b)));
		return (res);
	}

	template <typename U, typename V>
	bool				make_comp(U a, V b, char s) const
	{
		bool res;
		switch (s)
		{
			case '=':
				res = (a == b);
				break;
			case '<':
				res = (a < b);
				break ;
			default:
				res = (a > b);
				break ;
		}
		return (res);
	}

	const IOperand		*operator+(IOperand const &op) const
	{
		const IOperand *res;
		res = (op.getPrecision() > this->getPrecision()) ? (op + *this) : (factory.createOperand(this->type,
			std::to_string(conv_s_to_type(op.toString()) + conv_s_to_type(this->str))));
		return (res);
	}

	const IOperand		*oper_div_min(IOperand const &op, char c) const
	{
		const IOperand *res;
		switch(op.getType())
		{
			case Int16:
				res = calculate(static_cast<short int>(std::stoi(str)),
				static_cast<short int>(std::stoi(op.toString())), op.getType(), c);
				break ;
			case Int32:
				res = calculate(std::stoi(str), std::stoi(op.toString()), op.getType(), c);
				break ;
			case Float:
				res = calculate(std::stof(str), std::stof(op.toString()), op.getType(), c);
				break ;
			default:
				res = calculate(std::stod(str), std::stod(op.toString()), op.getType(), c);
				break ;
		}
		return (res);
	}

	const IOperand		*operator-(IOperand const &op) const
	{
		const IOperand *res;
		if (op.getPrecision() > this->getPrecision())
			res = oper_div_min(op, '-');
		else
			res = factory.createOperand(this->type, std::to_string(conv_s_to_type(this->str) - conv_s_to_type(op.toString())));
		return (res);
	}

	const IOperand		*operator*(IOperand const &op) const
	{
		const IOperand *res;
		if (op.getPrecision() > this->getPrecision())
			res = op * *this;
		else
			res = factory.createOperand(this->type, std::to_string(conv_s_to_type(op.toString()) * conv_s_to_type(this->str)));
		return (res);
	}

	const IOperand		*operator/(IOperand const &op) const
	{
		const IOperand *res;
		if (!conv_s_to_type(op.toString()))
			throw DivByZeroException();
		else if (op.getPrecision() > this->getPrecision())
			res = oper_div_min(op, '/');
		else
			res = factory.createOperand(this->type, std::to_string(conv_s_to_type(this->str) / conv_s_to_type(op.toString())));
		return (res);		
	}

	const IOperand		*operator%(IOperand const &op) const
	{
		const IOperand *res;
		if (!conv_s_to_type(op.toString()))
				throw DivByZeroException();
		else if (op.getPrecision() > this->getPrecision())
			res = oper_div_min(op, '%');
		else
			res = factory.createOperand(this->type, std::to_string(fmod(conv_s_to_type(this->str), conv_s_to_type(op.toString()))));
		return (res);		
	}

	bool				comp_oper(IOperand const &op, const char c) const
	{
		bool res;
		switch(this->getType())
		{
			case Int8:
				res = make_comp(conv_s_to_type(this->str), static_cast<char>(std::stoi(op.toString())), c);
				break ;
			case Int16:
				res = make_comp(conv_s_to_type(this->str), static_cast<short int>(std::stoi(op.toString())), c);
				break ;
			case Int32:
				res = make_comp(conv_s_to_type(this->str), std::stoi(op.toString()), c);
				break ;
			case Float:
				res = make_comp(conv_s_to_type(this->str), std::stof(op.toString()), c);
				break ;
			default:
				res = make_comp(conv_s_to_type(this->str), std::stod(op.toString()), c);
				break ;
		}
		return (res);
	}

	bool				operator==(IOperand const &op) const
	{
		return (comp_oper(op, '='));
	}

	bool				operator>(IOperand const &op) const
	{
		return (comp_oper(op, '>'));
	}
	
	bool				operator<(IOperand const &op) const
	{
		return (comp_oper(op, '<'));
	}

	std::string const	&toString(void) const
	{
		return (str);
	}
private:
	std::string		str;
	OperandFactory	factory;
	eOperandType	type;

	T				conv_s_to_type(std::string str) const
	{
		T res;
		if (type == Int8)
			res = static_cast<char>(std::stoi(str));
		else if (type == Int16)
			res = static_cast<short int>(std::stoi(str));
		else if (type == Int32)
			res = std::stoi(str);
		else if (type == Float)
			res = std::stof(str);
		else
			res = std::stod(str);
		return (res);
	}

	eOperandType	identifyType(void)
	{
		enum eOperandType res;
		try
		{
			const std::type_info& ti = typeid(T);
			if (ti == typeid(char))
				res = Int8;
			else if (ti == typeid(short int))
				res = Int16;
			else if (ti == typeid(int))
				res = Int32;
			else if (ti == typeid(float))
				res = Float;
			else if (ti == typeid(double))
				res = Double;
			else
				throw std::exception();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		return (res);
	}
};

#endif
