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

	//for(int i = 0 ; i < TotalThermo; ++i){
	std::cout<<"Debug statement"<<std::endl;

	bioCoder.next_step();
	bioCoder.LOOP(TotalThermo);

	std::cout<<"Debug statement2"<<std::endl;
		bioCoder.next_step();
		bioCoder.store_for(tube,95,Time(SECS,20));

		bioCoder.next_step();
		bioCoder.weigh(tube,"weightSensor");

		bioCoder.next_step();
		bioCoder.IF("WieghtSensor",LESS_THAN, 3.57);
		bioCoder.next_step();
		bioCoder.measure_fluid(PCRMix, tube);

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
		std::cout<<"Debug statement3"<<std::endl;
		bioCoder.END_LOOP();
		std::cout<<"Debug statement4"<<std::endl;


	bioCoder.next_step();
	bioCoder.store_for(tube,68,Time(MINS,5));
	std::cout<<"Debug statement5"<<std::endl;
	bioCoder.next_step();
	bioCoder.drain(tube,"PCR");
	std::cout<<"Debug statement6"<<std::endl;
	bioCoder.end_protocol();


	std::cout<<"Debug statemen7"<<std::endl;
	bioCoder.PrintLeveledProtocol();
	bioCoder.PrintTree();
	bioCoder.PrintTreeVisualization("PCRReplacement");

}

void BioSystemTest::PCR(){
	BioSystem bioCoder;

	Fluid *PCRMix = bioCoder.new_fluid("PCRMasterMix", Volume(MICRO_LITER,10));
	Fluid *SeperationMedium = bioCoder.new_fluid("Seperation Medium",Volume(MICRO_LITER,10));
	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(PCRMix,tube);

	bioCoder.next_step();
	bioCoder.incubate(tube,95,Time(5,SECS));

	bioCoder.next_step();
	bioCoder.LOOP(20);

	bioCoder.next_step();
	bioCoder.incubate(tube,53,Time(15,SECS));

	bioCoder.next_step();
	bioCoder.incubate(tube,72,Time(10,SECS));
	bioCoder.END_LOOP();

	bioCoder.next_step();
	bioCoder.ce_detect(tube,5,236,SeperationMedium);

	bioCoder.next_step();
	bioCoder.measure_fluorescence(tube,Time(3,MINS));

	bioCoder.next_step();
	bioCoder.end_protocol();
}
/*Result1 = Mix 10µL of Glucose with 10µL of Reagent for 10s
Reading1 = Measure the fluorescence of Result1 for 30s

Result2 = Mix 10µL of Glucose with 20µL of Reagent for 10s
Reading2 = Measure the fluorescence of Result2 for 30s

Result3 = Mix 10µL of Glucose with 40µL of Reagent for 10s
Reading3 = Measure the fluorescence of Result3 for 30s

Result4 = Mix 10µL of Glucose with 80µL of Reagent for 10s
Reading4 = Measure the fluorescence of Result4 for 30s

Result5 = Mix 10µL of Sample with 10µL of Reagent for 10s
Reading5 = Measure the fluorescence of Result5 for 30s

 * */
