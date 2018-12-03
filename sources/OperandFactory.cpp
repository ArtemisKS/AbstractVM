/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akupriia <akupriia@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 23:09:28 by akupriia          #+#    #+#             */
/*   Updated: 2018/11/17 23:09:29 by akupriia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OperandFactory.hpp"
#include "Operand.hpp"

const std::map<eOperandType, OperandFactory::method> OperandFactory::fact_map ={
	{Int8, &OperandFactory::createInt8},
	{Int16, &OperandFactory::createInt16},
	{Int32, &OperandFactory::createInt32},
	{Float, &OperandFactory::createFloat},
	{Double, &OperandFactory::createDouble}
};

IOperand const	*OperandFactory::createDouble(std::string const &value) const
{
	double num;
	try
	{
		num = std::stod(value);
		return new Operand<double>(num);
	}
	catch (const std::out_of_range&)
	{		
		throw OORException(value, "Double");
	}
	catch (const std::exception&)
	{
		throw OORAVMException(value, "Double");
	}
	return (NULL);
}

IOperand const	*OperandFactory::createInt32(std::string const &value) const
{
	int num;
	try
	{
		num = std::stoi(value);
		return new Operand<int>(num);
	}
	catch (const std::out_of_range&)
	{
		throw OORException(value, "Int32");
	}
	catch (const std::exception&)
	{
		throw OORAVMException(value, "Int32");
	}
	return (NULL);
}

IOperand const	*OperandFactory::createInt16(std::string const &value) const
{
	int num;
	try
	{
		num = std::stoi(value);
		if (num > INT16_MAX || num < INT16_MIN)
			throw OORException(value, "Int16");
		return new Operand<short int>(static_cast<short>(num));
	}
	catch (const std::out_of_range&)
	{
		throw OORException(value, "Int16");
	}
	catch (const std::exception&)
	{
		throw OORAVMException(value, "Int16");
	}
	return (NULL);
}

IOperand const	*OperandFactory::createFloat(std::string const &value) const
{
	float num;
	try
	{
		num = std::stof(value);	
		return new Operand<float>(num);
	}
	catch (const std::out_of_range&)
	{
		throw OORException(value, "Float");
	}
	catch (const std::exception&)
	{
		throw OORAVMException(value, "Float");
	}
	return (NULL);
}

IOperand const	*OperandFactory::createInt8(std::string const &value) const
{
	int num;
	try
	{
		num = std::stoi(value);
		if (num > INT8_MAX || num < INT8_MIN)
			throw OORException(value, "Int8");
		return new Operand<char>(static_cast<char>(num));
	}
	catch (const std::out_of_range&)
	{
		throw OORException(value, "Int8");
	}
	catch (const std::exception&)
	{
		throw OORAVMException(value, "Int8");
	}
	return (NULL);
}

IOperand const	*OperandFactory::createOperand(eOperandType type, std::string const &value) const
{
	return (this->*fact_map.at(type))(value);
}

OperandFactory::OperandFactory() {}
OperandFactory::OperandFactory(const OperandFactory&) {}
OperandFactory & OperandFactory::operator=(const OperandFactory&) { return *this; }
OperandFactory::~OperandFactory() {}
