# Introduction
Firmware for the laser/galvo controller.

# Getting Started
1. Download / clone code 
2. upload using standard Arduino IDE.

# Protocol
The laser controller is accesses using serial communication over USB. Each message is encoded with a simple encoding, but that makes it difficult to use a normal serial monitor to communicate with the board.

Best is to use the HWChecker tool included the IFO_SW project to send commands.

The protocol is as follows:

0xFF 0xFF msg_len byte0 byte1 byte2 ... byteN checksum

It starts with twice a 0xFF char. Then the length of the message that follows. Only the payload bytes are counted (not the checksum).
The checksum is calculated by subtracting the sum of msg_len and all the payload bytes from 256. This means that when the message arrives,
the receiver only needs to add all the bytes in the message, which should sum up to zero. If not, the message will be ignored.

## Version
It is possible to request the device ID and firmware version by sending a 0xFF instead of msg_len:

| byte_0 | byte_1 | byte_2 |
|:------:|:------:|:------:|
|  0xFF  |  0xFF  |  0xFF  |

The Laser will respond with:

"LSC-1.0.0\r\n" 

LSC indicates it is a LaserController, the 3 numbers after the hyphen indicate its version.

## Heartbeat
For quick polling whether the LaserController is still responding, a heartbeat request is availble:

| byte_0 | byte_1 |	byte_2 |
|:------:|:------:|:------:|
|  0xFF  |  0xFF  |  0xFE  |

The device will respond with a byte value of 1 (0x01).

## Strings up to 4 characters
The send payload consists of a multiple of bytes. The amount of bytes is between 10 and 14. Multiple projections can be added after eachother.
The content of the payload is as followed:
| byte_0 | byte_1 .. byte_4 | byte_5 & byte_6 | byte_7 & byte_8 | byte_9 | byte_10 & byte_11 | byte_12 & byte_13 |
|:------:|:----------------:|:---------------:|:---------------:|:------:|:-----------------:|:-----------------:|
| length |       data       |      x_pos      |      y_pos      | scaler |       x_vel       |       y_vel       |

The length (byte_0) is the amount of characters that is in the next bytes (with a minimum of 1 and a maximum of 4).

The projection however can be the following:
| data  |  projection |
|:-----:|:-----------:|
| ASCII | That string |

What is given in the data field will be projected with some exceptions:
| data |    projection   |
|:----:|:---------------:|
|  _.  |    Normal dot   |
| _C.  | Calibration dot |
| _[]  |      Square     |

x_pos and y_pos are the coordinate of the center of the projection and should be between 1 and 4000.

Scaler is the size of the projection, for the normal ASCII projection and the square, for the square however the scalar is multiplied by 20 internally to make it possible to project a square at full range.

The x_vel and y_vel are the velocity of the projection. By using this every projetction can be given a different velocity. The velocity is given in pixels/second. This means that a value of 4000 will have the projection traverse the complete range in 1 sec, a value of 200 will have it take 50ms.

In order to prevent numbers being 'squashed' against the limits of the projection area, numbers that fall out of bounds are NOT projected. So if you try to project a number of scaler (size) 40 at location 3961, nothing will be projected.

## Set mirror
The projections can also be mirrored by sending a special message:
| int16_0 | int16_1 |
|:-------:|:----:|
|   4097  |  0-1 |

Where the last byte is 0 or 1 (0 not rotating, 1 rotating).

## Set rotation
The rotation of a projection can be set by sending a special message:
| int16_0 | int16_1 |
|:-------:|:----:|
|   4096  |  0-3 |

When the first value needs equals 4096, the second value represents the rotation, each increment rotating the projection by 90 degrees clock-wise.

## Set white space
The amount of whitespace between two character can be set by sending 4098 as the first int16, followed by the whitespace value.
| int16_0 | int16_1 |
|:-------:|:----:|
|   4098  |  0-4000 |

The whitespace is expressed in the percentage of the scaler value. Note that a single character width is equal to two times the scaler, so a whitespace value of 100% means half a character width of whitespace.

# HWChecker
Constructing such a message is made easy by using the HWChecker. This program can be found as a project in the IFO_SW software.

After starting the program and connecting to the right COM-port up to 3 strings can be projected by filling in the boxes.

Also the special things (mirror, rotation, dots and squares) can be send via the hardware checker via the fields given.


# Version history
## 3.1.0
- Increased minor version number to avoid mix up with multiple 3.0.0 versions released.
## 3.0.0
- Rewrote system to handle strings with up to 4 characters.
## 2.0.0
- includes a Heartbeat reply function: the LaserController will respond to each Serial message with a '1'.
## 1.1.1
- increase the distance the laser moves to prevent iGuard kicking in when projecting a calibration dot

## 1.1.0
- increased the number of projectable numbers to 0-99