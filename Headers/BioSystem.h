/*
 * BioSystem.h
 *
 *  Created on: Aug 4, 2015
 *      Author: Chris
 */

#ifndef BIOSYSTEM_H_
#define BIOSYSTEM_H_
#include <iostream>
#include <vector>

#include "Fluid.h"
#include "Container.h"
#include "Time.h"
#include "Temperature.h"
#include "Volume.h"

#include "BioOperation.h"
#include "Conditional.h"



namespace BioCoder
{

class BioSystem
{
	int _opNum;
	int _currentStep;



	int total_time_required;
	FILE * fp;
	int options_flag;
	int option_no;
	//initiallization of flags
	int print_reagents;
	int print_parameters;
	int first;

	int sub_step_flag;
	int prev_cont;

	//initiallization of number of containers, equipment, etc.
	int equip_no;
	int microfuge_no;
	int centrifuge_no;
	int incubator_no;
	int electrophoresis_no;
	int mortar_no;
	int thermocycler_no;
	int electro_no;
	int shaker_no;
	int spectrophotometer_no;
	int cont_no;
	int container_count[24];
	char* containers[20];
	char* equipments[20];
	std::string filename;

	//variables required for keeping track of usage of fluids and containers
	int list_fluid_no;
	int list_container_no;
	std:: string prev_container;




	std::vector<Fluid *> usage_list_fluids;
	std::vector<Container *> usage_list_containers;

	// roots tree and user Defined Level Tree will contain the same operations
	std::vector<BioOperation*> _roots; //Dilution Tree
	std::vector<std::vector <BioOperation*> > _userDefinedLevelTree; //steppedProtocol
	std::vector<Container*> _containers;
	std::vector<Fluid*> _fluids;


	//Helper functions

	void ClearContainerOpList(Container *);
	void AddOpToContainer(BioOperation* , Container * );
	void BioGraphMaintance(BioOperation *);
	void SetOpsParent(BioOperation *, Container * );

	void TransferOperation(Container* source, Container* destination, bool warning = false);

	void TransferMethodHelper (Container * source, Container * destination, std:: string transferWording);
	void MixHelper(Container* container, MIX_TYPE mixtype, EXPERIMENT_EVENT event = EVENT_NOT_SPECIFIED, Time time = Time(), int =-1);
public:
	//Debug Tools
	void PrintLeveledProtocol();
	void PrintTree();



	//BIOCODER

	BioSystem(std:: string name = "BioCoderProtocol"): _opNum(0), _currentStep(-1)
	{
		equip_no = microfuge_no = centrifuge_no = incubator_no = electrophoresis_no = mortar_no = thermocycler_no = electro_no = shaker_no = spectrophotometer_no = cont_no = 0;
		list_fluid_no = list_container_no = 0;
		start_protocol(name);
	}







	//! Creates an html file with the given name and opens it for writing the protocol.
	/*!
	\param name the name of the protocol.
	\par Example:
	\code start_protocol("DNA Miniprep");\endcode
	\par Output:
	Internally, opens a new html file named "DNA Miniprep" for writing. The title of the protocol "DNA Miniprep" is also printed on the top left corner of the file.
	\sa end_protocol()
	 */
	void start_protocol(std::string name);
	//! Marks the first step of a protocol.
	/*!
	\par Example:
	\code first_step(); \endcode
	\par Output:
	\htmlonly 1. \endhtmlonly
	\sa next_step(), optional_step() and first_sub_step()
	 */
	void first_step();
	//! Marks the first step of a protocol. Also specifies the name of the step.
	/*!
	\param step_name the name of the step.
	\par Example:
	\code first_step(DNA extraction); \endcode
	\par Output:
	\htmlonly <h2>Steps:</h2>
	1. <b>DNA extraction</b> \endhtmlonly
	\sa next_step(), optional_step() and first_sub_step()
	 */
	void first_step(std::string step_name);

