/*
 * Store.h
 *
 *  Created on: Jun 2, 2017
 *      Author: chriscurtis
 */

#ifndef HEADERS_OPERATIONS_Store_H_
#define HEADERS_OPERATIONS_Store_H_
#include "Operation.h"
#include <vector>

namespace BioCoder
{

class Store:public BioCoder::Operation{

	Container* __inputs;
	std::vector<Property> __properties;
	EXPERIMENT_EVENT optionalEvent;

public:
	Store(Container* c1, Time t1){
		this->optionalEvent= EVENT_NOT_SPECIFIED;
		this->__inputs = c1;
		this->__properties.push_back(t1);
	}
	Store(Container* c1, Time t1, EXPERIMENT_EVENT event){
		this->optionalEvent = event;
		this->__inputs = c1;
		this->__properties.push_back(t1);
	}
	Store(Container* c1, Time t1, Temperature temp){
		this->optionalEvent = EVENT_NOT_SPECIFIED;
		this->__inputs = c1;
		this->__properties.push_back(t1);
		this->__properties.push_back(temp);
	}
	Store(Container* c1, Time t1, Temperature temp, EXPERIMENT_EVENT event){
		this->optionalEvent = event;
		this->__inputs = c1;
		this->__properties.push_back(t1);
		this->__properties.push_back(temp);
	}

	OPERATION_TYPE GetType(){
		return STORE;
	}
	std::string toString(std::string buffer)
	{
		std::string ret=buffer + "\"OPERATION\" : {\n";
		std::string buf = buffer + '\t';

		ret += buf + "\"NAME\" : \"Store\"\n";
		ret += buf + "\"ID\" :" + '0' /*getID*/+ "\n";
		ret += buf + "\"CLASSIFICATION\" : \"STORE\"\n";

		ret += buf + "\"NAME\" : \"Store\"\n";
		ret += buf + "\"INPUTS\" : [\n";
		ret += buf + "{\n";

		ret += buf + '\t' + "\"INPUT_TYPE\" : \"VARIABLE\",\n";
		ret += buf + '\t' + this->__inputs->toString(buf + '\t');

		if(this->__properties.size() == 0 && this->__type == EVENT_NOT_SPECIFIED)
			ret += buf + "}\n";
		else
			ret += buf + "},\n";

		ret += buf + "{\n";

		ret += buf + '\t' + "\"INPUT_TYPE\" : \"PROPERTY\",\n";

		for(Property p : this->__properties){
			ret += buf + "{";
			ret += buf + p.toString(buf);

			//if(this->__type == EVENT_NOT_SPECIFIED)
				ret += buf + "}\n";
			//else
			//	ret += buf + "},\n";
		}


		ret+= "}\n";

		return ret;
	}
	/*

    // Add concept properties
    indent buffer ;
    append {{} \n ;

    // Add Temperature property
    with indent {
      indent buffer ;
      append {"INPUT_TYPE" : "PROPERTY",} \n ;

      // TODO: Complete TextGen for Temperature Concept
      // indent buffer ;
      // append ${node.temp} ;
    }

    indent buffer ;
    append {}} ;

    // If last substance of list with no time, event_type, or storage_func then no comma should be added
    // otherwise add comma.
    if (node.container.next-sibling.isNotNull) {
      append {,} \n ;
    } else {
      // last element of series
      if (node.time.isNotNull || node.event.isNotNull || node.storage_func.isNotNull) {
        append {,} \n ;
      } else {
        append \n ;
      }
    }
    // End of Temperature property


    // Add if concept has Time property
    if (node.time.isNotNull) {
      indent buffer ;
      append {{} \n ;

      with indent {
        indent buffer ;
        append {"INPUT_TYPE" : "PROPERTY",} \n ;

        // TODO: Complete TextGen for Time Concept
        // indent buffer ;
        // append ${node.time} ;
      }

      indent buffer ;
      append {}} ;

      // If last substance of list with no time, temp, event_type, or storage_func then no comma should be added
      // otherwise add comma.
      if (node.container.next-sibling.isNotNull) {
        append {,} \n ;
      } else {
        // last element of series
        if (node.event.isNotNull || node.storage_func.isNotNull) {
          append {,} \n ;
        } else {
          append \n ;
        }
      }
    }
    // endif
    // end of Time property

    // Add if concept has Event property
    if (node.event.isNotNull) {
      // Add Event property
      indent buffer ;
      append {{} \n ;

      with indent {
        indent buffer ;
        append {"INPUT_TYPE" : "PROPERTY",} \n ;

        // TODO: Create textGen for Event
      }

      indent buffer ;
      append {}} ;

      // If last substance of list with storage_func then no comma should be added
      // otherwise add comma.
      if (node.container.next-sibling.isNotNull) {
        append {,} \n ;
      } else {
        // last element of series
        if (node.event.isNotNull || node.storage_func.isNotNull) {
          append {,} \n ;
        } else {
          append \n ;
        }
      }
    }
    // endif
    // end of Event property

    // Add if concept has Storage property
    if (node.storage_func.isNotNull) {
      indent buffer ;
      append {{} \n ;

      // Add Event property
      indent buffer ;
      append {"INPUT_TYPE" : "PROPERTY",} \n ;

      // TODO: Create textGen for Storage
      // indent buffer ;
      // append ${node.storage_func} ;

      indent buffer ;
      append {}} \n ;
    }
    // endif
    // end of Storage property

    // end of concept properties
  }

  indent buffer ;
  append {]} \n ;
}
	 */
};


}
#endif /* HEADERS_OPERATIONS_FLUIDICOPERATION_H_ */
