/*
 * BioSystem.cpp
 *
 *  Created on: Aug 11, 2015
 *      Author: Chris
 */

#include <iostream>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include "../Headers/BioSystem.h"

namespace BioCoder
{
void BioSystem :: ClearContainerOpList(Container * c)
{
	c->_instructionStack.clear();
}
void BioSystem :: AddOpToContainer(BioOperation* op, Container * container)
{
	container->_instructionStack.push_back(op);
}
void BioSystem :: BioGraphMaintance(BioOperation * op)
{
	if(op->_opType == DISPENSE)
		this->_roots.push_back(op);

	if(this->_userDefinedLevelTree.size()==0) {
		std::vector<BioOperation*> bioOperations;
		this->_userDefinedLevelTree.push_back(bioOperations);
	}

	this->_userDefinedLevelTree.at(_userDefinedLevelTree.size()-1).push_back(op);
}
void BioSystem :: SetOpsParent(BioOperation * op, Container * container)
{

	if(container->_instructionStack.size() == 0) {
		std::cerr<<"Error Parents stack is empty. Expected to find parents for OP:"<< op->Name() << std::endl;
		exit(-1);
	}

	for(unsigned int parentIndex = 0; parentIndex < container->_instructionStack.size(); ++parentIndex ) {
		container->_instructionStack.at(parentIndex)->_children.push_back(op);
		op->_parents.push_back(container->_instructionStack.at(parentIndex));
	}

}

void BioSystem :: TransferOperation(Container* source, Container* destination, bool warning)
{
	if(warning)
		std::cerr<<"Implicit transfer contents call." <<std::endl;

	for(unsigned int opIndex = 0; opIndex < source->_instructionStack.size(); ++opIndex){
		destination->_instructionStack.push_back(source->_instructionStack.at(opIndex));
	}

	source->_instructionStack.clear();
}

void BioSystem :: PrintLeveledProtocol()
{
	for(unsigned int level = 0; level < _userDefinedLevelTree.size(); ++level)
	{
		std::cout << "Level: " << level << "\n\t";
		for(unsigned int operation = 0; operation < _userDefinedLevelTree[level].size(); ++operation)
			std::cout<<_userDefinedLevelTree[level][operation]->Name() << " ";
		std::cout<<std::endl;
	}
}




void BioSystem :: start_protocol(std::string name)
{	//BioCoder Legacy
	filename = name;
	std::string fName = "Output/Bio_Instruc_";
	fName+= filename;
	fName+= ".html";
	fp = fopen(fName.c_str(), "w");
	fprintf(fp, "<h1 style=\"font-size = 25px;\">%s</h1>", name.c_str());
}

void BioSystem ::first_step()
{
	this->first_step("");
}

void BioSystem ::first_step(std::string step_name)
{
	_currentStep = 0;
	std::vector<BioOperation*> bioOperations;
	this->_userDefinedLevelTree.push_back(bioOperations);

	//BioCoder Legacy

	first = 1;
	fprintf(fp, "</ul><h2>Steps:</h2><ol>");
	fprintf(fp, "<p><li><b><font size=3>%s</font></b><br>", step_name.c_str());

}

void BioSystem :: next_step()
{
	this->next_step("");
}

void BioSystem :: next_step(std::string step_name)
{
	++_currentStep;
	std::vector<BioOperation*> bioOperations;
	this->_userDefinedLevelTree.push_back(bioOperations);

	//BioCoder Legacy

	first = 0;
	if (sub_step_flag == 1)
	{
		fprintf(fp,"</li></p></ol>");
		sub_step_flag = 0;
	}
	fprintf(fp, "</li></p><p><li><b><font size=3>%s</font></b><br>", step_name.c_str());
}

void BioSystem :: parallel_step()
{
	//BioCoder Legacy

	first = 0;
	if (sub_step_flag == 1)
	{
		fprintf(fp, "</li></p></ol>");
		sub_step_flag = 0;
	}
	fprintf(fp, "</li></p><p><li><b>Meanwhile:</b><br>");
}

void BioSystem :: first_sub_step()
{
	//BioCoder Legacy

	first = 0;
	fprintf(fp, "<ol type=\"a\"><p><li>");
	sub_step_flag = 1;
}
void BioSystem :: next_sub_step()
{
	//BioCoder Legacy

	fprintf(fp, "</li></p><p><li>");
	sub_step_flag = 1;
}
void BioSystem :: end_protocol()
{
	//proSteps->collectOutput();
	//int i;
	//export_graph(filename.c_str());

	//BioCoder Legacy

	fprintf(fp, "</li></p></ol>");
	display_equip();
	usage_details();
	timing_details();
	print_reagents = 1;
	print_parameters = 1;
	option_no = 1;
	equip_no = 0;
	microfuge_no = 1;
	centrifuge_no = 1;
	incubator_no = 1;
	electrophoresis_no = 1;
	mortar_no = 1;
	thermocycler_no = 1;
	electro_no = 1;
	shaker_no = 1;
	spectrophotometer_no = 1;
	cont_no = 0;
	prev_cont = 1;
	list_fluid_no = 0;
	total_time_required = 0;
	list_container_no = 0;
	fclose(fp);
}


void BioSystem :: display_equip()
{
	int i;
	fprintf(fp, "<div style=\"top: 25px; margin-top: 50px; margin-left: 700px; position: absolute; z-index: 1; visibility: show;\">");
	fprintf(fp, "<h2>Equipment:</h2><ul type=\"circle\">");
	for(i=0; i<equip_no; i++)
		fprintf(fp, "<li>%s</li>", equipments[i]);
	for(i=0; i<cont_no; i++)
		fprintf(fp, "<li>%s</li>", containers[i]);
	fprintf(fp, "</ul></div>");
}

void BioSystem :: usage_details()
{
	int i;
	int count1 = 0;
	int count2 = 0;
	for(i=0;i<list_fluid_no;i++)
	{
		if (usage_list_fluids[i]->used == 1)
			continue;
		if (count1 == 0)
		{
			fprintf(fp, "<font color = red>Warning: The following reagents/materials were declared but not used in the protocol:</font><ul>");
			fprintf(fp, "<li>%s</li>", usage_list_fluids[i]->original_name.c_str());
			count1++;
		}
		else
			fprintf(fp, "<li>%s</li>", usage_list_fluids[i]->original_name.c_str());
	}
	fprintf(fp, "</ul>");
	for(i=0;i<list_container_no;i++)
	{
		if (usage_list_containers[i]->used == 1)
			continue;
		if (count2 == 0)
		{
			fprintf(fp, "<font color = red>Warning: The following equipment were declared but not used in the protocol:</font><ul>");
			fprintf(fp, "<li>%s</li>", usage_list_containers[i]->name.c_str());
			count1++;
		}
		else
			fprintf(fp, "<li>%s</li>", usage_list_containers[i]->name.c_str());
	}
	fprintf(fp, "</ul>");
}

void BioSystem :: timing_details()
{
	int time_hrs = total_time_required/3600;
	int time_mins = (total_time_required%3600)/60;
	if (time_hrs == 0)
		fprintf(fp, "<p><b>TOTAL TIME REQUIRED FOR THE COMPLETION OF THE PROTOCOL :<font color=#357EC7>~ %d mins</font></b></p>", time_mins);
	else if (time_hrs == 1)
		fprintf(fp, "<p><b>TOTAL TIME REQUIRED FOR THE COMPLETION OF THE PROTOCOL :<font color=#357EC7>~ %d hr, %d mins</font></b></p>", time_hrs, time_mins);
	else
		fprintf(fp, "<p><b>TOTAL TIME REQUIRED FOR THE COMPLETION OF THE PROTOCOL :<font color=#357EC7>~ %d hrs, %d mins</font></b></p>", time_hrs, time_mins);
	total_time_required = 0;
}

Fluid * BioSystem :: new_fluid(std::string name)
{
	Volume vol;
	return this->new_fluid(name, "", -32767, vol);
}

Fluid * BioSystem :: new_fluid(std::string name, float temp)
{
	Volume vol;
	return this->new_fluid(name, "", temp, vol);
}

Fluid * BioSystem :: new_fluid(std::string name, std::string state)
{
	Volume vol;
	return this->new_fluid(name, state, -32767, vol);
}

Fluid * BioSystem :: new_fluid(std::string name, Volume volume1)
{
	return this->new_fluid(name, "", -32767, volume1);
}

Fluid * BioSystem :: new_fluid(std::string name, float temp, Volume volume1)
{
	return this->new_fluid(name, "", temp, volume1);
}

Fluid * BioSystem :: new_fluid (std::string name, std::string state, float temp)
{
	Volume vol;
	return this->new_fluid(name, state, temp, vol);
}

Fluid * BioSystem :: new_fluid(std::string name, std::string state, Volume volume1)
{
	return this->new_fluid(name, state, -32767, volume1);
}

Fluid * BioSystem :: new_fluid (std::string name, std::string state, float temp, Volume volume1)
{
	std:: string t1 = "";
	//	char* t2 = "";
	Fluid * result = new Fluid();
	result->state= state;
	result->original_name = name;
	result->new_name = name;
	result->type = FLUID;
	result->volume = volume1.GetValue();
	result->unit = volume1.GetVolumeUnits();
	if(print_reagents == 1)
	{
		fprintf(fp, "<h2 style=\"margin-top:50px;\">Solutions/reagents:</h2><ul type=\"circle\">");
		print_reagents++;
	}
	if(temp != -32767)
	{
		if(temp == ICE_COLD)
		{
			t1 = "ice-cold" + name;
			result->new_name = t1;
			fprintf(fp, "<li> <a name=\"%s\"> %s <i><br><tab><div style=\"margin-right: 600px;\">(%s)</div></i></a></li>", result->new_name.c_str(), result->new_name.c_str(), state.c_str());
		}
		else if(temp == ROOM_TEMPERATURE)
			fprintf(fp, "<li>%s stored at room temperature</li>", name.c_str());
		else if(temp >= 35)
			fprintf(fp, "<li> <a name=\"%s\">%s preheated in a water bath set at <b><font color=#357EC7>%g%cC</font></b> <i>%s</i></a></li>", name.c_str(), name.c_str(), temp, 0x00B0, state.c_str());
		else
			fprintf(fp, "<li> <a name=\"%s\">%s <i>%s</i> </a> at <b><font color=#357EC7>%g%cC</font></b></li>", name.c_str(), name.c_str(), state.c_str(), temp, 0x00B0);
	}
	usage_list_fluids.push_back(result);
	result->used = 0;
	result->usage_index = usage_list_fluids.size()-1;
	list_fluid_no++;
	return result;
}

/*Fluid BioSystem :: new_operation(char* name)
{
	Fluid result;
	result.new_name = name;
	result.original_name = name;
	result.type = OPERATION;
	return result;
}*/

/*void BioSystem :: store_container_names (int i, char* name)
{
	if(container_count[i] == 0)
	{
		containers[cont_no] = name;
		cont_no++;
	}
}*/

Container * BioSystem :: new_container(enum CONTAINER_TYPE cont_id)
{
	return this->new_container(cont_id, NULL);
}

Container * BioSystem :: new_container(enum CONTAINER_TYPE cont_id, Fluid* fluid1)
{
	Container * result = new Container();
	result->type = CONTAINER;
	if(fluid1 != NULL)
		result->contents = fluid1;
	//result->contents.push_back(fluid1);
	char* temp1 = (char *)calloc(50, sizeof(char));
	char* temp2 = (char *)calloc(50, sizeof(char));
	result->id = cont_id;
	switch(cont_id){
	case STERILE_MICROFUGE_TUBE:sprintf(temp2, " (%d)", container_count[0]+1);temp1 = strcat(temp1, "sterile 1.5-ml microcentrifuge tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[0]++;break;
	case CENTRIFUGE_TUBE_15ML:sprintf(temp2, " (%d)", container_count[1]+1);temp1 = strcat(temp1, "sterile 15-ml centrifuge tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[1]++;break;
	case FLASK:sprintf(temp2, " (%d)", container_count[2]+1);temp1 = strcat(temp1, "flask");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[2]++;break;
	case CENTRIFUGE_BOTTLE:sprintf(temp2, " (%d)", container_count[3]+1);temp1 = strcat(temp1, "centrifuge bottle");temp1 = strcat(temp1, temp2);result->name = temp1;break;
	case GRADUATED_CYLINDER:result->name = "a graduated cylinder";break;
	case RXN_TUBE:sprintf(temp2, " (%d)", container_count[5]+1);temp1 = strcat(temp1, "reaction tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[5]++;break;
	case FRESH_COLL_TUBE:sprintf(temp2, " (%d)", container_count[6]+1);temp1 = strcat(temp1, "fresh collection tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[6]++;break;
	case LIQUID_NITROGEN:result->name = "a container with liquid nitrogen";break;
	case PLG:result->name = "a 50- ml PLG tube"; break;
	case OAKRIDGE:sprintf(temp2, " (%d)", container_count[9]+1);temp1 = strcat(temp1, "Oakridge tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[9]++;break;
	case QIA_CARTRIDGE:result->name= "a QIAfilter cartridge";break;
	case CUVETTE_ICE:result->name= "a cuvette stored on ice";break;
	case SPEC_CUVETTE:result->name= "a spectrometry cuvette";break;
	case STOCK_PLATE_96:result->name = "a 96-well stock plate";break;
	case WELL_BLOCK_96:result->name = "a 96-well block";break;
	case PCR_PLATE:result->name = "a 96-well PCR plate";break;
	case LIQUID_BLOCK:result->name = "a 96-well liquid block";break;
	case CELL_CULT_CHAMBER:result->name = "a cell culture chamber";break;
	case EPPENDORF:sprintf(temp2, " (%d)", container_count[18]+1);temp1 = strcat(temp1, "Eppendorf tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[18]++;break;
	case STERILE_MICROFUGE_TUBE2ML:printf(temp2, " (%d)", container_count[19]+1);temp1 = strcat(temp1, "sterile 2-ml microcentrifuge tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[19]++;break;
	case STERILE_PCR_TUBE:sprintf(temp2, " (%d)", container_count[20]+1);temp1 = strcat(temp1, "sterile 0.6-ml microcentrifuge tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[20]++;break;
	case CENTRI_TUBE_50ML:sprintf(temp2, " (%d)", container_count[21]+1);temp1 = strcat(temp1, "50-ml centrifuge tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[21]++;break;
	case CRYO_VIAL:result->name = "a screw-topped cryo vial";break;
	case SCREW_CAP_TUBE:sprintf(temp2, " (%d)", container_count[23]+1);temp1 = strcat(temp1, "screw-cap tube");temp1 = strcat(temp1, temp2);result->name = temp1;container_count[23]++;break;
	default:break;
	}
	result->used = 0;
	usage_list_containers.push_back( result);
	result->usage_index = usage_list_containers.size()-1;
	list_container_no++;
	if(fluid1 != NULL)
		set_container(fluid1, result);
	return result;
}

void BioSystem :: set_container(Fluid* fluid1, Container* container1)
{
	if (usage_list_fluids[fluid1->usage_index]->original_name == fluid1->original_name)
	{
		usage_list_fluids[fluid1->usage_index]->used = 1;
	}
	else
	{
		fluid1->used = 1;
		usage_list_fluids[list_fluid_no] = fluid1;
		fluid1->usage_index = list_fluid_no;
		list_fluid_no++;
	}
	fluid1->container = container1->id;

	container1->contents = fluid1;
	container1->volume += fluid1->volume;
	//container1->contents.push_back(fluid1);
	//container1->volume = container1->contents[container1->contents.size()-1]->volume;
}

void BioSystem :: measure_fluid(Fluid* fluid1, Container* container1) // measure Fluid
{
	this->measure_fluid(fluid1, Volume(), container1);
}

void BioSystem :: measure_fluid(Fluid *fluid1, Volume volume1, Container* container1)
{

	BioOperation * disp = new BioOperation(_opNum++,DISPENSE,volume1,fluid1);

	this->AddOpToContainer(disp,container1);
	this->BioGraphMaintance(disp);

	if ((fluid1->volume == 0)|| (fluid1->volume < (volume1.GetValue(NANO_LITER))))
		fprintf(fp, "<font color = red>Warning: You are out of %s! Please make sure you have enough before carrying on with the protocol.<br></font>", fluid1->new_name.c_str());
	if (usage_list_fluids[fluid1->usage_index]->original_name == fluid1->original_name)
		usage_list_fluids[fluid1->usage_index]->used = 1;
	else
	{
		fluid1->used = 1;
		usage_list_fluids[list_fluid_no] = fluid1;
		fluid1->usage_index = list_fluid_no;
		list_fluid_no++;
	}
	if (usage_list_containers[container1->usage_index]->name == container1->name)
		usage_list_containers[container1->usage_index]->used = 1;
	else
	{
		container1->used = 1;
		usage_list_containers[list_container_no] = container1;
		container1->usage_index = list_container_no;
		list_container_no++;
	}

	if(volume1.GetVolumeUnits() != VOLUME_NOT_SPECIFIED)
	{
		fluid1->volume = volume1.GetValue();
		fluid1->unit=volume1.GetVolumeUnits();

		prev_container = container1->name;
		fprintf(fp, "Measure out ");
		volume1.display_vol(fp);
		fprintf(fp, "of ");
	}
	else
		fprintf(fp, "Measure out ");


	if (first == 1)
	{
		if (container1->contents == NULL || container1->contents->new_name == "")
		{
			if(fluid1->state == "")
				fprintf(fp, "<font color=#357EC7>%s</font> into %s.<br>", fluid1->new_name.c_str(), container1->name.c_str());
			else
				fprintf(fp, "<a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", fluid1->new_name.c_str(), fluid1->new_name.c_str(), container1->name.c_str());
			first = 0;
			prev_cont++;
		}
		else
		{
			if(fluid1->state == "")
				fprintf(fp, "<font color=#357EC7>%s</font> into %s.<br>", fluid1->new_name.c_str(), container1->contents->new_name.c_str());
			else
				fprintf(fp, "<a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", fluid1->new_name.c_str(), fluid1->new_name.c_str(), container1->contents->new_name.c_str());
			first = 0;
			prev_cont++;
		}
	}

	else if (prev_cont != 1 && prev_container == container1->name)
	{
		if(fluid1->state == "")
		{
			fprintf(fp, "<font color=#357EC7>%s</font>.<br>", fluid1->new_name.c_str());
		}
		else
		{
			fprintf(fp, "<a href=\"#%s\" ><font color=#357EC7>%s</font></a>.<br>", fluid1->new_name.c_str(), fluid1->new_name.c_str());
		}
	}
	else
	{
		if(fluid1->state == "")
		{
			fprintf(fp, "<font color=#357EC7>%s</font> into %s.<br>", fluid1->new_name.c_str(), container1->name.c_str());
			prev_container = container1->name;
		}
		else
		{
			fprintf(fp, "<a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", fluid1->new_name.c_str(), fluid1->new_name.c_str(), container1->name.c_str());
			prev_container = container1->name;
		}

		if(prev_cont == 1)
			prev_cont++;
	}
	fluid1->container = container1->id;
	container1->contents = fluid1;
	container1->volume = container1->volume + volume1.GetValue(NANO_LITER);
}

void BioSystem::measure_fluid(Container * source, Container * destination) // Transfer OPERATION
{


	this->TransferOperation(source, destination,true);


	if(source->contents ==NULL)
	{
		Fluid* f = this->new_fluid("temp");
		source->contents = f;
	}

	if (source->volume == 0)
		fprintf(fp, "<font color = red>Warning: You are out of %s! Please make sure you have enough before carrying on with the protocol.<br></font>", source->name.c_str());
	if (usage_list_containers[source->usage_index]->name == source->name)
		usage_list_containers[source->usage_index]->used = 1;
	else
	{
		source->used = 1;
		usage_list_containers[list_container_no] = source;
		source->usage_index = list_container_no;
		list_container_no++;
	}
	if (usage_list_containers[destination->usage_index]->name == destination->name)
		usage_list_containers[destination->usage_index]->used = 1;
	else
	{
		destination->used = 1;
		usage_list_containers[list_container_no] = destination;
		destination->usage_index = list_container_no;
		list_container_no++;
	}
	if (first == 1)
	{
		prev_container = destination->name;
		if (destination->contents == NULL || destination->contents->new_name == "")
		{
			if(source->contents !=NULL && source->contents->state == "")
				fprintf(fp, "Measure out %s into %s.<br>", source->contents->new_name.c_str(), destination->name.c_str());
			else
				fprintf(fp, "Measure out <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->name.c_str(), source->name.c_str(), destination->name.c_str());
			first = 0;
			prev_cont++;
		}
		else
		{
			if(source->contents!= NULL && destination->contents != NULL && source->contents->state == "")
				fprintf(fp, "Measure out %s into %s.<br>", source->contents->new_name.c_str(), destination->contents->new_name.c_str());
			else
				fprintf(fp, "Measure out <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->name.c_str(), source->name.c_str(), destination->name.c_str());
			first = 0;
			prev_cont++;
		}
	}
	else if (prev_cont == 1)
	{
		prev_container = destination->name;
		if( source->contents!= NULL && source->contents->state == "")
			fprintf(fp, "Measure out %s into %s.<br>", source->contents->new_name.c_str(), destination->name.c_str());
		else
			fprintf(fp, "Measure out <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->name.c_str(), source->name.c_str(), source->name.c_str());
		prev_cont++;
	}
	else if(prev_container == destination->name)
	{
		fprintf(fp, "Add <font color=#357EC7>%s</font> to %s.<br>", source->contents->new_name.c_str(), destination->contents->new_name.c_str());
		prev_container = destination->name;
	}
	else if(source->contents!= NULL && destination->contents != NULL && source->contents->state == "")
	{
		fprintf(fp, "Measure out %s into %s.<br>", source->contents->new_name.c_str(), destination->name.c_str());
		prev_container = destination->name;
	}
	else
	{
		fprintf(fp, "Measure out <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->contents->new_name.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
		prev_container = destination->name;
	}
	source->contents->container = destination->id;
	//proSteps.addToMixtureList(container,container1);
	destination->contents = source->contents;
	destination->volume = destination->volume + source->volume;
	source->volume=0;
//	container.tLinkName.clear();
}


void BioSystem :: measure_fluid(Container* source, Volume volume1, Container* destination, bool ensureMeasurement)
{

	BioOperation* split = new BioOperation(this->_opNum++, SPLIT, volume1);
	this->SetOpsParent(split,source);
	this->BioGraphMaintance(split);
	this->AddOpToContainer(split,source);
	this->AddOpToContainer(split,destination);
	this->BioGraphMaintance(split);


	//BioCoder Legacy
	if ((source->volume == 0) || (source->volume < volume1.GetValue(NANO_LITER)))
		fprintf(fp, "<font color = red>Warning: You are measuring out more than the available volume of %s! Please make sure you have enough before carrying on with the protocol.<br></font>", source->contents->new_name.c_str());
	if (usage_list_containers[source->usage_index]->name == source->name.c_str())
		usage_list_containers[source->usage_index]->used = 1;
	else
	{
		source->used = 1;
		usage_list_containers[list_container_no] = source;
		source->usage_index = list_container_no;
		list_container_no++;
	}
	if (usage_list_containers[destination->usage_index]->name == destination->name)
		usage_list_containers[destination->usage_index]->used = 1;
	else
	{
		destination->used = 1;
		usage_list_containers[list_container_no] = destination;
		destination->usage_index = list_container_no;
		list_container_no++;
	}
	if (first == 1)
	{
		source->contents->volume = source->contents->volume - volume1.GetValue(NANO_LITER);
		prev_container = destination->name;
		fprintf(fp, "Measure out ");
		volume1.display_vol(fp);
		if (destination->contents != NULL || destination->contents->new_name == "")
		{
			if(source->contents != NULL || source->contents->state == "")
				fprintf(fp, " of <font color=#357EC7>%s</font> into %s.<br>", source->contents->new_name.c_str(), destination->name.c_str());
			else
				fprintf(fp, " of <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->contents->new_name.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
			first = 0;
			prev_cont++;
		}
		else
		{
			if(source->contents->state == "")
				fprintf(fp, " of <font color=#357EC7>%s</font> into %s.<br>", source->contents->new_name.c_str(), destination->contents->new_name.c_str());
			else
				fprintf(fp, " of <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->contents->new_name.c_str(), source->contents->new_name.c_str(), destination->contents->new_name.c_str());
			first = 0;
			prev_cont++;
		}
	}
	else if (prev_cont == 1)
	{
		source->contents->volume = source->contents->volume - volume1.GetValue(NANO_LITER);
		prev_container = destination->name;
		fprintf(fp, "Measure out ");
		volume1.display_vol(fp);
		if(source->contents->state == "")
			fprintf(fp, " of <font color=#357EC7>%s</font> into %s.<br>", source->contents->new_name.c_str(), destination->name.c_str());
		else
			fprintf(fp, " of <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->contents->new_name.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
		prev_cont++;
	}
	else if (prev_container == destination->name)
	{
		source->contents->volume = source->contents->volume + volume1.GetValue(NANO_LITER);
		if(source->contents->state == "")
		{
			fprintf(fp, "Add ");
			volume1.display_vol(fp);
			fprintf(fp, " of <font color=#357EC7>%s</font>.<br>", source->contents->new_name.c_str());
		}
		else
		{
			fprintf(fp, "Add ");
			volume1.display_vol(fp);
			fprintf(fp, " of <a href=\"#%s\" ><font color=#357EC7>%s</font></a>.<br>", source->contents->new_name.c_str(), source->contents->new_name.c_str());
		}
	}
	else
	{
		source->contents->volume = source->contents->volume - volume1.GetValue(NANO_LITER);
		fprintf(fp, "Measure out ");
		volume1.display_vol(fp);
		if(source->contents->state == "")
		{
			fprintf(fp, " of <font color=#357EC7>%s</font> into %s.<br>", source->contents->new_name.c_str(), destination->name.c_str());
			prev_container = destination->name;
		}
		else
		{
			fprintf(fp, " of <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>", source->contents->new_name.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
			prev_container = destination->name;
		}
	}
	source->contents->container = destination->id;
	destination->contents = source->contents;
	destination->volume = destination->volume + volume1.GetValue(NANO_LITER);
	source->volume-= volume1.GetValue(NANO_LITER);
}

void measure_fluid(Container * source, int NumSplit, int piecesToDest, Container * Dest, bool ensureMeasurement = false)
{
	if(NumSplit < 2)
		return;
}



}

