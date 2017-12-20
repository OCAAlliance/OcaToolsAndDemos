/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Dante Media Clock Implementation
 *
 */

#ifndef _DANTEOCALITEMEDIACLOCK_H
#define _DANTEOCALITEMEDIACLOCK_H

#include <assert.h>

#include <OCC/ControlClasses/Agents/OcaLiteMediaClock.h>

// Specific gain object mapping to an actual gain implementation
class DanteLiteOcaMediaClock : public OcaLiteMediaClock
{
public:
    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   type                    The clock type.
     * @param[in]   domainID                The domain ID value.
     * @param[in]   ratesSupported          List with the supported rates of this instance.
     */
    DanteLiteOcaMediaClock(::OcaONo objectNumber,
                  ::OcaBoolean lockable,
                  const ::OcaLiteString& role,
                  ::OcaLiteMediaClockType type,
                  ::OcaUint16 domainID,
                  const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported,
                  const OcaLiteMediaClockRate& currentRate);

    /**
     * Destructor.
     */
    virtual ~DanteLiteOcaMediaClock();

    ::OcaLiteStatus Initialize();

    bool setNewDanteSampleRate(OcaUint32 newRate);

protected:

    /**
     * Sets the value of the Type property.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in] type  The media clock type.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetTypeValue(::OcaLiteMediaClockType type);

    /**
     * Gets the current domain ID.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] id    The domain ID.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetDomainIDValue(::OcaUint16& id) const;

    /**
     * Sets the value of the DomainID property.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in] id    The domain ID.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetDomainIDValue(::OcaUint16 id);

    /**
     * Gets the current sampling rate.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] rate The sampling rate.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetRateValue(::OcaLiteMediaClockRate& rate) const;

    /**
     * Sets the sampling rate
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in] rate  The sampling rate.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetRateValue(const ::OcaLiteMediaClockRate& rate);

    /**
     * Gets the current media clock lock state.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] state    The media clock lock state.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetLockStateValue(::OcaLiteMediaClockLockState& state) const;

private:
    OcaLiteMediaClockRate         m_Rate;
    OcaLiteMediaClockLockState    m_ClockState;
    OcaUint16                     m_DomainID;
    OcaLiteMediaClockType         m_ClockType;
};

#endif // _DANTEOCALITEMEDIACLOCK_H
