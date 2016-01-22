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

void BioSystem :: ClearAllContainerOpList(BioOperation* operation)
{
	for(Container* container: usage_list_containers) {
		this->ClearContainerOpList(container, operation);
	}
}
void BioSystem :: ClearContainerOpList(Container * c, BioOperation* operation)
{
	for (std::vector<BioOperation*>::reverse_iterator it = c->_instructionStack.rbegin(); it != c->_instructionStack.rend();  ++it){
		BioOperation * op = (*it);

		//if you hit a conditional operation and the caller is not a conditional terminator we stop, otherwise clean up that conditional.
		if(op->_opType == IF_OP || op->_opType == ELSE_IF_OP || op->_opType == ELSE_OP )
		{
			if( operation != NULL && operation->_opType == END_IF_OP) {
				if (op->_opType == IF_OP ) {
					//Set false branch location.

					//remove reference to if and terminates the End IF Call.
					c->_instructionStack.erase(--(it.base()));
					return;
				}
				else//remove reference to else if and else
					c->_instructionStack.erase(--(it.base()));

			}
			else if( operation != NULL && operation->_opType == END_WHILE_OP) {
				std::cerr<<"Error: IF statement not properly terminated on END_WHILE Call.\n";
				exit(-1);
			}

			else // caller was an operation within if statement.
				return;
		}
		else if(op->_opType == WHILE_OP){
			if( operation != NULL && operation->_opType == END_IF_OP) {
				std::cerr<<"Error: While statement not properly terminated on END_IF Call.\n";
				exit(-1);
				//c->_instructionStack.erase(--(it.base()));
				//return;
			}
			else if(operation != NULL && operation->_opType == END_WHILE_OP) {
				//set my child as the loop header. This completes loop DAG.
				operation->_children.push_back(op);
				op->_parents.push_back(operation);

				// delete the while header from the instruction stack to ensure that an instuction is not mistakenly taken as a while instuction.
				c->_instructionStack.erase(--(it.base()));
				return;
			}
			else// caller was an operation within while loop.
				return;
		}
		else {//erase element
			c->_instructionStack.erase(--(it.base()));
		}

	}
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
void BioSystem :: SetOpsParent(BioOperation * op)
{
	if (op->_opType == END_IF_OP)
		return SetEndIfparents(op);

	for(Container* container: usage_list_containers) {
		this->SetOpsParent(op, container);
	}
}
void BioSystem :: SetOpsParent(BioOperation * op, Container * container)
{

	if(container->_instructionStack.size() == 0) {
		return;
		//		std::cerr<<"Error Parents stack is empty. Expected to find parents for OP:"<< op->Name() << std::endl;
		//		exit(-1);
	}
	BioOperation* parent = container->_instructionStack.at(container->_instructionStack.size()-1);


	//if this is the operation after the END WHILE. Take the END_WHILE child (While Header) and set that as your parent.
	if (parent->_opType == END_WHILE_OP){
		BioOperation * whileHeader = parent->_children.at(0);

		//sets false branch.
		whileHeader->_children.push_back(op);
		op->_parents.push_back(whileHeader);
		return;
	}

	//if this is the first instruction after an if/else if/ else/ while. this is the instruction to execute for true branch.
	if(parent->_opType == IF_OP || parent->_opType == ELSE_IF_OP || parent->_opType == ELSE_OP  || parent->_opType == WHILE_OP ){
		if (!(op->_opType == ELSE_IF_OP || op->_opType == ELSE_OP)) {
			//else if and else can never be the target of a true branch.
			parent->_trueBranch = op;

			parent->_children.push_back(op);
			op->_parents.push_back(parent);

			return;

		}
	}

	if (op->_opType == DISPENSE || parent->_opType == OUTPUT || parent->_opType == WASTE)
		return;//A dispense can never have a parent other than a conditional object and an OUPUT can never have a child

	if(op->_opType != ELSE_IF_OP && op->_opType != ELSE_OP ) {
		parent->_children.push_back(op);
		op->_parents.push_back(parent);
	}


	for(int index = container->_instructionStack.size()-2; index >=0; --index )
	{
		parent = container->_instructionStack.at(index);

		if(parent->_opType == IF_OP || parent->_opType == ELSE_IF_OP || parent->_opType == ELSE_OP || parent->_opType == WHILE_OP ){
			if (op->_opType == ELSE_IF_OP || op->_opType == ELSE_OP) { // set the false branch.
				parent->_children.push_back(op);
				op->_parents.push_back(parent);
			}
			return;
		}
		parent->_children.push_back(op);
		op->_parents.push_back(parent);
	}
}
void BioSystem :: SetConditionalOpsParent(BioOperation * op, Container * container)
{
	//should be used for ELSE IF && ELSE Conditional OPS
	//Sets the branching order corectly.
	if(container->_instructionStack.size() == 0) {
		std::cerr<<"Error Parents stack is empty. Expected to find parents for OP:"<< op->Name() << std::endl;
		exit(-1);
	}

	for(int index = container->_instructionStack.size()-1; index >=0; --index )
	{
		BioOperation* parent = container->_instructionStack.at(index);

		if(parent->_opType == IF_OP || parent->_opType == ELSE_IF_OP) {
			parent->_children.push_back(op);
			op->_parents.push_back(parent);
			return;
		}


	}

	std::cerr<<"Error: Expected to find Conditional parent for OP:"<< op->Name() << std::endl;
	exit(-1);
}

void BioSystem:: SetEndIfparents(BioOperation * operation, Container * container)
{
	//TODO:: HANDLE While statements that can be nested.

	if(container->_instructionStack.size() == 0) {
		std::cerr<<"Error Parents stack is empty. Expected to find parents for OP:"<< operation->Name() << std::endl;
		exit(-1);
	}
	if (operation->_opType != END_IF_OP)
	{
		std::cerr<<"Error: Calling set Endif on Non END if OP";
		exit(-1);
	}
	//	bool bypassLine = false;

	bool elseFound = false;

	for(int index = container->_instructionStack.size()-1; index >=0; --index ) {
		BioOperation* parent = container->_instructionStack.at(index);

		if(parent->_opType == IF_OP){
			if (elseFound == false) {//setting false branch edge when an ELSE branch is not explictly called.
				parent->_children.push_back(operation);
				operation->_parents.push_back(parent);
			}
			return;
		}
		if( parent->_opType == ELSE_OP) {
			elseFound= true;
			continue;
		}
		else if(parent->_opType == ELSE_IF_OP) {
			if (elseFound == false) {//setting false branch edge when an ELSE branch is not explictly called.
				parent->_children.push_back(operation);
				operation->_parents.push_back(parent);
				elseFound = true;
			}
			continue;
		}
		else {
			parent->_children.push_back(operation);
			operation->_parents.push_back(parent);
		}
	}
}

void BioSystem:: SetEndIfparents(BioOperation * operation)
{
	for(Container* container: usage_list_containers) {
		std::cout <<"printing stack"<<std::endl;
		container->PrintOpStack();
		std::cout<<"END"<<std::endl;

		this->SetEndIfparents(operation, container);
	}
}

void BioSystem:: AddOPToAllContainers(BioOperation* operation)
{
	for(Container* container: usage_list_containers) {
		this->AddOpToContainer(operation, container);
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


void BioSystem :: check_container(Container* container1)
{
	if ((first == 1) || (prev_cont == 1)||(prev_container != container1->name))
	{
		prev_container = container1->name;
		fprintf(fp, "%s", container1->name.c_str());
	}
}

void BioSystem::TransferMethodHelper(Container * source, Container * destination, std::string transferWording)
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
				fprintf(fp, "%s %s into %s.<br>",transferWording.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
			else
				fprintf(fp, "%s <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>",transferWording.c_str(), source->name.c_str(), source->name.c_str(), destination->name.c_str());
			first = 0;
			prev_cont++;
		}
		else
		{
			if(source->contents!= NULL && destination->contents != NULL && source->contents->state == "")
				fprintf(fp, "%s %s into %s.<br>",transferWording.c_str(), source->contents->new_name.c_str(), destination->contents->new_name.c_str());
			else
				fprintf(fp, "%s <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>",transferWording.c_str(), source->name.c_str(), source->name.c_str(), destination->name.c_str());
			first = 0;
			prev_cont++;
		}
	}
	else if (prev_cont == 1)
	{
		prev_container = destination->name;
		if( source->contents!= NULL && source->contents->state == "")
			fprintf(fp, "%s %s into %s.<br>", transferWording.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
		else
			fprintf(fp, "%s <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>",transferWording.c_str() , source->name.c_str(), source->name.c_str(), source->name.c_str());
		prev_cont++;
	}
	else if(prev_container == destination->name)
	{
		fprintf(fp, "Add <font color=#357EC7>%s</font> to %s.<br>", source->contents->new_name.c_str(), destination->contents->new_name.c_str());
		prev_container = destination->name;
	}
	else if(source->contents!= NULL && destination->contents != NULL && source->contents->state == "")
	{
		fprintf(fp, "%s %s into %s.<br>", transferWording.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
		prev_container = destination->name;
	}
	else
	{
		fprintf(fp, "%s <a href=\"#%s\" ><font color=#357EC7>%s</font></a> into %s.<br>",transferWording.c_str(), source->contents->new_name.c_str(), source->contents->new_name.c_str(), destination->name.c_str());
		prev_container = destination->name;
	}
	source->contents->container = destination->id;
	//proSteps.addToMixtureList(container,container1);
	destination->contents = source->contents;
	destination->volume = destination->volume + source->volume;
	source->volume=0;
	//	container.tLinkName.clear();

}

void BioSystem::MixHelper(Container* container, MIX_TYPE mixtype, EXPERIMENT_EVENT event, Time time, int numInverting)
{
	std::string type;
	switch(mixtype)
	{
	case TAP:
		if(time.GetTimeUnits() == TIME_NOT_SPECIFIED)
			fprintf(fp, "Gently tap the mixture for a few secs.<br>");
		else
		{
			fprintf(fp, "Gently tap the mixture for ");
			time.display_time(fp,option_no,options_flag,total_time_required);
			fprintf(fp, " .<br>");
		}
		type = "Tapping";
		break;
	case STIR:
		if(time.GetTimeUnits() == TIME_NOT_SPECIFIED)
			fprintf(fp, "Stir the mixture for a few secs.<br>");
		else
		{
			fprintf(fp, "Stir the mixture for ");
			time.display_time(fp,option_no,options_flag,total_time_required);
			fprintf(fp, " .<br>");
		}
		type = "Stirring";
		break;
	case INVERT:
		if(time.GetTimeUnits() == TIME_NOT_SPECIFIED)
		{
			fprintf(fp, "Close the tube tightly and gently mix the contents by inverting the tube.<br>");
			if(numInverting != -1)
				fprintf(fp,"<b><font color=#357EC7>%d times</font></b>.", numInverting);
			fprintf(fp, "<br>");
		}
		else
		{
			fprintf(fp, "Close the tube tightly and invert the tube ");
			time.display_time(fp,option_no,options_flag,total_time_required);
			fprintf(fp, " times.<br>");
		}
		type = "Inverting";
		break;
	case VORTEX:
		if(time.GetTimeUnits() == TIME_NOT_SPECIFIED)
			fprintf(fp, "Vortex the mixture for a few secs.<br>");
		else
		{
			fprintf(fp, "Vortex the mixture for ");
			time.display_time(fp,option_no,options_flag,total_time_required);
			fprintf(fp, " .<br>");
		}
		type = "Vortexing";
		break;
	case RESUSPEND:fprintf(fp, "Resuspend pellet by vortexing by shaking vigorously.<br>");
	type = "Resuspending";
	break;
	case DISSOLVE:fprintf(fp, "Dissolve the pellet in the solution.<br>");
	type = "Dissolving";
	break;
	case PIPET:fprintf(fp, "Mix solution by pipetting up and down several times.<br>");
	type = "Pipetting";
	break;
	default: fprintf(fp, "Invalid entry.<br>");
	type = "";
	}

	switch(event)
	{
	case PPT_STOPS_STICKING:
		fprintf(fp, "Close the tube tightly and gently mix the contents by inverting the tube until precipitate stops sticking to walls of the tube.<br>");
		break;
	case PELLET_DISLODGES:
		fprintf(fp, "Gently mix the contents of the tube until the pellet dislodges.<br>");
		break;
	default:
		break;
	}

	BioOperation * mix = new BioOperation(this->_opNum++, MIX, mixtype);
	this->SetOpsParent(mix,container);
	this->BioGraphMaintance(mix);
	this->ClearContainerOpList(container);
	this->AddOpToContainer(mix, container);

}

void BioSystem:: name_sample(Container* container1, std::string new_name)
{
	container1->contents->new_name = new_name;
	container1->contents->state = "";
	container1->contents->used = 1;
	usage_list_fluids[list_fluid_no] = container1->contents;
	container1->contents->usage_index = list_fluid_no;
	list_fluid_no++;
}

void BioSystem:: name_container(Container* container1, std::string name)
{
	fprintf(fp, "Set aside a fresh %s. Call it %s. <br>", container1->name.c_str(), name.c_str());
	container1->name = name;
	usage_list_containers[container1->usage_index]->name = name;
	if (usage_list_containers[container1->usage_index]->name == container1->name)
		usage_list_containers[container1->usage_index]->used = 1;
	else
	{
		container1->used = 1;
		usage_list_containers[list_container_no] = container1;
		container1->usage_index = list_container_no;
		list_container_no++;
	}
}

void BioSystem :: PrintLeveledProtocol()
{
	for(unsigned int level = 0; level < _userDefinedLevelTree.size(); ++level)
	{
		std::cout << "Level: " << level << "\n\t";
		for(unsigned int operation = 0; operation < _userDefinedLevelTree[level].size(); ++operation)
			std::cout<< "(" << _userDefinedLevelTree[level][operation]->Name() << ") ";
		std::cout<<std::endl;
	}
}

void BioSystem :: PrintTree()
{
	for(std::vector< BioOperation*> op_vector: this->_userDefinedLevelTree)
	{
		for(BioOperation* op: op_vector)
			for (BioOperation* childOp: op->_children){
				std::cout << op->_ID << "->" << childOp->_ID;
				if(op->_opType == IF_OP || op->_opType == ELSE_IF_OP || op->_opType == WHILE_OP)
					if(op->_trueBranch == childOp)
						std::cout<<" *(" << *op->_expression << ")";
				std:: cout<< std::endl;
			}
	}
}

void BioSystem :: start_protocol(std::string name)
{	//BioCoder Legacy
	filename = name;
	std::string fName = "Bio_Instruc_";
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
	for(Container* container: usage_list_containers) {
		if(container->_instructionStack.empty() == false) {
			BioOperation *output = new BioOperation(this->_opNum++, OUTPUT, "output");
			this->SetOpsParent(output,container);
			this->BioGraphMaintance(output);
			this->ClearContainerOpList(container);
		}
	}

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
		fprintf(fp, "<li>%s</li>", equipments[i].c_str());
	for(i=0; i<cont_no; i++)
		fprintf(fp, "<li>%s</li>", containers[i].c_str());
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

	this->SetOpsParent(disp,container1);
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
	TransferMethodHelper(source, destination, "Measure out");
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

void BioSystem:: measure_fluid(Container * source, int NumSplit, int piecesToDest, Container * Dest, bool ensureMeasurement)
{
	//TODO:: Digital Microfluidics Split.
	std::cerr<<"Current Implementation only can handle splits into 2 droplets. 1 droplet stays in source, 1 to destination.\n";

	if(NumSplit < 2)
		return;
	fprintf(fp, "take %s and split it into %i equal parts. Send %i parts to %s and keep %i parts in %s", source->contents->new_name.c_str(), NumSplit, piecesToDest, Dest->contents->new_name.c_str(), NumSplit-piecesToDest, source->contents->new_name.c_str());


	BioOperation* split = new BioOperation(this->_opNum++, SPLIT, NumSplit);
	this->SetOpsParent(split,source);
	this->BioGraphMaintance(split);
	this->AddOpToContainer(split,source);
	this->AddOpToContainer(split,Dest);
	this->BioGraphMaintance(split);
}

void BioSystem :: set_temp(Container* container, double temp, TEMPERATURE_UNIT tempUnit)
{
	Temperature temperature(tempUnit, temp);
	BioOperation * heat = new BioOperation(this->_opNum++,HEAT, temperature);
	this->SetOpsParent(heat,container);
	this->BioGraphMaintance(heat);
	this->AddOpToContainer(heat, container);


	if (temp >= 35)
		fprintf(fp, "Pre-heat %s in a water bath set at <b><font color=#357EC7>65%cC</font></b>.<br>", container->contents->new_name.c_str(), 0x00B0);
	else
		fprintf(fp, "Set the temperature of %s to <b><font color=#357EC7>%g%cC</font></b>.<br>", container->contents->new_name.c_str(), temp, 0x00B0);

}

void BioSystem::transfer(Container * source, Container * destination)
{
	this->TransferMethodHelper(source, destination, "Transfer");
}
void BioSystem::discard(Container* container, std::string outputSink)
{
	BioOperation * discard = new BioOperation(this->_opNum++, WASTE, outputSink);
	this->SetOpsParent(discard,container);
	this->BioGraphMaintance(discard);
	this->AddOpToContainer(discard, container);

	fprintf(fp, "Discard %s.<br>", container->contents->new_name.c_str());
	container->volume = 0;
}

void BioSystem::tap(Container * container)
{
	return this->MixHelper(container, TAP);
}

void BioSystem::tap (Container* container, enum EXPERIMENT_EVENT event1)
{
	return this->MixHelper(container, TAP, event1);
}

void BioSystem::tap(Container* container, Time time)
{
	return this->MixHelper(container, TAP, EVENT_NOT_SPECIFIED, time);
}

void BioSystem::stir(Container* container)
{
	return this->MixHelper(container, STIR);
}

void BioSystem::stir(Container* container, Time time)
{
	return this->MixHelper(container, STIR, EVENT_NOT_SPECIFIED, time);
}

void BioSystem::invert(Container* container1)
{
	return this->MixHelper(container1, INVERT);
}

void BioSystem::invert(Container* container1, int times)
{
	return this->MixHelper(container1, INVERT, EVENT_NOT_SPECIFIED, Time(), times);
}

void BioSystem::invert(Container* container1, enum EXPERIMENT_EVENT event1)
{
	return this->MixHelper(container1, INVERT, event1);
}

void BioSystem::vortex(Container* container1)
{
	return this->MixHelper(container1, VORTEX);
}

void BioSystem::vortex(Container* container1, Time time1)
{
	return this->MixHelper(container1, VORTEX, EVENT_NOT_SPECIFIED, time1);
}

void BioSystem::resuspend(Container* container1)
{
	return this->MixHelper(container1, RESUSPEND);
}
void BioSystem::dissolve (Container* container1)
{
	return this->MixHelper(container1, DISSOLVE);
}
void BioSystem::pipet (Container* container1)
{
	return this->MixHelper(container1, PIPET);
}

void BioSystem::wait (Container* container, Time time1)
{

	BioOperation * store = new BioOperation(this->_opNum++, STORE, time1);
	this->SetOpsParent(store,container);
	this->BioGraphMaintance(store);
	this->ClearContainerOpList(container);
	this->AddOpToContainer(store, container);

	fprintf(fp, "Keep %s aside for ", container->contents->new_name.c_str());
	time1.display_time(fp,option_no,options_flag,total_time_required);
	fprintf(fp, ".<br>");
}

void BioSystem::combine_and_mix (MIX_TYPE mix, std::vector<Container*> containerList)
{
	return this->combine_and_mix(mix, Time(), containerList);
}

void BioSystem::combine_and_mix (MIX_TYPE mix, Time time1, std::vector<Container*> containerList)
{
	if (containerList.size()==0) {
		std::cerr<<"Error: tying to combine empty container List.\n";
		return;
	}
	Container * dest = containerList.at(0);

	for (unsigned int i = 1; i < containerList.size(); ++i) {
		this->TransferMethodHelper(containerList.at(i), dest, "Combine");
	}

	this->MixHelper(dest,mix,EVENT_NOT_SPECIFIED,time1);
}

void BioSystem:: store_for(Container* container, float temp, Time time1)
{
	BioOperation* operation;
	if(temp == ON_ICE)
	{
		operation = new BioOperation(this->_opNum++,COOL,temp,time1);
		fprintf(fp, "Store ");
		check_container(container);
		fprintf(fp, " <b><font color=#357EC7>on ice</font></b> for ");
	}
	else if(temp == BOILING_WATER)
	{
		operation = new BioOperation(this->_opNum++,HEAT,temp,time1);
		fprintf(fp, "Immerse ");
		check_container(container);
		fprintf(fp, " in boiling water for ");
	}
	else if (temp == ROOM_TEMPERATURE)
	{
		operation = new BioOperation(this->_opNum++,HEAT,temp,time1);
		fprintf(fp, "Store ");
		check_container(container);
		fprintf(fp, " at <b><font color=#357EC7><b><font color=#357EC7>room temperature</font></b></font></b> for ");
	}
	else
	{
		if (temp > ROOM_TEMPERATURE)
			operation = new BioOperation(this->_opNum++,HEAT,temp,time1);
		else
			operation = new BioOperation(this->_opNum++,COOL,temp,time1);

		fprintf(fp, "Store ");
		check_container(container);
		fprintf(fp, " at <b><font color=#357EC7>%g%cC</font></b> for ", temp, 0x00B0);
	}

	if(time1.GetTimeUnits() == TIME_NOT_SPECIFIED)
		time1.display_time(fp,option_no,options_flag,total_time_required);

	this->SetOpsParent(operation,container);
	this->BioGraphMaintance(operation);
	this->ClearContainerOpList(container);
	this->AddOpToContainer(operation, container);

}

void BioSystem:: store_for(Container* container, float temp, Time time1, STORAGE_FUNCTION function)
{

	if(function == 1)
	{
		fprintf(fp, "Denature ");
		check_container(container);
		fprintf(fp, " at <b><font color=#357EC7>%g%cC</font></b> for ", temp, 0x00B0);
	}
	else if (function == 2)
	{
		fprintf(fp, "Perform enzyme inactivation by storing ");
		check_container(container);
		fprintf(fp, " at <b><font color=#357EC7>%g%cC</font></b> for ", temp, 0x00B0);
	}

	if(time1.GetTimeUnits() == TIME_NOT_SPECIFIED)
		time1.display_time(fp,option_no,options_flag,total_time_required);

	BioOperation* store = new BioOperation(this->_opNum++,STORE,temp,time1);
	this->SetOpsParent(store,container);
	this->BioGraphMaintance(store);
	this->ClearContainerOpList(container);
	this->AddOpToContainer(store, container);

}
void BioSystem:: store_until(Container* container, float temp, EXPERIMENT_EVENT type)
{
	return this->store_until(container, temp, type, Time());
}
void BioSystem:: store_until(Container* container, float temp, EXPERIMENT_EVENT event, Time time1)
{
	switch(event)
	{
	case ETHANOL_EVAP:if (temp == ROOM_TEMPERATURE)
		fprintf(fp, "Store %s at <b><font color=#357EC7>room temperature</font></b> until the ethanol has evaporated and no fluid is visible in the tube.<br>", container->contents->new_name.c_str());
	else
		fprintf(fp, "Store %s at <b><font color=#357EC7>%g%cC</font></b> until the ethanol has evaporated and no fluid is visible in the tube.<br>", container->contents->new_name.c_str(), temp, 0x00B0);break;
	case OD:fprintf(fp, "Incubate %s at <b><font color=#357EC7>%g%cC</font></b> until the O.D.600 reaches 0.6.<br>", container->contents->new_name.c_str(), temp, 0x00B0);break;
	case THAW:fprintf(fp, "Allow %s to thaw at <b><font color=#357EC7>room temperature</font></b>.<br>", container->contents->new_name.c_str());break;
	case COOLED:fprintf(fp, "Keep %s at <b><font color=#357EC7>room temperature</font></b> until cooled.<br>", container->contents->new_name.c_str());break;
	case COLOUR_DEVELOPS:fprintf(fp, "Wait for the colour to develop.<br>");break;
	case THAW_ICE:fprintf(fp, "Allow %s to thaw on <b><font color=#357EC7>ice</font></b>.<br>", container->contents->new_name.c_str());break;
	default:break;
	}
	if(time1.GetTimeUnits() == TIME_NOT_SPECIFIED)
		time1.display_time(fp,option_no,options_flag,total_time_required);

	BioOperation* store = new BioOperation(this->_opNum++,STORE,temp,time1);
	this->SetOpsParent(store,container);
	this->BioGraphMaintance(store);
	this->ClearContainerOpList(container);
	this->AddOpToContainer(store, container);

}
void BioSystem:: incubate(Container* container1, float temp, Time time1)
{
	return incubate(container1,temp,time1,-1);
}
void BioSystem:: incubate(Container* container1, float temp, Time time1, int rpm)
{
	if (rpm != -1)
		std::cerr << "RPM not suitable for micrfluidic device. The information will not be passed along.\n";

	BioOperation* operation;
	if(temp == ROOM_TEMPERATURE)
	{
		operation = new BioOperation(this->_opNum++,HEAT, time1);
		fprintf(fp, "Incubate ");
		check_container(container1);
		fprintf(fp," at <b><font color=#357EC7><b><font color=#357EC7>room temperature</font></b></font></b> for ");
	}
	else if((temp == ON_ICE)||(temp == 0))
	{
		operation = new BioOperation(this->_opNum++,COOL, time1);
		fprintf(fp, "Incubate ");
		check_container(container1);
		fprintf(fp, " on <b><font color=#357EC7><b><font color=#357EC7>ice</font></b></font></b> for ");
	}
	else
	{
		operation = new BioOperation(this->_opNum++,HEAT, time1);
		fprintf(fp, "Incubate ");
		check_container(container1);
		fprintf(fp, " at <b><font color=#357EC7>%g%cC</font></b> for ", temp, 0x00B0);
	}

	time1.display_time(fp,option_no,options_flag,total_time_required);
	if(rpm != -1)
		fprintf(fp, " with shaking at %d rpm.<br>", rpm);

	if(incubator_no == 1)
	{
		equipments[equip_no] = "Incubator";
		equip_no++;
		incubator_no++;
	}
	this->SetOpsParent(operation,container1);
	this->BioGraphMaintance(operation);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(operation, container1);
}
void BioSystem:: incubate_and_mix(Container* container1, float temp, Time time1, Time time_mix, MIX_TYPE type)
{
	this->incubate(container1,temp,time1);
	this->MixHelper(container1,type, EVENT_NOT_SPECIFIED, time_mix);
}


void BioSystem:: drain(Container* container1, std::string outputSink)
{
	BioOperation* waste = new BioOperation(this->_opNum++,WASTE,outputSink);

	this->SetOpsParent(waste,container1);
	this->BioGraphMaintance(waste);
	this->ClearContainerOpList(container1);
	//Removed since Drain should not be a parent of anyone.
	this->AddOpToContainer(waste, container1);

	fprintf(fp,"Drain %s.<br>", container1->name.c_str());
	container1->volume = 0;

}
BioOperation * BioSystem:: ce_detect (Container* container1, float length, float volt_per_cm, Fluid* fluid1, std::string nickname)
{
	return this->ce_detect(container1,length, volt_per_cm, fluid1, Time(), nickname);
}
BioOperation * BioSystem:: ce_detect (Container* container1, float length, float volt_per_cm, Fluid* fluid1, Time time1, std::string nickname)
{
	//TODO Pass-on relevent info to SIMs.
	std::cerr<<"Current implementation doesnt pass extra info besides detect to MF_SIMULATORs.\n";

	if(container1 == NULL ) {
		std::cerr<<"Contatiner is NULL for CE Detect when it shouldnt be.\n";
		exit(-1);
	}
	if(fluid1 == NULL) {
		std::cerr<<"Fluid is NULL for CE Detect when when it shouldnt be.\n";
		exit(-1);
	}

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
	fprintf(fp, "Detect/separate %s by capillary electrophoresis with the following settings - <b><font color=#357EC7>%g</font></b> cm at <b><font color=#357EC7>%g</font></b> V/cm using %s for ", container1->contents->new_name.c_str(), length, volt_per_cm, fluid1->new_name.c_str());
	if (time1.GetTimeUnits() != TIME_NOT_SPECIFIED)
		time1.display_time(fp,option_no,options_flag,total_time_required);

	fprintf(fp, ".<br>");
	name_sample(container1, "separated flow");

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT,CE_DETECT,time1,nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}

BioOperation * BioSystem:: measure_fluorescence (Container* container1, Time time1, std::string nickname)
{
	fprintf(fp, "Measure the fluorescence of %s.<br>", container1->contents->new_name.c_str());


	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, MEASURE_FLUORESCENCE,time1, nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}

BioOperation * BioSystem:: electrophoresis(Container* container1, std::string nickname)
{
	return this->electrophoresis(container1,-1, nickname);
}

BioOperation * BioSystem:: electrophoresis(Container* container1, float agar_conc, std::string nickname)
{
	//TODO Pass-on relevent info to SIMs.
	if (agar_conc != -1)
		std::cerr<<"Current implementation doesnt pass extra info besides detect to MF_SIMULATORs.\n";

	if (agar_conc == -1)
		fprintf(fp,"Perform agarose gel electrophoresis of appropriate quantity of  %s mixed with ethidium bromide and visualize with UV transilluminator to confirm the presence of required product.<br>", container1->contents->new_name.c_str());
	else
		fprintf(fp,"Perform %g&#37 agarose gel electrophoresis of appropriate quantity of  %s mixed with ethidium bromide and visualize with UV transilluminator to confirm the presence of required product.<br>",agar_conc, container1->contents->new_name.c_str());

	if(electrophoresis_no==1)
	{
		equipments[equip_no]="Electrophoretic unit";
		equip_no++;
		electrophoresis_no++;
	}

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, ELECTROPHORESIS,Time(), nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}
BioOperation * BioSystem:: sequencing(Container* container1, std::string nickname)
{
	fprintf(fp,"Dilute %s to <font color=#357EC7>100ng/ µl</font> and send <font color=#357EC7>1 µg (10 µL)</font> for sequencing.<br>", container1->contents->new_name.c_str());

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, SEQUENCING, Time(), nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}

BioOperation * BioSystem:: weigh(Container* container1, std::string nickname)
{
	fprintf(fp, "Weigh the amount of %s present.<br>", container1->contents->new_name.c_str());

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, WEIGH, Time(), nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}

BioOperation * BioSystem:: facs(Container* container1, std:: string nickname)
{
	fprintf(fp, "FACS: sort %s based on fluorescence.", container1->contents->new_name.c_str());

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, FACS, Time(), nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}

BioOperation * BioSystem:: cell_culture(Container* cells, Fluid* medium, int centri_speed, float temp, float time, float percent_CO2, Fluid* for_wash_valves, Fluid* for_wash_chambers, Fluid* for_trypsinization, float for_feeding, std::string nickname)
{
	//TODO Pass-on relevent info to SIMs.
	std::cerr<<"Current implementation doesnt pass extra info besides detect to MF_SIMULATORs.\n";

	fprintf(fp,"Perform cell culture with the specified parameters.");

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, CELL_CULTURE, Time(), nickname );
	this->SetOpsParent(detect,cells);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(cells);
	this->AddOpToContainer(detect, cells);

	return detect;
}

BioOperation * BioSystem:: transfection(Container* container1, Fluid* medium, Fluid* dna, std::string nickname)
{
	//TODO Pass-on relevent info to SIMs.
	std::cerr<<"Current implementation doesnt pass extra info besides detect to MF_SIMULATORs.\n";

	fprintf(fp,"Transfect %s with %s.", container1->contents->new_name.c_str(), dna->new_name.c_str());

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, TRANSFECTION, Time(), nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}

BioOperation * BioSystem:: electroporate (Container* container1, float voltage, int no_pulses, std::string nickname)
{
	//TODO Pass-on relevent info to SIMs.
	std::cerr<<"Current implementation doesnt pass extra info besides detect to MF_SIMULATORs.\n";

	fprintf(fp, "Set the electroporator to deliver <b><font color=#357EC7>%g V</font></b>, and then press the PULSE button <b><font color=#357EC7>%d</font></b> times. <br>", voltage, no_pulses);
	if(electro_no==1)
	{
		equipments[equip_no]="Electroporator";
		equip_no++;
		electro_no++;
	}

	BioOperation *detect = new BioOperation(this->_opNum++, DETECT, ELECTROPORATE, Time(), nickname );
	this->SetOpsParent(detect,container1);
	this->BioGraphMaintance(detect);
	this->ClearContainerOpList(container1);
	this->AddOpToContainer(detect, container1);

	return detect;
}

void BioSystem:: IF(BioExpression* expression)
{
	BioOperation* if_statement = new BioOperation(this->_opNum++, IF_OP, expression);

	this->SetOpsParent(if_statement);
	this->BioGraphMaintance(if_statement);
	this->ClearAllContainerOpList(if_statement);
	this->AddOPToAllContainers(if_statement);


}


void BioSystem:: IF(BioOperation* lhs, ConditionalOps condition, BioOperation* rhs)
{
	this->IF(new BioExpression(lhs,condition,rhs));
}
void BioSystem:: IF(BioOperation* lhs, ConditionalOps condition, double constant)
{
	this->IF(new BioExpression(lhs,condition, constant));
}

void BioSystem:: ELSE_IF(BioExpression* expression)
{
	BioOperation* else_if_statement = new BioOperation(this->_opNum++, ELSE_IF_OP, expression);

	this->SetOpsParent(else_if_statement);
	this->BioGraphMaintance(else_if_statement);
	//this->ClearAllContainerOpList(else_if_statement);
	this->AddOPToAllContainers(else_if_statement);
}
void BioSystem:: ELSE_IF(BioOperation* lhs, ConditionalOps condition, BioOperation* rhs)
{
	return this->ELSE_IF(new BioExpression(lhs,condition,rhs));
}
void BioSystem:: ELSE_IF(BioOperation* lhs, ConditionalOps condition, double constant)
{
	return this->ELSE_IF(new BioExpression(lhs,condition,constant));
}


void BioSystem:: ELSE()
{
	BioOperation* else_operation = new BioOperation(this->_opNum++, ELSE_OP);

	this->SetOpsParent(else_operation);
	this->BioGraphMaintance(else_operation);
	//this->ClearAllContainerOpList(else_operation);
	this->AddOPToAllContainers(else_operation);
}

void  BioSystem:: END_IF()
{
	BioOperation* end_if = new BioOperation(this->_opNum++, END_IF_OP);

	this->SetOpsParent(end_if);
	this->BioGraphMaintance(end_if);
	this->ClearAllContainerOpList(end_if);
	this->AddOPToAllContainers(end_if);

}

void  BioSystem :: WHILE(BioExpression* expression)
{
	BioOperation* while_statement = new BioOperation(this->_opNum++, WHILE_OP, expression);

	this->SetOpsParent(while_statement );
	this->BioGraphMaintance(while_statement );
	this->ClearAllContainerOpList(while_statement );
	this->AddOPToAllContainers(while_statement );

}

void BioSystem :: WHILE(BioOperation * lhs, ConditionalOps condition, BioOperation* rhs)
{
	this->WHILE(new BioExpression (lhs,condition, rhs));
}

void BioSystem ::  WHILE(BioOperation* lhs, ConditionalOps condition, double constant)
{
	this ->WHILE(new BioExpression (lhs, condition, constant));
}
void BioSystem:: WHILE(double variable, ConditionalOps condition, double constant, incrementor funct)
{
	this->WHILE( new BioExpression(variable, condition, constant, funct));
}

void BioSystem :: END_WHILE()
{
	BioOperation* end_while = new BioOperation(this->_opNum++, END_WHILE_OP);

	this->SetOpsParent(end_while);
	this->BioGraphMaintance(end_while);
	this->ClearAllContainerOpList(end_while);
	this->AddOPToAllContainers(end_while);
}




} // Namespace BioCoder
