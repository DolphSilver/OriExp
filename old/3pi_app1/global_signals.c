/*
 * global_signal_container.c
 *
 * Created: 2017-12-08 07:42:04
 *  Author: a243403
 */ 

#include "global_signal.h"

static uint16_t sensors_raw[N_samp_RAW][N_SENSORS];

static uint16_t position_x;
static uint16_t position_y;
static uint16_t orientation;

void sensors_raw_Set()
pointer linked structure
in set function, also add position and time stamp to data