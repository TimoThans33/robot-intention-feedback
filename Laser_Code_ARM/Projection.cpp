#include "Arduino.h"
#include "SPI.h"
#include "Projection.h"
#include "wiring_private.h" // pinPeripheral() function
/*
#include "galvo_modern.h"
*/
int currentX;
int currentY;

int rotationIntern = 1;
int mirrorIntern = 1;

int8_t rot_mat[4] = {1, 0, 0, 1};

void initSPI();

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
      //SerialUSB.println("reached y");
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
