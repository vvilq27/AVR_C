/*
 * ADC.h
 *
 *  Created on: Oct 10, 2017
 *      Author: root
 */
#include <avr/io.h>

#ifndef ADC_H_
#define ADC_H_

// adc is set on port C
#define ADC_PORT C
#define PIN_PORT(x) sPIN_PORT(x)
#define sPIN_PORT(x) (DDR##x)


void adc_init(void);
uint16_t pomiar(uint8_t);

#endif /* ADC_H_ */
