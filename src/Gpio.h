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
	int gpio_set_value(unsigned int gpio, unsigned int value);
	int gpio_get_value(unsigned int gpio, unsigned int *value);
	int gpio_fd_open(unsigned int gpio);
};

#endif /* GPIO_H_ */
