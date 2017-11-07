/*
 * Heat.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_Heat_H_
#define HEADERS_OPERATIONS_Heat_H_
#include "Operation.h"
#include <vector>

namespace BioCoder
{

class Heat:public BioCoder::Operation{
	Container* __inputs;
	std::vector<Property*> __properties;

public:
	~Heat()
	{
		for(Property* p : __properties)
			delete p;
	}
	Heat(Container* c1, Temperature t1){
		this->__inputs = c1;
		this->__properties.push_back(new Temperature(t1._unit, t1._value));
	}
	Heat(Container* c1, Temperature temp1, Time time){
		this->__inputs = c1;
		this->__properties.push_back(new Temperature(temp1._unit, temp1._value));
		this->__properties.push_back(new Time(time._unit, time._value));
	}

	OPERATION_TYPE GetType(){
		return HEAT;
	}
	std::string toString(std::string buffer)
	{
		std::string ret=buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"Heat\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"HEAT\"\n";

		ret += buf + "\"NAME\" : \"Heat\"\n";

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
