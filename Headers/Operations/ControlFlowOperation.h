/*
 * ControlFlowOperation.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_CONTROLFLOWOPERATION_H_
#define HEADERS_OPERATIONS_CONTROLFLOWOPERATION_H_

#include "Operation.h"
#include "../BioExpression.h"

namespace BioCoder {




class ControlFlowOperation:public BioCoder::Operation {
public:
	BioExpression* condition;
	std::vector<Operation*> * conditionalBody;

	virtual ~ControlFlowOperation()
	{}
	ControlFlowOperation(BioExpression* exp):condition(exp) {
		conditionalBody = new std::vector<Operation*>();
	}

	virtual void AddOpertion(Operation* op){
		std::cout<< "controlFLow AddOperation\n";
		this->conditionalBody->push_back(op);
	}

	virtual std::string toString(std::string buffer)
	{
		return "NO TEXT GEN FOR CONTROLFLOW GENERIC";
	}


};



}



#endif /* HEADERS_OPERATIONS_CONTROLFLOWOPERATION_H_ */
