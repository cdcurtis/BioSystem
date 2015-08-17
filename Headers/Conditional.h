/*
 * Conditional.h
 *
 *  Created on: Jul 31, 2015
 *      Author: Chris
 */

#ifndef CONDITIONAL_H_
#define CONDITIONAL_H_

#include <iostream>
#include "BioOperation.h"



namespace BioCoder
{


enum BioConditionalOperators {CONDITIONAL_NOT, CONDITIONAL_AND, CONDITIONAL_OR,  CONDITIONAL_XOR, CONDITIONAL_NOR, CONDITIONAL_NAND, CONDITIONAL_LT, CONDITIONAL_LTE, CONDITIONAL_EQ, CONDITIONAL_NEQ, CONDITIONAL_GT, CONDITIONAL_GTE};

class Conditional
{
private:
	BioOperation * _sensor;
	BioOperation * _sensor2;
	double _sensorConstant;

	Conditional * _lhs;
	Conditional * _rhs;

	double* _doubleVariLHS;
	int* _intVariLHS;

	double* _doubleVariRHS;
	int* _intVariRHS;

	BioConditionalOperators _op;
public:
	Conditional(BioConditionalOperators op, Conditional* cond); // NOT OPERATOR

	Conditional(Conditional* lhs, BioConditionalOperators op, Conditional* rhs); // COMPOSITE CONDITIONAL STATEMENT

	Conditional(BioOperation* sensor, BioConditionalOperators op, double constantNum); // SENSOR READING

	Conditional(BioOperation* sensor, BioConditionalOperators op, BioOperation* sensor2); // SENSOR v SENSOR READING

	Conditional(double, BioConditionalOperators op, double ); // VARIABLE OPERATOR

	Conditional(int , BioConditionalOperators op, int); // VARIABLE OPERATOR

};

}

#endif /* CONDITIONAL_H_ */
