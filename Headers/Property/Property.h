/*
 * Property.h
 *
 *  Created on: Jun 7, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_PROPERTY_PROPERTY_H_
#define HEADERS_PROPERTY_PROPERTY_H_

namespace BioCoder{

	class Property{
		std::string unknownPropertyType;
		std::string unknownPropertyValue;
		std::string unknownPropertyUnits;
public:
		Property(){

		}

		Property(std::string type,std::string value, std::string units){
			this->unknownPropertyType = type;
			this->unknownPropertyUnits = units;
			this->unknownPropertyValue = value;
		}
		virtual std::string toString(std::string buffer)
		{
			std::string ret;
			ret += buffer + "\"PROPERTY\": {\n";
			std::string buf = buffer + '\t';

			ret+=buf + "\"VALUE\" : \"" + this->unknownPropertyValue + "\",\n";
			ret+=buf + "\"UNITS\" : \"" + this->unknownPropertyUnits + "\",\n";
			ret+= "}\n";

			return ret;
		}



	};

}



#endif /* HEADERS_PROPERTY_PROPERTY_H_ */
