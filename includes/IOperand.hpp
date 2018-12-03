/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOperand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akupriia <akupriia@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 23:14:16 by akupriia          #+#    #+#             */
/*   Updated: 2018/12/03 03:02:38 by akupriia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOPERAND_HPP
# define IOPERAND_HPP

# include <iostream>
# include <vector>
# include <map>
# include <cmath>
#include <fstream>
#include <iomanip>
#include <regex>
#include <climits>
#include <stdint.h>

enum eOperandType {Int8, Int16, Int32, Float, Double};

class IOperand
{
public:
	virtual int					getPrecision(void) const = 0;
	virtual eOperandType		getType(void) const = 0;

	virtual IOperand const*		operator+(IOperand const &op) const = 0;
	virtual IOperand const*		operator-(IOperand const &op) const = 0;
	virtual IOperand const*		operator*(IOperand const &op) const = 0;
	virtual IOperand const*		operator/(IOperand const &op) const = 0;
	virtual IOperand const*		operator%(IOperand const &op) const = 0;
	virtual bool				operator==(IOperand const &op) const = 0;
	virtual bool				operator>(IOperand const &op) const = 0;
	virtual bool				operator<(IOperand const &op) const = 0;

	virtual std::string const	&toString(void) const = 0;
	virtual						~IOperand(void){}
};

#endif
