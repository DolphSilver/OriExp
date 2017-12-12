/*
 * readBC.h
 *
 * Created: 2017-12-07 08:54:32
 *  Author: A243403
 */ 


#ifndef READBC_H_
#define READBC_H_

/*CALIBRATION PARAMETERS*/
//TESTED FOR READ BARCODE FUNCTION:
// N_SAMPLE_AVERAGE: BC_FOLLOW_SPEED < 40: 20 OK  
//					 BC_FOLLOW_SPEED < 60: 10 OK

#define SH_N_SAMPLES_AVERAGE 10 //number of samples to average over, connected to speed of robot
#define SH_TH0 300 //treshold value for sensors 0 and 4
#define SH_TH1 500 //treshold value for sensors 1 and 3
#define SH_TH2 800 //treshold value for sensor 2

#define SH_FOLLOW_SPEED 30 //speed value for follow function
#define SH_FOLLOW_P_GAIN_INV 20 //p-gain value inverse -> p = 1/BC_FOLLOW...
#define SH_FOLLOW_I_GAIN_INV 10000 //i-gain value inverse -> I = 1/BC_FOLLOW...

#define MAX_DIST_SAMP 200

#define ARRAYSIZE(x) (sizeof x/sizeof x[0])

//Definitions of valid sensor readings
//#define SR_ON_LINE (uint8_t)0x20
//#define SR_SHX_1 (uint8_t)0xA8
//#define SR_SHX_2 (uint8_t)0x70
//#define SR_SHX_3 (uint8_t)0x47
//#define SR_SHX_4 (uint8_t)0xC8

//Definitions of valid events
//#define EV_SH_4_w (uint16_t)0xAB88
//#define EV_SH_1_4_a (uint16_t)0x7548
//#define EV_SH_3 (uint16_t)0x2548
//#define EV_SH_2 (uint16_t)0x711C
//#define EV_SH_1_w (uint16_t)0x47C8

//Definitions of valid sensor readings
#define SR_ON_LINE (uint8_t)0x20
#define SR_SHX_1 (uint8_t)0xA8
#define SR_SHX_2 (uint8_t)0x70
#define SR_SHX_3 (uint8_t)0x47
#define SR_SHX_4 (uint8_t)0xC8
#define SR_SHX_5 (uint8_t)0xF8


//Definitions of valid events
#define EV_SH_4_w (uint16_t)0x257E
#define EV_SH_4_a (uint16_t)0x712A
#define EV_SH_3 (uint16_t)0x2548
#define EV_SH_2 (uint16_t)0x711C

#define EV_SH_1_a (uint16_t)0x712B

#define EV_SH_1_w1 (uint16_t)0xAFEA
#define EV_SH_1_w2 (uint16_t)0xAFDC
#define EV_SH_1_w3 (uint16_t)0xAFC8
#define EV_SH_1_w4 (uint16_t)0x77EA
#define EV_SH_1_w5 (uint16_t)0x77DC
#define EV_SH_1_w6 (uint16_t)0x77C8

void readSH(uint8_t *sh_nbr);



#endif /* READBC_H_ */