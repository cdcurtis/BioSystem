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


using namespace BioCoder;

int main (){
	BioSystem bioCoder;

	Fluid* blood = bioCoder.new_fluid("Blood",Volume(MICRO_LITER, 10));
	Fluid* water = bioCoder.new_fluid("Water",Volume(MICRO_LITER, 10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(blood, tube);
	bioCoder.measure_fluid(water, tube);
	tube->PrintOpStack();

	bioCoder.end_protocol();


	bioCoder.PrintLeveledProtocol();

	return 0;
}
