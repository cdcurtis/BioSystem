/*
 * BasicBlock.h
 *
 *  Created on: Aug 5, 2016
 *      Author: chriscurtis
 */

#ifndef HEADERS_BASICBLOCK_H_
#define HEADERS_BASICBLOCK_H_

#include <vector>
#include <map>
#include "BioOperation.h"
#include "Conditional.h"


namespace BioCoder {
class BasicBlock{
	std::map<int,std::vector<BioOperation* > > _chemicalInteractionGraph;
	std::vector<std::pair<Conditional*, BasicBlock*> > _destinations;
public:
	BasicBlock() {}

	void AddOperation(BioOperation* op, int interactionKey);
	void AddEdge(Conditional* c, BasicBlock* dest);
};
}
#endif /* HEADERS_BASICBLOCK_H_ */
