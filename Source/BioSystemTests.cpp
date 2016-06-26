/*
 * BioSystemTests.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Chris
 */

#include "../Headers/BioSystem.h"
#include "../Headers/BioSystemTests.h"

using namespace BioCoder;


void BioSystemTest::SimpleProtocol()
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
	bioCoder.incubate(tube, 100,Time(SECS,10));

	bioCoder.end_protocol();


	//	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTreeVisualization("SimpleProtocol");

}

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
	bioCoder.PrintTreeVisualization("NestedIF");

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
	bioCoder.PrintTreeVisualization("SimpleWhile");

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
	bioCoder.PrintTreeVisualization("IfinWhile");

}

void BioSystemTest:: SimplePCRLoop()
{
	BioSystem bioCoder;

	Fluid *PCRMix = bioCoder.new_fluid("PCRMasterMix", Volume(MICRO_LITER,10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(PCRMix,tube);

	bioCoder.next_step();
	bioCoder.WHILE("DNASensor", LESS_THAN, .95);

	bioCoder.next_step();
	bioCoder.store_for(tube,94,Time(SECS,1));

	bioCoder.next_step();
	bioCoder.store_for(tube,65,Time(SECS,1));

	bioCoder.next_step();
	bioCoder.measure_fluorescence(tube,Time(SECS,5),"DNASensor");
	bioCoder.END_WHILE();


	bioCoder.next_step();
	bioCoder.drain(tube, "Amplified PCR");
	bioCoder.end_protocol();


	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTree();
	bioCoder.PrintTreeVisualization("SimplePCRLoop");

}
void BioSystemTest:: ProbablisticPCR()
{
	int Total = 15;
	int Threshold = 10;
	int initial = 6;
	BioSystem bioCoder;

	Fluid *PCRMix = bioCoder.new_fluid("PCRMasterMix", Volume(MICRO_LITER,10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(PCRMix,tube);

	for(int i = 0 ; i < initial; ++i) {
		bioCoder.next_step();
		bioCoder.store_for(tube,94,Time(SECS,45));

		bioCoder.next_step();
		bioCoder.store_for(tube,65,Time(SECS,45));
	}

	for(int i = initial; i <= Threshold; ++i) {
		std::cout <<i<<std::endl;
		bioCoder.next_step();
		bioCoder.store_for(tube,94,Time(SECS,45));

		bioCoder.next_step();
		bioCoder.store_for(tube,65,Time(SECS,45));

		bioCoder.next_step();
		bioCoder.measure_fluorescence(tube,Time(SECS,5),"DNASensor");

		bioCoder.IF("DNASensor",GREATER_THAN, .85);
		for(int j = i; j < Total+(Threshold-i); ++j) {
			bioCoder.next_step();
			bioCoder.store_for(tube,94,Time(SECS,45));

			bioCoder.next_step();
			bioCoder.store_for(tube,65,Time(SECS,45));
		}

		bioCoder.next_step();
		bioCoder.drain(tube,"Amplified PCR");
		bioCoder.END_IF();

	}

	bioCoder.drain(tube,"waste");
	bioCoder.end_protocol();

	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTree();
	bioCoder.PrintTreeVisualization("ProbablisticPCR");
}
void BioSystemTest:: PCRDropletReplacement()
{
	int TotalThermo = 9;
	BioSystem bioCoder;

	Fluid *PCRMix = bioCoder.new_fluid("PCRMasterMix", Volume(MICRO_LITER,10));
	Fluid *Template = bioCoder.new_fluid("Template", Volume(MICRO_LITER,10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);
	//Container* tube2 = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(PCRMix,tube);

	bioCoder.next_step();
	bioCoder.vortex(tube,Time(SECS,1));
	bioCoder.measure_fluid(Template,tube);

	bioCoder.next_step();
	bioCoder.vortex(tube, Time(SECS,1));

	bioCoder.next_step();
	bioCoder.store_for(tube,95,Time(SECS,45));

	for(int i = 0 ; i < TotalThermo; ++i){
		bioCoder.next_step();
		bioCoder.store_for(tube,95,Time(SECS,20));

		bioCoder.next_step();
		bioCoder.weigh(tube,"weightSensor");

		bioCoder.next_step();
		bioCoder.IF("WieghtSensor",LESS_THAN, 3.57);
		bioCoder.next_step();
		bioCoder.measure_fluid(PCRMix,tube);

		bioCoder.next_step();
		bioCoder.store_for(tube, 95,Time(SECS,45));

//		bioCoder.next_step();
//		bioCoder.measure_fluid(tube2,tube);

		bioCoder.next_step();
		bioCoder.vortex(tube, Time(SECS,1));
		bioCoder.END_IF();

		bioCoder.next_step();
		bioCoder.store_for(tube,50,Time(SECS,30));

		bioCoder.next_step();
		bioCoder.store_for(tube,68,Time(SECS,45));
	}
	bioCoder.next_step();
	bioCoder.store_for(tube,68,Time(MINS,5));

	bioCoder.next_step();
	bioCoder.drain(tube,"PCR");
	bioCoder.end_protocol();


	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTree();
	bioCoder.PrintTreeVisualization("PCRReplacement");

}
