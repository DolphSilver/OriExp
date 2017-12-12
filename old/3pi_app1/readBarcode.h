/*
 * readBarcode.h
 *
 * Created: 2017-12-06 16:18:27
 *  Author: A243403
 */ 


#ifndef READBARCODE_H_
#define READBARCODE_H_

/*CALIBRATION PARAMETERS*/
// N_SAMPLE_AVERAGE: BC_FOLLOW_SPEED < 40: 20 OK
//					 BC_FOLLOW_SPEED < 60: 10 OK

#define N_SAMPLES_AVERAGE 10 //number of samples to average over, connected to speed of robot
#define TH0 400 //treshold value for sensors 0 and 4
#define TH1 700 //treshold value for sensors 1 and 3
#define TH2 800 //treshold value for sensor 2

#define BC_FOLLOW_SPEED 50 //speed value for follow function
#define BC_FOLLOW_P_GAIN_INV 20 //p-gain value inverse -> p = 1/BC_FOLLOW...
#define BC_FOLLOW_I_GAIN_INV 10000 //i-gain value inverse -> I = 1/BC_FOLLOW...

#define DEBUG_MODE 0
#define DEBUG_MODE_2 1

 
#define ARRAYSIZE(x) (sizeof x/sizeof x[0])

//Definitions of valid sensor readings
#define SR_ON_LINE (uint8_t)0x20
#define SR_BC_START (uint8_t)0x70
#define SR_BC_STOP (uint8_t)0x00
#define SR_LEFT_BIT (uint8_t)0xA0
#define SR_RIGHT_BIT (uint8_t)0x28
//Definitions of valid events
#define EV_BC_START (uint16_t)0x2070
#define EV_BC_CONT (uint16_t)0x7020
#define EV_BC_STOP (uint16_t)0x7000
#define EV_LEFT_BIT_HIGH_1 (uint16_t)0x20A0
#define EV_LEFT_BIT_HIGH_2 (uint16_t)0x28A0
#define EV_RIGHT_BIT_HIGH_1 (uint16_t)0x2028
#define EV_RIGHT_BIT_HIGH_2 (uint16_t)0xA028

uint8_t readBarcode(uint8_t *barcode, uint8_t *sh_code);

#endif /* READBARCODE_H_ */