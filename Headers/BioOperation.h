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
#include "BioExpression.h"

namespace BioCoder
{
/*! Specifies certain conditions that might have to be satisfied for the completion of a step. For use in store_until(), vortex(), etc. */
enum EXPERIMENT_EVENT{ETHANOL_EVAP/*!< "until all the ethanol has evaporated and no fluid is visible in the tube".*/, OD/*!< "until the O.D.600 reaches 0.6". */, THAW/*!< "until the sample has thawed". */, COOLED/*!< "until cooled". */, COLOUR_DEVELOPS/*!< "until the colour develops". */, PPT_STOPS_STICKING/*!< "until the precipitate stops sticking to the walls of the tube". */, PELLET_DISLODGES/*!< "until the pellet dislodges". */, THAW_ICE/*!< "keep on ice until the sample has thawed" */, EVENT_NOT_SPECIFIED};
enum OPERATION_TYPE {DISPENSE, DETECT, HEAT, COOL, STORE, MIX, SPLIT, WASTE, OUTPUT, IF_OP, ELSE_IF_OP, ELSE_OP, WHILE_OP, END_IF_OP, END_WHILE_OP, OPERATION_NOT_SPECIFED};
//enum ARCHITECTURE_TYPE {DIGITAL_MF, CONTINOUS_FLOW_MF, ARCHITECTURE_NOT_SPECIFIED};
enum DETECT_TYPE {CE_DETECT, MEASURE_FLUORESCENCE, ELECTROPHORESIS, SEQUENCING, WEIGH, FACS, CELL_CULTURE, TRANSFECTION, ELECTROPORATE, DETECT_NOT_SPECIFIED};
enum MIX_TYPE{TAP, STIR, INVERT, VORTEX, RESUSPEND, DISSOLVE, PIPET, MIX_NOT_SPECIFIED};
enum STORAGE_FUNCTION{DEWAX/*!< dewaxing */, DENATURE/*!< denaturation */, ENZYME_INAC/*!< enzyme inactivation */,STORAGE_FUNCTION_NOT_SPECIFIED};

class BioExpression;

struct BioOperation
{
public:
	int _ID;
	std::vector<BioOperation*> _parents;
	std::vector<BioOperation*> _children;
	std::vector<BioOperation*> _controlDependant;

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
	BioExpression* _expression;
	BioOperation* _trueBranch;
	std::string _nickname;


	BioOperation(int id)
	: _ID(id), _opType(OPERATION_NOT_SPECIFED), _volume(), _time(), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, Volume v, Fluid* f, Time t = Time()) //Dispense Operation.
	: _ID(id), _opType(op), _volume(v), _time(t), _temp(), _fluid(f), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, Fluid* f, Time t = Time()) //Dispense Operation.
	: _ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(f), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, DETECT_TYPE type, Time t = Time(),std::string nickname = "") //Detect Operation.
	: _ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(type), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname(nickname)
	{}

	BioOperation(int id, OPERATION_TYPE op, Temperature temp, Time t = Time()) //Heat/Cool/Store
	:_ID(id), _opType(op), _volume(), _time(t), _temp(temp), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, MIX_TYPE mix, Time t = Time()) // Mix Operation
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(mix), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, int numDrops, Time t = Time()) // Split Digital
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(numDrops), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, Volume destinationVol, Time t = Time()) //Split Continuous Flow
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(destinationVol),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, Time t)// Store Operation
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, std::string outputDest, Time t = Time()) //Output/Waste
	:_ID(id), _opType(op), _volume(), _time(t), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(new std::string(outputDest)), _numDrops(-1), _destinationVolume(),_expression(NULL),_trueBranch(NULL),_nickname("")
	{}

	BioOperation(int id, OPERATION_TYPE op, BioExpression* expression = NULL) //conditional
	: _ID(id), _opType(op), _volume(), _time(), _temp(), _fluid(NULL), _detectType(DETECT_NOT_SPECIFIED), _mixType(MIX_NOT_SPECIFIED), _outputName(NULL), _numDrops(-1), _destinationVolume(),_expression(expression),_trueBranch(NULL),_nickname("")
	{}


	std::string Name()
	{
		char buffer[25];
		switch(_opType){
		case COOL:
			sprintf(buffer,"Cool ID:%i", _ID);
			break;
		case DISPENSE:
			sprintf(buffer,"Dispense ID:%i", _ID);
			break;
		case DETECT:
			if(_nickname == "")
				sprintf(buffer,"Detect ID:%i", _ID);
			else
				sprintf(buffer,"Detect \'%s\' ID:%i",_nickname.c_str(), _ID);
			break;
		case HEAT:
			sprintf(buffer,"Heat ID:%i", _ID);
			break;
		case MIX:
			sprintf(buffer,"Mix ID:%i", _ID);
			break;
		case SPLIT:
			sprintf(buffer,"Split ID:%i", _ID);
			break;
		case STORE:
			sprintf(buffer,"Store ID:%i", _ID);
			break;
		case OUTPUT:
			sprintf(buffer,"Output ID:%i", _ID);
			break;
		case WASTE:
			sprintf(buffer,"Waste ID:%i", _ID);
			break;
		case IF_OP:
			sprintf(buffer,"if ID:%i", _ID);
			break;
		case ELSE_IF_OP:
			sprintf(buffer,"else if ID:%i", _ID);
			break;
		case ELSE_OP:
			sprintf(buffer,"else ID:%i", _ID);
			break;
		case WHILE_OP:
			sprintf(buffer,"whileID:%i", _ID);
			break;
		case END_IF_OP:
			sprintf(buffer,"END_IF ID:%i", _ID);
			break;
		case END_WHILE_OP:
			sprintf(buffer,"END_WHILE ID:%i", _ID);
			break;

		default:
			sprintf(buffer,"UNKNOWN%i", _ID);

		}

		return buffer;
	}
	BioExpression* Expression() {	return this->_expression; }
};



struct SensorReading {
	BioOperation* _op;
	SensorReading(): _op(NULL) {}
	SensorReading(BioOperation * op): _op(op) {}
};

}

#endif /* BIOOPERATION_H_ */
