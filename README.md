# Warning: Work In Progress
Note that the current software is still in development, and is not ready to be used. 

# kvh-yarp-devices
YARP Device Drivers for various  KVH ( http://www.kvh.com/ ) sensors.

## Rationale
The repo contains the following YARP devices : 
* [`kvh17xx`](kvh17xx) : YARP Driver for a KVH 17xx class of IMUS ( http://www.kvh.com/Military-and-Government/Gyros-and-Inertial-Systems-and-Compasses/Gyros-and-IMUs-and-INS/IMUs.aspx ). 

## Installation

##### Dependencies
- [YARP](https://github.com/robotology/yarp)
- [serial](https://github.com/wjwwood/serial)

##### Step-by-step installation on Ubuntu 16.04 
* Install [serial](https://github.com/wjwwood/serial) on your machine. It can be compiled as a usual cmake project (
with `cmake` and `make` and `make install`) but it requires the catkin build system, that can be easily installed in Ubuntu
16.04 with a `sudo apt install catkin`. 
* Install YARP on your platform, following the instructions on [YARP documentation](http://www.yarp.it/install.html). 
* Compile the code in this repository using [CMake](https://cmake.org/) and your preferred compiler. See [YARP documentation on how to compile a CMake project](http://www.yarp.it/using_cmake.html).
* Install the compiled devices. You can specify the installation directory using the [`CMAKE_INSTALL_PREFIX`](https://cmake.org/cmake/help/v3.0/variable/CMAKE_INSTALL_PREFIX.html) CMake option.
* Make sure that the directory `${CMAKE_INSTALL_PREFIX}/share/yarp` (where `${CMAKE_INSTALL_PREFIX}` needs to be substituted to the directory that you choose as the `CMAKE_INSTALL_PREFIX`) is in your `YARP_DATA_DIRS` enviromental variable (for more on the `YARP_DATA_DIRS` env variable, see [YARP documentation on data directories](http://www.yarp.it/yarp_data_dirs.html) ). 
* Once you do that, you should be able to find the devices compiled by this repo (for example the `kvh17xx`) using the command `yarp plugin kvh17xx`, which should have an output similar to:
~~~
Yes, this is a YARP plugin
  * library:        CMAKE_INSTALL_PREFIX/lib/yarp/kvh17xx.so
  * system version: 5
  * class name:     yarp::dev::kvh17xx
  * base class:     yarp::dev::DeviceDriver
~~~
If this is not the case, there could be some problems in finding the plugin. In that case, just move yourself to the `${CMAKE_INSTALL_PREFIX}/share/yarp` directory and launch the device from there.

## Device use 
At the moment the driver has only have been tested with a KVH 1775 IMU. Streaming of gyroscope and accelerometer raw data seem to work fine, while magnetomenter output and rotation estimation ( https://github.com/robotology-playground/kvh-yarp-devices/issues/2 ) are currently not supported. 
Furthermore only the streaming messages have been checked to be equivalent between 1750 (the model for which the C++ driver was originally written) and the 1775. For this reason we experience some strange behaviors when query the device for configuration option (see https://github.com/robotology-playground/kvh-yarp-devices/blob/master/kvh17xx/kvh17xx.cpp#L66), however this seems not to affect the streaming of data. 

### Connect the KVH 1775 to the computer 
We connected the KVH 1775 to a PC using the FOG/IMU Developer's Kit. 
Please follow precisly the instructions provided in the Quick Start Guide.
In particular **do not connect** the power plug to the power grid if the Y-Cable Power Switch is set to the On (|) position, put 
switch it to Off (o) and switch it back to ON only when the the plug is connected with the power grid. 

To launch the `kvh17xx` device with a KVH 1775 IMU, you can use the example configuration file [`kvh17xx/yri-kvh1775.xml`](`kvh17xx/yri-kvh1775.xml`) for the `yarprobotinterface`.
To do so, launch the yarpserver, then on a terminal launch the device:
~~~
yarprobotinterface --config yri-kv1775.xml
~~~

This should open a YARP port `/seesaw` , that you can read from the command line for example using the `yarp read` command:
~~~
yarp read ... /seesaw 
~~~

Alternativly, we can also launch the device using the old `yarpdev` command, using the example configuration file [`kvh17xx/yd-kvh1775.ini`](kvh17xx/yd-kvh1775.ini): 
~~~
yarpdev --from yd-kv1775.ini
~~~

