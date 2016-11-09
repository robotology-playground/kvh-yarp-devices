# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)
yarp_prepare_device(kvh17xx TYPE yarp::dev::kvh17xx
                            INCLUDE kvh17xx.h
                            WRAPPER inertial)

yarp_add_plugin(kvh17xx kvh17xx.cpp kvh17xx.h)

include_directories(${YARP_INCLUDE_DIRS})
target_link_libraries(kvh17xx ${YARP_LIBRARIES} vt_kvh17xx)  
message(STATUS "-----> YARP_PLUGINS_INSTALL_DIR: ${YARP_PLUGINS_INSTALL_DIR}")
yarp_install(TARGETS kvh17xx
             COMPONENT runtime
             LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})
yarp_install(FILES kvh17xx.ini  DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})


