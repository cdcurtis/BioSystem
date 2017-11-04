/*
 * Container.h
 *
 *  Created on: Jul 27, 2015
 *      Author: Chris
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_

#include <iostream>
#include "Fluid.h"
#include "BioOperation.h"

namespace BioCoder
{

struct Container
{
	//data members
	int uniqueID;
	std::vector<BioOperation*> _instructionStack;
	enum CONTAINER_TYPE id;/*!< The container's id. */
	Fluid * contents;
	//std::vector<Fluid*> contents;/*!< The contents of the container. */
	float volume;/*!< The volume of the contents of the container. Note: This is different from the content's volume i. e. container.volume != container.contents.volume */
	std::string name;/*!< The container's label. */
	FLUID_TYPE type;/*!< The type of this resource. */
	int used;/*!< Used internally to track the usage of the container. */
	int usage_index;/*!< Used internally to track the usage of the container. */
	//list<string> tLinkName;/*<translator Link Name:  Used to have an identifier for the links between the different nodes*/

	Container(int ID)
	:id(STERILE_MICROFUGE_TUBE),contents(NULL), volume(0), name(""),type(FLUID),used(0),usage_index(0),uniqueID(ID)
	{}

	Container(const Container & c)
	:id(c.id),contents(c.contents),volume(c.volume),name(c.name),type(c.type),used(c.used),usage_index(c.usage_index),uniqueID(c.uniqueID)
	{}

	void PrintOpStack()
	{
		for(unsigned int i = 0 ; i < _instructionStack.size(); ++i)
		{
			std::cout << _instructionStack[i]->Name() <<std::endl;
		}
	}

	/*~Container()
	{

	}*/

	std::string toString(std::string buffer)
	{
		std:: stringstream ss;
		ss << uniqueID;
		std::string id;
		ss>> id;

		std::string ret = buffer + "\"VARIABLE\" : {\n";
		ret += buffer + "\t" + "\"NAME\" : " + id + "\n";
		ret += buffer + "}\n";
		return ret;
	}
};

}
#endif /* CONTAINER_H_ */
