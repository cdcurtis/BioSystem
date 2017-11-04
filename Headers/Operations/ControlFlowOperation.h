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

	ControlFlowOperation(BioExpression* exp):condition(exp) {
		conditionalBody = new std::vector<Operation*>();
	}

	void AddOpertion(Operation* op){
		this->conditionalBody->push_back(op);
	}


};



}



#endif /* HEADERS_OPERATIONS_CONTROLFLOWOPERATION_H_ */