	//! Marks the ending of a step and the beginning of the next step of a protocol.
	/*!
	\par Example:
	\code next_step();\endcode
	\par Output:
	\htmlonly 2. \endhtmlonly
	\sa first_step(), optional_step() and next_sub_step()
	 */
	void next_step();
	//! Marks the ending of a step and the beginning of the next step of a protocol. Also specifies the name of the step.
	/*!
	\param step_name the name of the step.
	\par Example:
	\code next_step("DNA lysis");\endcode
	\par Output:
	\htmlonly 2.<b>DNA lysis</b> \endhtmlonly
	\sa first_step(), optional_step() and next_sub_step()
	 */
	void next_step(std::string step_name);
	//! Marks a parallel step in the protocol.
	/*!
	\par Example:
	\code parallel_step(); \endcode
	\par Output:
	\htmlonly 3. <b> Meanwhile: </b> \endhtmlonly
	\sa first_step(), next_step(), first_sub_step() and next_sub_step()
	 */
	void parallel_step();
	//! Marks the first sub-step of a step in the protocol.
	/*!
	\par Example:
	\code first_sub_step(); \endcode
	\par Output:
	\htmlonly a. \endhtmlonly
	\sa next_step(), optional_step() and next_sub_step()
	 */
	void first_sub_step();
	//! Marks the end of a sub-step and the start of the subsequent sub-step of a step in the protocol.
	/*!
	\par Example:
	\code next_sub_step();\endcode
	\par Output:
	\htmlonly b. \endhtmlonly
	\sa next_step(), optional_step() and next_sub_step()
	 */
	void next_sub_step();
	//! Marks the end of the protocol (calls display_equipment, closes the file and resets all the variables).
	/*!
	\par Example:
	\code end_protocol; \endcode
	\par Output:
	Internally, closes the html file.
	\sa start_protocol()
	 */
	void end_protocol();
	void display_equip();
	void usage_details();
	void timing_details();

	//! Used to declare a new fluid with the given name.
	/*!
	\param name the name of the fluid.
	\return Fluid
	\par Example:
	\code Fluid solution1 = new_fluid("Solution 1"); \endcode
	\par Output:
	\htmlonly <ul type="circle"><li>Solution 1</li></ul> \endhtmlonly
	\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid(std::string name);
	//! Used to declare a new fluid with the given name.
	/*!
\param name the name of the fluid.
\param volume1 the initial volume of the fluid.
\return Fluid
\par Example:
\code Fluid solution1 = new_fluid("Solution 1", vol(10, ML)); \endcode
\par Output:
\htmlonly <ul type="circle"><li>10 ml Solution 1</li></ul> \endhtmlonly
\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid(std::string name, Volume volume1);
	//! Used to declare a new fluid with the given name. The solid has been stored at the specified temperature.
	/*!
\param name the name of the fluid.
\param temp the temperature of storage of the fluid.
\return Fluid
\par Example:
\code Fluid solution1 = new_fluid("Solution 1", ICE_COLD, vol(10, ML)); \endcode
\par Output:
\htmlonly <ul type="circle"><li>10 ml ice-cold Solution 1</li></ul> \endhtmlonly
\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid(std::string name, float temp);
	//! Used to declare a new fluid with the given name. The solid has been stored at the specified temperature.
	/*!
\param name the name of the fluid.
\param temp the temperature of storage of the fluid.
\param volume1 the initial volume of the fluid.
\return Fluid
\par Example:
\code Fluid solution1 = new_fluid("Solution 1", ICE_COLD); \endcode
\par Output:
\htmlonly <ul type="circle"><li>ice-cold Solution 1</li></ul> \endhtmlonly
\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid(std::string name, float temp, Volume volume1);
	//! Used to declare a new fluid with the given name and characteristics.
	/*!
\param name the name of the fluid.
\param state the characteristics of the fluid (Eg: "freshly prepared" or composition of a solution).
\return Fluid
\par Example:
\code Fluid solution1 = new_fluid("Solution 1", "50mM SDS, 20mM TE"); \endcode
\par Output:
\htmlonly <ul type="circle"><li>Solution 1<br><i>(50mM SDS, 20mM TE)</i></li></ul> \endhtmlonly
\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid(std::string name, std::string state);
	//! Used to declare a new fluid with the given name and characteristics.
	/*!
\param name the name of the fluid.
\param state the characteristics of the fluid (Eg: "freshly prepared" or composition of a solution).
\param volume1 the initial volume of the fluid.
\return Fluid
\par Example:
\code Fluid solution1 = new_fluid("Solution 1", "50mM SDS, 20mM TE", vol(10, ML)); \endcode
\par Output:
\htmlonly <ul type="circle"><li>10 ml Solution 1<br><i>(50mM SDS, 20mM TE)</i></li></ul> \endhtmlonly
\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid(std::string name, std::string state, Volume volume1);
	//! Used to declare a new fluid with the given name and characteristics. The fluid has been stored at the specified temperature.
	/*!
\param name the name of the fluid.
\param state the characteristics of the fluid (Eg: "freshly prepared" or composition of a solution).
\param temp the temperature of storage of the fluid.
\return Fluid
\par Example:
\code Fluid solution1 = new_fluid("Solution 1", "50mM SDS, 20mM TE", RT); \endcode
\par Output:
\htmlonly <ul type="circle"><li>Solution 1 at room temperature<br><i>(50mM SDS, 20mM TE)</i></li></ul> \endhtmlonly
\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid (std::string name, std::string state, float temp);
	//! Used to declare a new fluid with the given name and characteristics. The fluid has been stored at the specified temperature.
	/*!
\param name the name of the fluid.
\param state the characteristics of the fluid (Eg: "freshly prepared" or composition of a solution).
\param temp the temperature of storage of the fluid.
\param volume1 the initial volume of the fluid.
\return Fluid
\par Example:
\code Fluid solution1 = new_fluid("Solution 1", "50mM SDS, 20mM TE", RT, vol(10, ML)); \endcode
\par Output:
\htmlonly <ul type="circle"><li>10 ml Solution 1 at room temperature<br><i>(50mM SDS, 20mM TE)</i></li></ul> \endhtmlonly
\sa new_solid(), new_container() and new_plate()
	 */
	Fluid * new_fluid (std::string name, std::string state, float temp, Volume volume1);

