/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "kvh17xx.h"

#include <cassert>
#include <cmath>

#include <yarp/os/LockGuard.h>

#include <kvh1750/iomodule.h>
#include <kvh1750/tov_file.h>
#include <kvh1750/imu.h>


#define NUMBER_OF_CHANNELS_IN_YARP_IMU 3
#define KVH_RAD2DEG (180.0/M_PI)

yarp::dev::kvh17xx::kvh17xx(): m_rawAccelerometerOutputInMForSsquare(3),
                               m_rawGyroOutputInDegForS(3),
                               m_sensorReading(NUMBER_OF_CHANNELS_IN_YARP_IMU),
                               m_status(false),
                               m_isOpened(false),
                               m_isClosing(false)
{
}

yarp::dev::kvh17xx::~kvh17xx()
{
}

bool yarp::dev::kvh17xx::open(yarp::os::Searchable &config)
{
    if( m_isOpened )
    {
        yError("kvh17xx: tryng to open an already existing device, close it before");
        return false;
    }

    yarp::os::LockGuard guard(m_externalBuffersMutex);

    uint32_t baud = 921600;

    uint32_t wait = 100;

    std::string ttyFileName = "/dev/ttyUSB0";

    std::shared_ptr<kvh::IOModule> mod(new kvh::TOVFile(ttyFileName, baud, wait));

    m_imu.reset(new kvh::IMU1750(mod));

    // Make sure that we are reading angular velocity
    bool use_delta_angles = false;
    if(!m_imu->set_angle_units(use_delta_angles))
    {
        yError("kvh17xx: could not set imu to send angular velocity, exiting.");
        m_imu.reset(nullptr);
        return false;
    }

    bool is_using_delta_angles = true;
    bool query_successfull = m_imu->query_angle_units(is_using_delta_angles);

    if( !query_successfull || is_using_delta_angles )
    {
        yError("kvh17xx: could not set imu to send angular velocity, exiting.");
        m_imu.reset(nullptr);
        return false;
    }

    // Check the rate
    int rate_in_hz = -1;
    query_successfull = m_imu->query_data_rate(rate_in_hz);
    if( !query_successfull || is_using_delta_angles )
    {
        yError("kvh17xx: could not get rate from IMU, exiting.");
        m_imu.reset(nullptr);
        return false;
    }
    yInfo("kvh17xx: the data rate is %d hz",rate_in_hz);


    // Create thread that continuously run fillBuffersFromSensor until close is called
    m_isClosing = false;
    m_readingThread = std::thread(&kvh17xx::readingLoop, this);

    if( !(m_readingThread.joinable()) )
    {
        yError("kvh17xx: could not launch reading thread, exiting.");
        m_imu.reset(nullptr);
        return false;
    }

    m_isOpened = true;

    return true;
}

bool yarp::dev::kvh17xx::close()
{
    if( !m_isOpened )
    {
        yError("kvh17xx: could not close device that was never opened.");
        return false;
    }

    // Set isClosing to true
    m_isClosing = true;

    // Waiting for the reading thread to finish
    m_readingThread.join();

    m_isOpened = false;
    
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

void yarp::dev::kvh17xx::readingLoop()
{
    while( !m_isClosing )
    {
        kvh::Message msg;

        switch(m_imu->read(msg))
        {
        case kvh::IMU1750::VALID:
            if( msg.valid() )
            {
                this->fillBuffersFromSensorMsg(msg);
                m_status = true;
            }
            else
            {
                yError("kvh17xx: non valid data from KVH, ignoring.");
                m_status = false;
            }
            break;
        case kvh::IMU1750::BAD_READ:
        case kvh::IMU1750::BAD_CRC:
            yError("kvh17xx: bad data from KVH, ignoring.");
            m_status = false;
            break;
        case kvh::IMU1750::FATAL_ERROR:
            yError("kvh17xx: lost connection to IMU.");
            m_status = false;
            break;
        case kvh::IMU1750::OVER_TEMP:
            yError("kvh17xx: IMU is overheating!");
            m_status = false;
            break;
        case kvh::IMU1750::PARTIAL_READ:
        default:
            break;
        }
    }
}

void yarp::dev::kvh17xx::fillBuffersFromSensorMsg(const kvh::Message& msg)
{
    m_rawAccelerometerOutputInMForSsquare[0] = msg.accel_x();
    m_rawAccelerometerOutputInMForSsquare[1] = msg.accel_y();
    m_rawAccelerometerOutputInMForSsquare[2] = msg.accel_z();

    m_rawGyroOutputInDegForS[0] = KVH_RAD2DEG*msg.gyro_x();
    m_rawGyroOutputInDegForS[1] = KVH_RAD2DEG*msg.gyro_y();
    m_rawGyroOutputInDegForS[2] = KVH_RAD2DEG*msg.gyro_z();

    yarp::os::LockGuard guard(m_externalBuffersMutex);

    // See https://github.com/robotology-playground/kvh-yarp-devices/issues/2
    // For now we just set the orientation and magnenotometer part to zero

    // RPY orientation
    m_sensorReading[0] = 0.0;
    m_sensorReading[1] = 0.0;
    m_sensorReading[2] = 0.0;

    // Accelerometers (in m/s^2)
    m_sensorReading[3] = m_rawAccelerometerOutputInMForSsquare[0];
    m_sensorReading[4] = m_rawAccelerometerOutputInMForSsquare[1];
    m_sensorReading[5] = m_rawAccelerometerOutputInMForSsquare[2];

    // Gyro (in deg/sec)
    m_sensorReading[6] = m_rawGyroOutputInDegForS[0];
    m_sensorReading[7] = m_rawGyroOutputInDegForS[1];
    m_sensorReading[8] = m_rawGyroOutputInDegForS[2];

    // Magnenotometer
    m_sensorReading[9]  = 0.0;
    m_sensorReading[10] = 0.0;
    m_sensorReading[11] = 0.0;
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


