#include "Arduino.h"
#include "SPI.h"
#include "Projection.h"
#include "wiring_private.h" // pinPeripheral() function

#include "galvo_modern.h"

#define laserOn()           \
  delayMicroseconds(150); \
  digitalWrite(LASERL, HIGH)
#define laserOff()          \
  delayMicroseconds(150); \
  digitalWrite(LASERL, LOW)

int currentX;
int currentY;

int rotationIntern = 1;
int mirrorIntern = 1;
int charspaceIntern = 90; //white space between chars, expressed in percentage of the scaler value (note that a char is two scaler's wide!)

int8_t rot_mat[4] = {1, 0, 0, 1};

void initSPI();
void project(int pos_x, int pos_y, int number, int scaler, bool withLaserOn);
void projectChar(int pos_x, int pos_y, char character, int scaler, bool withLaserOn);

void initProjection()
{
  currentX = 0;
  currentY = 0;

  initSPI();
}

SPIClass mySPI(&sercom1, PIN_SPI_MISO, PIN_SPI_SCK, PIN_SPI_MOSI, PAD_SPI_TX, PAD_SPI_RX);

void initSPI()
{
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  mySPI.begin();

  // assign SPI pins to SERCOM
  pinPeripheral(PIN_SPI_MISO, PIO_SERCOM);
  pinPeripheral(PIN_SPI_SCK, PIO_SERCOM);
  pinPeripheral(PIN_SPI_MOSI, PIO_SERCOM);

  mySPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  /*DDRF |= (1<<CS);
    DDRB |= (1<<SCK) | (1<<MOSI);
    PORTF |= (1<<CS);
    SPCR = (1<<SPE) | (1<<MSTR);
    SPSR = (1 << SPI2X);*/

  // Setup device

  // Power-UP DAC
  digitalWrite(CS, LOW);
  mySPI.transfer(0b00011111);
  mySPI.transfer(0x00);
  mySPI.transfer(0x00);
  digitalWrite(CS, HIGH);

  // Set internal reference
  digitalWrite(CS, LOW);
  mySPI.transfer(0b01101111);
  mySPI.transfer(0x00);
  mySPI.transfer(0x00);
  digitalWrite(CS, HIGH);
}

void SPIsend(uint8_t commandByte, uint16_t message)
{
  digitalWrite(CS, LOW);

  mySPI.transfer(commandByte);
  mySPI.transfer((message & 0xFF00) >> 8);
  mySPI.transfer(message & 0xFF);

  /*SPDR = commandByte;
    while(!(SPSR & (1<<SPIF)));

    SPDR = (message & 0xFF00) >> 8;
    while(!(SPSR & (1<<SPIF)));
    SPDR = (message & 0x00FF);
    while(!(SPSR & (1<<SPIF)));*/

  digitalWrite(CS, HIGH);
}

/* moves laser beam to an absolute position */
void moveAbs(int target_x, int target_y)
{

  /* do any coordinate conversion here (scaling, limitaion, inversion) */
  if (target_x > MAX_X)
    target_x = MAX_X;
  else if (target_x < MIN_X)
    target_x = MIN_X;

  if (target_y > MAX_Y)
    target_y = MAX_Y;
  else if (target_y < MIN_Y)
    target_y = MIN_Y;

  /* THIS -90 DEG ROTATION IS (SEMI)ARBITRARY, AND COULD BE REMOVED  *
   * BUT IN THAT CASE ALL CALIBRATIONS NEED TO BE RE-DONE!           */
  int tgt_x = target_x; //temporarily save x
  target_x = target_y;
  target_y = MAX_X - tgt_x;

  bool y_reached = false, x_reached = false;

  while (true)
  {
    int error_y = currentY - target_y;
    int error_x = currentX - target_x;

    if (error_y > 0)
    {
      if (error_y >= STEPSIZE)
      {
        currentY -= STEPSIZE;
      }
      else
      {
        currentY = target_y;
      }
    }
    else if (error_y < 0)
    {
      if (error_y <= -STEPSIZE)
      {
        currentY += STEPSIZE;
      }
      else
      {
        currentY = target_y;
      }
    }
    else
    { //error_y == 0
      y_reached = true;
      // SerialUSB.println("reached y");
    }

    if (error_x > 0)
    {
      if (error_x >= STEPSIZE)
      {
        currentX -= STEPSIZE;
      }
      else
      {
        currentX = target_x;
      }
    }
    else if (error_x < 0)
    {
      if (error_x <= -STEPSIZE)
      {
        currentX += STEPSIZE;
      }
      else
      {
        currentX = target_x;
      }
    }
    else
    { //error_x == 0
      x_reached = true;
      //SerialUSB.println("reached x");
    }

    //SerialUSB.println(currentX);
    //SerialUSB.println(currentY);
    //do the actual moving
    if (y_reached && x_reached)
    {
      delayMicroseconds(300);
      break;
    }
    else
    {
      moveMirrors(currentX, currentY);
      delayMicroseconds(1);
    }
  }
}