	//Fluid new_operation(char* name);

	//	void store_container_names(int i,char* name);

	//! Used to declare a new container of the type specified by \c cont_id.
	/*!
	\param cont_id the type of container.
	\return Container
	\par Example:
	\code Container microfuge_tube1 = new_container(STERILE_MICROFUGE_TUBE);\endcode
	\par Output:
	Internally, sets aside a sterile microfuge tube with the name "a sterile microfuge tube".
	\sa enum container_type, new_fluid(), new_solid() and new_plate()
	 */
	Container * new_container(enum CONTAINER_TYPE cont_id);
	//! Used to declare a new container of the type specified by \c cont_id that contains a fluid at the start of the protocol.
	/*!
	\param cont_id the type of container.
	\param fluid1 the contents of the container.
	\return Container
	\par Example:
	\code Container microfuge_tube1 = new_container(STERILE_MICROFUGE_TUBE, comp_cells);\endcode
	\par Output:
	Internally, sets aside a sterile microfuge tube with the name "a sterile microfuge tube" and \c comp_cells as contents.
	\sa enum container_type, new_fluid(), new_solid() and new_plate()
	 */
	Container * new_container(enum CONTAINER_TYPE cont_id, Fluid* fluid1);
	//! Sets the specified container as the working container for the specified sample.
	/*!
	\param fluid1 the sample whose container is being set.
	\param container1 the container which contains \c sample1.
	\par Example:
	\code set_container(culture, flask);\endcode
	\par Output:
	Internally, set the contents of \c flask to \c culture.
	\sa new_container(), enum container_type
	 */

	void set_container(Fluid* fluid1, Container* container1);

	//! Measures out \c container's into another specified container.
	/*!
	\param container the container whose contents have to be measured out.
	\param container1 the container into which \c fluid1 has to be measured.
	\par Example:
	\code measure_fluid(rxn_tube, eppendorf); \endcode
	\par Output:
	\htmlonly Measure out <font color=#357EC7>(rxn_tube.contents.name)</font> into (eppendorf.name).<br>\endhtmlonly
	\sa measure_solid() and measure_prop()
	 */

