/*
 * Volume.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#ifndef VOLUME_H_
#define VOLUME_H_
#include <iostream>
#include <sstream>
#include <cstdio>
#include "Property.h"


namespace BioCoder
{
enum VOLUME_UNIT { NANO_LITER, MICRO_LITER, MILLI_LITER, LITER, VOLUME_NOT_SPECIFIED};
class Volume: public BioCoder::Property
{
private:
	VOLUME_UNIT _unit;
	double _value;

	std::string stringifyVolume(VOLUME_UNIT volume)
	{
		switch(volume)
		{
		case NANO_LITER:
			return "NANO_LITER";
		case MICRO_LITER:
			return "MICRO_LITER";
		case MILLI_LITER:
			return "MILLI_LITER";
		case LITER:
			return "LITER";
		default:
			return "UNKNOWN";
		}
	}
public:

	Volume(): _unit(VOLUME_NOT_SPECIFIED), _value(0) {}
	Volume(VOLUME_UNIT u, double v): _unit(u), _value(v) {}

	VOLUME_UNIT GetVolumeUnits() const { return _unit; }
	double GetValue(VOLUME_UNIT unit = VOLUME_NOT_SPECIFIED) const
	{
		switch(unit)
		{
		case NANO_LITER:
			switch(_unit)
			{
			case NANO_LITER:
				return _value;
			case MICRO_LITER:
				return _value * 1000;
			case MILLI_LITER:
				return _value * 1000000;
			case LITER:
				return _value * 1000000000;
			default:
				return -1;
			}
			break;
		case MICRO_LITER:
			switch(_unit)
			{
			case NANO_LITER:
				return _value / 1000;
			case MICRO_LITER:
				return _value;
			case MILLI_LITER:
				return _value * 1000;
			case LITER:
				return _value * 1000000;
			default:
				return -1;
			}
			break;
		case MILLI_LITER:
			switch(_unit)
			{
				case NANO_LITER:
					return _value / 1000000;
				case MICRO_LITER:
					return _value / 1000;
				case MILLI_LITER:
					return _value ;
				case LITER:
					return _value * 1000;
				default:
					return -1;
			}
			break;
		case LITER:
			switch(_unit)
			{
				case NANO_LITER:
					return _value / 1000000000;
				case MICRO_LITER:
					return _value / 1000000;
				case MILLI_LITER:
					return _value / 1000;
				case LITER:
					return _value ;
				default:
					return -1;
			}
			break;
		default:
			return _value;
		}
	}

	void display_vol(FILE* fp)
	{
		switch(_unit)
		{
		case NANO_LITER:
			fprintf(fp, "<b><font color=#357EC7>%g %s</font></b>", _value, "nl");
			break;
		case MICRO_LITER:
			fprintf(fp, "<b><font color=#357EC7>%g %s</font></b>", _value, "ul");
			break;
		case MILLI_LITER:
			fprintf(fp, "<b><font color=#357EC7>%g %s</font></b>", _value, "ml");
			break;
		case LITER:
			fprintf(fp, "<b><font color=#357EC7>%g %s</font></b>", _value, "l");
			break;
		default:
			break;
		}

	}

	std::string toString(std::string buffer)
	{
		std::string ret;
		ret += buffer + "\"VOLUME\": {\n";
		std::string buf = buffer + '\t';

		std::stringstream s;
		s << this->_value;
		std::string tempValue = s.str();


		ret+=buf + "\"VALUE\" : \"" + tempValue + "\",\n";
		ret+=buf + "\"UNITS\" : \"" + stringifyVolume(this->_unit) + "\",\n";
		ret+=buffer + "}\n";

		return ret;
	}

};

/*std::ostream& operator<<(std::ostream& os, const Volume& obj)
	{

		switch ( obj.GetVolumeUnits()) {
		case NANO_LITER:
			os << obj.GetValue() << " " << "Nanoliters";
			break;
		case MILLI_LITER:
			os << obj.GetValue() << " " << "Milliliters";
			break;
		case LITER:
			os << obj.GetValue() << " " << "Liters";
			break;
		case VOLUME_NOT_SPECIFIED:
		default:
			std::cerr<<"Error Volume Not Specified."<<std::endl;
		}
		return os;
	}*/
}




#endif /* VOLUME_H_ */
