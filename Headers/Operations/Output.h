/*
 * Output.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_Output_H_
#define HEADERS_OPERATIONS_Output_H_
#include "Operation.h"
#include <vector>

namespace BioCoder
{

class Output:public BioCoder::Operation{

	Container* __inputs;
	std::vector<Property> __properties;
	bool isWaste;
public:
	Output(Container* c1, bool isWaste=false){
		this->__inputs = c1;
		this->isWaste = isWaste;
	}

	OPERATION_TYPE GetType(){
		if(this->isWaste)
			return WASTE;
		return OUTPUT;
	}
	std::string toString(std::string buffer)
	{
		std::string ret=buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"Output\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"OUTPUT\"\n";

		ret += buf + "\"NAME\" : \"Output\"\n";
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