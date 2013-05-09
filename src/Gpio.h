/*
 * Gpio.h
 *
 *  Created on: May 7, 2013
 *      Author: tomer
 */

#ifndef GPIO_H_
#define GPIO_H_

namespace securityServer {

class Gpio {
public:
	Gpio(int number);
	void init();
	void poll();
	virtual ~Gpio();
};

} /* namespace securityServer */
#endif /* GPIO_H_ */