void moveMirrors(uint16_t x, uint16_t y)
{

  byte conAddrDacA = 0b00110000;
  byte conAddrDacB = 0b00110001;

  x = x << 4;
  y = y << 4;

  SPIsend(conAddrDacA, x);
  SPIsend(conAddrDacB, y);

  //_delay_us(1); // wait for mirrors to physically arrive
}

int zero_n = 5;
int pattern_zero[] = {-1, -1, 1, -1, 1, 1, -1, 1, -1, -1};
int one_n = 2;
int pattern_one[] = {-1, 0, 1, 0};
int two_n = 6;
int pattern_two[] = {-1, 1, -1, -1, 0, -1, 0, 1, 1, 1, 1, -1};
int three_n = 7;
int pattern_three[] = {-1, 1, -1, -1, 0, -1, 0, 0, 0, -1, 1, -1, 1, 1};
int four_n = 5;
int pattern_four[] = {-1, 1, 0, 1, 0, -1, -1, -1, 1, -1};
int five_n = 6;
int pattern_five[] = {-1, -1, -1, 1, 0, 1, 0, -1, 1, -1, 1, 1};
int six_n = 6;
int pattern_six[] = {-1, -1, -1, 1, 1, 1, 1, -1, 0, -1, 0, 1};

int seven_n = 3;
int pattern_seven[] = {-1, 1, -1, -1, 1, -1};
int eight_n = 7;
int pattern_eight[] = {0, -1, -1, -1, -1, 1, 1, 1, 1, -1, 0, -1, 0, 1};
int nine_n = 6;
int pattern_nine[] = {0, -1, 0, 1, -1, 1, -1, -1, 1, -1, 1, 1};

void projectString(int pos_x, int pos_y, int projectionLength, char projection[MAX_NUMBERS_OF_CHARS_PER_PROJECTION], int scaler, bool withLaserOn)
{
  //calculate the whitespace between characters in lasersteps.
  // charspaceIntern is this value in percentage of the scaler value (note that a single characters is two scaler-values wide, from -1 to 1 * scaler)
  int16_t whitespace = (scaler * charspaceIntern) / 100;

  //calculate distance between the centers of two chars in case we want to project more than one
  int16_t seperation = scaler + whitespace / 2;
  
	if (projectionLength == 1)
	{
		projectChar(pos_x, pos_y, projection[0], scaler, withLaserOn);
	}
	else if (projectionLength == 2)
	{
		int x_offset = rot_mat[0] * ( 1 * seperation ) * mirrorIntern + rot_mat[1] * ( 0 ); //zero beccause we only add x offset
    int y_offset = rot_mat[2] * ( 1 * seperation ) * mirrorIntern + rot_mat[3] * ( 0 );

    projectChar(pos_x - x_offset, pos_y - y_offset, projection[0], scaler, withLaserOn);
    projectChar(pos_x + x_offset, pos_y + y_offset, projection[1], scaler, withLaserOn);
	}
	else if (projectionLength == 3)
	{
		int x_offset = rot_mat[0] * ( 2 * seperation ) * mirrorIntern + rot_mat[1] * ( 0 ); //zero beccause we only add x offset
    int y_offset = rot_mat[2] * ( 2 * seperation ) * mirrorIntern + rot_mat[3] * ( 0 );

    projectChar(pos_x - x_offset, pos_y - y_offset, projection[0], scaler, withLaserOn);
    projectChar(pos_x,            pos_y,            projection[1], scaler, withLaserOn);
    projectChar(pos_x + x_offset, pos_y + y_offset, projection[2], scaler, withLaserOn);
	}
	else if (projectionLength == 4)
	{
    int x_offsetInner = rot_mat[0] * ( 1 * seperation ) * mirrorIntern + rot_mat[1] * ( 0 ); //zero beccause we only add x offset
    int y_offsetInner = rot_mat[2] * ( 1 * seperation ) * mirrorIntern + rot_mat[3] * ( 0 );
    
    int x_offsetOuter = rot_mat[0] * ( 3 * seperation ) * mirrorIntern + rot_mat[1] * ( 0 );
    int y_offsetOuter = rot_mat[2] * ( 3 * seperation ) * mirrorIntern + rot_mat[3] * ( 0 );
    
		projectChar(pos_x - x_offsetOuter, pos_y - y_offsetOuter, projection[0], scaler, withLaserOn);
		projectChar(pos_x - x_offsetInner, pos_y - y_offsetInner, projection[1], scaler, withLaserOn);
		projectChar(pos_x + x_offsetInner, pos_y + y_offsetInner, projection[2], scaler, withLaserOn);
		projectChar(pos_x + x_offsetOuter, pos_y + y_offsetOuter, projection[3], scaler, withLaserOn);
	}
	else
	{
		// Longer, not implemented
	}
}

