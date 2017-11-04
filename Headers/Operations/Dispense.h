/*
 * Dispense.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_Dispense_H_
#define HEADERS_OPERATIONS_Dispense_H_
#include "Operation.h"
#include <vector>

namespace BioCoder
{

class Dispense:public BioCoder::Operation{
	Fluid* __chemical;
	Container* __input;
	Volume __properties;

public:
	Dispense(Fluid* f1, Container* c1, Volume v1){
		this->__chemical = f1;
		this->__input = c1;
		this->__properties = v1;
	}

	OPERATION_TYPE GetType(){
		return DISPENSE;
	}
	std::string toString(std::string buffer)
	{
		std::string ret=buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"Dispense\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"DISPENSE\"\n";

		ret += buf + "\"NAME\" : \"Dispense\"\n";
		ret += buf + "\"INPUTS\" : [\n";
		ret += buf + '\t' + "{\n";

		ret += buf + "\t\t" + "\"INPUT_TYPE\" : \"VARIABLE\",\n";
		ret += this->__chemical->toString(buf + "\t\t", Volume(__chemical->unit, __chemical->volume));



		ret += buf + "\t}\n";
		ret += buf + "],\n";

		ret += buf + "\"OUTPUTS\" : [\n";
		ret += buf + '\t' + "{\n";
		ret += __input->toString(buf +"\t\t");
		ret += buf + '\t' + "}\n";
		ret += buf + "]\n";


		ret +=buffer + "}";

		return ret;

	}
};


}
#endif /* HEADERS_OPERATIONS_FLUIDICOPERATION_H_ */
