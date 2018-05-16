/*
 * UDPError.h
 *
 *   Created on: Mar 9, 2018
 *       Author: Tuan Duc Vo
 *  Description: Error handling utility
 */

#ifndef UDPERROR_H_
#define UDPERROR_H_
#include <stdexcept>
#include <string>
using namespace std;

class UDPError {
public:
       /*
 	* Throws runtime error
        *
        * @param msg error message
        * @return N/A
        */
	static void raiseError(const char* msg) {
		throw runtime_error((string("Error: ") + string(msg)).c_str());
	}
};


#endif /* UDPERROR_H_ */
