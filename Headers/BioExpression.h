/*
 * BioExpression.h
 *
 *  Created on: Jan 13, 2016
 *      Author: Chris
 */

#ifndef BIOEXPRESSION_H_
#define BIOEXPRESSION_H_
#include<iostream>
#include<ostream>
#include<cstdlib>

#include "BioOperation.h"

namespace BioCoder
{

class BioOperation;

enum ConditionalOps { GREATER_THAN, GREATER_THAN_or_EQUAL, LESS_THAN, LESS_THAN_or_EQUAL, EQUAL, NOT_EQUAL, AND, OR, NOT };


typedef void (*incrementor)(double);

struct BioExpression
{
	incrementor _fucnt;
	BioExpression * _lhs;
	BioExpression * _rhs;
	ConditionalOps _conditionalOperation;
	BioOperation * _terminalLHS;
	BioOperation * _terminalRHS;
	double _variable;
	double _constant;


	//used for creating complex Expressions.
	BioExpression(BioExpression* lhs, ConditionalOps condition, BioExpression* rhs);

	BioExpression(ConditionalOps condition, BioExpression* notExpression);

	//terminal Expressions
	BioExpression(BioOperation* lhs, ConditionalOps condition, BioOperation* rhs);

	BioExpression(BioOperation* lhs, ConditionalOps condition, double constant);

	BioExpression(double variable,ConditionalOps condition, double constant, incrementor funct);

};


std::ostream& operator<<(std::ostream& os, const  BioExpression& obj);


}

#endif /* BIOEXPRESSION_H_ */
