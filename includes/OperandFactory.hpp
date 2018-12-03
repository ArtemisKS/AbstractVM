/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdzhanaz <vdzhanaz@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 23:13:33 by akupriia          #+#    #+#             */
/*   Updated: 2018/12/03 02:51:00 by vdzhanaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACTORY
# define FACTORY

# include "IOperand.hpp"

class OperandFactory
{
public:
	OperandFactory();
	OperandFactory(const OperandFactory &ref);
	OperandFactory& operator=(const OperandFactory &ref);

	~OperandFactory();

	IOperand const	*createOperand	(eOperandType type, std::string const &value) const;
	class OORException: public std::exception
	{
		private:
			std::string str;
		public:
			OORException(std::string value, std::string type)
			{
				str ="Value (" + value + ") overflows the span of " + type;
			}
			const char *what() const throw()
			{
				return str.c_str();
			}
	};
	class OORAVMException: public std::exception
	{
		private:
			std::string str;
		public:
			OORAVMException(std::string value, std::string type)
			{
				str = "Value (" + value + ") is bigger or smaller than boundaries of " + type;
			}
			const char *what() const throw()
			{
				return str.c_str();
			}
	};
private:
	IOperand const	*createInt8		(std::string const &value) const;
	IOperand const	*createInt16	(std::string const &value) const;
	IOperand const	*createInt32	(std::string const &value) const;
	IOperand const	*createFloat	(std::string const &value) const;
	IOperand const	*createDouble	(std::string const &value) const;
	
	typedef IOperand const	*(OperandFactory::*method)(std::string const & value) const;
	static const std::map<eOperandType, method> fact_map;
};

#endif