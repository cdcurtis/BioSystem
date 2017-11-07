/*
 * Branching.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_BRANCHING_H_
#define HEADERS_OPERATIONS_BRANCHING_H_

#include "ControlFlowOperation.h"
#include "ElseifBranch.h"

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
	//	std::cout<< "Branch AddOperation\n";

		this->currentWorkingBranch->push_back(op);
//		std::cout << currentWorkingBranch<< "Size: " << currentWorkingBranch->size()<<std::endl;
	}

	void AddElseIfBranch(ElseIfBranch * branch){
		this->elseIfBranches->push_back(branch);
	//	std::cout<<"MOVING BRANCH FROM "<< this->conditionalBody << " TO " << branch->conditionalBody<<" "<<std::endl;
		this->currentWorkingBranch = branch->conditionalBody;

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

		ret += buf + "\"CONDITION\" : \"" + this->condition->toString("") +"\"\n";

		ret += buf + "\"TRUE_BRANCH\" : [\n";
		for(int i =0 ; i < this->conditionalBody->size(); ++i){
			Operation * input = this->conditionalBody->at(i);
			ret += buf + "\t{\n";

			ret += input->toString(buf + "\t\t") +"\n";

			if(i < this->conditionalBody->size()-1)
				ret += buf + "\t},\n";
			else
				ret += buf +"\t}\n";
		}
		ret += buf + "],\n";

		ret += buf + "\"FALSE_BRANCH\" : [\n";
		for(int i =0 ; i < this->elseBranch->size(); ++i){
			Operation * input = this->elseBranch->at(i);
			ret += buf + "\t{\n";

			ret += input->toString(buf + "\t\t") +"\n";

			if(i < this->elseBranch->size()-1)
				ret += buf + "\t},\n";
			else
				ret += buf +"\t}\n";
		}
		ret += buf + "]";

		if(this->elseIfBranches->size() > 0)
			ret += ",\n";
		else
			ret+= "\n";
		for(int i =0 ; i < this->elseIfBranches->size(); ++i){
			Operation * input = this->elseIfBranches->at(i);

			ret += input->toString(buf) +"\n";

			if(i < this->elseBranch->size()-1)
				ret += ",\n";
			else
				ret += "\n";
		}


		ret+= buffer + "}";

		return ret;
	}

};



}



#endif /* HEADERS_OPERATIONS_BRANCHING_H_ */
