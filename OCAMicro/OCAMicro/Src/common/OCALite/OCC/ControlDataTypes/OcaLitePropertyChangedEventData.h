/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLitePropertyChangedEventData
 *
 */

#ifndef OCALITEPROPERTYCHANGEDEVENTDATA_H
#define OCALITEPROPERTYCHANGEDEVENTDATA_H

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- Include local include files ----
#include "OcaLiteEventData.h"
#include "OcaLitePropertyID.h"
#include "OcaLiteString.h"
#include "OcaLiteTemplateHelpers.h"
#include "../ControlClasses/OcaLiteRoot.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * Enumeration describing property change type.
 */
enum OcaLitePropertyChangeType  /* maps onto OcaUint8 */
{
    /** Current value has changed. */
    OCAPROPERTYCHANGETYPE_CURRENT_CHANGED       = 1,
    /** Minimum value has changed. */
    OCAPROPERTYCHANGETYPE_MIN_CHANGED           = 2,
    /** Maximum value has changed. */
    OCAPROPERTYCHANGETYPE_MAX_CHANGED           = 3,
    /** An item has been added to a list or a map property. */
    OCAPROPERTYCHANGETYPE_ITEM_ADDED            = 4,
    /** An item of a list or a map property has been changed. */
    OCAPROPERTYCHANGETYPE_ITEM_CHANGED          = 5,
    /** An item of a list or a map property has been deleted. */
    OCAPROPERTYCHANGETYPE_ITEM_DELETED          = 6,
    /** Maximum value used for range checking */
    OCAPROPERTYCHANGETYPE_MAXIMUM
};

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Template class that defines the event data parameter for events that return property values,
 * such as the PropertyChanged event defined in OcaRoot, and the PeriodicObservation event defined
 * in OcaNumericObserver. The template parameter is the datatype of the changed property.
 * @tparam  DataType        The data type of the changed property.
 */
template <class DataType>
class OcaLitePropertyChangedEventData : public ::OcaLiteEventData
{
public:
    /**
     * Constructor.
     *
     * @param[in]   emitterONo      The object number of the emitter of the property changed event.
     * @param[in]   propertyID      The property ID of the property that has changed.
     * @param[in]   propertyValue   New value of the property. Type of property value parameter
     *                              is defined by template parameter DataType.
     * @param[in]   changeType      Type of change.
     */
    OcaLitePropertyChangedEventData(::OcaONo emitterONo,
                                const ::OcaLitePropertyID& propertyID,
                                const DataType& propertyValue,
                                ::OcaLitePropertyChangeType changeType)
        : ::OcaLiteEventData(::OcaLiteEvent(emitterONo, ::OcaLiteEventID(static_cast< ::OcaUint16>(1)/*OcaRoot is always level 1*/, 
                                                             static_cast< ::OcaUint16>(::OcaLiteRoot::OCA_EVENT_PROPERTY_CHANGED)))),
          m_propertyID(propertyID),
          m_propertyValue(propertyValue),
          m_changeType(changeType)
    {
        assert(m_changeType > 0);
        assert(m_changeType < OCAPROPERTYCHANGETYPE_MAXIMUM);
    }

    /**
     * Destructor.
     */
    virtual ~OcaLitePropertyChangedEventData()
    {
    }

    /**
     * Getter for PropertyID.
     *
     * @return The property ID of the property that has changed.
     */
    const ::OcaLitePropertyID& GetPropertyID() const
    {
        return m_propertyID;
    }

    /**
     * Getter for PropertyValue.
     *
     * @return The new value of the property.
     */
    const DataType& GetPropertyValue() const
    {
        return m_propertyValue;
    }

