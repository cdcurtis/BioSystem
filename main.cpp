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

	bioCoder.next_step();
	bioCoder.tap(tube,Time(SECS, 10));

	bioCoder.next_step();
	BioOperation * sensor1 = bioCoder.electrophoresis(tube);

	bioCoder.next_step();
	bioCoder.IF(sensor1, GREATER_THAN, .89);
	bioCoder.incubate(tube, ON_ICE, Time(SECS, 10) );

	bioCoder.ELSE_IF(sensor1, LESS_THAN, .5);
	bioCoder.measure_fluid(water, tube);
	bioCoder.vortex(tube);

	bioCoder.ELSE();
	bioCoder.drain(tube, "Waste");
	bioCoder.measure_fluid(blood,tube);

	bioCoder.END_IF();

	bioCoder.next_step();
	bioCoder.incubate_and_mix(tube,ROOM_TEMPERATURE,Time(SECS,10), Time(SECS,20),VORTEX);

	tube->PrintOpStack();

	bioCoder.end_protocol();


	bioCoder.PrintLeveledProtocol();

	bioCoder.PrintTree();
	std::cout<<"Terminated \n";
	return 0;
}
