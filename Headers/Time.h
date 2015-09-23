/*
 * Time.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#ifndef TIME_H_
#define TIME_H_

#include <iostream>
#include <cstdio>

namespace BioCoder
{
	enum TIME_UNIT { NANO_SECS, MILLI_SECS, SECS, MINS, HRS, DAYS, OVERNIGHT, FOREVER, XVAL, TIME_NOT_SPECIFIED};
	class Time
	{
	private:
		TIME_UNIT _unit;
		double _value;
	public:

		Time(): _unit(TIME_NOT_SPECIFIED), _value(0) {}
		Time(TIME_UNIT u, double v): _unit(u), _value(v) {}

		TIME_UNIT GetTimeUnits() const { return _unit; }
		double GetValue() const { return _value; }
		void display_time(FILE* fp,int option_no,int options_flag, int & total_time_required)
		{
			std::string unit;
			int mul ;

			float max_time_value = 0;
			switch(_unit)
			{
			case SECS:if (_value == 1) unit = "sec"; else unit = "secs";mul = 1;break;
			case MINS:if (_value == 1) unit = "min"; else unit = "mins";mul = 60;break;
			case HRS:if (_value == 1) unit = "hr"; else unit = "hrs";mul = 3600;break;
			case DAYS:if(_value == 1) unit = "day"; else unit = "days";mul = 86400;break;
			case OVERNIGHT: fprintf (fp, "<b><font color=#357EC7>12 hrs</font></b>(overnight)"); break;
			case FOREVER: fprintf(fp, "<b><font color=#357EC7>forever</font></b>(until taken out from the thermocycler)"); break;
			case XVAL: fprintf(fp, "<b><font color=#357EC7>X</font></b>");
			default:break;
			}


			if(_unit == SECS || _unit == MINS || _unit == HRS || _unit == DAYS)
				fprintf (fp, "<b><font color=#357EC7>%g %s</font></b>", _value, unit.c_str());
			if((option_no == 2) && (options_flag == 1))
				max_time_value = _value * mul;
			else if(option_no > 2)
			{
				if(max_time_value > _value*mul)
					total_time_required = total_time_required + max_time_value;
				else
				{
					total_time_required = total_time_required + _value * mul;
					max_time_value = _value*mul;
				}
			}
			else
				total_time_required = total_time_required + _value * mul;
		}
	};

	/*std::ostream& operator<<(std::ostream& os, const Time& obj)
	{

		switch ( obj.GetTimeUnits()) {
		case NANO_SECS:
			os << obj.GetValue() << " " << "Nanoseconds";
			break;
		case MILLI_SECS:
			os << obj.GetValue() << " " << "Milliseconds";
			break;
		case SECS:
			os << obj.GetValue() << " " << "Seconds";
			break;
		case MINS:
			os << obj.GetValue() << " " << "Minutes";
			break;
		case HRS:
			os << obj.GetValue() << " " << "Hours";
			break;
		case DAYS:
			os << obj.GetValue() << " " << "Days";
			break;
		case TIME_NOT_SPECIFIED:
		default:
			std::cerr<<"Error Time Not Specified."<<std::endl;
		}
		return os;
	}*/
}
#endif /* TIME_H_ */