	void measure_fluid(Container* source, Container* destination);
	//! Measures out \c fluid1 into the specified container.
	/*!
	\param fluid1 the fluid that has to be measured out.
	\param container1 the container into which \c fluid1 has to be measured.
	\par Example:
	\code measure_fluid(solution1, eppendorf);
	 measure_fluid(solution2, eppendorf);\endcode
	\par Output:
	\htmlonly Measure out <font color=#357EC7>(solution1.name)</font> into (eppendorf.name).<br>
	Add <font color=#357EC7>(solution2.name)</font> to (solution1.name).\endhtmlonly
	\sa measure_solid() and measure_prop()
	 */
	void measure_fluid(Fluid* fluid1, Container* container1);
	//! Measures out a specified volume of \c fluid1.
	/*!
	\param fluid1 the fluid that has to be measured out.
	\param volume1 the volume of \c fluid1 required.
	\par Example:
	\code measure_fluid(solution1, vol(1, ML));\endcode
	\par Output:
	\htmlonly Measure out <b><font color=#357EC7>1 ml</font></b> of <font color=#357EC7>(solution1.name)</font>.\endhtmlonly
	\sa measure_solid() and measure_prop()
	 */
	//	void measure_fluid(Fluid& fluid1, Volume volume1);


	//! Measures out a specified volume of \c fluid1 into the given container.
	/*!
	\param fluid1 the fluid that has to be measured out.
	\param volume1 the volume of \c fluid1 required.
	\param container1 the container into which \c fluid1 has to be measured.
	\par Example:
	\code measure_fluid(solution1, vol(1, ML), tube1);
	 measure_fluid(solution2, vol(1, ML), tube1);\endcode
	\par Output:
	\htmlonly Measure out <b><font color=#357EC7>1 ml</font></b> of <font color=#357EC7>(solution1.name)</font> into (tube1.name).<br>
	Add <b><font color=#357EC7>1 ml</font></b> of <font color=#357EC7>(solution2.name)</font>.\endhtmlonly
	\sa measure_solid() and measure_prop
	 */
	void measure_fluid(Fluid* fluid1, Volume volume1, Container* container1);
	//! Measures out a specified volume of contents of container \c source into another container, \c destination.
	/*!
	\param source the container whose contents have to be measured out.
	\param volume1 the volume of \c con 's contents required.
	\param dsetination the destination container of the measured contents of \c con.
	\par Example:
	\code measure_fluid(eppendorf1, vol(1, ML), tube1);\endcode
	\par Output:
	\htmlonly Measure out <b><font color=#357EC7>1 ml</font></b> of <font color=#357EC7>(eppendorf1.contents.name)</font> into (tube1.name).\endhtmlonly
	\sa measure_solid() and measure_prop
	 */
	void measure_fluid(Container * source, Volume volume1, Container* destination, bool ensureMeasurement = false);
	void measure_fluid(Container * source, int NumSplit, int piecesToDest, Container * Dest, bool ensureMeasurement = false);
	//! Sets the temperature of the contents of the given container as specified.
	/*!
		\param container1 the container whose contents have to be set to a particular temperature.
		\param temp the temperature setting.
		\par Example:
		\code set_temp(eppendorf1, 37); \endcode
		\par Output:
		\htmlonly Set the temperature of (eppendorf1.contents.name) to <b><font color=#357EC7>37 C</font></b>. \endhtmlonly
		\sa store(), store_for(), store_until(), incubate()
	 */
	void set_temp(Container*, double temp, TEMPERATURE_UNIT temp_unit = FAHRENHEIT);



	//! Transfers the contents of a container \c container1 to another specified container \c con2.
	/*!
		\param container1 the container whose contents have to be transferred.
		\param con2 the container to which \c container1 's contents have to be transferred.
		\par Example:
		\code transfer(tube1, tube2);\endcode
		\par Output:
		\htmlonly Transfer (tube1.contents.name) into (tube2.name). \endhtmlonly
		\sa combine_and_mix()
	 */
	void transfer(Container* source, Container* destination);

