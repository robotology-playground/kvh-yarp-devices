# Copyright: (C) 2016 iCub Facility - Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro <silvio.traversaro@iit.it>
# CopyPolicy: Released under the terms of the GNU LGPL v2.0+

cmake_minimum_required(VERSION 2.8.9)
project(kvh-yarp-devices)

# Create YARP-indipendent library, based on https://github.com/TRECVT/ros_kvh1750
add_subdirectory(vt_kvh17xx)

# Find YARP and include the CMake code to compile plugins
find_package(YARP REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
include(YarpPlugin)
include(YarpInstallationHelpers)
set(YARP_FORCE_DYNAMIC_PLUGINS TRUE)
yarp_configure_external_installation(yarp WITH_PLUGINS)

add_subdirectory(kvh17xx)
