#define DEV_ID "LSC"
#define VERSION "3.3.0"

#include "Projection.h"

#define CIRCLETAYLSER 0
#define SQUARE 1
#define CIRCLELUT 2
//#define DEBUG

#define DATAPOINTS 100
#define NOPROJECTIONS 100
 
char flag;
char data[2];
int i;
int k;
int circleX[DATAPOINTS] = {2100,2100,2099,2098,2097,2095,2093,2090,2087,2084,2081,2077,2072,2068,2063,2058,2053,2047,2042,2036,2030,2024,2017,2011,2005,1998,1992,1986,1980,1973,1967,1961,1956,1950,1945,1939,1935,1930,1925,1921,1918,1914,1911,1908,1906,1904,1902,1901,1900,1900,1900,1900,1901,1902,1904,1906,1908,1911,1914,1918,1921,1925,1930,1935,1939,1945,1950,1956,1961,1967,1973,1980,1986,1992,1998,2005,2011,2017,2024,2030,2036,2042,2047,2053,2058,2063,2068,2072,2077,2081,2084,2087,2090,2093,2095,2097,2098,2099,2100,2100};

int circleY[DATAPOINTS] = {2000,2006,2013,2019,2025,2031,2037,2043,2049,2054,2059,2064,2069,2073,2078,2081,2085,2088,2091,2093,2095,2097,2098,2099,2100,2100,2100,2099,2098,2096,2095,2092,2090,2087,2083,2080,2076,2071,2067,2062,2057,2051,2046,2040,2034,2028,2022,2016,2010,2003,1997,1990,1984,1978,1972,1966,1960,1954,1949,1943,1938,1933,1929,1924,1920,1917,1913,1910,1908,1905,1904,1902,1901,1900,1900,1900,1901,1902,1903,1905,1907,1909,1912,1915,1919,1922,1927,1931,1936,1941,1946,1951,1957,1963,1969,1975,1981,1987,1994,2000};

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
  while (SerialUSB.available() < 2) {
    if (flag == 1){
      SerialUSB.println("waiting for radius of cirkel via USB Serial. MAX X = 4000 MAX Y = 4000");
      flag = 0;
    }
  }
  SerialUSB.print("You have send:");
  for (int n=0; n<2; n++){
      data[n] = SerialUSB.read();
      SerialUSB.print(data[n]);
      SerialUSB.print(":");
  }
  SerialUSB.print("\n");
  switch (data[0]) {
    case CIRCLETAYLSER:
      while(i< NOPROJECTIONS*DATAPOINTS){
        int targetX = 10 * data[1] * cos(i*2*3.14 / 100) + 2200;
        int targetY = 10 * data[1] * sin(i*2*3.14 / 100) + 2000;
        moveAbs(targetX, targetY);
        i++;
      }
      i = 0;
      break;
     case SQUARE:
      break;

     case CIRCLELUT:
       while(k < NOPROJECTIONS){
        while(i < DATAPOINTS){
          int targetX = circleX[i];
          int targetY = circleY[i];
          moveAbs(targetX, targetY);
          i++;
        }
        i = 0;
        k++;
       }
       k = 0;
       i = 0;
      break;
  }
  flag = 1;
}
