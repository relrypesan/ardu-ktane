/*
  Copyright (c) 2013 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef COMPACT_ARDUINO_H_
#define COMPACT_ARDUINO_H_

#include <Arduino.h>

void setPin(unsigned int pin, bool mode) {
	if (pin > 13) {
		return;
	}
	if (pin >= 8) {
		DDRB |= (mode << (pin - 8));
	}
	else {
		DDRD |= (mode << pin);
	}
}

void writePin(unsigned int pin, bool state) {
	if (pin > 13) {
		return;
	}

	if (state) {
		if (pin >= 8) {
			PORTB |= (1 << (pin - 8));
		}
		else {
			PORTD |= (1 << pin);
		}
	}
	else {
		if (pin >= 8) {
			PORTB &= ~(1 << (pin - 8));
		}
		else {
			PORTD &= ~(1 << pin);
		}
	}
}

bool readPin(unsigned int pin) {
	if (pin > 13) {
		return false;
	}
	else if (pin >= 8) {
		return (PINB & (1 << (pin - 8)));
	}
	else {
		return (PIND & (1 << pin));
	}
}

#endif /* COMPACT_ARDUINO_H_ */
