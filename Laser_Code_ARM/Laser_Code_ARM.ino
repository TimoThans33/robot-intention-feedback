#define DEV_ID "LSC"
#define VERSION "3.3.0"

#include "Projection.h"

//#define DEBUG

char flag;
char data[4];

void setup()
{

	SerialUSB.begin(115200);

	while (!Serial)
	{
		;
	}
	SerialUSB.setTimeout(50);
	SerialUSB.println("Booting...");
	initProjection();

}

void loop()
{
  while (SerialUSB.available() < 4) {
    if (flag == 1){
      SerialUSB.println("waiting for X & Y coordinates via USB Serial. MAX X = 4000 MAX Y = 4000");
      flag = 0;
    }
  }
  SerialUSB.print("You have send:");
  for (int n=0; n<4; n++){
      data[n] = SerialUSB.read();
      SerialUSB.print(data[n]);
      SerialUSB.print(":");
  }
  SerialUSB.print("\n");
  int targetX = data[0];
  int targetY = data[1];
  
  moveAbs(targetX, targetY);
  flag = 1;
}
