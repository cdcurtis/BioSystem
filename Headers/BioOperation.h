/*
 * BioOperation.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#ifndef BIOOPERATION_H_
#define BIOOPERATION_H_
#include <iostream>
#include <cstdio>
#include "Time.h"
#include "Temperature.h"
#include "Fluid.h"

namespace BioCoder
{
/*! Specifies certain conditions that might have to be satisfied for the completion of a step. For use in store_until(), vortex(), etc. */
enum EXPERIMENT_EVENT{ETHANOL_EVAP/*!< "until all the ethanol has evaporated and no fluid is visible in the tube".*/, OD/*!< "until the O.D.600 reaches 0.6". */, THAW/*!< "until the sample has thawed". */, COOLED/*!< "until cooled". */, COLOUR_DEVELOPS/*!< "until the colour develops". */, PPT_STOPS_STICKING/*!< "until the precipitate stops sticking to the walls of the tube". */, PELLET_DISLODGES/*!< "until the pellet dislodges". */, THAW_ICE/*!< "keep on ice until the sample has thawed" */, EVENT_NOT_SPECIFIED};
enum OPERATION_TYPE {DISPENSE, DETECT, HEAT, COOL, STORE, MIX, SPLIT, WASTE, OUTPUT, OPERATION_NOT_SPECIFED};
//enum ARCHITECTURE_TYPE {DIGITAL_MF, CONTINOUS_FLOW_MF, ARCHITECTURE_NOT_SPECIFIED};
enum DETECT_TYPE {DETECT_NOT_SPECIFIED};
enum MIX_TYPE{TAP, STIR, INVERT, VORTEX, RESUSPEND, DISSOLVE, PIPET, MIX_NOT_SPECIFIED};
enum STORAGE_FUNCTION{DEWAX/*!< dewaxing */, DENATURE/*!< denaturation */, ENZYME_INAC/*!< enzyme inactivation */,STORAGE_FUNCTION_NOT_SPECIFIED};



struct BioOperation
{
public:
	int _ID;
	std::vector<BioOperation*> _parents;
	std::vector<BioOperation*> _children;

	OPERATION_TYPE _opType;
	Volume _volume;
	Time _time;
	Temperature _temp;
	Fluid* _fluid;
	DETECT_TYPE _detectType;
	MIX_TYPE _mixType;
	//	ARCHITECTURE_TYPE _archType;
	std::string* _outputName;
	int _numDrops;
	Volume _destinationVolume;


	BioOperation(int id)
: _ID(id), _opType(OPERATION_NOT_SPECIFED), _volume(), _time(), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume()
{}

	BioOperation(int id, OPERATION_TYPE op, Volume v, Fluid* f, Time t = Time()) //Dispense Operation.
	: _ID(id), _opType(op), _volume(v), _time(t), _temp(), _fluid(f), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume()
	{}

	BioOperation(int id, OPERATION_TYPE op, Fluid* f, Time t = Time()) //Dispense Operation.
	: _ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(f), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume()
	{}

	BioOperation(int id, OPERATION_TYPE op, DETECT_TYPE type, Time t = Time()) //Detect Operation.
	: _ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(type), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume()
	{}

	BioOperation(int id, OPERATION_TYPE op, Temperature temp, Time t = Time()) //Heat/Cool/Store
	:_ID(id), _opType(op), _volume(), _time(t), _temp(temp), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume()
	{}

	BioOperation(int id, OPERATION_TYPE op, MIX_TYPE mix, Time t = Time()) // Mix Operation
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(mix), _outputName(NULL), _numDrops(-1), _destinationVolume()
	{}

	BioOperation(int id, OPERATION_TYPE op, int numDrops, Time t = Time()) // Split Digital
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(numDrops), _destinationVolume()
	{}

	BioOperation(int id, OPERATION_TYPE op, Volume destinationVol, Time t = Time()) //Split Continuous Flow
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(destinationVol)
	{}

	BioOperation(int id, OPERATION_TYPE op, Time t)// Store Operation
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume()
	{}

	BioOperation(int id, OPERATION_TYPE op, std::string outputDest, Time t = Time()) //Output/waste
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(new std::string(outputDest)), _numDrops(-1), _destinationVolume()
	{}



	std::string Name()
	{
		char buffer[25];
		switch(_opType){
		case COOL:
			sprintf(buffer,"Cool%i", _ID);
			break;
		case DISPENSE:
			sprintf(buffer,"Dispense%i", _ID);
			break;
		case DETECT:
			sprintf(buffer,"Detect%i", _ID);
			break;
		case HEAT:
			sprintf(buffer,"Heat%i", _ID);
			break;
		case MIX:
			sprintf(buffer,"Mix%i", _ID);
			break;
		case SPLIT:
			sprintf(buffer,"Split%i", _ID);
			break;
		case STORE:
			sprintf(buffer,"Store%i", _ID);
			break;
		case OUTPUT:
			sprintf(buffer,"Output%i", _ID);
			break;
		case WASTE:
			sprintf(buffer,"Waste%i", _ID);
			break;
		default:
			sprintf(buffer,"UNKNOWN%i", _ID);

		}

		return buffer;
	}
};

struct SensorReading {
	BioOperation* _op;
	SensorReading(): _op(NULL) {}
	SensorReading(BioOperation * op): _op(op) {}
};

}

#endif /* BIOOPERATION_H_ */
