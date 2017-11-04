/*
 * Branching.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_BRANCHING_H_
#define HEADERS_OPERATIONS_BRANCHING_H_

#include "ControlFlowOperation.h"

namespace BioCoder {




class Branch:public BioCoder::ControlFlowOperation {
public:

	std::vector<Operation*> * elseIfBranches;
	std::vector<Operation*> * elseBranch;

	std::vector<Operation*> *currentWorkingBranch;
	Branch(BioExpression* expression):ControlFlowOperation(expression) {

		elseIfBranches = new std::vector<Operation*>;
		elseBranch = new std::vector<Operation *>();
		currentWorkingBranch = this->conditionalBody;
	}

	void AddOpertion(Operation* op){
		this->currentWorkingBranch->push_back(op);
	}

	void AddElseIfBranch(ControlFlowOperation * branch){
		this->elseIfBranches->push_back(branch);
		currentWorkingBranch = elseIfBranches;
	}
	void SetElse(){
		currentWorkingBranch = elseBranch;
	}
	std::string toString(std::string buffer)
	{
		std::string ret = buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"IF\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"CFG_BRANCH\"\n";

		ret += buf + "\"CONDITION\" : " + this->condition->toString(buf) + "\n";

		ret += buf + '\t' + "\"TRUE_BRANCH\" : [\n";
		for(int i =0 ; i < this->conditionalBody->size(); ++i){
			Operation * input = this->conditionalBody->at(i);
			ret += buf + "{\n";

			ret += buf + '\t' + input->toString(buf + '\t');

			if(i < this->conditionalBody->size()-1)
				ret += buf + "},\n";
			else
				ret + buf +"}\n";
		}
		ret += buf + "],\n";

		ret += buf + '\t' + "\"FALSE_BRANCH\" : [\n";
		for(int i =0 ; i < this->elseBranch->size(); ++i){
			Operation * input = this->elseBranch->at(i);
			ret += buf + "{\n";

			ret += buf + '\t' + input->toString(buf + '\t');

			if(i < this->elseBranch->size()-1)
				ret += buf + "},\n";
			else
				ret + buf +"}\n";
		}
		ret += buf + "],\n";

		ret += buf + '\t' + "\"ELSEIF_BRANCH\" : [\n";
		for(int i =0 ; i < this->elseIfBranches->size(); ++i){
			Operation * input = this->elseIfBranches->at(i);
			ret += buf + "{\n";

			ret += buf + '\t' + input->toString(buf + '\t');

			if(i < this->elseIfBranches->size()-1)
				ret += buf + "},\n";
			else
				ret + buf +"}\n";
		}
		ret += buf + "]\n";

		ret+= buffer + "}\n";

		return ret;
	}

};



}



#endif /* HEADERS_OPERATIONS_BRANCHING_H_ */
