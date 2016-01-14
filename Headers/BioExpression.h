/*
 * BioExpression.h
 *
 *  Created on: Jan 13, 2016
 *      Author: Chris
 */

#ifndef BIOEXPRESSION_H_
#define BIOEXPRESSION_H_
#include<iostream>
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
	BioExpression(BioExpression* lhs, ConditionalOps condition, BioExpression* rhs)
	:_fucnt(NULL),_lhs(lhs), _rhs(rhs),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(-1), _constant(-1)
	{
		if(condition != AND && condition != OR){
			std::cerr<<"Error: Constructor not valid for specified operation. Expected to see AND/OR.\n";
			exit(-1);
		}
	}

	BioExpression(ConditionalOps condition, BioExpression* notExpression)
	:_fucnt(NULL),_lhs(notExpression), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(-1), _constant(-1)
	{
		if(condition != NOT){
			std::cerr<<"Error: Constructor not valid for specified operation. Expected to see NOT.\n";
			exit(-1);
		}

	}

	//terminal Expressions
	BioExpression(BioOperation* lhs, ConditionalOps condition, BioOperation* rhs)
	:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(lhs),_terminalRHS(rhs), _variable(-1), _constant(-1)
	{
		if(condition != NOT){
			std::cerr<<"Error: Constructor not valid for specified operation. Expected to see NOT.\n";
			exit(-1);
		}
	}
	BioExpression(BioOperation* lhs, ConditionalOps condition, double constant)
	:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(lhs),_terminalRHS(NULL), _variable(-1), _constant(constant)
	{
		if(condition == NOT){
			std::cerr<<"Error: Constructor not valid for NOT operator.\n";
			exit(-1);
		}
		else if( condition == AND) {
			std::cerr<<"Error: Constructor not valid for AND operator.\n";
			exit(-1);
		}
		else if ( condition == OR) {
			std::cerr<<"Error: Constructor not valid for OR operator.\n";
			exit(-1);
		}

	}
	BioExpression(double variable,ConditionalOps condition, double constant, incrementor funct)
	:_fucnt(funct),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(variable), _constant(constant)
	{
		if(condition == NOT){
			std::cerr<<"Error: Constructor not valid for NOT operator.\n";
			exit(-1);
		}
		else if( condition == AND) {
			std::cerr<<"Error: Constructor not valid for AND operator.\n";
			exit(-1);
		}
		else if ( condition == OR) {
			std::cerr<<"Error: Constructor not valid for OR operator.\n";
			exit(-1);
		}
	}

};

}

#endif /* BIOEXPRESSION_H_ */