void projectChar(int pos_x, int pos_y, char character, int scaler, bool withLaserOn)
{
  int x, y;
  int pattern[MAX_PATTERN_N * 2];
  int pattern_n;

  pattern_n = getPattern(character, pattern, mirrorIntern);

  for (int i = 0; i < pattern_n; i++)
  {
    x = pos_x + rot_mat[0] * pattern[i * 2] * scaler + rot_mat[1] * pattern[i * 2 + 1] * scaler;
    y = pos_y + rot_mat[2] * pattern[i * 2] * scaler + rot_mat[3] * pattern[i * 2 + 1] * scaler;

    if (i == 0)
    {
      moveAbs(x, y);       //move to this coordinate
      delayMicroseconds(30); //wait for mirrors to physically arrive
      if (withLaserOn)
      {
        laserOn(); //after first move we turn laser on
      }
    }
    else
    {
      moveAbs(x, y);
    }
  }
  // SerialUSB.println();

  delayMicroseconds(30);
  laserOff(); //lastly always turn laser off
}

void projectNumber(int pos_x, int pos_y, int number, int scaler, bool withLaserOn)
{
  if (number < 10)
  {
    project(pos_x, pos_y, number, scaler, withLaserOn);
  }
  else
  {
    //calculate offset
    int x_offset = rot_mat[1] * ((scaler / 2) + 10);
    int y_offset = rot_mat[3] * ((scaler / 2) + 10);

    if (number >= 20)
    {
      x_offset = rot_mat[1] * ((scaler / 2) + scaler);
      y_offset = rot_mat[3] * ((scaler / 2) + scaler);
    }

    projectNumber(pos_x + x_offset, pos_y + y_offset, (int)(number / 10), scaler, withLaserOn);
    projectNumber(pos_x - x_offset, pos_y - y_offset, number % 10, scaler, withLaserOn);
  }
}

void project(int pos_x, int pos_y, int number, int scaler, bool withLaserOn)
{
  int x, y;
  int *pattern;
  int pattern_n;

  switch (number)
  {
  case 0:
    pattern = pattern_zero;
    pattern_n = zero_n;
    break;
  case 255: //we cannot send a zero, so 255 is a zero
    pattern = pattern_zero;
    pattern_n = zero_n;
    break;
  case 1:
    pattern = pattern_one;
    pattern_n = one_n;
    break;
  case 2:
    pattern = pattern_two;
    pattern_n = two_n;
    break;
  case 3:
    pattern = pattern_three;
    pattern_n = three_n;
    break;
  case 4:
    pattern = pattern_four;
    pattern_n = four_n;
    break;
  case 5:
    pattern = pattern_five;
    pattern_n = five_n;
    break;
  case 6:
    pattern = pattern_six;
    pattern_n = six_n;
    break;
  case 7:
    pattern = pattern_seven;
    pattern_n = seven_n;
    break;
  case 8:
    pattern = pattern_eight;
    pattern_n = eight_n;
    break;
  case 9:
    pattern = pattern_nine;
    pattern_n = nine_n;
    break;
  default:
    return;
  }

  if (PositionIsOutOfBounds(pos_x, pos_y, scaler))
    return;

  for (int i = 0; i < pattern_n; i++)
  {
    x = pos_x + rot_mat[0] * pattern[i * 2] * scaler + rot_mat[1] * pattern[i * 2 + 1] * scaler;
    y = pos_y + rot_mat[2] * pattern[i * 2] * scaler + rot_mat[3] * pattern[i * 2 + 1] * scaler;

    if (i == 0)
    {
      moveAbs(x, y);       //move to this coordinate
      delayMicroseconds(30); //wait for mirrors to physically arrive
      if (withLaserOn)
      {
        laserOn(); //after first move we turn laser on
      }
    }
    else
    {
      moveAbs(x, y);
    }
  }

  delayMicroseconds(30);
  laserOff(); //lastly always turn laser off
}

