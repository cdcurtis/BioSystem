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

enum OPERATION_TYPE {DISPENSE, DETECT, HEAT, COOL, STORE, MIX, SPLIT, WASTE, OUTPUT, OPERATION_NOT_SPECIFED};
//enum ARCHITECTURE_TYPE {DIGITAL_MF, CONTINOUS_FLOW_MF, ARCHITECTURE_NOT_SPECIFIED};
enum DETECT_TYPE {DETECT_NOT_SPECIFIED};
enum MIX_TYPE{MIX_NOT_SPECIFIED};



struct BioOperation
{
public:
	int _ID;
	std::vector<BioOperation*> _parents;
	std::vector<BioOperation*> _children;

	OPERATION_TYPE _opType;
	Volume _volume;
	Time* _time;
	Temperature* _temp;
	Fluid* _fluid;
	DETECT_TYPE _detectType;
	MIX_TYPE _mixType;
	//	ARCHITECTURE_TYPE _archType;
	std::string* _outputName;
	int _numDrops;
	Volume* _destinationVolume;


	BioOperation(int id)
: _ID(id), _opType(OPERATION_NOT_SPECIFED), _volume(), _time(NULL), _temp(NULL), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(NULL)
{}

	BioOperation(int id, OPERATION_TYPE op, Volume v, Fluid* f, Time* t = new Time()) //Dispense Operation.
	: _ID(id), _opType(op), _volume(v), _time(t), _temp(NULL), _fluid(f), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(NULL)
	{}

	BioOperation(int id, OPERATION_TYPE op, Fluid* f, Time* t = new Time()) //Dispense Operation.
	: _ID(id), _opType(op), _volume(), _time(t), _temp(NULL), _fluid(f), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(NULL)
	{}

	BioOperation(int id, OPERATION_TYPE op, DETECT_TYPE type, Time* t = new Time()) //Detect Operation.
	: _ID(id), _opType(op), _volume(), _time(t), _temp(NULL), _fluid(NULL), _detectType(type), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(NULL)
	{}

	BioOperation(int id, OPERATION_TYPE op, Temperature* temp, Time* t = new Time()) //Heat/Cool/Store
	:_ID(id), _opType(op), _volume(), _time(NULL), _temp(temp), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(NULL)
	{}

	BioOperation(int id, OPERATION_TYPE op, MIX_TYPE mix, Time* t = new Time()) // Mix Operation
	:_ID(id), _opType(op), _volume(), _time(t), _temp(NULL), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(mix), _outputName(NULL), _numDrops(-1), _destinationVolume(NULL)
	{}

	BioOperation(int id, OPERATION_TYPE op, int numDrops, Time* t = new Time()) // Split Digital
	:_ID(id), _opType(op), _volume(), _time(t), _temp(NULL), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(numDrops), _destinationVolume(NULL)
	{}

	BioOperation(int id, OPERATION_TYPE op, Volume* destinationVol, Time* t = new Time()) //Split Continuous Flow
	:_ID(id), _opType(op), _volume(), _time(t), _temp(NULL), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(destinationVol)
	{}

	BioOperation(int id, OPERATION_TYPE op, std::string outputDest, Time* t = new Time()) //Output/waste
	:_ID(id), _opType(op), _volume(), _time(t), _temp(NULL), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(new std::string(outputDest)), _numDrops(-1), _destinationVolume(NULL)
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
