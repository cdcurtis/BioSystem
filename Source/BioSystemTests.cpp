/*
 * BioSystemTests.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Chris
 */

#include "../Headers/BioSystem.h"
#include "../Headers/BioSystemTests.h"

using namespace BioCoder;


void BioSystemTest:: NestedIFTest()
{
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
	BioOperation * sensor2 = bioCoder.weigh(tube);

	bioCoder.next_step();
	bioCoder.IF(sensor1, GREATER_THAN, .89);
	bioCoder.incubate(tube, ON_ICE, Time(SECS, 10) );

	bioCoder.ELSE_IF(sensor1, LESS_THAN, .5);
	bioCoder.IF(sensor2, GREATER_THAN, .5);

	bioCoder.incubate(tube, ON_ICE, Time(SECS, 10) );
	bioCoder.END_IF();


	bioCoder.measure_fluid(water, tube);
	bioCoder.vortex(tube);

	bioCoder.ELSE();
	bioCoder.drain(tube, "Waste");
	bioCoder.measure_fluid(blood,tube);

	bioCoder.END_IF();

	bioCoder.next_step();
	bioCoder.incubate_and_mix(tube,ROOM_TEMPERATURE,Time(SECS,10), Time(SECS,20),VORTEX);

	bioCoder.end_protocol();

	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTree();

}

void BioSystemTest:: SimpleWhileSensorConditional()
{
	BioSystem bioCoder;

	Fluid* blood = bioCoder.new_fluid("Blood",Volume(MICRO_LITER, 10));
	Fluid* water = bioCoder.new_fluid("Water",Volume(MICRO_LITER, 10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(blood, tube);
	bioCoder.measure_fluid(water, tube);

	bioCoder.next_step();
	BioOperation * weightReading1 = bioCoder.weigh(tube);
	bioCoder.tap(tube,Time(SECS, 10));

	bioCoder.next_step();
	bioCoder.weigh(tube,"WeightSensor2");

	bioCoder.next_step();
	bioCoder.WHILE(weightReading1, EQUAL, "WeightSensor2" );
	bioCoder.incubate(tube,200, Time(SECS, 10));
	bioCoder.weigh(tube, "WeightSensor2");

	bioCoder.END_WHILE();

	bioCoder.next_step();
	bioCoder.incubate(tube,ON_ICE, Time(MINS, 60));

	bioCoder.end_protocol();


	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTree();

}

void BioSystemTest:: NestedIFInWhile()
{
	BioSystem bioCoder;

	Fluid* blood = bioCoder.new_fluid("Blood",Volume(MICRO_LITER, 10));
	Fluid* water = bioCoder.new_fluid("Water",Volume(MICRO_LITER, 10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(blood, tube);
	bioCoder.measure_fluid(water, tube);

	bioCoder.next_step();
	BioOperation * weightReading1 = bioCoder.weigh(tube);
	bioCoder.tap(tube,Time(SECS, 10));

	bioCoder.next_step();
	bioCoder.weigh(tube,"WeightSensor2");

	bioCoder.next_step();
	bioCoder.WHILE(weightReading1, EQUAL, "WeightSensor2" );
	bioCoder.incubate(tube,200, Time(SECS, 10));
	bioCoder.weigh(tube, "WeightSensor2");

	bioCoder.next_step();
	bioCoder.IF(weightReading1, LESS_THAN, .2);
	bioCoder.measure_fluid(blood,tube);
	bioCoder.vortex(tube);

	bioCoder.END_IF();
	bioCoder.END_WHILE();

	bioCoder.next_step();
	bioCoder.incubate(tube,ON_ICE, Time(MINS, 60));

	bioCoder.end_protocol();


	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTree();

}
