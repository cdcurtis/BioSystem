/*
 * ControlFlowGraph.cpp
 *
 *  Created on: Aug 5, 2016
 *      Author: chriscurtis
 */




#include"../Headers/ControlFlowGraph.h"

namespace BioCoder {
	void ControlFlowGraph::AddOp(BioOperation* op, int key)
	{
		if (this->_basicBlocks.empty()) // first instruction of program
		{
			BasicBlock* firstBasicBlock = new BasicBlock();
			_currentBlock = firstBasicBlock;
			this->_basicBlocks.push_back(firstBasicBlock);
		}

		if(op->_opType == IF_OP){ //push current BB to the bookeeping list.  Creates new BB for True branch. add condition to block, push BB to stack
			std::vector<BasicBlock*> startif;
			startif.push_back(this->_currentBlock);

			BasicBlock* ifBB = new BasicBlock();
			startif.push_back(ifBB);

			this->_conditionalBookkeeping.push_back(startif);
		}
		if(op->_opType == ELSE_IF_OP) //Creates new BB for true branch, add condition to first block
			;
		else if (op->_opType == ELSE_OP) // terminates last BB. previous IF/ELSE jumps Here(create new block).
			;
		else if(op->_opType ==END_IF_OP) // Triggers the collecting of all the if branches
			;
		else if(op->_opType == LOOP_OP || op->_opType == WHILE_OP) //ENDS BB, Creates Single BB for while and ends that block too.
			;
		else if(op->_opType == END_LOOP_OP || END_WHILE_OP) //ENDS BB and ADDS edge back to loop insert.
			;

	}

}