void BioSystemTest:: GlucoseDetection(){
	BioSystem bioCoder;

	Fluid *Glucose = bioCoder.new_fluid("Ion exchange beads", Volume(MICRO_LITER,160));
	Fluid *Reagent = bioCoder.new_fluid("Fluoride ions",Volume(MICRO_LITER,50));
	Fluid *Sample = bioCoder.new_fluid("HCL",Volume(MICRO_LITER,10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);
	Container* tube2 = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);
	Container* tube3 = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);
	Container* tube4 = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);
	Container* tube5 = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(Glucose,Volume(MICRO_LITER,10),tube);
	bioCoder.measure_fluid(Glucose,Volume(MICRO_LITER,10),tube2);
	bioCoder.measure_fluid(Glucose,Volume(MICRO_LITER,10),tube3);
	bioCoder.measure_fluid(Glucose,Volume(MICRO_LITER,10),tube4);
	bioCoder.measure_fluid(Glucose,Volume(MICRO_LITER,10),tube5);

	bioCoder.measure_fluid(Reagent,Volume(MICRO_LITER,10),tube);
	bioCoder.measure_fluid(Reagent,Volume(MICRO_LITER,20),tube2);
	bioCoder.measure_fluid(Reagent,Volume(MICRO_LITER,40),tube3);
	bioCoder.measure_fluid(Reagent,Volume(MICRO_LITER,80),tube4);

	bioCoder.measure_fluid(Sample,Volume(MICRO_LITER,80),tube5);

	bioCoder.next_step();
	bioCoder.measure_fluorescence(tube,Time(5,SECS));
	bioCoder.measure_fluorescence(tube2,Time(5,SECS));
	bioCoder.measure_fluorescence(tube3,Time(5,SECS));
	bioCoder.measure_fluorescence(tube4,Time(5,SECS));
	bioCoder.measure_fluorescence(tube5,Time(5,SECS));

	bioCoder.next_step();
	bioCoder.end_protocol();
}

void BioSystemTest::ImageProbSynthesis(){
	BioSystem bioCoder;

	Fluid *IonBeads = bioCoder.new_fluid("Ion exchange beads", Volume(MICRO_LITER,10));
	Fluid *Fluoride = bioCoder.new_fluid("Fluoride ions",Volume(MICRO_LITER,10));
	Fluid *HCL = bioCoder.new_fluid("HCL",Volume(MICRO_LITER,10));
	Fluid *MeCNSolution = bioCoder.new_fluid("MeCN solution",Volume(MICRO_LITER,10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(IonBeads,tube);
	bioCoder.measure_fluid(Fluoride,tube);

	bioCoder.next_step();
	bioCoder.vortex(tube,Time(30,SECS));

	bioCoder.next_step();
	bioCoder.incubate(tube,100,Time(30,SECS));

	bioCoder.next_step();
	bioCoder.incubate(tube,120,Time(30,SECS));

	bioCoder.next_step();
	bioCoder.incubate(tube,135, Time(3,MINS));

	bioCoder.next_step();
	bioCoder.measure_fluid(MeCNSolution,tube);

	bioCoder.next_step();
	bioCoder.vortex(tube,Time(30,SECS));

	bioCoder.next_step();
	bioCoder.incubate(tube,100,Time(30,SECS));

	bioCoder.next_step();
	bioCoder.incubate(tube,120,Time(50,SECS));

	bioCoder.next_step();
	bioCoder.measure_fluid(HCL,tube);

	bioCoder.next_step();
	bioCoder.vortex(tube,Time(60,SECS));

	bioCoder.next_step();
	bioCoder.incubate(tube,60,Time(60,SECS));

	bioCoder.next_step();
	bioCoder.end_protocol();
}
/*Mixture = Mix <no value>µL of Sample with <no value>µL of Reagent for 50s
Perform Capillary Electrophoresis ( 9 cm at 223 V/cm) on Mixture Seperate with <no value>µL of electrophoresis buffer
Measure the fluorescence of Mixture for 10s
*/
void BioSystemTest::neurotransmitterSensing(){
	BioSystem bioCoder;

	Fluid *Sample = bioCoder.new_fluid("Sample", Volume(MICRO_LITER,10));
	Fluid *Reagent = bioCoder.new_fluid("Reagent",Volume(MICRO_LITER,10));
	Fluid *SeperationMedium = bioCoder.new_fluid("Seperation Medium",Volume(MICRO_LITER,10));

	Container* tube = bioCoder.new_container(STERILE_MICROFUGE_TUBE2ML);

	bioCoder.first_step();
	bioCoder.measure_fluid(Sample,tube);
	bioCoder.measure_fluid(Reagent,tube);

	bioCoder.next_step();
	bioCoder.vortex(tube,Time(50,SECS));

	bioCoder.next_step();
	bioCoder.ce_detect(tube,9,223,SeperationMedium);

	bioCoder.next_step();
	bioCoder.measure_fluorescence(tube,Time(10,SECS));

	bioCoder.next_step();
	bioCoder.end_protocol();
}
