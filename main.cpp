/*
 * main.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#include <iostream>
#include "Headers/Temperature.h"
#include "Headers/Time.h"
#include "Headers/Volume.h"

#include "Headers/Fluid.h"
#include "Headers/Container.h"
#include "Headers/Conditional.h"
#include "Headers/BioOperation.h"
#include "Headers/BioSystem.h"

#include "Headers/BioSystemTests.h"
using namespace BioCoder;

int main ()
{
	BioSystemTest::SimpleProtocol();
	BioSystemTest::NestedIFTest();
	BioSystemTest::SimpleWhileSensorConditional();
	BioSystemTest::NestedIFInWhile();
	std::cout<<"Terminated \n";
	return 0;
}