    /**
     * Getter for ChangeType.
     *
     * @return The type of change.
     */
    ::OcaLitePropertyChangeType GetChangeType() const
    {
        return m_changeType;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLitePropertyChangedEventData<DataType>& operator=(const ::OcaLitePropertyChangedEventData<DataType>& source)
    {
        if (this != &source)
        {
            OcaLiteEventData::operator=(source);
            m_propertyID = source.m_propertyID;
            m_propertyValue = source.m_propertyValue;
            m_changeType = source.m_changeType;
        }

        return *this;
    }

    // ---- OcaLiteEventData methods ----

    virtual bool operator==(const ::OcaLiteEventData& rhs) const
    {
        bool result(OcaLiteEventData::operator==(rhs));

        if (result)
        {
            const ::OcaLitePropertyChangedEventData<DataType>* pRhs(dynamic_cast<const ::OcaLitePropertyChangedEventData<DataType>*>(&rhs));
            if (NULL != pRhs)
            {
                result = result && (m_propertyID == pRhs->m_propertyID);
                result = result && CompareValue<DataType>(m_propertyValue, pRhs->m_propertyValue);
                result = result && (m_changeType == pRhs->m_changeType);
            }
            else
            {
                result = false;
            }
        }

        return result;
    }

    virtual ::OcaLiteEventData* Clone() const
    {
        return new ::OcaLitePropertyChangedEventData<DataType>(*this);
    }

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
    {
        OcaLiteEventData::Marshal(destination, writer);
        m_propertyID.Marshal(destination, writer);
        MarshalValue<DataType>(m_propertyValue, destination, writer);
        MarshalValue< ::OcaLitePropertyChangeType>(m_changeType, destination, writer);
    }

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
    {
        bool result(OcaLiteEventData::Unmarshal(bytesLeft, source, reader));
        result = result && m_propertyID.Unmarshal(bytesLeft, source, reader);
        result = result && UnmarshalValue<DataType>(m_propertyValue, bytesLeft, source, reader);
        result = result && UnmarshalValue< ::OcaLitePropertyChangeType>(m_changeType, bytesLeft, source, reader);

        if (!result)
        {
            m_propertyID = ::OcaLitePropertyID();
            m_propertyValue = DataType();
            m_changeType = OCAPROPERTYCHANGETYPE_CURRENT_CHANGED;
        }

        return result;
    }

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const
    {
        ::OcaUint32 length(OcaLiteEventData::GetSize(writer));
        length += m_propertyID.GetSize(writer);
        length += GetSizeValue<DataType>(m_propertyValue, writer);
        length += GetSizeValue< ::OcaLitePropertyChangeType>(m_changeType, writer);

        return length;
    }

    /**
     * Unmarshals the property ID and prevents the source and bytesLeft to be updated.
     * This can be used to peek into the data and find the property ID. It is assumed that the property ID is at the start of
     * the data pointed to by source.
     *
     * @param[out] propertyID   The unmarshaled property id.
     * @param[in]  bytesLeft    Number of bytes left in the array from which the
     *                          object is deserialized. Decremented with the number of
     *                          bytes read.
     * @param[in]  source       Pointer to the input byte array from which the object
     *                          is deserialized.
     * @param[in]  reader       The binary reader that helps in deserializing the object.
     * @return true when the unmarshal was successful, false otherwise.
     */
    static bool UnmarshalPropertyID(::OcaLitePropertyID& propertyID, ::OcaUint32 bytesLeft, const ::OcaUint8* source, const ::IOcaLiteReader& reader)
    {
        return propertyID.Unmarshal(bytesLeft, &source, reader);
    }

protected:
    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLitePropertyChangedEventData(const ::OcaLitePropertyChangedEventData<DataType>& source)
        : ::OcaLiteEventData(source),
          m_propertyID(source.m_propertyID),
          m_propertyValue(source.m_propertyValue),
          m_changeType(source.m_changeType)
    {
    }

private:
    /** The property ID of the property that has changed. */
    ::OcaLitePropertyID         m_propertyID;

    /** The new value of the property. */
    DataType                m_propertyValue;

    /** The type of change. */
    ::OcaLitePropertyChangeType m_changeType;
};

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::OcaLitePropertyChangeType>(const ::OcaLitePropertyChangeType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLitePropertyChangeType>(::OcaLitePropertyChangeType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLitePropertyChangeType>(const ::OcaLitePropertyChangeType& value, const ::IOcaLiteWriter& writer);

#endif // OCALITEPROPERTYCHANGEDEVENTDATA_H
