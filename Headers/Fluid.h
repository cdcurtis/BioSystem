/*
 * Fluid.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#ifndef FLUID_H_
#define FLUID_H_

#include <iostream>
#include <vector>
#include "Property/Volume.h"

namespace BioCoder
{

/*! Specifies the type of the subtance. Used internally. */
enum FLUID_TYPE {FLUID, SOLID, CONTAINER, OPERATION} ;

/*! Types of containers. For use in new_container(). */
enum CONTAINER_TYPE{STERILE_MICROFUGE_TUBE/*!< Sterile 1.5-ml microfuge tube */, CENTRIFUGE_TUBE_15ML/*!< 15ml centrifuge tube */, FLASK/*!< Flask */, CENTRIFUGE_BOTTLE/*!< Centrifuge bottle */, GRADUATED_CYLINDER/*!< Graduated cylinder */, HUMIDIFIED_CHAMBER/*!< container that can be placed in a humidified chamber */, RXN_TUBE/*!< Reaction tube */, FRESH_COLL_TUBE/*!< Fresh collection tube */, LIQUID_NITROGEN/*!< container with liquid nitrogen */, PLG/*!< 50ml PLG tube */, OAKRIDGE/*!< Oakridge tube */, QIA_CARTRIDGE/*!< Qiacartridge */, CUVETTE_ICE/*!< Cuvette stored on ice */, SPEC_CUVETTE/*!< spectrometry cuvette */, STOCK_PLATE_96/*!< 96-well stock plate */, WELL_BLOCK_96/*!< 96-well block */, PCR_PLATE/*!< 96-well PCR plate */, LIQUID_BLOCK/*!< 96-well liquid block */, CELL_CULT_CHAMBER/*!< Cell culture chamber */, EPPENDORF/*!< Eppendorf tube */, STERILE_MICROFUGE_TUBE2ML/*!< Sterile 2 ml microcentrifuge tube */, STERILE_PCR_TUBE/*!< Sterile 0.6-ml tube */, CENTRI_TUBE_50ML/*!< 50-ml centrifuge tube */, CRYO_VIAL/*!< screw-topped cryo vial */, SCREW_CAP_TUBE/*!< Screw-cap tube of appropriate volume */};

struct Fluid
{
	//graphNode* node;/*!< The current graph node. */
	std::string original_name;/*!< The name of the fluid. */
	std::string new_name;/*!< The name of the fluid after renaming. Initially, original_name = new_name */
	CONTAINER_TYPE container;/*!< The container of the fluid. */
	std::string state;/*!< The state of the fluid. */
	float volume;/*!< The volume of the fluid. */
	enum VOLUME_UNIT unit;/*!< The unit of volume. */
	FLUID_TYPE type;/*!< The type of this resource. */
	int used;/*!< Used internally to track the usage of the fluid. */
	int usage_index;/*!< Used internally to track the usage of the fluid. */

	Fluid()
	:original_name(""),new_name(""),container(STERILE_MICROFUGE_TUBE),state(""),volume(0),unit(LITER),type(FLUID), used(0),usage_index(0)
	{}
	Fluid(const Fluid & f)
	:original_name(f.original_name),new_name(f.new_name),container(f.container),state(f.state),volume(f.volume),unit(f.unit), type(f.type),used(f.used),usage_index(f.usage_index)
	{}

	~Fluid()
	{
	}

	std::string toString(){
		return original_name;

	}

	std::string toString(std::string buffer, Volume v)
	{
		std::string ret = buffer + "\"VARIABLE\" : {\n";
		ret += buffer + "\t" + "\"NAME\" : " + original_name + ",\n";
		ret += v.toString(buffer+"\t");
		ret += buffer + "}\n";
		return ret;
	}
};






}
#endif /* FLUID_H_ */
