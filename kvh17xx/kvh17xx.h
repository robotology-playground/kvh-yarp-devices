/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_KVH17XX_H
#define YARP_KVH17XX_H

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/GenericSensorInterfaces.h>
#include <yarp/dev/PreciselyTimed.h>

#include <yarp/sig/Vector.h>

#include <atomic>
#include <memory>
#include <thread>

namespace kvh
{
    class IMU1750;
    class Message;
}

namespace yarp {
namespace dev {

class kvh17xx : public yarp::dev::DeviceDriver,
                public yarp::dev::IGenericSensor,
                public yarp::dev::IPreciselyTimed
{
private:
    // Prevent copy 
    kvh17xx(const kvh17xx & other);
    kvh17xx & operator=(const kvh17xx & other);
    
    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_rawAccelerometerOutputInMForSsquare;
    yarp::sig::Vector m_rawGyroOutputInDegForS;

    // 12-size buffers, compatible with structure documented in
    // http://www.yarp.it/classyarp_1_1dev_1_1ServerInertial.html
    // note that some issue are present on this definitions:
    // https://github.com/robotology/yarp/issues/802
    // Use a mutex to in methods accessing external buffers
    yarp::os::Mutex m_externalBuffersMutex;
    yarp::sig::Vector m_sensorReading;
    yarp::os::Stamp m_timestamp;
    
    // Status of the sensor (true if data is available, false otherwise
    bool m_status;

    // Check if the driver is already opened
    bool m_isOpened;

    // Device used to read data from the KVH serial interface
    std::unique_ptr<kvh::IMU1750> m_imu;

    // Thread that reads from the serial port, always executing
    // fillBuffersFromSensor until close is called
    std::thread m_readingThread;

    // Function of the internal thread of the driver,
    // that blocks on serial read (TODO(traversaro): check if this is true)
    void readingLoop();
    void fillBuffersFromSensorMsg(const kvh::Message & msg);

    // Atomic flag, set to true by close
    std::atomic_bool m_isClosing;
    
public:
    kvh17xx();
    virtual ~kvh17xx();

    // DeviceDriver interface 
    bool open(yarp::os::Searchable &config);
    bool close();

    // IGenericSensor interface
    virtual bool read(yarp::sig::Vector &out);
    virtual bool getChannels(int *nc);
    virtual bool calibrate(int ch, double v);
    
    // IPreciselyTimed interface
    virtual yarp::os::Stamp getLastInputStamp();
};

}
}

#endif