	//! Discards the contents of the specified container.
	/*!
		\par Example:
		\code discard(epepndorf1);\endcode
		\par Output:
		\htmlonly Discard (eppendorf1.contents.name). \endhtmlonly
		\param container1 the container whose contents have to be discarded.
	 */
	void discard(Container* container, std::string outputSink);
	//! Mixes the contents of the given container by tapping the container for a few seconds.
	/*!
	\param container1 the container whose contents have to be mixed.
	\par Example:
	\code tap(eppendorf);\endcode
	\par Output:
	\htmlonly Gently tap the mixture for a few secs. \endhtmlonly
	\sa stir(), invert(), vortex(), dissolve(), resuspend(), pipet() and combine_and_mix()
	 */
	void tap(Container * container);
	//! Mixes the contents of the given container by tapping until the specified result is obtained.
	/*!
		\param container1 the container whose contents have to be mixed.
		\param event1 the event until which the container has to be mixed. (See enum until)
		\par Example:
		\code tap(eppendorf, PELLET_DISLODGES); \endcode
		\par Output:
		\html only Close the tube tightly and gently tap the mixture for a few secs until the pellet dislodges. \endhtmlonly
		\sa stir(), invert(), vortex(), dissolve(), resuspend(), pipet(), combine_and_mix() and enum until
	 */
	void tap(Container* container1, enum EXPERIMENT_EVENT event1);

	//! Mixes the contents of the given container by tapping the container for the specified duration of time.
	/*!
	\param container1 the container whose contents have to be mixed.
	\param time1 the duration of tapping.
	\par Example:
	\code tap(eppendorf, time(10, SECS));\endcode
	\par Output:
	\htmlonly Gently tap the mixture for <b><font color=#357EC7>10 secs</font></b>. \endhtmlonly
	\sa invert(), vortex(), dissolve(), resuspend(), pipet() and combine_and_mix()
	 */
	 void tap(Container* container1, Time time1);
	 //! Mixes the contents of the given container by stirring the container for a few seconds.
	/*!
	\param container1 the container whose contents have to be mixed.
	\par Example:
	\code stir(eppendorf);\endcode
	\par Output:
	\htmlonly Gently stir the mixture for a few secs. \endhtmlonly
	\sa tap(), invert(), vortex(), dissolve(), resuspend(), pipet() and combine_and_mix()
	 */
	void stir(Container* container);
	//! Mixes the contents of the given container by stirring the container for the specified duration of time.
	/*!
	\param container1 the container whose contents have to be mixed.
	\param time1 the duration of tapping.
	\par Example:
	\code stir(eppendorf, time(10, SECS));\endcode
	\par Output:
	\htmlonly Gently stir the mixture for <b><font color=#357EC7>10 secs</font></b>. \endhtmlonly
	\sa tap(), invert(), vortex(), dissolve(), resuspend(), pipet() and combine_and_mix()
	 */
	void stir(Container* container, Time time1);
	//! Mixes the contents of the given container by inverting the container for a few seconds.
	/*!
	\param container1 the container whose contents have to be mixed.
	\par Example:
	\code invert(eppendorf);\endcode
	\par Output:
	\htmlonly Close the tube tightly and gently mix the contents by inverting the tube. \endhtmlonly
	\sa tap(), stir(), vortex(), dissolve(), resuspend(), pipet() and combine_and_mix()
	 */
	void invert(Container* container1);
	//! Mixes the contents of the given container by inverting it for the specified number of times.
	/*!
	\param container1 the container whose contents have to be mixed.
	\param times the number of times that the container has to be inverted.
	\par Example:
	\code invert(eppendorf, 5);\endcode
	\par Output:
	\htmlonly Close the tube tightly and gently mix the contents by inverting the tube <b><font color=#357EC7>5 times</font></b>. \endhtmlonly
	\sa tap(), stir(), vortex(), dissolve(), resuspend(), pipet() and combine_and_mix()
	 */
	void invert(Container* container1, int times);
	//! Mixes the contents of the given container by inverting it until the specified result is obtained.
	/*!
	\param container1 the container whose contents have to be mixed.
	\param event1 the event until which the container has to be mixed. (See enum until)
	\par Example:
	\code invert(eppendorf, PPT_STOPS_STICKING);\endcode
	\par Output:
	\htmlonly Close the tube tightly and gently mix the contents by inverting the tube until the precipitate stops sticking to the walls of the tube. \endhtmlonly
	\sa tap(), stir(), vortex(), dissolve(), resuspend(), pipet(), combine_and_mix() and enum until
	 */
	void invert(Container* container1, enum EXPERIMENT_EVENT event1);
};



}

#endif /* BIOSYSTEM_H_ */
