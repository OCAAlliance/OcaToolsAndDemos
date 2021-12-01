/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCALiteDanteLvlSensor
 *
 */

#ifndef _OCALiteDanteLvlSensor_h
#define _OCALiteDanteLvlSensor_h

#include <assert.h>

#include <OCC/ControlClasses/Workers/Sensors/OcaLiteLevelSensor.h>

class OCALiteDanteLvlSensor : public OcaLiteLevelSensor
{
public:
    OCALiteDanteLvlSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, 
                        ::OcaDB minRefLevel, ::OcaDB maxRefLevel) :
        OcaLiteLevelSensor(objectNumber, lockable, role, ports, minRefLevel, maxRefLevel)
        , m_FFRole(role)
        , m_Level(minRefLevel)
        , m_bChanged(static_cast< ::OcaBoolean>(false))
    {
    }

    OcaLiteStatus Initialize();
    void Terminate(void);
    OcaLiteStatus updateMeter(OcaDB value);
    OcaLiteStatus updateMeter(OcaUint8 value);
    void Run();

protected:
    virtual OcaLiteStatus GetReadingValue(::OcaDB& reading) const;
    virtual OcaLiteStatus GetReadingStateValue(::OcaLiteSensorReadingState& state) const;

private:
    OcaLiteString m_FFRole;
    OcaDB m_Level;
    OcaBoolean m_bChanged;
};

#endif // _OCALiteDanteLvlSensor_h
