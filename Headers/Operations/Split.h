/*
 * Split.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_Split_H_
#define HEADERS_OPERATIONS_Split_H_
#include "Operation.h"
#include <vector>

namespace BioCoder
{

class Split:public BioCoder::Operation{
	Container*__inputs;
	std::vector<Property> __properties;
	int numSplit;
	int piecesToDest;
	Container* output;

public:
	Split(Container * source, int NumSplit, int piecesToDest, Container * Dest){
		this->__inputs = source;
		this->numSplit = numSplit;
		this->piecesToDest = piecesToDest;
		this->output = output;

	}
	Split (Container* source, Volume volume1, Container* destination){
		this->__inputs = source;
		__properties.push_back(volume1);
		this->output = destination;
		this->numSplit =-1;
		this->piecesToDest = -1;
	}

	OPERATION_TYPE GetType(){
		return SPLIT;
	}

	std::string toString(std::string buffer)
	{
		std::string ret=buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"Split\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"SPLIT\"\n";

		ret += buf + "\"NAME\" : \"Split\"\n";
		ret += buf + "\"INPUTS\" : [\n";
		ret += buf + "{\n";

		ret += buf + '\t' + "\"INPUT_TYPE\" : \"VARIABLE\",\n";
		ret += buf + '\t' + this->__inputs->toString(buf + '\t');

		if(this->__properties.size() == 0 && this->__type == EVENT_NOT_SPECIFIED)
			ret += buf + "}\n";
		else
			ret += buf + "},\n";

		ret += buf + "{\n";

		ret += buf + '\t' + "\"INPUT_TYPE\" : \"PROPERTY\",\n";

		for(Property p : this->__properties){
			ret += buf + "{";
			ret += buf + p.toString(buf);

			//if(this->__type == EVENT_NOT_SPECIFIED)
			ret += buf + "}\n";
			//else
			//	ret += buf + "},\n";
		}


		ret+= "}\n";

		return ret;
	}
};


}
#endif /* HEADERS_OPERATIONS_FLUIDICOPERATION_H_ */
