#define DEV_ID "LSC"
#define VERSION "3.3.0"

#include "Projection.h"

#define LASER_PIN LASERL

//#define DEBUG

struct projection
{
	uint8_t length;
	char chars[MAX_NUMBERS_OF_CHARS_PER_PROJECTION];
	float x;
	float y;
	uint8_t scl;
	int16_t vx;
	int16_t vy;
	int32_t x_counter;
	int32_t y_counter;
};

// #define DOT 32766
// #define CALIBRATION_DOT 32767

#define MAX_NUMBER_OF_PROJECTIONS 10
struct projection projections[MAX_NUMBER_OF_PROJECTIONS];

uint8_t count = 0;

#define BUFFER_LEN 255
byte buf[BUFFER_LEN];
uint8_t fin_msg_length;

void getSerialData();
void interpretMessage();
int processSerial();

void handleHeartBeat();

void setup()
{

	SerialUSB.begin(115200);

	while (!Serial)
	{
		;
	}
	SerialUSB.setTimeout(10);
	SerialUSB.println("Booting...");

	initProjection();

	pinMode(LASER_PIN, OUTPUT);
}

void loop()
{
	if (processSerial())
	{
		interpretMessage();
	}

	if (count == 0)
	{								  //no projections in memory
		digitalWrite(LASER_PIN, LOW); //laserOff
	}
	else
	{
    updateMovingProjections();
    for (int i = 0; i < count; i++)
		{
			if (projections[i].chars[0] == '_') // Check for the escape character
			{
				if (projections[i].chars[1] == 'C') // Something with calibration
				{
					if (projections[i].chars[2] == '.') // Calibration dot
					{
						projectCalibrationDot(projections[i].x, projections[i].y);
						updateMovingProjections();
					}
				}
				else if (projections[i].chars[1] == '.') // Project a normal dot
				{
					projectDot(projections[i].x, projections[i].y);
				}
				else if (projections[i].chars[1] == '[' && projections[i].chars[2] == ']') // Project a SQUARE
				{
					projectSquare(projections[i].x, projections[i].y, projections[i].scl * 10, projections[i].scl * 10);
				}
			}
			else // We are projection something 'normal'
			{
				if (projections[i].length <= MAX_NUMBERS_OF_CHARS_PER_PROJECTION)
				{
					projectString(projections[i].x, projections[i].y, projections[i].length, projections[i].chars, projections[i].scl, true);
				}
			}
		}
	}
}

void updateMovingProjections()
{
  for (int index = 0; index < count; index++)
  {
	  projections[index].x += ((float)(millis() - projections[index].x_counter) / (float)1000) * (float)projections[index].vx;
	  projections[index].y += ((float)(millis() - projections[index].y_counter) / (float)1000) * (float)projections[index].vy;
	  projections[index].x_counter = millis();
	  projections[index].y_counter = millis();
  }
}

enum State
{
	START1,
	START2,
	MSGLEN,
	CONTENT,
	CRC
};

int processSerial()
{
	static State currentState = START1;
	static uint8_t msgLen;
	static uint8_t read_counter;
	static uint8_t CRC_sum;
	while (SerialUSB.available())
	{

		byte c = SerialUSB.read();
#ifdef DEBUG
		SerialUSB.print(c, HEX);
		SerialUSB.print(' '); //echo char
#endif
		switch (currentState)
		{
		case START1:
			if (c == 0xFF)
				currentState = START2;
			else
				currentState = START1;
			break;
		case START2:
			if (c == 0xFF)
			{
				currentState = MSGLEN;
				CRC_sum = 0; //reset CRC_sum
			}
			else
				currentState = START1; //wrong start byte detected, we start over
			break;
		case MSGLEN:
#ifdef DEBUG
			SerialUSB.println();
			SerialUSB.print("Message len: ");
			SerialUSB.println(c);
#endif
			if (c == 0xFF)
			{ //version request
				sendVersion();
				currentState = START1; //begin all over
				break;
			}

			//HeartBeat
			else if (c == 254)
			{
				handleHeartBeat();
				
				currentState = START1; //begin all over
				break;
			}
			else
			{
				count = 0;
			}

			CRC_sum += c;	 //add message length to the CRC
			msgLen = c;		  //save the length
			read_counter = 0; // reset read_counter
			currentState = CONTENT;
			break;
		case CONTENT:
			if (read_counter >= BUFFER_LEN)
			{
				currentState = START1; //if we exceed our buffer length, we start over again
				break;
			}
			buf[read_counter++] = c; //read current byte into buffer
			CRC_sum += c;			 //add to checksum
			if (read_counter == msgLen)
			{						//if we have reached the number of char as indicated in the message header
				currentState = CRC; //continue to next state
			}
			break;
		case CRC:
			// the sum of the whole message (including message length)
			// + the CRC included as last char in the message, should
			// add up to 0
			CRC_sum += c;
			if (CRC_sum != 0)
			{
				//CRC_sum is incorrect, discard message and wait for the next start of the message seq.
				currentState = START1;
#ifdef DEBUG
				SerialUSB.print(CRC_sum);
				SerialUSB.println("<- CRC failed");
#endif
			}
			else
			{ //we have received a valid message
				fin_msg_length = msgLen;
#ifdef DEBUG
				SerialUSB.println("MSG received");
#endif
				currentState = START1; //we are done, start over again
				return 1;
			}
			break;
		}
	}
	return 0;
}