/*
 * If a number might fall of screen, and therefore deforms (as moveAbs saturates if it reaches a limit),
 * function returns true.
 * A simple check, that only checks if the requested postion plus and minus the scaler is not more or 
 * less than the MAX_xx or MIN_xx allowed value.
 * This is technically not correct for for example the number one, as it is much less wide as the other
 * numbers, but to keep the check simple, this fact is ignored, and the number 1 considered as wide as
 * any number.
 */
bool PositionIsOutOfBounds(int x, int y, int scaler)
{
  if (x + scaler > MAX_X)
    return true;
  if (x - scaler < MIN_X)
    return true;
  if (y + scaler > MAX_Y)
    return true;
  if (y - scaler < MIN_Y)
    return true;

  return false;
}

void projectSquare(int pos_x, int pos_y, int width, int height)
{
  moveAbs(pos_x, pos_y); //move to start

  laserOn();

  moveAbs(pos_x + width, pos_y);
  moveAbs(pos_x + width, pos_y + height);
  moveAbs(pos_x, pos_y + height);
  moveAbs(pos_x, pos_y);

  laserOff();
}

void drawTriangle(int x, int y, int bigness)
{
  moveAbs(x + 0, y + 0);
  laserOn();
  moveAbs(x + 0, y + bigness);
  moveAbs(x + (bigness / 2), y + (bigness / 2));
  moveAbs(x + 0, y + 0);
  laserOff();
}

void projectDot(int pos_x, int pos_y)
{
  moveAbs(pos_x, pos_y); //move to start
  //project laser at quarter duty cycle, so it is not too bright
  delayMicroseconds(3);
  laserOn();
  delayMicroseconds(1);
  laserOff();
}

void setRotation(int8_t rotation)
{
  if (rotation >= 0 & rotation < 4)
  {
    rotationIntern = rotation;

    switch (rotation)
    {
    case 0:
      rot_mat[0] = 1;
      rot_mat[1] = 0;
      rot_mat[2] = 0;
      rot_mat[3] = 1;
      break;
    case 1:
      rot_mat[0] = 0;
      rot_mat[1] = -1;
      rot_mat[2] = 1;
      rot_mat[3] = 0;
      break;
    case 2:
      rot_mat[0] = -1;
      rot_mat[1] = 0;
      rot_mat[2] = 0;
      rot_mat[3] = -1;
      break;
    case 3:
      rot_mat[0] = 0;
      rot_mat[1] = 1;
      rot_mat[2] = -1;
      rot_mat[3] = 0;
      break;
    }
  }
  return;
}

void setMirror(int8_t mirror)
{

  if (mirror >= 0 & mirror < 2)
  {
    switch (mirror)
    {
    case 0:
      mirrorIntern = 1;
      break;
    case 1:
      mirrorIntern = -1;
      break;
    }
  }
  return;
}

void setCharspace(int16_t charspace)
{
  charspaceIntern = charspace;
  return;
}

void projectCalibrationDot(int pos_x, int pos_y)
{
  moveAbs(pos_x, pos_y); //move to start
  laserOn();
  delayMicroseconds(5);

  int temp_x; //we create a temporary x location to send the mirrors to, to prevent the safety from turning of the laser
  if (pos_x < 3000)
    temp_x = pos_x + 60;
  else
    temp_x = pos_x - 60;

  laserOff();
  moveAbs(temp_x, pos_y);
}

