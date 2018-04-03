/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCC Worker data types.
 *
 */

#ifndef OCALITEWORKERDATATYPES_H
#define OCALITEWORKERDATATYPES_H

// ---- Include system wide include files ----

// ---- Include local include files ----

// ---- Referenced classes and types ----
#include <OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>

// ---- Helper types and constants ----

/** A ratio expressed in dB. Typically used for gain settings. */
typedef ::OcaFloat32    OcaDB;

/**
 * An absolute analogue level expressed in dB re 1 Volt. This datatype may only be
 * used for parameters that reflect analogue signal values, e.g. pre-ADC input
 * signals and post-DAC output signals.
 */
typedef ::OcaDB         OcaDBV;

/**
 * An absolute analogue level expressed in dB re 0.774 Volts. This datatype may only be
 * used for parameters that reflect analogue signal values, e.g. pre-ADC input
 * signals and post-DAC output signals.
 */
typedef ::OcaDB         OcaDBu;

/** An absolute level expressed in dB relative to the device maximum internal digital signal level. */
typedef ::OcaDB         OcaDBfs;

/**
 * An absolute level expressed in dB relative to the device zero level.
 * The device zero level is the absolute level which is (OcaDeviceManager.Headroom) dB below the
 * internal digital maximum level.
 */
typedef ::OcaDB         OcaDBz;

/** Mute states. */
enum OcaLiteMuteState   /* maps onto OcaUint8 */
{
    /** Muted. */
    OCAMUTESTATE_MUTED              = 1,
    /** Unmuted. */
    OCAMUTESTATE_UNMUTED            = 2,
    /** Maximum value used for range checking */
    OCAMUTESTATE_MAXIMUM
};

/** Polarity states. */
enum OcaLitePolarityState   /* maps onto OcaUint8 */
{
    /** Non-inverted. */
    OCAPOLARITYSTATE_NON_INVERTED   = 1,
    /** Inverted. */
    OCAPOLARITYSTATE_INVERTED       = 2,
    /** Maximum value used for range checking */
    OCAPOLARITYSTATE_MAXIMUM
};

/** Enumeration of types of delay units that are available in OCA. */
enum OcaLiteDelayUnit       /* maps onto OcaUint8 */
{
    /** Time. */
    OCADELAYUNIT_TIME               = 1,
    /** Distance. */
    OCADELAYUNIT_DISTANCE           = 2,
    /** Samples. */
    OCADELAYUNIT_SAMPLES            = 3,
    /** Maximum value used for range checking */
    OCADELAYUNIT_MAXIMUM
};

/** Frequency value in Hertz. */
typedef ::OcaFloat32   OcaFrequency;

/**
 * Time of day in OCA standard format (Network Time Protocol format)
 * which is 32-bits for the number of seconds since January 1, 1900
 * followed by a 32-bit part for the fractional second.
 */
typedef ::OcaUint64     OcaTimeOfDay;

/** Time interval in seconds. */
typedef ::OcaFloat32    OcaTimeInterval;

/**
 * General-purpose period of time in milliseconds. As this type is mostly used for management
 * purposes and integer base type is used and it is expressed in milliseconds.
 */
typedef ::OcaUint32     OcaPeriod;

/**
 * Enumeration of units of measure that can be used in OCA classes. Only SI
 * (base or derived) units are specified, so that internal calculations will not
 * need to convert. If conversion is needed it should only be done in user
 * interfaces. The datatype of a reading expressed in one of these units of
 * measure is OcaFloat32.
 */
enum OcaLiteUnitOfMeasure   /* maps onto OcaUint8 */
{
    /** No reading. */
    OCAUNITOFMEASURE_NONE               = 0,
    /** Frequency in Hertz. */
    OCAUNITOFMEASURE_HERTZ              = 1,
    /** Temperature in degree Celsius. */
    OCAUNITOFMEASURE_DEGREE_CELSIUS     = 2,
    /** Voltage in Volts. */
    OCAUNITOFMEASURE_VOLT               = 3,
    /** Electric current in Amperes. */
    OCAUNITOFMEASURE_AMPERE             = 4,
    /** Resistance, magnitude of reactance, or magnitude of impedance in Ohms. */
    OCAUNITOFMEASURE_OHM                = 5,
    /** Maximum value used for range checking */
    OCAUNITOFMEASURE_MAXIMUM
};

/**
 * Enumeration of presentation units that can be used in OCA classes. Property
 * values of OCA objects are always in SI units (unless explicitly documented
 * otherwise), but the presentation unit can also be stored to indicate in which
 * unit the value was presented in a user interface. This way another controller
 * can also present it in that unit (i.e. doing a conversion on the controller
 * before presenting it) to keep the user presentation uniform. Note that the
 * presentation unit may be equal to the unit of the property (in which case of
 * course no conversion is needed). */
enum OcaLitePresentationUnit    /* maps onto OcaUint8 */
{
    /** dB(0.775 VRMS) - voltage relative to 0.775 volts. */
    OCAPRESENTATIONUNIT_DBU    = 0,
    /** dB(1 VRMS) - voltage relative to 1 volt. */
    OCAPRESENTATIONUNIT_DBV    = 1,
    /** Voltage in plain volts. */
    OCAPRESENTATIONUNIT_V      = 2,
    /** Maximum value used for range checking */
    OCAPRESENTATIONUNIT_MAXIMUM
};

/**
 * Enumeration of the types of level detector characteristics. Used in dynamics classes and for sensors.
 */
enum OcaLiteLevelDetectionLaw    /* maps onto OcaUint8 */
{
    /** No detection (i.e. never trigger) */
    OCALEVELDETECTIONLAW_NONE       = 0,
    /** Detect RMS value */
    OCALEVELDETECTIONLAW_RMS        = 1,
    /** Detect peak value. */
    OCALEVELDETECTIONLAW_PEAK       = 2,
    /** Maximum value used for range checking */
    OCALEVELDETECTIONLAW_MAXIMUM
};

/** Temperature in degree Celsius. */
typedef ::OcaFloat32    OcaTemperature;

// ---- Specialized Template Function Definition ----

#endif // OCLITEAWORKERDATATYPES_H 
