# Copyright: (C) 2016 iCub Facility - Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro <silvio.traversaro@iit.it>
# CopyPolicy: Released under the terms of the GNU LGPL v2.0+

cmake_minimum_required(VERSION 2.8.12)
project(kvh-yarp-devices)

# Enable explicitly C++11 support 
# (only supported in CMake >= 3.1, fallback to manually specify the flags otherwise)
set(CMAKE_CXX_STANDARD 11)
if(CMAKE_VERSION VERSION_LESS 3.1)
    add_definitions(-std=c++11)
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR
   ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(FATAL_ERROR "kvh drivers support do not support macOS or Windows systems at the moment. ")
endif()

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
