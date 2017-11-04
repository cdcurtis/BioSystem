/*
 * BasicBlockEdge.h
 *
 *  Created on: May 19, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_BASICBLOCK_BASICBLOCKEDGE_H_
#define HEADERS_BASICBLOCK_BASICBLOCKEDGE_H_
namespace BioCoder {

class BasicBlockEdge{
public:
	BasicBlock* parent;
	BasicBlock* child;

	std::string condition;

public:
	BasicBlockEdge():parent(nullptr), child(nullptr), condition("") {}
	BasicBlockEdge(BasicBlock* p, BasicBlock* c, std::string cond = "UNCONDITIONAL"):parent(p), child(c), condition(cond) {}
};

}

#endif /* HEADERS_BASICBLOCK_BASICBLOCKEDGE_H_ */
