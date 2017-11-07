/*
 * BioExpression.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: Chris
 */



#include "../Headers/BioExpression.h"
namespace BioCoder
{
BioExpression:: BioExpression(BioExpression* lhs, ConditionalOps condition, BioExpression* rhs)
:_fucnt(NULL),_lhs(lhs), _rhs(rhs),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(-1), _constant(-1),_lhsNickName(""),_rhsNickname("")
{
	if(condition != AND && condition != OR){
		std::cerr<<"Error: Constructor not valid for specified operation. Expected to see AND/OR.\n";
		exit(-1);
	}
}

BioExpression:: BioExpression(ConditionalOps condition, BioExpression* notExpression)
:_fucnt(NULL),_lhs(NULL), _rhs(notExpression),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(-1), _constant(-1),_lhsNickName(""),_rhsNickname("")
{
	if(condition != NOT){
		std::cerr<<"Error: Constructor not valid for specified operation. Expected to see NOT.\n";
		exit(-1);
	}

}

BioExpression::BioExpression(std::string lhs, ConditionalOps condition, std::string rhs)
:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(-1), _constant(-1),_lhsNickName(lhs),_rhsNickname(rhs)
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

BioExpression:: BioExpression(BioOperation* lhs, ConditionalOps condition, std::string rhs)
:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(lhs),_terminalRHS(NULL), _variable(-1), _constant(-1),_lhsNickName(""),_rhsNickname(rhs)
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

BioExpression:: BioExpression(std::string lhs, ConditionalOps condition,  BioOperation* rhs)
:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(rhs), _variable(-1), _constant(-1),_lhsNickName(lhs),_rhsNickname("")
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

BioExpression:: BioExpression(std::string lhs , ConditionalOps condition, double constant)
:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(-1), _constant(constant),_lhsNickName(lhs),_rhsNickname("")
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


//terminal Expressions
BioExpression:: BioExpression(BioOperation* lhs, ConditionalOps condition, BioOperation* rhs)
:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(lhs),_terminalRHS(rhs), _variable(-1), _constant(-1),_lhsNickName(""),_rhsNickname("")
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

BioExpression:: BioExpression(BioOperation* lhs, ConditionalOps condition, double constant)
:_fucnt(NULL),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(lhs),_terminalRHS(NULL), _variable(-1), _constant(constant),_lhsNickName(""),_rhsNickname("")
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

BioExpression:: BioExpression(double variable,ConditionalOps condition, double constant, incrementor funct)
:_fucnt(funct),_lhs(NULL), _rhs(NULL),_conditionalOperation(condition), _terminalLHS(NULL),_terminalRHS(NULL), _variable(variable), _constant(constant),_lhsNickName(""),_rhsNickname("")
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

std::ostream& operator<<(std::ostream& os, const  BioExpression& obj)
{
	if (obj._lhs != NULL)
		os<<obj._lhs;
	else if(obj._terminalLHS != NULL)
		os<<obj._terminalLHS->Name();
	else if (obj._variable != -1 )
		os<<obj. _variable;
	else if (obj._lhsNickName != "")
		os <<obj._lhsNickName;

	os << " ";
	switch (obj._conditionalOperation) {
	case GREATER_THAN:
		os << ">";
		break;
	case GREATER_THAN_or_EQUAL:
		os << ">=";
		break;
	case LESS_THAN:
		os << "<";
		break;
	case LESS_THAN_or_EQUAL:
		os << ">=";
		break;
	case EQUAL:
		os << "==";
		break;
	case NOT_EQUAL:
		os << "!=";
		break;
	case NOT:
		os << "!" ;
		break;
	case AND:
		os << "&&";
		break;
	case OR:
		os << "||";
		break;
	default:
		break;
	}

	os << " ";
	if (obj._rhs != NULL)
		os<<obj._rhs;
	else if(obj._terminalRHS != NULL)
		os<<obj._terminalRHS->Name();
	else if (obj._constant != -1 )
		os<< obj._constant;
	else if (obj._rhsNickname != "")
		os <<obj._rhsNickname;




	return os;
}

std::string BioExpression::toString(std::string buffer)
{
	std::string lhs = "";
	if (this->_lhs != NULL)
		lhs=this->_lhs->toString("");
	else if(this->_terminalLHS != NULL)
		lhs = this->_terminalLHS->Name();
	else if (this->_variable != -1 )
		lhs = this->_variable;
	else if (this->_lhsNickName != "")
		lhs = this->_lhsNickName;

	std::string condition="";
	switch (this->_conditionalOperation) {
	case GREATER_THAN:
		condition = ">";
		break;
	case GREATER_THAN_or_EQUAL:
		condition = ">=";
		break;
	case LESS_THAN:
		condition = "<";
		break;
	case LESS_THAN_or_EQUAL:
		condition = ">=";
		break;
	case EQUAL:
		condition = "==";
		break;
	case NOT_EQUAL:
		condition = "!=";
		break;
	case NOT:
		condition = "!" ;
		break;
	case AND:
		condition = "&&";
		break;
	case OR:
		condition = "||";
		break;
	default:
		break;
	}

	std::string rhs="";
	if (this->_rhs != NULL)
		rhs = this->_rhs->toString("");
	else if(this->_terminalRHS != NULL)
		rhs = this->_terminalRHS->Name();
	else if (this->_constant != -1 )
		rhs = this->_constant;
	else if (this->_rhsNickname != "")
		rhs = this->_rhsNickname;

	return buffer + lhs + " " + condition + " " + rhs;
}


}
