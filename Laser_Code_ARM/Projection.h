#ifndef PROJECTION_H
#define PROJECTION_H

#define LASERL                4

#define STEPSIZE              30
#define STEPSIZEWHILELASEROFF 40

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


#define MAX_NUMBERS_OF_CHARS_PER_PROJECTION 4

#define ESCAPE_CHAR '_'

#define DOT ESCAPE_CHAR + ".__"
#define CALIBRATION_DOT "_C._"
#define SQUARE "_[]_"

#include <avr/io.h>

#include "galvo_modern.h"




void initProjection();

void moveAbs(int target_x, int target_y);
//void moveAbs(int target_x, int target_y, int stepsize);

void drawTriangle(int x, int y, int bigness);

void addOffset(int dx, int dy);
void addOffsetSimul(int dx, int dy);
void addOffsetWhileLaserIsOff(int dx, int dy);

void projectString(int pos_x, int pos_y, int projectionLength, char projection[MAX_NUMBERS_OF_CHARS_PER_PROJECTION], int scaler, bool withLaserOn);

void projectNumber(int pos_x, int pos_y, int number, int scaler, bool withLaserOn);
//void projectNumber10(int pos_x, int pos_y, int number, int scaler, bool withLaserOn);

void projectSquare(int xOffset, int yOffset, int width, int height);

void drawPlus(int xPos, int yPos, int scaler);
void drawBlob(int pos_x, int pos_y, int scaler, bool laserOn);
void drawBlobs(int* blobs, uint8_t n);
void drawC(int xPos, int yPos, int scaler);
void drawDiamond(int xPos, int yPos, int scaler);
void moveMirrors(uint16_t x, uint16_t y);

void drawThree(int pos_x, int pos_y, int scaler);
void projectDot(int pos_x, int pos_y);

void projectCalibrationDot(int pos_x, int pos_y);

void setRotation(int8_t rotation);
void setMirror(int8_t mirror);
void setCharspace(int16_t charspace);

bool PositionIsOutOfBounds(int x,int y,int scaler);
#endif
