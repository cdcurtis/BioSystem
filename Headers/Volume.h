/*
 * Volume.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#ifndef VOLUME_H_
#define VOLUME_H_
#include <iostream>
#include <cstdio>

namespace BioCoder
{
	enum VOLUME_UNIT { NANO_LITER, MICRO_LITER, MILLI_LITER, LITER, VOLUME_NOT_SPECIFIED};
	class Volume
	{
	private:
		VOLUME_UNIT _unit;
		double _value;
	public:

		Volume(): _unit(VOLUME_NOT_SPECIFIED), _value(0) {}
		Volume(VOLUME_UNIT u, double v): _unit(u), _value(v) {}

		VOLUME_UNIT GetVolumeUnits() const { return _unit; }
		double GetValue() const { return _value; }
		double GetUL()
		{
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
