/*
 * ElseifBranch.h
 *
 *  Created on: Nov 6, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_ELSEIFBRANCH_H_
#define HEADERS_OPERATIONS_ELSEIFBRANCH_H_

#include <iostream>

namespace BioCoder {

class ElseIfBranch : public ControlFlowOperation
{
public:
	ElseIfBranch(BioExpression* expression):ControlFlowOperation(expression) {
	}


	std::string toString(std::string buffer)
	{
		std::string ret = buffer + "\"ELSEIF_BRANCH\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"CLASSIFICATION\" : \"CFG_BRANCH\"\n";

		ret += buf + "\"CONDITION\" : \"" + this->condition->toString("") +"\"\n";

		ret += buf + "\"BODY\" : [\n";
		for(int i =0 ; i < this->conditionalBody->size(); ++i){
			Operation * input = this->conditionalBody->at(i);
			ret += buf + "\t{\n";

			ret += input->toString(buf + "\t\t") +"\n";

			if(i < this->conditionalBody->size()-1)
				ret += buf + "\t},\n";
			else
				ret += buf +"\t}\n";
		}
		ret += buf + "]\n";





		ret+= buffer + "}";

		return ret;
	}
};

}


#endif /* HEADERS_OPERATIONS_ELSEIFBRANCH_H_ */
