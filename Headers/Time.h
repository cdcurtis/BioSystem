/*
 * Time.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Chris
 */

#ifndef TIME_H_
#define TIME_H_

#include <iostream>

namespace BioCoder
{
	enum TIME_UNIT { NANO_SECS, MILLI_SECS, SECS, MINS, HRS, DAYS, TIME_NOT_SPECIFIED};
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
