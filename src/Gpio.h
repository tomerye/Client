/*
 * Gpio.h
 *
 *  Created on: May 16, 2013
 *      Author: tomer
 */

#ifndef GPIO_H_
#define GPIO_H_

class Gpio {
public:
	Gpio();
	virtual ~Gpio();
	int init(unsigned int gpioNum);



};

#endif /* GPIO_H_ */
