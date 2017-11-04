/*
 * BasicBlock.cpp
 *
 *  Created on: Aug 5, 2016
 *      Author: chriscurtis
 */

#include "../Headers/BasicBlock/BasicBlock.h"


namespace BioCoder{

 void BasicBlock::AddEdge(Conditional* c, BasicBlock* dest){
	 this->_destinations.push_back(std::pair<Conditional*, BasicBlock*>(c, dest));
 }

 void BasicBlock::AddOperation(BioOperation* op, int key)
 {
	 if(this->_chemicalInteractionGraph.count(key) == 0){
		 std::vector<BioOperation*> ops;
		 ops.push_back(op);
		 this->_chemicalInteractionGraph.insert(std::pair<int,std::vector<BioOperation*> >(key,ops));

	 }
	 else {
		 _chemicalInteractionGraph.at(key).push_back(op);
	 }
 }

 }

