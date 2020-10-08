#ifndef PROJECTION_H
#define PROJECTION_H

#define STEPSIZE 1000
//software limits of laser (in uint16_t )
#define MAX_X 4000
#define MIN_X 0
#define MAX_Y 4000
#define MIN_Y 0

#define PIN_SPI_MISO  12
#define PIN_SPI_MOSI  11
#define PIN_SPI_SCK   13
#define PAD_SPI_TX    SPI_PAD_0_SCK_1 
#define PAD_SPI_RX    SERCOM_RX_PAD_3


#define CS 5
//#define SCK DDB1
//#define MOSI DDB2



#include <avr/io.h>
/*
#include "galvo_modern.h"
*/


void initProjection();

void moveAbs(int target_x, int target_y);

void moveMirrors(uint16_t x, uint16_t y);

#endif