void interpretMessage()
{
	for (int i = 0; i < MAX_NUMBER_OF_PROJECTIONS; i++)
	{
		// Reset projection characters
		for (int j = 0; j < MAX_NUMBERS_OF_CHARS_PER_PROJECTION; j++)
		{
			projections[i].chars[j] == 0x00;
		}
	}

	uint8_t i = 0; // current index of read buffer
	uint8_t p = 0; //number of projections
	while (i < fin_msg_length & p < MAX_NUMBER_OF_PROJECTIONS)
	{
		uint8_t j = 0;

		// check for SETROTATION message
		if ((buf[i + 0] << 8 | buf[i + 1]) == 4096)
		{
			#ifdef DEBUG
			SerialUSB.println("Setrotation received!");
			#endif
			setRotation(buf[i + 2] << 8 | buf[i + 3]);
			return; //discard rest of message
		}
		// Check for SETMIRROR message
		if ((buf[i + 0] << 8 | buf[i + 1]) == 4097)
		{
			#ifdef DEBUG
			SerialUSB.println("SetMirror received!");
			#endif
			setMirror(buf[i + 2] << 8 | buf[i + 3]);
			return;
		}
    // Check for SETCHARSPACE message
    if ((buf[i + 0] << 8 | buf[i + 1]) == 4098)
    {
      #ifdef DEBUG
      SerialUSB.println("CharSpace received!");
      #endif
      setCharspace(buf[i + 2] << 8 | buf[i + 3]);
      return;
    }

		// cast the projection values
		uint8_t projectionLength = buf[i + j++];

		projections[p].length = projectionLength;

		for (uint8_t k = 0; k < projectionLength; k++)
		{
			projections[p].chars[k] = buf[i + j++];
		}
		// projections[p].num = (buf[i + j++] << 8 | buf[i + 1]);
		projections[p].x = (float)((int16_t)(buf[i + j++] << 8 | buf[i + j++]));
		projections[p].y = (float)((int16_t)(buf[i + j++] << 8 | buf[i + j++]));
		projections[p].scl = buf[i + j++];
		projections[p].vx = (buf[i + j++] << 8 | buf[i + j++]);
		projections[p].vy = (buf[i + j++] << 8 | buf[i + j++]);

#ifdef DEBUG
		printProjection(p);
#endif

		if (projectionLength == 0 & projections[p].x == 0 & projections[p].y == 0 & projections[p].scl == 0 & projections[p].vx == 0 & projections[p].vy == 0)
		{ //we have received a clear all command
			p = 0;
#ifdef DEBUG
			SerialUSB.println("Clear projections!");
#endif
			break;
		}

		//reset the counters
		projections[p].x_counter = millis();
		projections[p].y_counter = millis();

		p++;	 // next projection
		i += 10 + projectionLength; //next part of message
	}
	count = p;
}

int get_sign(int16_t number)
{
	int sign = 0;
	if (number > 0)GALVO_MODERN_H
		sign = 1;
	else if (number < 0)
		sign = -1;
	return sign;
}

void printProjection(uint8_t index)
{
	SerialUSB.print("Projection: (length: ");
	SerialUSB.print(projections[index].length);
	SerialUSB.print(") ");
	SerialUSB.println(projections[index].chars);
	SerialUSB.print("Location: ");
	SerialUSB.print(projections[index].x);
	SerialUSB.print("; ");
	SerialUSB.println(projections[index].y);
	SerialUSB.print("Scaler: ");
	SerialUSB.println(projections[index].scl);
	SerialUSB.print("Velocity: ");
	SerialUSB.print(projections[index].vx);
	SerialUSB.print("; ");
	SerialUSB.println(projections[index].vy);
}

void sendVersion()
{
	SerialUSB.print("v");
	SerialUSB.print(DEV_ID);
	SerialUSB.print("-");
	SerialUSB.println(VERSION);
}

void handleHeartBeat()
{
	SerialUSB.write(0x01);
}
