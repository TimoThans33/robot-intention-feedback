# robot-intention-feedback
This repository includes the code used for the bachelor thesis 'robot intention feedback' in the Delft Centre for Systems and Control.

## Introduction
Firmware for the laser/galvo controller.
## Output
![Plot of TU Delft logo](Figures/TUDplot.png)
![Projection of TU Delft logo](Figures/TUD.png)
## Getting Started
1. Download / clone code 
2. upload using standard Arduino IDE.
3. Connect with USB cable
4. Start the USBserial python example

## Design Projections
In the Projections.py file we have designed a Circle and HelloWorld projection. In EdgeTrace.py the design process is automated. Simply download an image into this repository and change the name accordingly in the file. We are using the contour detection from open CV. Make sure you pick an image with not too much detail, as the projector can only handle around 150 datapoints. If you need to transform the image you can adjust the parameter THETA accordingly in the script.

When you have designed a projection copy the x-y data into static memory of the microcontroller by simply spawning a C array in Laser_Code_ARM.ino.

## Dependecies
For the USB serial python example you need the pyserial library.
```
pip install pyserial
```
For the EdgeTrace you need the python wrapper for Open CV
```
pip install cv2
```