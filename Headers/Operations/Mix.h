/*
 * Mix.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_Mix_H_
#define HEADERS_OPERATIONS_Mix_H_
#include "Operation.h"
#include <vector>

namespace BioCoder
{

class Mix:public BioCoder::Operation{
	std::vector<Container*> __inputs;
	std::vector<Container*> __outputs;
	std::vector<Property*> __properties;
	MIX_TYPE mixType;

public:
	~Mix()
	{
		for(Property* p: __properties)
			delete p;
	}
	Mix(Container* c1, Container* c2){
		this->mixType = MIX_NOT_SPECIFIED;
		this->__inputs.push_back(c1);
		this->__inputs.push_back(c2);
		this->__outputs.push_back(c1);
	}
	Mix(Container* c1, MIX_TYPE mt, Container* c2,Time t1){
		this->mixType = mt;
		this->__inputs.push_back(c1);
		this->__inputs.push_back(c2);
		this->__outputs.push_back(c1);
		this->__properties.push_back(new Time(t1._unit, t1._value));
	}
	Mix(Container* c1, MIX_TYPE mt, Time t1){
		this->mixType = mt;
		this->__inputs.push_back(c1);
		this->__outputs.push_back(c1);

		this->__properties.push_back(new Time(t1._unit, t1._value));
	}

	OPERATION_TYPE GetType(){
		return MIX;
	}
	std::string toString(std::string buffer)
	{
		std::string ret=buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"Mix\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"MIX\"\n";

		ret += buf + "\"NAME\" : \"Mix\"\n";
		ret += buf + "\"INPUTS\" : [\n";

		for(int i =0 ; i < __inputs.size(); ++i){
			Container * input = __inputs.at(i);
			ret += buf + "\t{\n";

			ret += buf + "\t\t" + "\"INPUT_TYPE\" : \"VARIABLE\",\n";
			ret += input->toString(buf + "\t\t");

			if(i < __inputs.size()-1)
				ret += buf + "\t},\n";
			else
				ret += buf +"\t}";
		}
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
		ret += buf + "],\n";
		ret += buf + "\"OUTPUTS\" : [\n";

		for(int i =0 ; i < __outputs.size(); ++i){
			Container * input = __outputs.at(i);
			ret += buf + "\t{\n";
			ret += input->toString(buf + "\t\t");

			if(i < __outputs.size()-1)
				ret += buf + "},\n";
			else
				ret += buf +"\t}\n";
		}
		ret += buf + "]\n";

		ret+=buffer + "}";

		return ret;
	}
};


}
#endif /* HEADERS_OPERATIONS_FLUIDICOPERATION_H_ */
