/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "kvh17xx.h"

#include <cassert>

#include <yarp/os/LockGuard.h>

#define NUMBER_OF_CHANNELS_IN_YARP_IMU 3

yarp::dev::kvh17xx::kvh17xx(): m_rpyAnglesInDeg(3),
                               m_rawAccelerometerOutputInMForSsquare(3),
                               m_rawGyroOutputInDegForS(3),
                               m_rawMagnetometerOutputInGauss(3),
                               m_sensorReading(NUMBER_OF_CHANNELS_IN_YARP_IMU),
                               m_status(false)
{

}

yarp::dev::kvh17xx::~kvh17xx()
{
}

bool yarp::dev::kvh17xx::open(yarp::os::Searchable &config)
{
    yarp::os::LockGuard guard(m_externalBuffersMutex);

    return true;
}

bool yarp::dev::kvh17xx::close()
{
    yarp::os::LockGuard guard(m_externalBuffersMutex);

    isClosing.test_and_set();
    
    return true;
}

yarp::dev::kvh17xx::kvh17xx(const yarp::dev::kvh17xx& /*other*/)
{
    // Copy is disabled 
    assert(false);
}

yarp::dev::kvh17xx& yarp::dev::kvh17xx::operator=(const yarp::dev::kvh17xx& other)
{
    assert(false);
}

bool yarp::dev::kvh17xx::read(yarp::sig::Vector &out)
{
    yarp::os::LockGuard guard(m_externalBuffersMutex);
    
    out = m_sensorReading;
    
    return m_status;
}

void yarp::dev::kvh17xx::fillBuffersFromSensor()
{

    return;
}

bool yarp::dev::kvh17xx::calibrate(int ch, double v)
{
    return false;
}

bool yarp::dev::kvh17xx::getChannels(int* nc)
{
    if( nc )
    {
        *nc = NUMBER_OF_CHANNELS_IN_YARP_IMU;
        return true;
    }
    else
    {
        return false;
    }
}

yarp::os::Stamp yarp::dev::kvh17xx::getLastInputStamp()
{
    yarp::os::LockGuard guard(m_externalBuffersMutex);

    return m_timestamp;
}


