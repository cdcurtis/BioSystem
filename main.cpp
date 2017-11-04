/*
 * main.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#include <iostream>
#include "Headers/Property/Temperature.h"
#include "Headers/Property/Time.h"
#include "Headers/Property/Volume.h"

#include "Headers/Fluid.h"
#include "Headers/Container.h"
#include "Headers/Conditional.h"
#include "Headers/BioOperation.h"
#include "Headers/BioSystem.h"

#include "Headers/BioSystemTests.h"
using namespace BioCoder;

int main ()
{
	//BioSystemTest::SimpleProtocol();
	BioSystemTest::SimpleIfProtocol();
	//BioSystemTest::NestedIFTest();
	//BioSystemTest::SimpleWhileSensorConditional();
	//BioSystemTest::NestedIFInWhile();
	//BioSystemTest::SimplePCRLoop();
	//BioSystemTest::ProbablisticPCR();
	//BioSystemTest::PCRDropletReplacement();


	//std::cin>>foo;
	std::cout<<"Terminated \n";
	return 0;
}
