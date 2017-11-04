/*
 * Operation.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_OPERATION_H_
#define HEADERS_OPERATIONS_OPERATION_H_

namespace BioCoder
{
	enum OPERATION_TYPE {DISPENSE, DETECT, HEAT, COOL, STORE, MIX, SPLIT, WASTE, OUTPUT, IF_OP, ELSE_IF_OP, ELSE_OP, WHILE_OP, END_IF_OP, END_WHILE_OP,LOOP_OP, END_LOOP_OP, OPERATION_NOT_SPECIFED};


	class Operation{
	protected:
		OPERATION_TYPE __type;

	public:

		Operation(){
			__type = OPERATION_NOT_SPECIFED;
		}

		Operation(OPERATION_TYPE t): __type(t){}

		OPERATION_TYPE GetType() {return __type;}


		virtual std::string toString(std::string buffer)
			{
				std::string ret=buffer + "\"OPERATION\" : {\n";
				std::string buf = buffer + '\t';

				ret += buf + "\"NAME\" : \"Generic\"\n";
				ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
				ret += buf + "\"CLASSIFICATION\" : \"GENERIC\"\n";

				ret += buf + "\"NAME\" : \"Generic\"\n";


				ret+= "}\n";

				return ret;
			}
	};

}
#endif /* HEADERS_OPERATIONS_OPERATION_H_ */
