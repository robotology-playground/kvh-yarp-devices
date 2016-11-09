# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile a static library based on https://github.com/TRECVT/ros_kvh1750
# on commit 5db4ed7be8de2933f96d14eddb585dfa14af1ba1
# We compile it as a static library for avoid having to install it, 
# but this can be easily moved to a shared library and installed 

add_library(vt_kvh17xx STATIC
            src/iomodule.cpp
            src/types.cpp
            src/imu.cpp
            src/tov_file.cpp
            src/bit_message.cpp)
            
target_include_directories(vt_kvh17xx PUBLIC include)

