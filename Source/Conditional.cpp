#include "../Headers/Conditional.h"

#include <cstdlib>
namespace BioCoder
{

Conditional::Conditional(BioConditionalOperators op, Conditional* cond) // NOT OPERATOR
: _sensor(NULL),_sensor2(NULL),_sensorConstant(-1), _lhs(NULL), _rhs(NULL), _doubleVariLHS(NULL), _intVariLHS(NULL), _doubleVariRHS(NULL), _intVariRHS(NULL), _op(op)
{
	if(op != CONDITIONAL_NOT){
		std::cerr<<"Error: operator not supported. Expected NOT operator."<<std::endl;
	}

}

Conditional::Conditional(Conditional* lhs, BioConditionalOperators op, Conditional* rhs) // BOOLEAN OPERATIONS
: _sensor(NULL),_sensor2(NULL),_sensorConstant(-1), _lhs(lhs), _rhs(rhs), _doubleVariLHS(NULL), _intVariLHS(NULL), _doubleVariRHS(NULL), _intVariRHS(NULL), _op(op)
{
	std::string operation = "";
	switch(op)
	{
	case CONDITIONAL_NOT:
		operation = "NOT";
		break;
	case CONDITIONAL_GT:
		operation = "GT";
		break;
	case CONDITIONAL_GTE:
		operation = "GTE";
		break;
	case CONDITIONAL_LT:
		operation = "LT";
		break;
	case CONDITIONAL_LTE:
		operation = "LTE";
		break;
	case CONDITIONAL_EQ:
		operation = "EQ";
		break;
	case CONDITIONAL_NEQ:
		operation = "NEQ";
		break;
	default:
		break;
	}

	if(operation != "") {
		std::cerr<<operation << "not supported for composite conditional statement." <<std::endl;
		exit(-1);
	}
}

Conditional::Conditional(BioOperation* sensor, BioConditionalOperators op, double constantNum) // SENSOR CONDTIONAL
: _sensor(sensor),_sensor2(NULL), _sensorConstant(constantNum), _lhs(NULL), _rhs(NULL), _doubleVariLHS(NULL), _intVariLHS(NULL), _doubleVariRHS(NULL), _intVariRHS(NULL), _op(op)
{
	if(sensor == NULL) {
		std::cerr<<"Error: Invalid Sensor. Reading NULL." << std::endl;
	}

	std::string operation = "";
	switch(op)
	{
	case CONDITIONAL_NOT:
		operation = "NOT";
		break;
	case CONDITIONAL_AND:
		operation = "AND";
		break;
	case CONDITIONAL_NAND:
		operation = "NAND";
		break;
	case CONDITIONAL_OR:
		operation = "OR";
		break;
	case CONDITIONAL_NOR:
		operation = "NOR";
		break;
	case CONDITIONAL_XOR:
		operation = "XOR";
		break;
	default:
		break;
	}

	if(operation != "") {
		std::cerr<<operation << "not supported for comparison conditional statement." <<std::endl;
		exit(-1);
	}
}

Conditional::Conditional(BioOperation* sensor, BioConditionalOperators op, BioOperation* sensor2) // SENSOR v SENSOR READING
: _sensor(sensor),_sensor2(sensor2), _sensorConstant(-1), _lhs(NULL), _rhs(NULL), _doubleVariLHS(NULL), _intVariLHS(NULL), _doubleVariRHS(NULL), _intVariRHS(NULL), _op(op)
{
	if(sensor == NULL) {
		std::cerr<<"Error: Invalid Sensor 1. Reading NULL." << std::endl;
	}

	if(sensor2 == NULL) {
		std::cerr<<"Error: Invalid Sensor 2. Reading NULL." << std::endl;
	}

	std::string operation = "";
	switch(op)
	{
	case CONDITIONAL_NOT:
		operation = "NOT";
		break;
	case CONDITIONAL_AND:
		operation = "AND";
		break;
	case CONDITIONAL_NAND:
		operation = "NAND";
		break;
	case CONDITIONAL_OR:
		operation = "OR";
		break;
	case CONDITIONAL_NOR:
		operation = "NOR";
		break;
	case CONDITIONAL_XOR:
		operation = "XOR";
		break;
	default:
		break;
	}

	if(operation != "") {
		std::cerr<<operation << "not supported for comparison conditional statement." <<std::endl;
		exit(-1);
	}
}

Conditional::Conditional(double lhs, BioConditionalOperators op, double rhs) // VARIABLE OPERATOR
:_sensor(NULL),_sensor2(NULL), _sensorConstant(-1), _lhs(NULL), _rhs(NULL), _doubleVariLHS(new double(lhs)), _intVariLHS(NULL), _doubleVariRHS(new double(rhs)), _intVariRHS(NULL), _op(op)
{
	std::string operation = "";
	switch(op)
	{
	case CONDITIONAL_NOT:
		operation = "NOT";
		break;
	case CONDITIONAL_AND:
		operation = "AND";
		break;
	case CONDITIONAL_NAND:
		operation = "NAND";
		break;
	case CONDITIONAL_OR:
		operation = "OR";
		break;
	case CONDITIONAL_NOR:
		operation = "NOR";
		break;
	case CONDITIONAL_XOR:
		operation = "XOR";
		break;
	default:
		break;
	}

	if(operation != "") {
		std::cerr<<operation << "not supported for comparison conditional statement." <<std::endl;
		exit(-1);
	}

}

Conditional::Conditional(int lhs, BioConditionalOperators op, int rhs)  // VARIABLE OPERATOR
:_sensor(NULL),_sensor2(NULL), _sensorConstant(-1), _lhs(NULL), _rhs(NULL), _doubleVariLHS(NULL), _intVariLHS(new int(lhs)), _doubleVariRHS(NULL), _intVariRHS(new int(rhs)), _op(op)
{
	std::string operation = "";
	switch(op)
	{
	case CONDITIONAL_NOT:
		operation = "NOT";
		break;
	case CONDITIONAL_AND:
		operation = "AND";
		break;
	case CONDITIONAL_NAND:
		operation = "NAND";
		break;
	case CONDITIONAL_OR:
		operation = "OR";
		break;
	case CONDITIONAL_NOR:
		operation = "NOR";
		break;
	case CONDITIONAL_XOR:
		operation = "XOR";
		break;
	default:
		break;
	}

	if(operation != "") {
		std::cerr<<operation << "not supported for comparison conditional statement." <<std::endl;
		exit(-1);
	}

}


}
