/*
 * Temperature.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_
#include <iostream>

namespace BioCoder
{
enum TEMPERATURE_UNIT { FAHRENHEIT, CELSIUS, KELVIN, TEMPERATURE_NOT_SPECIFIED};

#define BOILING_WATER 101
#define ICE_COLD 3
#define ON_ICE 3
#define ROOM_TEMPERATURE 28

class Temperature
{
private:
	TEMPERATURE_UNIT _unit;
	double _value;
public:

	Temperature(): _unit(TEMPERATURE_NOT_SPECIFIED), _value(0) {}
	Temperature(TEMPERATURE_UNIT u, double v): _unit(u), _value(v) {}

	TEMPERATURE_UNIT GetTemperatureUnits() const { return _unit; }
	double GetValue() const { return _value; }

};

/*std::ostream& operator<<(std::ostream& os, const Temperature& obj)
	{

		switch ( obj.GetTemperatureUnits()) {
		case FAHRENHEIT:
			os << obj.GetValue() << " " << "Fahrenheit";
			break;
		case CELSIUS:
			os << obj.GetValue() << " " << "Celsius";
			break;
		case KELVIN:
			os << obj.GetValue() << " " << "Kelvin";
			break;
		case TEMPERATURE_NOT_SPECIFIED:
		default:
			std::cerr<<"Error Temperature Not Specified."<<std::endl;
		}
		return os;
	}*/
}

#endif /* TEMPERATURE_H_ */
