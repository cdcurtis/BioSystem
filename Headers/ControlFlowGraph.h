/*
 * ControlFlowGraph.h
 *
 *  Created on: Aug 5, 2016
 *      Author: chriscurtis
 */

#ifndef HEADERS_CONTROLFLOWGRAPH_H_
#define HEADERS_CONTROLFLOWGRAPH_H_

#include <vector>
#include"BasicBlock/BasicBlock.h"

namespace BioCoder {

class ControlFlowGraph{
	std::vector<BasicBlock*> _basicBlocks;
	/*
	 * Conditionals could potentially have several BBs in them. This vector keeps track of the opening
	 * and closing of each. END loop need to jump back to beginning loop.
	 *
	 * At the end of each IF/ ELSE IF block needs to jump to Block directly after the END IF command.
	 * I will not know the where to jump until after finishing the IF and starting the new blocks.
	 *
	 * IF || WHILE
	 * 	creates new vector for new set of conditional block.
	 * else if/else
	 * 	pushes into vector
	 * end if || ENDWHILE
	 * 	pops vector.
	 */

	std:: vector <std::vector<BasicBlock*> > _conditionalBookkeeping;

	BasicBlock* _currentBlock;


public:
	ControlFlowGraph(){}

	void AddOp(BioOperation* op, int key);

};

}


#endif /* HEADERS_CONTROLFLOWGRAPH_H_ */
