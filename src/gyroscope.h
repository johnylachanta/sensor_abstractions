/// \file   gyroscope.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the gyroscope-sensor base class.

#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <sensor.h>
#include <types.h>

namespace Sensor{

struct RawGyroscopeData{
    int nP_Data;
    int nR_Data;
    int nY_Data;
};

class Gyroscope : public SensorBase<DegreePerSecond, RawGyroscopeData> {
public:
    virtual bool ReadSensorDataOnce(RawGyroscopeData & OutData)=0;
    virtual DegreePerSecond ConvertToSIUnit(RawGyroscopeData Data)=0;
};

}
#endif
