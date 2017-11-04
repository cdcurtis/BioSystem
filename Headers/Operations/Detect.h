/*
 * Detect.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_Detect_H_
#define HEADERS_OPERATIONS_Detect_H_
#include "Operation.h"
#include <vector>

namespace BioCoder
{

class Detect:public BioCoder::Operation{
	std::string __detectType;
	std::string __outputName;
	Container* __inputs;
	std::vector<Property*> __properties;

public:
	~Detect()
	{
		for (Property* p : __properties)
			delete p;
	}
	Detect(Container* c1, std::string detectType, std::string outputName, std::vector<Property*> properties){
		this->__detectType = detectType;
		this->__outputName = outputName;
		this->__inputs = c1;
		this->__properties = properties;
	}

	OPERATION_TYPE GetType(){
		return DETECT;
	}
	std::string toString(std::string buffer)
	{
		std::string ret=buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"Detect\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"DETECT\"\n";

		ret += buf + "\"NAME\" : \"Detect\"\n";
		ret += buf + "\"INPUTS\" : [\n";
		ret += buf + "\t{\n";
		ret += buf + "\t\t" + "\"INPUT_TYPE\" : \"VARIABLE\",\n";
		ret += __inputs->toString(buf + "\t\t");


		ret += buf +"\t}";

		if(this->__properties.size() > 0)
		{
			ret+=",\n";

			ret += buf + "\t{\n";

			ret += buf + "\t\t" + "\"INPUT_TYPE\" : \"PROPERTY\",\n";

			for(Property* p : this->__properties){
				ret += buf + "\t\t{\n";
				ret += p->toString(buf+"\t\t\t");
				ret += buf + "\t\t}\n";
			}
			ret += buf + "\t}\n";
		}
		else
			ret+="\n";
		ret += buf + "],\n";

		ret += buf + "\"OUTPUTS\" : [\n";
		ret += buf + "\t{\n";

		ret += buf + "\t\t \"VARIABLE\" : {\n";
		ret += buf + "\t\t\t \"NAME\" : \"" + this->__outputName + "\"\n";
		ret += buf + "\t\t }\n";

		ret += buf + "\t}\n";
		ret += buf + "]\n";

		ret += buffer +"}";

		return ret;
	}

};


}
#endif /* HEADERS_OPERATIONS_FLUIDICOPERATION_H_ */
