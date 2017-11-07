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
	std::vector<Property*> __properties;
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
		ret += buf + "\t{\n";
		ret += buf + "\t\t" + "\"INPUT_TYPE\" : \"VARIABLE\",\n";
		ret += __inputs->toString(buf + "\t\t");


		ret += buf +"\t}";

		if(this->__properties.size() > 0)
		{
			ret+=",\n";
			for(int i = 0; i < this->__properties.size(); ++i){
				Property* p = this->__properties.at(i);
				ret += buf + "\t{\n";

				ret += buf + "\t\t" + "\"INPUT_TYPE\" : \"PROPERTY\",\n";

				ret += p->toString(buf+"\t\t");

				if(i < this->__properties.size()-1)
					ret += buf + "\t},\n";
				else
					ret += buf + "\t}\n";
			}
		}
		else
			ret += "\n";
		ret += buf + "]\n";
		ret += buffer +"}";


		return ret;
	}
};


}
#endif /* HEADERS_OPERATIONS_FLUIDICOPERATION_H_ */
