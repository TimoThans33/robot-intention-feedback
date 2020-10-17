# robot-intention-feedback
This repository includes the code usd for the bachelor thesis 'robot intention feedback' in the Delft Centre for Systems and Control.

## Introduction
X11 User Interface made with QT. The idea is to run the UI code on a server or Jetson TX2. The UI is projected by the DLP Lightcrafter 2000 by making use of the X11 protocol.

## Dependecies
On server (host computer)
```
pip install PyQt5
```
```
sudo apt=-get install pyqt5-dev-tools
```
On the client a.k.a BeagleBone Black (assuming we are running Debian without desktop interface) we have to install a bunch of dependecies.
```
sudo apt install xorg
```
According to Qt for Debian-base GNU/Linux systems we need to install a bunch of drivers. Some might already be have been installed some not. Just run the shell script I included.
```
./QtInstallDependecies.sh
```
## Getting started
To run the Qt-examples we need to start an X11 server on the BeagleBone Black.
```
sudo startx
```
Then you can SSH into the server / host computer passing -X
```
ssh -X username@192.168.7.1
```
```
python3 robot-intention-feedback/Qt-examples/example-window.py
```
This should get you started.

## OpenGL & CUDA support
OpenGL and CUDA are API's that make use of the graphics card on the server / host computer. The installation instructions from NVIDIA for Linux Ubuntu 20.04 x86_64
```
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/cuda-ubuntu2004.pin
```
```
sudo mv cuda-ubuntu2004.pin /etc/apt/preferences.d/cuda-repository-pin-600
```
```
wget https://developer.download.nvidia.com/compute/cuda/11.1.0/local_installers/cuda-repo-ubuntu2004-11-1-local_11.1.0-455.23.05-1_amd64.deb
```
```
sudo dpkg -i cuda-repo-ubuntu2004-11-1-local_11.1.0-455.23.05-1_amd64.deb
```
```
sudo apt-key add /var/cuda-repo-ubuntu2004-11-1-local/7fa2af80.pub
```
```
sudo apt-get update
```
```
sudo apt-get -y install cuda
```