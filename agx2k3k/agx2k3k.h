/****************************************************************************
 *                   Agilent 2000 and 3000 X-Series Oscilloscopes
 *
 * Title:    agx2k3k.h
 * Purpose:  Agilent 2000 3000 X-Series Oscilloscopes
 *           instrument driver declarations.
 *
 ****************************************************************************/

#ifndef __AGX2K3K_HEADER
#define __AGX2K3K_HEADER

#include <ivi.h>
#include <iviscope.h>

 /*************************************************
 *
 * Entry Point types
 *
 */
#if !defined(WIN32) && !defined(__WIN32__)
 //#ifndef WIN32
#define FENTRY
#define STDCALL
#else
#include <windows.h>
#define DllExport	__declspec( DllExport )
#define DllExport	__declspec( dllexport )
#define FENTRY		DllExport
   #ifndef _WIN64
   #define STDCALL		__stdcall
   #endif
#define	huge
#endif  // WIN32

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/
#define AGX2K3K_MAJOR_VERSION                  3   /* Instrument driver major version   */
#define AGX2K3K_MINOR_VERSION                  2   /* Instrument driver minor version   */

#define AGX2K3K_CLASS_SPEC_MAJOR_VERSION       4   /* Class specification major version */
#define AGX2K3K_CLASS_SPEC_MINOR_VERSION       1   /* Class specification minor version */

#define AGX2K3K_SUPPORTED_INSTRUMENT_MODELS    "MSO-X 4022A, MSO-X 4032A, MSO-X 4052A, MSO-X 4024A, MSO-X 4034A, MSO-X 4054A, MSO-X 4104A, MSO-X 4154A, MSO-X 3102A, MSO-X 3104A, MSO-X 3014A, MSO-X 3024A, MSO-X 3034A, MSO-X 3054A, MSO-X 3012A, MSO-X 3022A, MSO-X 3032A, MSO-X 3052A, MSO-X 3012T, MSO-X 3022T, MSO-X 3032T, MSO-X 3052T, MSO-X 3102T, MSO-X 3014T, MSO-X 3024T, MSO-X 3034T, MSO-X 3054T, MSO-X 3104T, MSO-X 2004A, MSO-X 2014A, MSO-X 2024A, MSO-X 2002A, MSO-X 2012A, MSO-X 2022A, DSO-X 4022A, DSO-X 4032A, DSO-X 4052A, DSO-X 4024A, DSO-X 4034A, DSO-X 4054A, DSO-X 4104A, DSO-X 4154A, DSO-X 3102A, DSO-X 3104A, DSO-X 3014A, DSO-X 3024A, DSO-X 3034A, DSO-X 3054A, DSO-X 3012A, DSO-X 3022A,DSO-X 3032A,DSO-X 3052A, DSO-X 2004A, DSO-X 2014A, DSO-X 2024A, DSO-X 2002A, DSO-X 2012A, DSO-X 2022A, DSO-X 3012T, DSO-X 3022T, DSO-X 3032T, DSO-X 3052T, DSO-X 3102T, DSO-X 3014T, DSO-X 3024T, DSO-X 3034T, DSO-X 3054T, DSO-X 3104T"

#define AGX2K3K_DRIVER_VENDOR                  "National Instruments"

#ifdef _IVI_mswin64_
#define AGX2K3K_DRIVER_DESCRIPTION             "Agilent 2000 and 3000 X-Series Oscilloscopes [Compiled for 64-bit]"
#else
#define AGX2K3K_DRIVER_DESCRIPTION             "Agilent 2000 and 3000 X-Series Oscilloscopes"
#endif

/****************************************************************************
 *------------------------------ Useful Macros -----------------------------*
 ****************************************************************************/

/* Defined values for the status parameter of the agx2k3k_Acquisition_Status function */
#define AGX2K3K_VAL_ACQ_COMPLETE                  IVISCOPE_VAL_ACQ_COMPLETE
#define AGX2K3K_VAL_ACQ_IN_PROGRESS               IVISCOPE_VAL_ACQ_IN_PROGRESS
#define AGX2K3K_VAL_ACQ_STATUS_UNKNOWN            IVISCOPE_VAL_ACQ_STATUS_UNKNOWN

/*
 *  Defined values for the measurementFunction parameter of the
 *  agx2k3k_ReadWaveformMeasurement function.
 */
#define AGX2K3K_VAL_RISE_TIME                     IVISCOPE_VAL_RISE_TIME
#define AGX2K3K_VAL_FALL_TIME                     IVISCOPE_VAL_FALL_TIME
#define AGX2K3K_VAL_FREQUENCY                     IVISCOPE_VAL_FREQUENCY
#define AGX2K3K_VAL_PERIOD                        IVISCOPE_VAL_PERIOD
#define AGX2K3K_VAL_VOLTAGE_RMS                   IVISCOPE_VAL_VOLTAGE_RMS
#define AGX2K3K_VAL_VOLTAGE_PEAK_TO_PEAK          IVISCOPE_VAL_VOLTAGE_PEAK_TO_PEAK
#define AGX2K3K_VAL_VOLTAGE_MAX                   IVISCOPE_VAL_VOLTAGE_MAX
#define AGX2K3K_VAL_VOLTAGE_MIN                   IVISCOPE_VAL_VOLTAGE_MIN
#define AGX2K3K_VAL_VOLTAGE_HIGH                  IVISCOPE_VAL_VOLTAGE_HIGH
#define AGX2K3K_VAL_VOLTAGE_LOW                   IVISCOPE_VAL_VOLTAGE_LOW
#define AGX2K3K_VAL_VOLTAGE_AVERAGE               IVISCOPE_VAL_VOLTAGE_AVERAGE
#define AGX2K3K_VAL_WIDTH_NEG                     IVISCOPE_VAL_WIDTH_NEG
#define AGX2K3K_VAL_WIDTH_POS                     IVISCOPE_VAL_WIDTH_POS
#define AGX2K3K_VAL_DUTY_CYCLE_POS                IVISCOPE_VAL_DUTY_CYCLE_POS
#define AGX2K3K_VAL_AMPLITUDE                     IVISCOPE_VAL_AMPLITUDE
#define AGX2K3K_VAL_OVERSHOOT                     IVISCOPE_VAL_OVERSHOOT
#define AGX2K3K_VAL_PRESHOOT                      IVISCOPE_VAL_PRESHOOT
#define AGX2K3K_VAL_COUNTER                       (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 1L)
#define AGX2K3K_VAL_DELAY                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 2L)
#define AGX2K3K_VAL_PHASE                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 3L)
#define AGX2K3K_VAL_X_AT_MAX                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 4L)
#define AGX2K3K_VAL_X_AT_MIN                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 5L)
#define AGX2K3K_VAL_BURST_WIDTH                   (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 6L)
#define AGX2K3K_VAL_NEGATIVE_EDGE                 (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 7L)
#define AGX2K3K_VAL_NEGATIVE_PULSE                (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 8L)
#define AGX2K3K_VAL_POSITIVE_EDGE                 (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 9L)
#define AGX2K3K_VAL_POSITIVE_PULSE                (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 10L)
#define AGX2K3K_VAL_VOLTAGE_RATIO                 (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 11L)
#define AGX2K3K_VAL_BIT_RATE                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 12L)
#define AGX2K3K_VAL_NEGATIVE_DUTY                 (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 13L)
#define AGX2K3K_VAL_RDS_ON                        (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 14L)
#define AGX2K3K_VAL_VCE_SAT                       (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 15L)
#define AGX2K3K_VAL_DUAL_CHARGE                   (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 16L)
#define AGX2K3K_VAL_DUAL_VERTICAL_AMPLITUDE       (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 17L)
#define AGX2K3K_VAL_DUAL_AVERAGE_VALUE            (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 18L)
#define AGX2K3K_VAL_DUAL_WAVEFORM_BASE_VALUE      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 19L)
#define AGX2K3K_VAL_DUAL_VERTICAL_PEAK_TO_PEAK    (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 20L)
#define AGX2K3K_VAL_DUAL_VRMS                     (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE + 21L)

/* Define values for ConfigureMarkerUnits function */
#define AGX2K3K_VAL_MARKER_DIMENSION_X            (0)
#define AGX2K3K_VAL_MARKER_DIMENSION_Y            (1)

/****************************************************************************
 *---------------------------- Attribute Defines ---------------------------*
 ****************************************************************************/

/*- IVI Inherent Instrument Attributes ---------------------------------*/

/* User Options */
#define AGX2K3K_ATTR_RANGE_CHECK                                 IVI_ATTR_RANGE_CHECK             /* ViBoolean */
#define AGX2K3K_ATTR_QUERY_INSTRUMENT_STATUS                     IVI_ATTR_QUERY_INSTRUMENT_STATUS /* ViBoolean */
#define AGX2K3K_ATTR_CACHE                                       IVI_ATTR_CACHE                   /* ViBoolean */
#define AGX2K3K_ATTR_SIMULATE                                    IVI_ATTR_SIMULATE                /* ViBoolean */
#define AGX2K3K_ATTR_RECORD_COERCIONS                            IVI_ATTR_RECORD_COERCIONS        /* ViBoolean */
#define AGX2K3K_ATTR_INTERCHANGE_CHECK                           IVI_ATTR_INTERCHANGE_CHECK       /* ViBoolean */

/* Driver Information  */
#define AGX2K3K_ATTR_SPECIFIC_DRIVER_PREFIX                      IVI_ATTR_SPECIFIC_DRIVER_PREFIX                   /* ViString, read-only  */
#define AGX2K3K_ATTR_SUPPORTED_INSTRUMENT_MODELS                 IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS              /* ViString, read-only  */
#define AGX2K3K_ATTR_GROUP_CAPABILITIES                          IVI_ATTR_GROUP_CAPABILITIES                       /* ViString, read-only  */
#define AGX2K3K_ATTR_INSTRUMENT_MANUFACTURER                     IVI_ATTR_INSTRUMENT_MANUFACTURER                  /* ViString, read-only  */
#define AGX2K3K_ATTR_INSTRUMENT_MODEL                            IVI_ATTR_INSTRUMENT_MODEL                         /* ViString, read-only  */
#define AGX2K3K_ATTR_INSTRUMENT_FIRMWARE_REVISION                IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION             /* ViString, read-only  */
#define AGX2K3K_ATTR_SPECIFIC_DRIVER_REVISION                    IVI_ATTR_SPECIFIC_DRIVER_REVISION                 /* ViString, read-only  */
#define AGX2K3K_ATTR_SPECIFIC_DRIVER_VENDOR                      IVI_ATTR_SPECIFIC_DRIVER_VENDOR                   /* ViString, read-only  */
#define AGX2K3K_ATTR_SPECIFIC_DRIVER_DESCRIPTION                 IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION              /* ViString, read-only  */
#define AGX2K3K_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define AGX2K3K_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

/* Advanced Session Information */
#define AGX2K3K_ATTR_LOGICAL_NAME                                IVI_ATTR_LOGICAL_NAME           /* ViString, read-only  */
#define AGX2K3K_ATTR_IO_RESOURCE_DESCRIPTOR                      IVI_ATTR_IO_RESOURCE_DESCRIPTOR /* ViString, read-only  */
#define AGX2K3K_ATTR_DRIVER_SETUP                                IVI_ATTR_DRIVER_SETUP           /* ViString, read-only  */

/*- Instrument-Specific Attributes -------------------------------------*/
/*- Acquisition Sub-system ---------------------------------------------*/
#define AGX2K3K_ATTR_ACQUISITION_TYPE            IVISCOPE_ATTR_ACQUISITION_TYPE        /* ViInt32 */
#define AGX2K3K_ATTR_ACQUISITION_MODE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20L) /* ViInt32 */
#define AGX2K3K_ATTR_INITIATE_CONTINUOUS         IVISCOPE_ATTR_INITIATE_CONTINUOUS     /* ViBoolean */
#define AGX2K3K_ATTR_NUM_AVERAGES                IVISCOPE_ATTR_NUM_AVERAGES            /* ViInt32 */
#define AGX2K3K_ATTR_SAMPLE_MODE                 IVISCOPE_ATTR_SAMPLE_MODE             /* ViInt32, Read-only */
#define AGX2K3K_ATTR_HORZ_TIME_PER_RECORD        IVISCOPE_ATTR_HORZ_TIME_PER_RECORD    /* ViReal64 */
#define AGX2K3K_ATTR_HORZ_MIN_NUM_PTS            IVISCOPE_ATTR_HORZ_MIN_NUM_PTS        /* ViInt32 */
#define AGX2K3K_ATTR_HORZ_RECORD_LENGTH          IVISCOPE_ATTR_HORZ_RECORD_LENGTH      /* ViInt32, Read-only  */
#define AGX2K3K_ATTR_ACQUISITION_START_TIME      IVISCOPE_ATTR_ACQUISITION_START_TIME  /* ViReal64, percentage of record length  */
#define AGX2K3K_ATTR_HORZ_SAMPLE_RATE            IVISCOPE_ATTR_HORZ_SAMPLE_RATE        /* ViReal64, Read-only */
#define AGX2K3K_ATTR_HORZ_RECORD_MODE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 21L) /* ViInt32 */

/*- with option SGM -*/
#define AGX2K3K_ATTR_SEGMENT_ANALYZE_ENABLE      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 30L) /* ViBoolean, write-only */
#define AGX2K3K_ATTR_SEGMENT_COUNT               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 31L) /* ViInt32 */
#define AGX2K3K_ATTR_SEGMENT_INDEX               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 32L) /* ViInt32 */

/*- Channel Sub-system -------------------------------------------------*/
#define AGX2K3K_ATTR_CHANNEL_ENABLED             IVISCOPE_ATTR_CHANNEL_ENABLED           /* ViBoolean, Multi-channel */
#define AGX2K3K_ATTR_PROBE_ATTENUATION           IVISCOPE_ATTR_PROBE_ATTENUATION         /* ViReal64, Multi-channel */
#define AGX2K3K_ATTR_PROBE_SENSE_VALUE           IVISCOPE_ATTR_PROBE_SENSE_VALUE         /* ViReal64, Multi-channel, Read-only */
#define AGX2K3K_ATTR_VERTICAL_RANGE              IVISCOPE_ATTR_VERTICAL_RANGE            /* ViReal64, Multi-channel, volts */
#define AGX2K3K_ATTR_VERTICAL_OFFSET             IVISCOPE_ATTR_VERTICAL_OFFSET           /* ViReal64, Multi-channel, volts */
#define AGX2K3K_ATTR_VERTICAL_COUPLING           IVISCOPE_ATTR_VERTICAL_COUPLING         /* ViInt32, Multi-channel */
#define AGX2K3K_ATTR_MAX_INPUT_FREQUENCY         IVISCOPE_ATTR_MAX_INPUT_FREQUENCY       /* ViReal64,  Multi-Channel, hertz  */
#define AGX2K3K_ATTR_INPUT_IMPEDANCE             IVISCOPE_ATTR_INPUT_IMPEDANCE           /* ViReal64, Multi-channel, ohms */
#define AGX2K3K_ATTR_VERNIER                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 40L)   /* ViBoolean, Multi-channel */
#define AGX2K3K_ATTR_INVERT                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 41L)   /* ViBoolean, Multi-channel */
#define AGX2K3K_ATTR_PROBE_SKEW                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 42L)   /* ViReal64, Multi-channel */
#define AGX2K3K_ATTR_PROBE_UNITS                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 43L)   /* ViInt32, Multi-channel */
#define AGX2K3K_ATTR_CHANNEL_LABEL               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 44L)   /* ViString, Multi-channel */
#define AGX2K3K_ATTR_DIG_SIZE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 45L)   /* ViInt32, Multi-channel */
#define AGX2K3K_ATTR_DIG_THRESHOLD               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 46L)   /* ViInt32 */
#define AGX2K3K_ATTR_DIG_POSITION                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 47L)   /* ViInt32, Multi-channel  */
#define AGX2K3K_ATTR_DIG_CHANNEL_ENABLED         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 49L)   /* ViBoolean, Multi-channel */
#define AGX2K3K_ATTR_CHANNEL_PROBE_TEK_MODEL     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1101L) /* ViInt32		*/
/*- Trigger Sub-system -------------------------------------------------*/
#define AGX2K3K_ATTR_TRIGGER_TYPE                IVISCOPE_ATTR_TRIGGER_TYPE              /* ViInt32  */
#define AGX2K3K_ATTR_TRIGGER_HOLDOFF             IVISCOPE_ATTR_TRIGGER_HOLDOFF           /* ViReal64, seconds   */
#define AGX2K3K_ATTR_TRIGGER_SOURCE              IVISCOPE_ATTR_TRIGGER_SOURCE            /* ViString */
#define AGX2K3K_ATTR_TRIGGER_MODIFIER            IVISCOPE_ATTR_TRIGGER_MODIFIER          /* ViInt32 */
#define AGX2K3K_ATTR_TRIGGER_NOISE_REJECT        IVISCOPE_VAL_NOISE_REJECT               /* ViBoolean */
#define AGX2K3K_ATTR_TRIGGER_HF_REJECT           IVISCOPE_VAL_HF_REJECT                  /* ViBoolean */
#define AGX2K3K_ATTR_TRIGGER_LEVEL_HIGH          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 60L)   /* ViReal64 */
#define AGX2K3K_ATTR_TRIGGER_LEVEL_LOW           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 61L)   /* ViReal64 */

/*- Edge Trigger Attributes -*/
#define AGX2K3K_ATTR_TRIGGER_COUPLING            IVISCOPE_ATTR_TRIGGER_COUPLING        /* ViInt32, enum */
#define AGX2K3K_ATTR_TRIGGER_SLOPE               IVISCOPE_ATTR_TRIGGER_SLOPE           /* ViInt32, enum */
#define AGX2K3K_ATTR_TRIGGER_LEVEL               IVISCOPE_ATTR_TRIGGER_LEVEL           /* ViReal64, volts */
#define AGX2K3K_ATTR_TRIGGER_REJECT_FILTER       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 70L) /* ViInt32, enum */

/*- TV Trigger Attributes -*/
#define AGX2K3K_ATTR_TV_TRIGGER_SIGNAL_FORMAT    IVISCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT /* ViInt32 */
#define AGX2K3K_ATTR_TV_TRIGGER_EVENT            IVISCOPE_ATTR_TV_TRIGGER_EVENT         /* ViInt32 */
#define AGX2K3K_ATTR_TV_TRIGGER_LINE_NUMBER      IVISCOPE_ATTR_TV_TRIGGER_LINE_NUMBER   /* ViInt32 */
#define AGX2K3K_ATTR_TV_TRIGGER_POLARITY         IVISCOPE_ATTR_TV_TRIGGER_POLARITY      /* ViInt32 */

/*- Glitch Trigger Attributes -*/
#define AGX2K3K_ATTR_GLITCH_POLARITY             IVISCOPE_ATTR_GLITCH_POLARITY         /* ViInt32 */
#define AGX2K3K_ATTR_GLITCH_CONDITION            IVISCOPE_ATTR_GLITCH_CONDITION        /* ViInt32 */
#define AGX2K3K_ATTR_GLITCH_WIDTH                IVISCOPE_ATTR_GLITCH_WIDTH            /* ViReal64, Max Width */
#define AGX2K3K_ATTR_GLITCH_MIN_WIDTH            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 80L) /* ViReal64, Min Width */
#define AGX2K3K_ATTR_GLITCH_RANGE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 81L) /* ViString */

/*- Pattern Trigger Attribute -*/
#define AGX2K3K_ATTR_PATTERN                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 90L) /* ViString */
#define AGX2K3K_ATTR_PATTERN_QUALIFIER           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 91L) /* ViInt32 */
#define AGX2K3K_ATTR_PATTERN_MIN                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 92L) /* ViReal64 */
#define AGX2K3K_ATTR_PATTERN_MAX                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 93L) /* ViReal64 */
#define AGX2K3K_ATTR_PATTERN_RANGE               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 94L) /* ViString */

/*- 3000 Series specific Trigger attributs -------------------------------*/
/*- Runt Trigger Attribute -*/
#define AGX2K3K_ATTR_RUNT_POLARITY           IVISCOPE_ATTR_RUNT_POLARITY             /* ViInt32, enum */
#define AGX2K3K_ATTR_RUNT_QUALIFIER          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 101L)  /* ViInt32, enum */
#define AGX2K3K_ATTR_RUNT_TIME               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 102L)  /* ViReal64 */
#define AGX2K3K_ATTR_RUNT_HIGH_THRESHOLD     IVISCOPE_ATTR_RUNT_HIGH_THRESHOLD       /* ViReal64 */
#define AGX2K3K_ATTR_RUNT_LOW_THRESHOLD      IVISCOPE_ATTR_RUNT_LOW_THRESHOLD        /* ViReal64 */

/*- Transition Trigger Attribute -*/
#define AGX2K3K_ATTR_TRANSITION_QUALIFIER    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 110L)  /* ViInt32, enum */
#define AGX2K3K_ATTR_TRANSITION_SLOPE        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 111L)  /* ViInt32, enum */
#define AGX2K3K_ATTR_TRANSITION_TIME         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 112L)  /* ViReal64 */

/*- Setup Hold Trigger Attribute -*/
#define AGX2K3K_ATTR_SETUPHOLD_SLOPE         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 120L)  /* ViInt32, enum */
#define AGX2K3K_ATTR_SETUPHOLD_CLKSRC        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 121L)  /* ViString */
#define AGX2K3K_ATTR_SETUPHOLD_DATASRC       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 122L)  /* ViString */
#define AGX2K3K_ATTR_SETUPHOLD_HOLD_TIME     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 123L)  /* ViReal64 */
#define AGX2K3K_ATTR_SETUPHOLD_SETUP_TIME    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 124L)  /* ViReal64 */

/*- Edge Burst Trigger Attributes -*/
#define AGX2K3K_ATTR_EBURST_COUNT            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 130L)    /* ViInt32 */
#define AGX2K3K_ATTR_EBURST_IDLE             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 131L)    /* ViReal64 */
#define AGX2K3K_ATTR_EBURST_SLOPE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 132L)    /* ViInt32, enum */

/*- USB Trigger Attributes -*/
#define AGX2K3K_ATTR_USB_TRIGGER             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 140L)   /* ViInt32, enum */
#define AGX2K3K_ATTR_USB_SPEED               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 141L)   /* ViInt32, enum */
#define AGX2K3K_ATTR_USB_DPLUS               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 142L)   /* ViString */
#define AGX2K3K_ATTR_USB_DMINUS              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 143L)   /* ViString */

/*- Serial Bus Sub-system -------------------------------------------------*/
/*- CAN Serial Bus Attributes -*/
#define AGX2K3K_ATTR_SBUS_CAN_BAUDRATE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 150L)   /* ViInt32, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_SIGNAL                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 151L)   /* ViInt32, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_TRIGGER                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 152L)   /* ViInt32, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_SOURCE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 153L)   /* ViString, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_DATA                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 154L)   /* ViString, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_DATA_LENGTH                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 155L)   /* ViInt32, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_ID                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 156L)   /* ViString, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_ID_MODE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 157L)   /* ViInt32, enum, multi-channel */
#define AGX2K3K_ATTR_SBUS_CAN_DISPLAY                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18001L) /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_CAN_FD_SAMPLE_POINT                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18002L) /* ViReal64		*/
#define AGX2K3K_ATTR_SBUS_CAN_SIGNAL_FD_BAUDRATE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18003L) /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_CAN_TRIGGER_IDFILTER_ENABLED          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18004L) /* ViBoolean		*/
#define AGX2K3K_ATTR_SBUS_CAN_TRIGGER_PATTERN_DATA_DLC          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18005L) /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_CAN_TRIGGER_PATTERN_DATA_START        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18006L) /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_CAN_TRIGGER_SYMBOLIC_MESSAGE          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18007L) /* ViString		*/
#define AGX2K3K_ATTR_SBUS_CAN_TRIGGER_SYMBOLIC_SIGNAL           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18008L) /* ViString		*/
#define AGX2K3K_ATTR_SBUS_CAN_TRIGGER_SYMBOLIC_VALUE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18009L) /* ViReal64		*/

/*- IIC Serial Bus Attributes -*/
#define AGX2K3K_ATTR_SBUS_IIC_SCL                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 160L) /* ViString, multi-channel */
#define AGX2K3K_ATTR_SBUS_IIC_SDA                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 161L) /* ViString, multi-channel */
#define AGX2K3K_ATTR_SBUS_IIC_TYPE                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 162L) /* ViInt32, multi-channel */
#define AGX2K3K_ATTR_SBUS_IIC_QUALIFIER                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 163L) /* ViInt32, multi-channel */
#define AGX2K3K_ATTR_SBUS_IIC_ADDRESS                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 164L) /* ViInt32 , multi-channel*/
#define AGX2K3K_ATTR_SBUS_IIC_DATA                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 165L) /* ViInt32 , multi-channel*/
#define AGX2K3K_ATTR_SBUS_IIC_DATA2                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 166L) /* ViInt32, multi-channel */
#define AGX2K3K_ATTR_SBUS_IIC_SIZE                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 167L) /* ViInt32, enum, multi-channel */

/*- I2S Serial Bus Attributes -*/
#define AGX2K3K_ATTR_SBUS_I2S_ALIGNMENT                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 170L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_I2S_CLOCK_SLOPE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 172L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_I2S_RECEIVER_WIDTH                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 173L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_I2S_TRANSMIT_WIDTH                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 174L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_I2S_SOURCE_CLOCK                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 175L) /* ViString */
#define AGX2K3K_ATTR_SBUS_I2S_SOURCE_DATA                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 176L) /* ViString */
#define AGX2K3K_ATTR_SBUS_I2S_SOURCE_WS                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 177L) /* ViString */
#define AGX2K3K_ATTR_SBUS_I2S_TRIGGER                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 178L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_I2S_TRIGGER_AUDIO                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 179L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_I2S_TRIGGER_DATA                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 180L) /* ViString */
#define AGX2K3K_ATTR_SBUS_I2S_TRIGGER_RANGE_LOW                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 181L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_I2S_TRIGGER_RANGE_HIGH                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 182L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_I2S_WS_LOW                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 183L) /* ViInt32, enum */

/*- LIN Serial Bus Attributes -*/
#define AGX2K3K_ATTR_SBUS_LIN_BAUDRATE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 190L)  /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_LIN_TRIGGER                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 191L)  /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_LIN_SOURCE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 192L)  /* ViString */
#define AGX2K3K_ATTR_SBUS_LIN_PARITY                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 193L)  /* ViBoolean */
#define AGX2K3K_ATTR_SBUS_LIN_STANDARD                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 194L)  /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_LIN_SYNC_BREAK                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 195L)  /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_LIN_TRIGGER_ID                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 196L)  /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_LIN_TRIGGER_DATA                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 197L)  /* ViString */
#define AGX2K3K_ATTR_SBUS_LIN_TRIGGER_DATA_LENGTH               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 198L)  /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_LIN_DISPLAY                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8010L) /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_LIN_TRIGGER_SYMBOLIC_FRAME            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8011L) /* ViString		*/
#define AGX2K3K_ATTR_SBUS_LIN_TRIGGER_SYMBOLIC_SIGNAL           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8012L) /* ViString		*/
#define AGX2K3K_ATTR_SBUS_LIN_TRIGGER_SYMBOLIC_VALUE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8013L) /* ViReal64		*/

/*- SENT Serial Bus Attributes -*/
#define AGX2K3K_ATTR_SBUS_SENT_CLOCK                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8014L)     /* ViReal64		*/
#define AGX2K3K_ATTR_SBUS_SENT_CRC                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8015L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_DISPLAY                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8016L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_FORMAT                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8017L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_IDLE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8018L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_LENGTH                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8019L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_PPULSE_ENABLED                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8020L)     /* ViBoolean		*/
#define AGX2K3K_ATTR_SBUS_SENT_SIGNAL_DISPLAY_ENABLED           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8021L)     /* ViBoolean		*/
#define AGX2K3K_ATTR_SBUS_SENT_SIGNAL_LENGTH                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8022L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_SIGNAL_MULTIPLIER                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8023L)     /* ViReal64		*/
#define AGX2K3K_ATTR_SBUS_SENT_SIGNAL_OFFSET                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8024L)     /* ViReal64		*/
#define AGX2K3K_ATTR_SBUS_SENT_SIGNAL_ORDER                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8025L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_SIGNAL_START                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8026L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_TRIGGER_TOLERANCE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8027L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_TRIGGER_SLOW_LENGTH              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8028L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_TRIGGER                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8029L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_TOLERANCE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8030L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_SOURCE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8031L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_TRIGGER_FAST_DATA                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8032L)     /* ViString		*/
#define AGX2K3K_ATTR_SBUS_SENT_TRIGGER_SLOW_DATA                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8033L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_TRIGGER_SLOW_ID                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8034L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_SENT_SIGNAL_NAME                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8035L)     /* ViString		*/

/*- SPI Serial Bus Attributes -*/
#define AGX2K3K_ATTR_SBUS_SPI_MISO_WIDTH                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 200L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_SPI_MOSI_WIDTH                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 201L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_SPI_MISO_DATA                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 203L) /* ViString */
#define AGX2K3K_ATTR_SBUS_SPI_MOSI_DATA                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 204L) /* ViString */
#define AGX2K3K_ATTR_SBUS_SPI_TRIGGER_TYPE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 205L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_SPI_CLKSOURCE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 206L) /* ViString */
#define AGX2K3K_ATTR_SBUS_SPI_DSOURCE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 208L) /* ViString */
#define AGX2K3K_ATTR_SBUS_SPI_FSOURCE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 209L) /* ViString */
#define AGX2K3K_ATTR_SBUS_SPI_SLOPE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 207L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_SPI_FRAME                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 210L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_SPI_WORD_WIDTH                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 202L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_SPI_TIMEOUT                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 211L) /* ViReal64 */
#define AGX2K3K_ATTR_SBUS_SPI_BIT_ORDER                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 212L) /* ViInt32, enum */

/*- UART Serial Bus Attributes -*/
#define AGX2K3K_ATTR_SBUS_UART_BAUDRATE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 250L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_UART_BIT_ORDER                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 251L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_UART_FRAMING                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 256L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_UART_PARITY                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 257L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_UART_POLARITY                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 258L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_UART_WIDTH                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 266L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_UART_SOURCE_RX                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 259L) /* ViString */
#define AGX2K3K_ATTR_SBUS_UART_SOURCE_TX                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 260L) /* ViString */
#define AGX2K3K_ATTR_SBUS_UART_TRIGGER_BURST                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 261L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_UART_TRIGGER_DATA                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 262L) /* ViInt32 */
#define AGX2K3K_ATTR_SBUS_UART_TRIGGER_IDLE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 263L) /* ViReal64 */
#define AGX2K3K_ATTR_SBUS_UART_TRIGGER_QUALIFIER                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 264L) /* ViInt32, enum */
#define AGX2K3K_ATTR_SBUS_UART_TRIGGER_TYPE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 265L) /* ViInt32, enum */

#define AGX2K3K_ATTR_SBUS_UART_COUNT_RESET                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 252L) /* Write-only, ViBoolean */
#define AGX2K3K_ATTR_SBUS_UART_COUNT_ERROR                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 253L) /* Read-only ViInt32 */
#define AGX2K3K_ATTR_SBUS_UART_COUNT_TX                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 254L) /* Read-only ViInt32 */
#define AGX2K3K_ATTR_SBUS_UART_COUNT_RX                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 255L) /* Read-only ViInt32 */

/*- Waveform Measurement Attributes ---------------------*/
#define AGX2K3K_ATTR_MEAS_HIGH_REF                              IVISCOPE_ATTR_MEAS_HIGH_REF            /* ViReal64, percentage */
#define AGX2K3K_ATTR_MEAS_LOW_REF                               IVISCOPE_ATTR_MEAS_LOW_REF             /* ViReal64, percentage */
#define AGX2K3K_ATTR_MEAS_MID_REF                               IVISCOPE_ATTR_MEAS_MID_REF             /* ViReal64, percentage */

#define AGX2K3K_ATTR_MEASURE_REF                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 270L) /* ViString */
#define AGX2K3K_ATTR_MEASURE_SOURCE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 271L) /* ViString */
#define AGX2K3K_ATTR_MEASURE_DELAY                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 272L) /* ViString */

/*- Math Attributes ------------------------------------*/
#define AGX2K3K_ATTR_MATH_CHANNEL_ENABLED                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 280L)   /* ViBoolean */
#define AGX2K3K_ATTR_MATH_OPERATION                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 282L)   /* ViInt32 */
#define AGX2K3K_ATTR_MATH_SCALE                                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 283L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_OFFSET                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 284L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_SPAN                                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 285L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_CENTER                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 286L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_WINDOW                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 287L)   /* ViInt32 */
#define AGX2K3K_ATTR_MATH_SOURCE1                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 288L)   /* ViString */
#define AGX2K3K_ATTR_MATH_SOURCE2                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 289L)   /* ViString */
#define AGX2K3K_ATTR_MATH_WINDOW_MULTICHANNEL                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 290L)   /* ViInt32 */
#define AGX2K3K_ATTR_MATH_SCALE_MULTICHANNEL                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 291L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_OFFSET_MULTICHANNEL                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 292L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_SPAN_MULTICHANNEL                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 293L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_CENTER_MULTICHANNEL                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 294L)   /* ViReal64 */
#define AGX2K3K_ATTR_MATH_CHANNEL_ENABLED_MULTICHANNEL          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 295L)   /* ViBoolean */
#define AGX2K3K_ATTR_MATH_SMOOTH_POINTS                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14001L) /* ViInt32		*/
#define AGX2K3K_ATTR_MATH_FFT_FREQUENCY_START                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14002L) /* ViReal64		*/
#define AGX2K3K_ATTR_MATH_FFT_FREQUENCY_STOP                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14003L) /* ViReal64		*/

/*- Timebase Attributes ------------------------------------------------*/
#define AGX2K3K_ATTR_TIMEBASE_MODE                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 300L) /* ViInt32, enum */
#define AGX2K3K_ATTR_TIMEBASE_POSITION                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 301L) /* ViReal64 */
#define AGX2K3K_ATTR_TIMEBASE_REFERENCE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 302L) /* ViInt32, enum */
#define AGX2K3K_ATTR_TIMEBASE_VERNIER                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 303L) /* ViBoolean */
#define AGX2K3K_ATTR_TIMEBASE_SCALE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 304L) /* ViReal64 */

#define AGX2K3K_ATTR_WIN_TIMEBASE_POSITION                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 310L) /* ViReal64 */
#define AGX2K3K_ATTR_WIN_TIMEBASE_RANGE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 311L) /* ViReal64 */
#define AGX2K3K_ATTR_WIN_TIMEBASE_SCALE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 312L) /* ViReal64 */

/*- Waveform Generator Sub-system --------------------------------------*/
#define AGX2K3K_ATTR_WAVEGEN_FREQUENCY                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 350L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_FUNCTION                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 351L) /* ViInt32 enum */
#define AGX2K3K_ATTR_WAVEGEN_PULSE_WIDTH                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 352L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_RAMP_SYMMETRY                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 353L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_SQUARE_DUTY_CYCLE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 354L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_OUTPUT_ENABLE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 355L) /* ViBoolean */
#define AGX2K3K_ATTR_WAVEGEN_LOAD_IMPEDANCE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 356L) /* ViInt32 enum */
#define AGX2K3K_ATTR_WAVEGEN_PERIOD                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 357L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_RESET                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 358L) /* ViBoolean */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 359L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE_HIGH                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 360L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE_LOW                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 361L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE_OFFSET                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 362L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_FUNCTION_MULTICHANNEL              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 363L) /* ViInt32 enum */
#define AGX2K3K_ATTR_WAVEGEN_LOAD_IMPEDANCE_MULTICHANNEL        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 364L) /* ViInt32 enum */
#define AGX2K3K_ATTR_WAVEGEN_PERIOD_MULTICHANNEL                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 365L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE_MULTICHANNEL               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 366L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE_HIGH_MULTICHANNEL          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 367L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE_LOW_MULTICHANNEL           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 368L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_VOLTAGE_OFFSET_MULTICHANNEL        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 369L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_FREQUENCY_MULTICHANNEL             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 390L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_PULSE_WIDTH_MULTICHANNEL           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 391L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_RAMP_SYMMETRY_MULTICHANNEL         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 392L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_SQUARE_DUTY_CYCLE_MULTICHANNEL     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 393L) /* ViReal64 */
#define AGX2K3K_ATTR_WAVEGEN_OUTPUT_ENABLE_MULTICHANNEL         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 394L) /* ViBoolean */

/*- Memory Waveform Sub-system -----------------------------------------*/
#define AGX2K3K_ATTR_WMEMORY_COPY                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 370L) /* ViString */
#define AGX2K3K_ATTR_WMEMORY_SKEW                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 371L) /* ViReal64 */
#define AGX2K3K_ATTR_WMEMORY_Y_OFFSET                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 372L) /* ViReal64 */
#define AGX2K3K_ATTR_WMEMORY_Y_RANGE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 373L) /* ViReal64 */
#define AGX2K3K_ATTR_WMEMORY_Y_SCALE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 374L) /* ViReal64 */

/*- Save Recall Sub-system ---------------------------------------------*/
#define AGX2K3K_ATTR_RECALL_WMEMORY_FILE_PATH                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 380L)   /* ViString */
#define AGX2K3K_ATTR_SAVE_WMEMORY_SOURCE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 381L)   /* ViString */
#define AGX2K3K_ATTR_SAVE_WMEMORY_FILE_PATH                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 382L)   /* ViString */
#define AGX2K3K_ATTR_SAVE_RESULTS_FORMAT_CURSOR_ENABLED         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17001L) /* ViBoolean		*/
#define AGX2K3K_ATTR_SAVE_RESULTS_FORMAT_MASK_ENABLED           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17002L) /* ViBoolean		*/
#define AGX2K3K_ATTR_SAVE_RESULTS_FORMAT_MEASUREMENT_ENABLED    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17003L) /* ViBoolean		*/
#define AGX2K3K_ATTR_SAVE_RESULTS_FORMAT_SEARCH_ENABLED         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17004L) /* ViBoolean		*/
#define AGX2K3K_ATTR_SAVE_RESULTS_FORMAT_SEGMENTED_ENABLED      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17005L) /* ViBoolean		*/

/*- Miscellaneous Attributes ---------------------------*/
#define AGX2K3K_ATTR_ID_QUERY_RESPONSE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 273L) /* ViString (Read Only) */

/*---------------------------------------------------------------------------
 *------------------------ New Attribute Defined in version 2.0 ----------*
 * ---------------------------------------------------------------------------*/
#define AGX2K3K_ATTR_WGEN_ARBITRARY_BYTEORDER                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1001L)                  /* ViInt32      */
#define AGX2K3K_ATTR_WGEN_ARBITRARY_INTERPOLATE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1002L)                  /* ViBoolean    */
#define AGX2K3K_ATTR_WGEN_MODULATION_NOISE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1003L)                  /* ViInt32      */
#define AGX2K3K_ATTR_WGEN_ARBITRARY_BYTEORDER_MULTICHANNEL      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1004L)                  /* ViInt32      */
#define AGX2K3K_ATTR_WGEN_MODULATION_NOISE_MULTICHANNEL         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1005L)                  /* ViInt32      */

/*- Search Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_A429_LABEL                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2001L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_A429_MODE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2002L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_A429_PATTERN_DATA            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2003L)      /* ViString     */
#define AGX2K3K_ATTR_SEARCH_SERIAL_A429_PATTERN_SDI             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2004L)      /* ViString     */
#define AGX2K3K_ATTR_SEARCH_SERIAL_FLEXRAY_CYCLE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2005L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_FLEXRAY_DATA                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2006L)      /* ViString     */
#define AGX2K3K_ATTR_SEARCH_SERIAL_FLEXRAY_DATA_LENGTH          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2007L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_FLEXRAY_FRAME                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2008L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_FLEXRAY_MODE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2009L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_M1553_MODE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2010L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_M1553_PATTERN_DATA           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2011L)      /* ViString     */
#define AGX2K3K_ATTR_SEARCH_SERIAL_M1553_RTA                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2012L)      /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_EVENT                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19001L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_PEAK_EXCURSION                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19002L)     /* ViReal64		*/
#define AGX2K3K_ATTR_SEARCH_PEAK_MAXIMUM_PEAKS                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19003L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_PEAK_THRESHOLD                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19004L)     /* ViReal64		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_CAN_SYMBOLIC_MESSAGE         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19005L)     /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_CAN_SYMBOLIC_SIGNAL          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19006L)     /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_CAN_SYMBOLIC_VALUE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19007L)     /* ViReal64		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_LIN_SYMBOLIC_FRAME           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19008L)     /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_LIN_SYMBOLIC_SIGNAL          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19009L)     /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_LIN_SYMBOLIC_VALUE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19010L)     /* ViReal64		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_SENT_MODE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19011L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_SENT_SLOW_DATA               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19012L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_SENT_SLOW_ID                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19013L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_PEAK_SOURCE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19014L)     /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_SENT_FAST_DATA               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19015L)     /* ViString		*/

/*- Save Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_SAVE_MAX_WAVE_ENABLED                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3001L)     /* ViBoolean    */

/*- Demo Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_DEMO_FUNCTION                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4001L)     /* ViInt32      */

/*- Trigger Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_TRIGGER_PATTERN_FORMAT                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5014L)     /* ViInt32      */
#define AGX2K3K_ATTR_TRIGGER_TV_UDTV_ENUMBER                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5019L)     /* ViInt32      */
#define AGX2K3K_ATTR_TRIGGER_TV_UDTV_HSYNC_ENABLED              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5020L)     /* ViBoolean    */
#define AGX2K3K_ATTR_TRIGGER_TV_UDTV_HTIME                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5021L)     /* ViReal64     */
#define AGX2K3K_ATTR_TRIGGER_TV_UDTV_PGTHAN                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5022L)     /* ViReal64     */
#define AGX2K3K_ATTR_TRIGGER_DELAY_ARM_SLOPE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5026L)     /* ViInt32      */
#define AGX2K3K_ATTR_TRIGGER_DELAY_ARM_SOURCE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5027L)     /* ViInt32      */
#define AGX2K3K_ATTR_TRIGGER_DELAY_TDELAY_TIME                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5028L)     /* ViReal64     */
#define AGX2K3K_ATTR_TRIGGER_DELAY_TRIGGER_COUNT                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5029L)     /* ViInt32      */
#define AGX2K3K_ATTR_TRIGGER_DELAY_TRIGGER_SLOPE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5030L)     /* ViInt32      */
#define AGX2K3K_ATTR_TRIGGER_DELAY_TRIGGER_SOURCE               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5031L)     /* ViInt32      */
#define AGX2K3K_ATTR_TRIGGER_ZONE_SOURCE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20001L)    /* ViInt32		*/
#define AGX2K3K_ATTR_TRIGGER_ZONE_STATE_ENABLED                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20002L)    /* ViBoolean		*/
#define AGX2K3K_ATTR_TRIGGER_ZONE_MODE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20003L)    /* ViInt32		*/

#define AGX2K3K_ATTR_MULTICHANNEL_TRIGGER_ZONE_STATE_ENABLED    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20009L)    /* ViBoolean		*/

/*- Marker Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_MARKER_XUNIT                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6001L)     /* ViInt32      */
#define AGX2K3K_ATTR_MARKER_YUNIT                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6002L)     /* ViInt32      */

/*- Function Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_FUNCTION_BUS_CLOCK                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7002L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_BUS_SLOPE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7003L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_BUS_YINCREMENT                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7004L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_BUS_YORIGIN                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7005L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_BUS_YUNITS                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7006L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_FREQ_HIGHPASS                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7007L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_FREQ_LOWPASS                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7008L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_LINEAR_GAIN                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7009L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_LINEAR_OFFSET                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7010L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_TREND_MEASUREMENT                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7011L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_VTYPE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7012L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_INTEGRATE_IOFFSET                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7013L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_BUS_CLOCK_MULTICHANNEL            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7014L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_BUS_SLOPE_MULTICHANNEL            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7015L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_BUS_YUNITS_MULTICHANNEL           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7016L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_TREND_MEASUREMENT_MULTICHANNEL    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7017L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_VTYPE_MULTICHANNEL                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7018L)                   /* ViInt32      */
#define AGX2K3K_ATTR_FUNCTION_BUS_YINCREMENT_MULTICHANNEL       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7019L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_BUS_YORIGIN_MULTICHANNEL          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7020L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_FREQ_HIGHPASS_MULTICHANNEL        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7021L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_FREQ_LOWPASS_MULTICHANNEL         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7022L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_LINEAR_GAIN_MULTICHANNEL          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7023L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_LINEAR_OFFSET_MULTICHANNEL        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7024L)                   /* ViReal64     */
#define AGX2K3K_ATTR_FUNCTION_INTEGRATE_IOFFSET_MULTICHANNEL    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7025L)                   /* ViReal64     */

/*- Measurement Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_MEAS_STAT_RESD_ENABLED                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8001L)     /* ViBoolean    */
#define AGX2K3K_ATTR_MEAS_STAT_MCOUNT                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8002L)     /* ViInt32      */

/*- Power Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_POWER_SIGNALS_AUTOSETUP_TYPE               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9001L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SIGNALS_VOLT_FIRST_SOURCE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9002L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SIGNALS_VOLT_SECOND_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9003L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SIGNALS_CURRENT_FIRST_SOURCE         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9004L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SIGNALS_CURRENT_SECOND_SOURCE        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9005L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SIGNALS_VMAXIMUM                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9006L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_SIGNALS_VSTEADY                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9007L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_SIGNALS_OVERSHOOT                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9008L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SIGNALS_IEXPECTED                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9009L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_SIGNALS_DURATION                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9010L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_SIGNALS_CYCLES                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9011L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_ENABLED                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9012L)         /* ViBoolean    */
#define AGX2K3K_ATTR_POWER_HARMONICS_DISPLAY_STYLE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9013L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_HARMONICS_LINE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9014L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_HARMONICS_STANDARD                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9015L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_MODULATION_SOURCE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9016L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_MODULATION_TYPE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9017L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_ANALYSIS_ENABLED                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9018L)         /* ViBoolean    */
#define AGX2K3K_ATTR_POWER_PSRR_FREQUENCY_MAX                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9019L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_PSRR_FREQUENCY_MIN                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9020L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_PSRR_RMAX                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9021L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_QUALITY_TYPE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9022L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SLEW_SOURCE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9023L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SWITCH_CONDUCTION                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9024L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SWITCH_IREFERENCE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9025L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SWITCH_VREFERENCE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9026L)         /* ViInt32      */
#define AGX2K3K_ATTR_POWER_SWITCH_RDS                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9027L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_SWITCH_VCE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9028L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_TRANSIENT_IINITIAL                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9029L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_TRANSIENT_INEW                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9030L)         /* ViReal64     */
#define AGX2K3K_ATTR_POWER_CLRESPONSE_FREQUENCY_START           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16001L)        /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_CLRESPONSE_FREQUENCY_STOP            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16002L)        /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_CLRESPONSE_YMAXIMUM                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16003L)        /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_CLRESPONSE_YMINIMUM                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16004L)        /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_EFFICIENCY_TYPE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16005L)        /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_REAL_POWER_TYPE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16006L)        /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_REAL_POWER_VALUE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16007L)        /* ViReal64		*/

/*- SBus Group Group Attributes ---------------*/
#define AGX2K3K_ATTR_SBUS_MODE                                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10001L)        /* ViInt32      */
#define AGX2K3K_ATTR_A429_TRIGGER_TYPE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10002L)        /* ViInt32      */
#define AGX2K3K_ATTR_A429_TRIGGER_PATTERN_DATA                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10003L)        /* ViString     */
#define AGX2K3K_ATTR_M1553_TRIGGER_PATTERN_DATA                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10004L)        /* ViString     */
#define AGX2K3K_ATTR_M1553_TRIGGER_RTA                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10005L)        /* ViInt32      */
#define AGX2K3K_ATTR_M1553_TRIGGER_TYPE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10006L)        /* ViInt32      */
#define AGX2K3K_ATTR_A429_TRIGGER_PATTERN_SDI                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10007L)        /* ViString     */
#define AGX2K3K_ATTR_A429_TRIGGER_PATTERN_SSM                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10008L)        /* ViString     */
#define AGX2K3K_ATTR_A429_TRIGGER_LABEL                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10009L)        /* ViReal64     */
#define AGX2K3K_ATTR_A429_BASE                                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10010L)        /* ViInt32      */
#define AGX2K3K_ATTR_M1553_BASE                                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10011L)        /* ViInt32      */
#define AGX2K3K_ATTR_M1553_SOURCE                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10012L)        /* ViInt32      */
#define AGX2K3K_ATTR_A429_FORMAT                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10013L)        /* ViInt32      */
#define AGX2K3K_ATTR_A429_SIGNAL                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10014L)        /* ViInt32      */
#define AGX2K3K_ATTR_A429_SOURCE                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10015L)        /* ViInt32      */
#define AGX2K3K_ATTR_A429_SPEED                                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10016L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_BAUDRATE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10017L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_CHANNEL                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10018L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_NULL_COUNT                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10019L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_SOURCE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10020L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10021L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER_ERROR_TYPE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10022L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER_EVENT_BASS_ID              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10023L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER_EVENT_TYPE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10024L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER_FRAME_CCREPETITION         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10026L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER_FRAME_ID                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10027L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER_FRAME_TYPE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10028L)        /* ViInt32      */
#define AGX2K3K_ATTR_FLEXRAY_TRIGGER_FRAME_CC_BASE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10029L)        /* ViInt32      */
#define AGX2K3K_ATTR_SEARCH_SERIAL_A429_PATTERN_SSM             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10032L)

/* New attributes added in version 3.0 */
/*- IviScope Interpolation Group Attributes ---------------*/
#define AGX2K3K_ATTR_INTERPOLATION               IVISCOPE_ATTR_INTERPOLATION                                    /* ViInt32		*/

/*- Digital Voltmeter Group Attributes ---------------*/
#define AGX2K3K_ATTR_DVM_ENABLED                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10101L)                       /* ViBoolean	*/
#define AGX2K3K_ATTR_DVM_AUTO_RANGE_ENABLED      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10102L)                       /* ViBoolean	*/
#define AGX2K3K_ATTR_DVM_CURRENT                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10103L)                       /* ViReal64		*/
#define AGX2K3K_ATTR_DVM_FREQUENCY               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10104L)                       /* ViReal64		*/
#define AGX2K3K_ATTR_DVM_MODE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10105L)                       /* ViInt32		*/
#define AGX2K3K_ATTR_DVM_ANALOG_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10106L)                       /* ViInt32		*/

/*- Acquisition Group Attributes ---------------*/
#define AGX2K3K_ATTR_REFERENCE_SIGNAL_MODE       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10201L)                       /* ViInt32		*/

/*- Function Group Attributes ---------------*/
#define AGX2K3K_ATTR_FUNCTION_VERTICAL_AXIS      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10301L)                       /* ViReal64		*/
#define AGX2K3K_ATTR_FUNCTION_REFERENCE_LEVEL    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10302L)                       /* ViReal64		*/
#define AGX2K3K_ATTR_FUNCTION_NUM_AVERAGES       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10303L)                       /* ViInt32		*/


/*- Measurement Group Attributes ---------------*/
#define AGX2K3K_ATTR_MEASURE_POWER_PHASE_ANGLE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10501L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_APPARENT_POWER                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10502L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_AREA                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10503L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_POWER_LOSS_PER_CYCLE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10504L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_CREST_FACOTR                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10505L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_ENERGY_LOSS                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10506L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_POWER_FACTOR                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10507L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_INPUT_POWER                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10508L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_OFF_TIME                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10509L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_ON_TIME                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10510L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_OUTPUT_POWER                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10511L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_POWER_LOSS                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10512L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_REACTIVE_POWER                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10513L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_REAL_POWER                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10514L)         /* ViReal64		*/
#define AGX2K3K_ATTR_MEASURE_CONTINUOUSLY_DISPLAYED_RESULTS    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10515L)         /* ViString		*/
#define AGX2K3K_ATTR_MEASURE_INFORMATION_TYPE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10516L)         /* ViInt32		*/

/*- Power Group Attributes ---------------*/

#define AGX2K3K_ATTR_POWER_HARMONICS_TABLE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10601L)         /* ViString		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_FAIL_COUNT                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10602L)         /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_POWER_FACTOR              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10603L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_RUN_ITERATION_COUNT       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10604L)         /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_FAIL_PASS_STATUS          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10605L)         /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_TOTAL_DISTORTION          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10606L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_HARMONICS_CYCLE_COUNT               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10607L)         /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_QUALITY_CYCLE_COUNT                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10608L)         /* ViInt32		*/
#define AGX2K3K_ATTR_POWER_EFFICIENCY_DURATION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10609L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_MODULATION_DURATION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10610L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_OFF_DURAQTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10611L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_ON_DURATION                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10612L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_RIPPLE_DURATION                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10613L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_TRANSIENT_DURATION                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10614L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_IRUSH_AMPLITUDE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10615L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_IRUSH_MAX_VOLTAGE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10616L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_OFF_MAX_VOLTAGE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10617L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_ON_MAX_VOLTAGE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10618L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_OFF_DC_VOLTAGE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10619L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_ON_DC_VOLTAGE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10620L)         /* ViReal64		*/
#define AGX2K3K_ATTR_POWER_TRANSIENT_DC_VOLTAGE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10621L)         /* ViReal64		*/

/*- Serial Bus Subsystem Group Attributes ---------------*/
#define AGX2K3K_ATTR_SBUS_A429_ERROR_COUNT                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10701L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_A429_WORD_COUNT                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10702L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_A429_TRIGGER_RANGE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10703L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_FLEX_SYNC_FRAME_COUNT                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10704L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_FLEX_TOTAL_FRAME_COUNT               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10705L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_FLEX_EVENT_TRIGGER_MODE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10706L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_BASE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10707L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_DIFFERENTIAL_SOURCE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10708L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_D_MINUS_SOURCE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10709L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_D_PLUS_SOURCE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10710L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_SPEED                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10711L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_MODE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10712L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_ADDRESS_PORTION          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10713L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_CRC_PORTION              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10714L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_DATA_PORTION             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10715L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_DATA_LENGTH              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10716L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_ENDPOINT_PORTION         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10717L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_ET_PORTION               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10718L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_FRAME_PORTION            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10719L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_HUB_ADDRESS_PORTION      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10720L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_PID_CHECK_PORTION        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10721L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_DATA_PID                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10722L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_HANDSHAKE_PID            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10723L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_SPECIAL_PID              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10724L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_TOKEN_PID                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10725L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_PORT_PORTION             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10726L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_SC_PORTION               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10727L)         /* ViString		*/
#define AGX2K3K_ATTR_SBUS_USB_TRIGGER_SEU_PORTION              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10728L)         /* ViString		*/

/*- Search Group Attributes ---------------*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_ADDRESS_PORTION         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10801L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_CRC_PORTION             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10802L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_DATA_PORTION            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10803L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_DATA_LENGTH             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10804L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_ENDPOINT_PORTION        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10805L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_ET_PORTION              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10806L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_FRAME_PORTION           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10807L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_HUB_ADDRESS_PORTION     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10808L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_SEARCH_MODE             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10809L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_DATA_PID                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10810L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_HANDSHAKE_PID           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10811L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_SPECIAL_PID             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10812L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_TOKEN_PID               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10813L)         /* ViInt32		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_PORT_PORTION            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10814L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_SC_PORTION              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10815L)         /* ViString		*/
#define AGX2K3K_ATTR_SEARCH_SERIAL_USB_SEU_PORTION             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10816L)         /* ViString		*/

/*- WGEN Group Attributes ---------------*/
#define AGX2K3K_ATTR_WAVEGEN_DOWNLOAD_FLOAT_ARB_WAVEFORM       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10902L)         /* ViString		*/
#define AGX2K3K_ATTR_WAVEGEN_ARB_WAVEFORM_POINT_COUNT          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10903L)         /* ViInt32		*/
#define AGX2K3K_ATTR_WAVEGEN_DOWNLOAD_DAC_ARB_WAVEFORM         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10904L)         /* ViString		*/
#define AGX2K3K_ATTR_WAVEGEN_STORE_ARB_WAVEFORM                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10905L)         /* ViInt32		*/
#define AGX2K3K_ATTR_WAVEGEN_AM_MODULATION_COUNT               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10906L)         /* ViInt32		*/
#define AGX2K3K_ATTR_WAVEGEN_AM_MODULATION_FREQUENCY           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10907L)         /* ViReal64		*/
#define AGX2K3K_ATTR_WAVEGEN_FM_MODULATION_DEVIATION           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10908L)         /* ViReal64		*/
#define AGX2K3K_ATTR_WAVEGEN_FM_MODULATION_FREQUENCY           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10909L)         /* ViReal64		*/
#define AGX2K3K_ATTR_WAVEGEN_FSKEY_MODULATION_HOP_FREQUENCY    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10910L)         /* ViReal64		*/
#define AGX2K3K_ATTR_WAVEGEN_FSKEY_MODULATION_RATE             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10911L)         /* ViReal64		*/
#define AGX2K3K_ATTR_WAVEGEN_MODULATION_SIGNAL_SHAPE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10912L)         /* ViInt32		*/
#define AGX2K3K_ATTR_WAVEGEN_MODULATION_RAMP_SYMMETRY          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10913L)         /* ViInt32		*/
#define AGX2K3K_ATTR_WAVEGEN_MODULATION_ENABLED                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10914L)         /* ViBoolean	*/
#define AGX2K3K_ATTR_WAVEGEN_MODULATION_TYPE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10915L)         /* ViInt32		*/
#define AGX2K3K_ATTR_WAVEGEN_OUTPUT_POLARITY_INVERTED          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10916L)         /* ViBoolean	*/
#define AGX2K3K_ATTR_WAVEGEN_TRACKING_ENABLED                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10917L)         /* ViBoolean	*/
#define AGX2K3K_ATTR_WAVEGEN_AMPLITUDE_TRACKING_ENABLED        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10918L)         /* ViBoolean	*/
#define AGX2K3K_ATTR_WAVEGEN_FREQUENCY_TRACKING_ENABLED        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10919L)         /* ViBoolean	*/
#define AGX2K3K_ATTR_WAVEGEN_OUTPUT_MODE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 11001L)         /* ViInt32		*/
#define AGX2K3K_ATTR_WAVEGEN_TRACK_PHASE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 11002L)         /* ViInt32		*/

/*- Compliance Group Attributes ---------------*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_HUBS                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12001L)             /* ViInt32		*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_ADJACENT            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12002L)             /* ViInt32		*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12003L)             /* ViInt32		*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_TEST_CONNECTION            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12004L)             /* ViInt32		*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_TEST_TYPE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12005L)             /* ViInt32		*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_DIFFERENTIAL        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12006L)             /* ViInt32		*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_DMINUS              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12007L)             /* ViInt32		*/
#define AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_DPLUS               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12008L)             /* ViInt32		*/

/*- Counter Group Attributes ---------------*/
#define AGX2K3K_ATTR_COUNTER_ENABLE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5001L) /* ViBoolean		*/
#define AGX2K3K_ATTR_COUNTER_MODE                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5002L) /* ViInt32		*/
#define AGX2K3K_ATTR_COUNTER_NUMBER_OF_DIGITS                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5003L) /* ViInt32		*/
#define AGX2K3K_ATTR_COUNTER_SOURCE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5004L) /* ViInt32		*/
#define AGX2K3K_ATTR_COUNTER_TOTALIZE_GATE_ENABLED             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5005L) /* ViBoolean		*/
#define AGX2K3K_ATTR_COUNTER_TOTALIZE_GATE_POLARITY            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5006L) /* ViInt32		*/
#define AGX2K3K_ATTR_COUNTER_TOTALIZE_GATE_SOURCE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5007L) /* ViInt32		*/
#define AGX2K3K_ATTR_COUNTER_TOTALIZE_SLOPE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5008L) /* ViInt32		*/

/*- FFT Group Attributes ---------------*/
#define AGX2K3K_ATTR_FFT_AVERAGE_COUNT                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13001L) /* ViInt32		*/
#define AGX2K3K_ATTR_FFT_FREQUENCY_CENTER                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13002L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_DISPLAY_ENABLED                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13003L) /* ViBoolean		*/
#define AGX2K3K_ATTR_FFT_DISPLAY_MODE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13004L) /* ViInt32		*/
#define AGX2K3K_ATTR_FFT_FREQUENCY_START                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13005L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_FREQUENCY_STOP                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13006L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_VERTICAL_OFFSET                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13007L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_VERTICAL_RANGE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13008L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_VERTICAL_REFERENCE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13009L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_VERTICAL_SCALE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13010L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_SOURCE1                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13011L) /* ViInt32		*/
#define AGX2K3K_ATTR_FFT_FREQUENCY_SPAN                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13012L) /* ViReal64		*/
#define AGX2K3K_ATTR_FFT_VERTICAL_UNIT                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13013L) /* ViInt32		*/
#define AGX2K3K_ATTR_FFT_WINDOW                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13014L) /* ViInt32		*/

/* New attributes added in version 3.2 */
/*- Channel Group Attributes ---------------*/
#define AGX2K3K_ATTR_CHANNEL_BANDWIDTH                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1200L) /* ViReal64		*/

/*- Histogram Group Attributes ---------------*/
#define AGX2K3K_ATTR_HISTOGRAM_MEASUREMENT                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1201L) /* ViInt32		*/
#define AGX2K3K_ATTR_HISTOGRAM_MODE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1202L) /* ViInt32		*/
#define AGX2K3K_ATTR_HISTOGRAM_AXIS                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1203L) /* ViInt32		*/
#define AGX2K3K_ATTR_HISTOGRAM_WINDOW_BLIMIT                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1204L) /* ViReal64		*/
#define AGX2K3K_ATTR_HISTOGRAM_WINDOW_LLIMIT                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1205L) /* ViReal64		*/
#define AGX2K3K_ATTR_HISTOGRAM_WINDOW_RLIMIT                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1206L) /* ViReal64		*/
#define AGX2K3K_ATTR_HISTOGRAM_WINDOW_TLIMIT                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1207L) /* ViReal64		*/
#define AGX2K3K_ATTR_HISTOGRAM_WINDOW_SOURCE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1208L) /* ViString		*/

/*- Jitter Group Attributes ---------------*/
#define AGX2K3K_ATTR_JITTER_MEASUREMENT                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1209L) /* ViInt32		*/
#define AGX2K3K_ATTR_JITTER_ENABLE                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1210L) /* ViBoolean		*/

/*- Real Time Eye Group Attributes ---------------*/
#define AGX2K3K_ATTR_RTEYE_SOURCE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1211L) /* ViString		*/
#define AGX2K3K_ATTR_RTEYE_ENABLE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1212L) /* ViBoolean		*/

/*- Save Group Attributes ---------------*/
#define AGX2K3K_ATTR_SAVE_RESULTS_FORMAT_CGRADE_ENABLED       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1213L) /* ViBoolean		*/
#define AGX2K3K_ATTR_SAVE_RESULTS_FORMAT_HISTOGRAM_ENABLED    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1214L) /* ViBoolean		*/

/*- System Group Attributes ---------------*/
#define AGX2K3K_ATTR_SYSTEM_PRECISION_ENABLED                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1215L) /* ViBoolean		*/
#define AGX2K3K_ATTR_SYSTEM_PRECISION_LENGTH                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1216L) /* ViInt32		*/

/*- Trigger Group Attributes ---------------*/
#define AGX2K3K_ATTR_TRIGGER_JFREE_ENABLED                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1217L) /* ViBoolean		*/

/*- CAN Bus Group Attributes ---------------*/
#define AGX2K3K_ATTR_SBUS_CAN_FDSTANDARD                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1218L) /* ViInt32		*/

/*- Waveform Group Attributes ---------------*/
#define AGX2K3K_ATTR_WAVEFORM_SOURCE_SUBSOURCE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1219L)   /* ViInt32		*/

/****************************************************************************
 *------------------------ Attribute Value Defines -------------------------*
 ****************************************************************************/
/*- AGX2K3K_ATTR_PROBE_ATTENUATION Values -*/
#define AGX2K3K_VAL_PROBE_SENSE_ON        IVISCOPE_VAL_PROBE_SENSE_ON

/*- AGX2K3K_ATTR_VERTICAL_COUPLING Values -*/
#define AGX2K3K_VAL_AC                    IVISCOPE_VAL_AC
#define AGX2K3K_VAL_DC                    IVISCOPE_VAL_DC

/*- AGX2K3K_ATTR_HORZ_RECORD_MODE Values -*/
#define AGX2K3K_VAL_RECORD_NORMAL         0
#define AGX2K3K_VAL_RECORD_MAX            1
#define AGX2K3K_VAL_RECORD_RAW            2

/*- AGX2K3K_ATTR_TRIGGER_TYPE Values -*/
#define AGX2K3K_VAL_EDGE_TRIGGER          IVISCOPE_VAL_EDGE_TRIGGER
#define AGX2K3K_VAL_GLITCH_TRIGGER        IVISCOPE_VAL_GLITCH_TRIGGER
#define AGX2K3K_VAL_RUNT_TRIGGER          IVISCOPE_VAL_RUNT_TRIGGER
#define AGX2K3K_VAL_TV_TRIGGER            IVISCOPE_VAL_TV_TRIGGER
#define AGX2K3K_VAL_PATTERN_TRIGGER       (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 1L)
#define AGX2K3K_VAL_EDGE_BURST_TRIGGER    (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 2L)
#define AGX2K3K_VAL_SETUP_HOLD_TRIGGER    (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 3L)
#define AGX2K3K_VAL_TRANSITION_TRIGGER    (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 4L)
#define AGX2K3K_VAL_USB_TRIGGER           (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 8L)
#define AGX2K3K_VAL_DELAY_TRIGGER         (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 9L)
#define AGX2K3K_VAL_OR_TRIGGER            (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 10L)
#define AGX2K3K_VAL_SBUS1_TRIGGER         (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 11L)
#define AGX2K3K_VAL_SBUS2_TRIGGER         (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 12L)
/*- Source Values -*/
#define AGX2K3K_VAL_CHANNEL1              "CHAN1"
#define AGX2K3K_VAL_CHANNEL2              "CHAN2"
#define AGX2K3K_VAL_CHANNEL3              "CHAN3"
#define AGX2K3K_VAL_CHANNEL4              "CHAN4"
#define AGX2K3K_VAL_LINE                  "LINE"
#define AGX2K3K_VAL_MATH                  "FUNC"
#define AGX2K3K_VAL_NONE                  "NONE"
#define AGX2K3K_VAL_MATH_SOURCE1_GOFT     "GOFT"
#define AGX2K3K_VAL_MATH_SOURCE2_NONE     "NONE"

/*- AGX2K3K_ATTR_TRIGGER_MODIFIER Values -*/
#define AGX2K3K_VAL_AUTO                  IVISCOPE_VAL_AUTO          /* default */
#define AGX2K3K_VAL_NO_TRIGGER_MOD        IVISCOPE_VAL_NO_TRIGGER_MOD

/*- AGX2K3K_ATTR_TRIGGER_COUPLING Values -*/
/* #define AGX2K3K_VAL_AC                 DEFINED ABOVE */
/* #define AGX2K3K_VAL_DC                 DEFINED ABOVE */
#define AGX2K3K_VAL_LF_REJECT         IVISCOPE_VAL_LF_REJECT

/*- AGX2K3K_ATTR_TRIGGER_REJECT_FILTER values -*/
#define AGX2K3K_VAL_REJ_FILTER_OFF    0
/* #define AGX2K3K_VAL_LF_REJECT          DEFINED ABOVE */
#define AGX2K3K_VAL_HF_REJECT         IVISCOPE_VAL_HF_REJECT

/*- AGX2K3K_ATTR_TRIGGER_SLOPE Values -*/
#define AGX2K3K_VAL_POSITIVE          IVISCOPE_VAL_POSITIVE
#define AGX2K3K_VAL_NEGATIVE          IVISCOPE_VAL_NEGATIVE
#define AGX2K3K_VAL_EITHER            (102L)
#define AGX2K3K_VAL_ALTERNATE         (103L)

/*- AGX2K3K_ATTR_EBURST_SLOPE -*/
/* #define AGX2K3K_VAL_POSITIVE           DEFINED ABOVE */
/* #define AGX2K3K_VAL_NEGATIVE           DEFINED ABOVE */

/*- AGX2K3K_ATTR_TV_TRIGGER_SIGNAL_FORMAT Values -*/
#define AGX2K3K_VAL_NTSC                      IVISCOPE_VAL_NTSC
#define AGX2K3K_VAL_PALM                      (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE + 2L)
#define AGX2K3K_VAL_PAL                       IVISCOPE_VAL_PAL
#define AGX2K3K_VAL_SECAM                     IVISCOPE_VAL_SECAM

/*- AGX2K3K_ATTR_TV_TRIGGER_EVENT Values -*/
#define AGX2K3K_VAL_TV_EVENT_FIELD1           IVISCOPE_VAL_TV_EVENT_FIELD1
#define AGX2K3K_VAL_TV_EVENT_FIELD2           IVISCOPE_VAL_TV_EVENT_FIELD2
#define AGX2K3K_VAL_TV_EVENT_ANY_FIELD        IVISCOPE_VAL_TV_EVENT_ANY_FIELD
#define AGX2K3K_VAL_TV_EVENT_ANY_LINE         IVISCOPE_VAL_TV_EVENT_ANY_LINE
#define AGX2K3K_VAL_TV_EVENT_LFIELD1          (IVISCOPE_VAL_TV_TRIGGER_EVENT_SPECIFIC_EXT_BASE + 2L)
#define AGX2K3K_VAL_TV_EVENT_LFIELD2          (IVISCOPE_VAL_TV_TRIGGER_EVENT_SPECIFIC_EXT_BASE + 3L)
#define AGX2K3K_VAL_TV_EVENT_LALTERNATE       (IVISCOPE_VAL_TV_TRIGGER_EVENT_SPECIFIC_EXT_BASE + 4L)
#define AGX2K3K_VAL_TV_EVENT_LINE_NUMBER      IVISCOPE_VAL_TV_EVENT_LINE_NUMBER /* This value is not supported by this instrument */

/*- AGX2K3K_ATTR_RUNT_POLARITY -*/
#define AGX2K3K_VAL_RUNT_POSITIVE             IVISCOPE_VAL_RUNT_POSITIVE
#define AGX2K3K_VAL_RUNT_NEGATIVE             IVISCOPE_VAL_RUNT_NEGATIVE
#define AGX2K3K_VAL_RUNT_EITHER               IVISCOPE_VAL_RUNT_EITHER

/*- AGX2K3K_ATTR_RUNT_QUALIFIER -*/
#define AGX2K3K_VAL_RUNT_QUALIFIER_GREATER    0
#define AGX2K3K_VAL_RUNT_QUALIFIER_LESS       1
#define AGX2K3K_VAL_RUNT_QUALIFIER_NONE       2

/*- AGX2K3K_ATTR_SETUPHOLD_SLOPE -*/
/* #define AGX2K3K_VAL_POSITIVE           DEFINED ABOVE */
/* #define AGX2K3K_VAL_NEGATIVE           DEFINED ABOVE */

/*- AGX2K3K_ATTR_TRANSITION_QUALIFIER -*/
#define AGX2K3K_VAL_TRANSITION_QUALIFIER_GREATER    0
#define AGX2K3K_VAL_TRANSITION_QUALIFIER_LESS       1

/*- AGX2K3K_ATTR_TRANSITION_SLOPE -*/
/* #define AGX2K3K_VAL_POSITIVE           DEFINED ABOVE */
/* #define AGX2K3K_VAL_NEGATIVE           DEFINED ABOVE */

/* Instrument specific attribute value definitions */

/*- Defined values for AGX2K3K_ATTR_TV_TRIGGER_POLARITY -*/
#define AGX2K3K_VAL_TV_POSITIVE                         IVISCOPE_VAL_TV_POSITIVE
#define AGX2K3K_VAL_TV_NEGATIVE                         IVISCOPE_VAL_TV_NEGATIVE

/*- Defined values for AGX2K3K_ATTR_GLITCH_CONDITION -*/
#define AGX2K3K_VAL_GLITCH_LESS_THAN                    IVISCOPE_VAL_GLITCH_LESS_THAN
#define AGX2K3K_VAL_GLITCH_GREATER_THAN                 IVISCOPE_VAL_GLITCH_GREATER_THAN
#define AGX2K3K_VAL_GLITCH_RANGE                        (103L)

/*- Defined values for AGX2K3K_ATTR_GLITCH_POLARITY -*/
#define AGX2K3K_VAL_GLITCH_POSITIVE                     IVISCOPE_VAL_GLITCH_POSITIVE
#define AGX2K3K_VAL_GLITCH_NEGATIVE                     IVISCOPE_VAL_GLITCH_NEGATIVE

/*- AGX2K3K_ATTR_ACQUISITION_TYPE Values -*/
#define AGX2K3K_VAL_NORMAL                              IVISCOPE_VAL_NORMAL /* default */
#define AGX2K3K_VAL_PEAK_DETECT                         IVISCOPE_VAL_PEAK_DETECT
#define AGX2K3K_VAL_AVERAGE                             IVISCOPE_VAL_AVERAGE
#define AGX2K3K_VAL_HI_RES                              IVISCOPE_VAL_HI_RES

/*- AGX2K3K_ATTR_SAMPLE_MODE Values -*/
#define AGX2K3K_VAL_REAL_TIME                           IVISCOPE_VAL_REAL_TIME
#define AGX2K3K_VAL_EQUIVALENT_TIME                     IVISCOPE_VAL_EQUIVALENT_TIME

/*- AGX2K3K_ATTR_SBUS_CAN_SIGNAL Values -*/
#define AGX2K3K_VAL_CAN_SIGNAL_RX                       0
#define AGX2K3K_VAL_CAN_SIGNAL_TX                       1
#define AGX2K3K_VAL_CAN_SIGNAL_CAN_H                    2
#define AGX2K3K_VAL_CAN_SIGNAL_CAN_L                    3
#define AGX2K3K_VAL_CAN_SIGNAL_DIFFERENTIAL             4

/*- AGX2K3K_ATTR_SBUS_CAN_TRIGGER Values -*/
#define AGX2K3K_VAL_CAN_TRIGGER_SOF                     0
#define AGX2K3K_VAL_CAN_TRIGGER_DATA                    1
#define AGX2K3K_VAL_CAN_TRIGGER_ERROR                   2
#define AGX2K3K_VAL_CAN_TRIGGER_IDDATA                  3
#define AGX2K3K_VAL_CAN_TRIGGER_IDEITHER                4
#define AGX2K3K_VAL_CAN_TRIGGER_IDREMOTE                5
#define AGX2K3K_VAL_CAN_TRIGGER_ALL_ERRORS              6
#define AGX2K3K_VAL_CAN_TRIGGER_OVERLOAD                7
#define AGX2K3K_VAL_CAN_TRIGGER_ACK_ERROR               8
#define AGX2K3K_VAL_CAN_TRIGGER_EOF                     9
#define AGX2K3K_VAL_CAN_TRIGGER_FDDATA                  10
#define AGX2K3K_VAL_CAN_TRIGGER_FORMERROR               11
#define AGX2K3K_VAL_CAN_TRIGGER_STUFFERROR              12
#define AGX2K3K_VAL_CAN_TRIGGER_CRCERROR                13
#define AGX2K3K_VAL_CAN_TRIGGER_SPECERROR               14
#define AGX2K3K_VAL_CAN_TRIGGER_BRSBIT                  15
#define AGX2K3K_VAL_CAN_TRIGGER_CRCDBIT                 16
#define AGX2K3K_VAL_CAN_TRIGGER_EBACTIVE                17
#define AGX2K3K_VAL_CAN_TRIGGER_EBPASSIVE               18
#define AGX2K3K_VAL_CAN_TRIGGER_MESSAGE                 19
#define AGX2K3K_VAL_CAN_TRIGGER_MSIGNAL                 20
#define AGX2K3K_VAL_CAN_TRIGGER_FDMSIGNAL               21

/*- AGX2K3K_ATTR_SBUS_CAN_ID_MODE -*/
#define AGX2K3K_VAL_CAN_ID_MODE_STANDARD                0
#define AGX2K3K_VAL_CAN_ID_MODE_EXTENDED                1

/*- AGX2K3K_ATTR_SBUS_IIC_SIZE -*/
#define AGX2K3K_VAL_IIC_SIZE_BIT7                       0
#define AGX2K3K_VAL_IIC_SIZE_BIT8                       1

/*- AGX2K3K_ATTR_SBUS_I2S_ALIGNMENT -*/
#define AGX2K3K_VAL_I2S_ALIGNMENT_I2S                   0
#define AGX2K3K_VAL_I2S_ALIGNMENT_LJ                    1
#define AGX2K3K_VAL_I2S_ALIGNMENT_RJ                    2

/*- AGX2K3K_ATTR_SBUS_I2S_CLOCK_SLOPE -*/
#define AGX2K3K_VAL_I2S_CLOCK_SLOPE_NEGATIVE            0
#define AGX2K3K_VAL_I2S_CLOCK_SLOPE_POSITIVE            1

/*- AGX2K3K_ATTR_SBUS_I2S_TRIGGER -*/
#define AGX2K3K_VAL_I2S_TRIGGER_EQUAL                   0
#define AGX2K3K_VAL_I2S_TRIGGER_NOT_EQUAL               1
#define AGX2K3K_VAL_I2S_TRIGGER_LESS                    2
#define AGX2K3K_VAL_I2S_TRIGGER_GREATER                 3
#define AGX2K3K_VAL_I2S_TRIGGER_IN_RANGE                4
#define AGX2K3K_VAL_I2S_TRIGGER_OUT_RANGE               5
#define AGX2K3K_VAL_I2S_TRIGGER_INCREASE                6
#define AGX2K3K_VAL_I2S_TRIGGER_DECREASE                7

/*- AGX2K3K_ATTR_SBUS_I2S_TRIGGER_AUDIO -*/
#define AGX2K3K_VAL_I2S_TRIGGER_AUDIO_RIGHT             0
#define AGX2K3K_VAL_I2S_TRIGGER_AUDIO_LEFT              1
#define AGX2K3K_VAL_I2S_TRIGGER_AUDIO_EITHER            2

/*- AGX2K3K_ATTR_SBUS_I2S_WS_LOW -*/
#define AGX2K3K_VAL_I2S_WS_LOW_LEFT                     0
#define AGX2K3K_VAL_I2S_WS_LOW_RIGHT                    1

/*- AGX2K3K_ATTR_SBUS_UART_BIT_ORDER -*/
#define AGX2K3K_VAL_UART_BIT_ORDER_LSB_FIRST            0
#define AGX2K3K_VAL_UART_BIT_ORDER_MSB_FIRST            1

/*- AGX2K3K_ATTR_SBUS_UART_PARITY -*/
#define AGX2K3K_VAL_UART_PARITY_EVEN                    0
#define AGX2K3K_VAL_UART_PARITY_ODD                     1
#define AGX2K3K_VAL_UART_PARITY_NONE                    2

/*- AGX2K3K_ATTR_SBUS_UART_POLARITY -*/
#define AGX2K3K_VAL_UART_POLARITY_HIGH                  0
#define AGX2K3K_VAL_UART_POLARITY_LOW                   1

/*- AGX2K3K_ATTR_SBUS_UART_TRIGGER_QUALIFIER -*/
#define AGX2K3K_VAL_UART_TRIGGER_QUALIFIER_EQUAL        0
#define AGX2K3K_VAL_UART_TRIGGER_QUALIFIER_NOT_EQUAL    1
#define AGX2K3K_VAL_UART_TRIGGER_QUALIFIER_GREATER      2
#define AGX2K3K_VAL_UART_TRIGGER_QUALIFIER_LESS         3

/*- AGX2K3K_ATTR_SBUS_UART_TRIGGER_TYPE -*/
#define AGX2K3K_VAL_UART_TRIGGER_RSTART                 0
#define AGX2K3K_VAL_UART_TRIGGER_RSTOP                  1
#define AGX2K3K_VAL_UART_TRIGGER_RDATA                  2
#define AGX2K3K_VAL_UART_TRIGGER_RD1                    3
#define AGX2K3K_VAL_UART_TRIGGER_RD0                    4
#define AGX2K3K_VAL_UART_TRIGGER_RDX                    5
#define AGX2K3K_VAL_UART_TRIGGER_PARITY_ERROR           6
#define AGX2K3K_VAL_UART_TRIGGER_TSTART                 7
#define AGX2K3K_VAL_UART_TRIGGER_TSTOP                  8
#define AGX2K3K_VAL_UART_TRIGGER_TDATA                  9
#define AGX2K3K_VAL_UART_TRIGGER_TD1                    10
#define AGX2K3K_VAL_UART_TRIGGER_TD0                    11
#define AGX2K3K_VAL_UART_TRIGGER_TDX                    12

/*- AGX2K3K_ATTR_SBUS_LIN_STANDARD -*/
#define AGX2K3K_VAL_LIN_STANDARD_LIN13                  0
#define AGX2K3K_VAL_LIN_STANDARD_LIN20                  1

/*- AGX2K3K_ATTR_SBUS_LIN_SYNC_BREAK -*/
#define AGX2K3K_VAL_LIN_SYNC_BREAK_11_CLOCK             0
#define AGX2K3K_VAL_LIN_SYNC_BREAK_12_CLOCK             1
#define AGX2K3K_VAL_LIN_SYNC_BREAK_13_CLOCK             2

/*- AGX2K3K_ATTR_PATTERN_QUALIFIER Values -*/
#define AGX2K3K_VAL_PATTERN_ENTERED                     0
#define AGX2K3K_VAL_PATTERN_GREATERTHAN                 1
#define AGX2K3K_VAL_PATTERN_LESSTHAN                    2
#define AGX2K3K_VAL_PATTERN_INRANGE                     3
#define AGX2K3K_VAL_PATTERN_OUTRANGE                    4
#define AGX2K3K_VAL_PATTERN_TIMEOUT                     5

/*- AGX2K3K_ATTR_IIC_TYPE Values -*/
#define AGX2K3K_VAL_IIC_TYPE_START                      0
#define AGX2K3K_VAL_IIC_TYPE_STOP                       1
#define AGX2K3K_VAL_IIC_TYPE_NACK                       2
#define AGX2K3K_VAL_IIC_TYPE_RESTART                    3
#define AGX2K3K_VAL_IIC_TYPE_READEEPROM                 4
#define AGX2K3K_VAL_IIC_TYPE_READ7                      5
#define AGX2K3K_VAL_IIC_TYPE_WRITE7                     6
#define AGX2K3K_VAL_IIC_TYPE_WRITE10                    7
#define AGX2K3K_VAL_IIC_TYPE_READ7_DATA2                8
#define AGX2K3K_VAL_IIC_TYPE_WRITE7_DATA2               9
#define AGX2K3K_VAL_IIC_TYPE_ADDRESS_NACK               10

/*- AGX2K3K_ATTR_IIC_QUALIFIER Values -*/
#define AGX2K3K_VAL_IIC_QUALIFIER_EQUAL                 0
#define AGX2K3K_VAL_IIC_QUALIFIER_NEQUAL                1
#define AGX2K3K_VAL_IIC_QUALIFIER_LESSTHAN              2
#define AGX2K3K_VAL_IIC_QUALIFIER_GREATERTHAN           3

/*- AGX2K3K_ATTR_LIN_TRIGGER Values -*/
#define AGX2K3K_VAL_LIN_TRIGGER_SYNCBREAK               0
#define AGX2K3K_VAL_LIN_TRIGGER_ID                      1
#define AGX2K3K_VAL_LIN_TRIGGER_DATA                    2
#define AGX2K3K_VAL_LIN_TRIGGER_PARITYERROR             3
#define AGX2K3K_VAL_LIN_TRIGGER_CSUMERROR               4
#define AGX2K3K_VAL_LIN_TRIGGER_FRAME                   5
#define AGX2K3K_VAL_LIN_TRIGGER_FSIGNAL                 6

/*- AGX2K3K_ATTR_USB_TRIGGER Values -*/
#define AGX2K3K_VAL_USB_TRIGGER_SOP                     0
#define AGX2K3K_VAL_USB_TRIGGER_EOP                     1
#define AGX2K3K_VAL_USB_TRIGGER_RES                     2
#define AGX2K3K_VAL_USB_TRIGGER_ENT                     3
#define AGX2K3K_VAL_USB_TRIGGER_EXIT                    4

/*- AGX2K3K_ATTR_USB_SPEED Values -*/
#define AGX2K3K_VAL_USB_SPEED_LOW                       0
#define AGX2K3K_VAL_USB_SPEED_FULL                      1

/*- AGX2K3K_ATTR_SPI_SLOPE Values -*/
#define AGX2K3K_VAL_SPI_SLOPE_NEGATIVE                  0
#define AGX2K3K_VAL_SPI_SLOPE_POSITIVE                  1

/*- AGX2K3K_ATTR_SBUS_SPI_TRIGGER_TYPE -*/
#define AGX2K3K_VAL_SPI_TRIGGER_MOSI                    0
#define AGX2K3K_VAL_SPI_TRIGGER_MISO                    1

/*- AGX2K3K_ATTR_SBUS_SPI_FRAME Values -*/
#define AGX2K3K_VAL_SPI_FRAME_NCHIP                     0
#define AGX2K3K_VAL_SPI_FRAME_CHIP                      1
#define AGX2K3K_VAL_SPI_FRAME_TIMEOUT                   2

/*- AGX2K3K_ATTR_SBUS_SPI_BIT_ORDER -*/
#define AGX2K3K_VAL_SPI_BITORDER_MSB_FIRST              0
#define AGX2K3K_VAL_SPI_BITORDER_LSB_FIRST              1

/*- AGX2K3K_ATTR_ACQUISITION_MODE -*/
#define AGX2K3K_VAL_ACQ_MODE_REALTIME                   0
#define AGX2K3K_VAL_ACQ_MODE_SEGMENT                    1

/*- AGX2K3K_ATTR_PROBE_UNITS -*/
#define AGX2K3K_VAL_PROBE_UNITS_VOLTS                   0
#define AGX2K3K_VAL_PROBE_UNITS_AMPS                    1

/*- AGX2K3K_ATTR_MATH_OPERATION -*/
#define AGX2K3K_VAL_MATH_OPERATION_ADD                  0
#define AGX2K3K_VAL_MATH_OPERATION_SUBTRACT             1
#define AGX2K3K_VAL_MATH_OPERATION_MULTIPLY             2
#define AGX2K3K_VAL_MATH_OPERATION_INTEGRATE            3
#define AGX2K3K_VAL_MATH_OPERATION_DIFFERENTIATE        4
#define AGX2K3K_VAL_MATH_OPERATION_FFT                  5
#define AGX2K3K_VAL_MATH_OPERATION_SQUARE_ROOT          6
#define AGX2K3K_VAL_MATH_OPERATION_GOFT_PLUS            7
#define AGX2K3K_VAL_MATH_OPERATION_GOFT_MINUS           8
#define AGX2K3K_VAL_MATH_OPERATION_GOFT_MULTIPLY        9
#define AGX2K3K_VAL_MATH_OPERATION_NONE                 10
#define AGX2K3K_VAL_MATH_OPERATION_MAGNIFY              11
#define AGX2K3K_VAL_MATH_OPERATION_ABSOLUTE             12
#define AGX2K3K_VAL_MATH_OPERATION_SQUARE               13
#define AGX2K3K_VAL_MATH_OPERATION_LN                   14
#define AGX2K3K_VAL_MATH_OPERATION_LOG                  15
#define AGX2K3K_VAL_MATH_OPERATION_EXP                  16
#define AGX2K3K_VAL_MATH_OPERATION_TEN                  17
#define AGX2K3K_VAL_MATH_OPERATION_LOWPASS              18
#define AGX2K3K_VAL_MATH_OPERATION_HIGHPASS             19
#define AGX2K3K_VAL_MATH_OPERATION_DIVIDE               20
#define AGX2K3K_VAL_MATH_OPERATION_LINEAR               21
#define AGX2K3K_VAL_MATH_OPERATION_TREND                22
#define AGX2K3K_VAL_MATH_OPERATION_BTIMING              23
#define AGX2K3K_VAL_MATH_OPERATION_BSTATE               24
#define AGX2K3K_VAL_MATH_OPERATION_AVERAGE              25
#define AGX2K3K_VAL_MATH_OPERATION_SMOOTH               26
#define AGX2K3K_VAL_MATH_OPERATION_ENVELOPE             27
#define AGX2K3K_VAL_MATH_OPERATION_MAXHOLD              28
#define AGX2K3K_VAL_MATH_OPERATION_MINHOLD              29

/*- AGX2K3K_ATTR_MATH_WINDOW -*/
#define AGX2K3K_VAL_MATH_WINDOW_HANNING                 0
#define AGX2K3K_VAL_MATH_WINDOW_FLATTOP                 1
#define AGX2K3K_VAL_MATH_WINDOW_RECTANGULAR             2
#define AGX2K3K_VAL_MATH_WINDOW_BLACKMAN_HARRIS         3

/*- AGX2K3K_ATTR_DIG_SIZE -*/
#define AGX2K3K_VAL_DIGITAL_SIZE_SMALL                  0
#define AGX2K3K_VAL_DIGITAL_SIZE_MEDIUM                 1
#define AGX2K3K_VAL_DIGITAL_SIZE_LARGE                  2

/*- AGX2K3K_ATTR_TIMEBASE_MODE -*/
#define AGX2K3K_VAL_TIMEBASE_NORMAL                     (0)
#define AGX2K3K_VAL_TIMEBASE_WINDOW                     (1)
#define AGX2K3K_VAL_TIMEBASE_XY                         (2)
#define AGX2K3K_VAL_TIMEBASE_ROLL                       (3)

/*- AGX2K3K_ATTR_TIMEBASE_REFERENCE -*/
#define AGX2K3K_VAL_TIMEBASE_REF_LEFT                   (0)
#define AGX2K3K_VAL_TIMEBASE_REF_CENTER                 (1)
#define AGX2K3K_VAL_TIMEBASE_REF_RIGHT                  (2)

/*- AGX2K3K_ATTR_WAVEGEN_FUNCTION -*/
#define AGX2K3K_VAL_WAVEGEN_SINE                        0
#define AGX2K3K_VAL_WAVEGEN_SQUARE                      1
#define AGX2K3K_VAL_WAVEGEN_RAMP                        2
#define AGX2K3K_VAL_WAVEGEN_PULSE                       3
#define AGX2K3K_VAL_WAVEGEN_DC                          4
#define AGX2K3K_VAL_WAVEGEN_NOISE                       5

/*- AGX2K3K_ATTR_WAVEGEN_LOAD_IMPEDANCE -*/
#define AGX2K3K_VAL_WAVEGEN_IMP_FIFTY                   0
#define AGX2K3K_VAL_WAVEGEN_IMP_ONE_MEGA                1

/*---------------------------------------------------------------------------
 *------------------------ Attribute Value Defined in version 2.0 ----------*
 * ---------------------------------------------------------------------------*/

/*- Defined values for attribute AGX2K3K_ATTR_MARKER_XUNIT -*/
#define AGX2K3K_VAL_SECONDS           (1)
#define AGX2K3K_VAL_HERTZ             (2)
#define AGX2K3K_VAL_DEGREES           (3)
#define AGX2K3K_VAL_X_PERCENT         (4)

/*- Defined values for attribute AGX2K3K_ATTR_MARKER_YUNIT -*/
#define AGX2K3K_VAL_BASE              (1)
#define AGX2K3K_VAL_Y_PERCENT         (2)

/*- Defined values for attribute AGX2K3K_ATTR_FUNCTION_VTYPE -*/
#define AGX2K3K_VAL_VTYPE_DECIBEL     (1)
#define AGX2K3K_VAL_VTYPE_VRMS        (2)

#define AGX2K3K_VAL_SLOPE_NEGATIVE    (1)
#define AGX2K3K_VAL_SLOPE_POSITIVE    (2)

/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_TRANSITION_SOURCE -*/
/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_SHOLD_CLOCK_SOURCE -*/
/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_SHOLD_DATA_SOURCE -*/
/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_DELAY_ARM_SOURCE -*/
/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_DELAY_TRIGGER_SOURCE -*/
#define AGX2K3K_VAL_CHANNEL_1       (1)
#define AGX2K3K_VAL_CHANNEL_2       (2)
#define AGX2K3K_VAL_CHANNEL_3       (3)
#define AGX2K3K_VAL_CHANNEL_4       (4)
#define AGX2K3K_VAL_DIGITAL_0       (5)
#define AGX2K3K_VAL_DIGITAL_1       (6)
#define AGX2K3K_VAL_DIGITAL_2       (7)
#define AGX2K3K_VAL_DIGITAL_3       (8)
#define AGX2K3K_VAL_DIGITAL_4       (9)
#define AGX2K3K_VAL_DIGITAL_5       (10)
#define AGX2K3K_VAL_DIGITAL_6       (11)
#define AGX2K3K_VAL_DIGITAL_7       (12)
#define AGX2K3K_VAL_DIGITAL_8       (13)
#define AGX2K3K_VAL_DIGITAL_9       (14)
#define AGX2K3K_VAL_DIGITAL_10      (15)
#define AGX2K3K_VAL_DIGITAL_11      (16)
#define AGX2K3K_VAL_DIGITAL_12      (17)
#define AGX2K3K_VAL_DIGITAL_13      (18)
#define AGX2K3K_VAL_DIGITAL_14      (19)
#define AGX2K3K_VAL_DIGITAL_15      (20)

/*- Defined values for attribute AGX2K3K_ATTR_WGEN_ARBITRARY_BYTEORDER -*/
#define AGX2K3K_VAL_MSB_FIRST       (1)
#define AGX2K3K_VAL_LSB_FIRST       (2)

/*- Defined values for attribute AGX2K3K_ATTR_FUNCTION_BUS_SLOPE -*/
#define AGX2K3K_VAL_BUS_NEGATIVE    (1)
#define AGX2K3K_VAL_BUS_POSITIVE    (2)
#define AGX2K3K_VAL_BUS_EITHER      (3)

/*- Defined values for attribute AGX2K3K_ATTR_FUNCTION_BUS_YUNITS -*/
#define AGX2K3K_VAL_BUS_VOLT        (1)
#define AGX2K3K_VAL_BUS_AMPERE      (2)
#define AGX2K3K_VAL_BUS_NONE        (3)

/*- Defined values for attribute AGX2K3K_ATTR_FUNCTION_TREND_MEASUREMENT -*/
/*- Defined values for attribute AGX2K3K_ATTR_POWER_MODULATION_TYPE -*/
#define AGX2K3K_VAL_VAVERAGE      (4)
#define AGX2K3K_VAL_ACRMS         (5)
#define AGX2K3K_VAL_VRATIO        (10)
#define AGX2K3K_VAL_PWIDTH        (6)
#define AGX2K3K_VAL_NWIDTH        (7)
#define AGX2K3K_VAL_DUTY_CYCLE    (8)
#define AGX2K3K_VAL_RISETIME      (9)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_HARMONICS_DISPLAY_STYLE -*/
#define AGX2K3K_VAL_TABLE         (1)
#define AGX2K3K_VAL_BAR           (2)
#define AGX2K3K_VAL_OFF           (3)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_HARMONICS_STANDARD -*/
#define AGX2K3K_VAL_STANDARD_A    (1)
#define AGX2K3K_VAL_STANDARD_B    (2)
#define AGX2K3K_VAL_STANDARD_C    (3)
#define AGX2K3K_VAL_STANDARD_D    (4)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_MODULATION_SOURCE -*/
/*- Defined values for attribute AGX2K3K_ATTR_POWER_SLEW_SOURCE -*/
#define AGX2K3K_VAL_V    (1)
#define AGX2K3K_VAL_I    (2)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_PSRR_FREQUENCY_MAX -*/
/*- Defined values for attribute AGX2K3K_ATTR_POWER_PSRR_FREQUENCY_MIN -*/
#define AGX2K3K_VAL_1_HZ        (0)
#define AGX2K3K_VAL_10_HZ       (1)
#define AGX2K3K_VAL_100_HZ      (2)
#define AGX2K3K_VAL_1_KHZ       (3)
#define AGX2K3K_VAL_10_KHZ      (4)
#define AGX2K3K_VAL_100_KHZ     (5)
#define AGX2K3K_VAL_1_MHZ       (6)
#define AGX2K3K_VAL_10_MHZ      (7)
#define AGX2K3K_VAL_20_MHZ      (8)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_QUALITY_TYPE -*/
#define AGX2K3K_VAL_FACTOR      (1)
#define AGX2K3K_VAL_REAL        (2)
#define AGX2K3K_VAL_APPARENT    (3)
#define AGX2K3K_VAL_REACTIVE    (4)
#define AGX2K3K_VAL_CREST       (5)
#define AGX2K3K_VAL_ANGLE       (6)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_SWITCH_CONDUCTION -*/
#define AGX2K3K_VAL_WAVEFORM    (1)
#define AGX2K3K_VAL_RDS         (2)
#define AGX2K3K_VAL_VCE         (3)

/*- Defined values for attribute AGX2K3K_ATTR_A429_BASE -*/
/*- Defined values for attribute AGX2K3K_ATTR_M1553_BASE -*/
#define AGX2K3K_VAL_BASE_BINARY            (1)
#define AGX2K3K_VAL_BASE_HEX               (2)

/*- Defined values for attribute AGX2K3K_ATTR_A429_FORMAT -*/
#define AGX2K3K_VAL_FORMAT_LDSDI           (1)
#define AGX2K3K_VAL_FORMAT_LDSSM           (2)
#define AGX2K3K_VAL_FORMAT_LDATA           (3)

/*- Defined values for attribute AGX2K3K_ATTR_A429_SIGNAL -*/
#define AGX2K3K_VAL_SIGNAL_A               (1)
#define AGX2K3K_VAL_SIGNAL_B               (2)
#define AGX2K3K_VAL_SIGNAL_DIFFERENTIAL    (3)

/*- Defined values for attribute AGX2K3K_ATTR_A429_SPEED -*/
#define AGX2K3K_VAL_A429_LOW               (1)
#define AGX2K3K_VAL_A429_HIGH              (2)

/*- Defined values for attribute AGX2K3K_ATTR_A429_TRIGGER_TYPE -*/
#define AGX2K3K_VAL_WSTART                 (1)
#define AGX2K3K_VAL_WSTOP                  (2)
#define AGX2K3K_VAL_LABEL                  (3)
#define AGX2K3K_VAL_LBITS                  (4)
#define AGX2K3K_VAL_LRANGE                 (5)
#define AGX2K3K_VAL_PERROR                 (6)
#define AGX2K3K_VAL_WERROR                 (7)
#define AGX2K3K_VAL_GERROR                 (8)
#define AGX2K3K_VAL_WGERRORS               (9)
#define AGX2K3K_VAL_ALLERRORS              (10)
#define AGX2K3K_VAL_ABITS                  (11)
#define AGX2K3K_VAL_AZBITS                 (12)
#define AGX2K3K_VAL_AOBITS                 (13)

/*- Defined values for attribute AGX2K3K_ATTR_FLEXRAY_BAUDRATE -*/
#define AGX2K3K_VAL_2_AND_HALF_MB          (1)
#define AGX2K3K_VAL_5_MB                   (2)
#define AGX2K3K_VAL_10_MB                  (3)

/*- Defined values for attribute AGX2K3K_ATTR_FLEXRAY_CHANNEL -*/
#define AGX2K3K_VAL_CHANNEL_A              (1)
#define AGX2K3K_VAL_CHANNEL_B              (2)

/*- Defined values for attribute AGX2K3K_ATTR_FLEXRAY_TRIGGER -*/
#define AGX2K3K_VAL_FRAME                  (1)
#define AGX2K3K_VAL_ERROR                  (2)
#define AGX2K3K_VAL_EVENT                  (3)

/*- Defined values for attribute AGX2K3K_ATTR_FLEXRAY_TRIGGER_ERROR_TYPE -*/
#define AGX2K3K_VAL_ALL                    (-1)
#define AGX2K3K_VAL_HCRC                   (2)
#define AGX2K3K_VAL_FCRC                   (3)

/*- Defined values for attribute AGX2K3K_ATTR_FLEXRAY_TRIGGER_EVENT_TYPE -*/
#define AGX2K3K_VAL_WAKEUP                 (1)
#define AGX2K3K_VAL_TSS                    (2)
#define AGX2K3K_VAL_FES_DTS                (3)
#define AGX2K3K_VAL_BSS                    (4)

/*- Defined values for attribute AGX2K3K_ATTR_FLEXRAY_TRIGGER_FRAME_CCREPETITION -*/
#define AGX2K3K_VAL_REP_ALL                (1)
#define AGX2K3K_VAL_REP_2                  (2)
#define AGX2K3K_VAL_REP_4                  (3)
#define AGX2K3K_VAL_REP_8                  (4)
#define AGX2K3K_VAL_REP_16                 (5)
#define AGX2K3K_VAL_REP_32                 (6)
#define AGX2K3K_VAL_REP_64                 (7)

/*- Defined values for attribute AGX2K3K_ATTR_FLEXRAY_TRIGGER_FRAME_TYPE -*/
#define AGX2K3K_VAL_STRARTUP               (2)
#define AGX2K3K_VAL_NULL                   (3)
#define AGX2K3K_VAL_SYNC                   (4)
#define AGX2K3K_VAL_NSTARTUP               (5)
#define AGX2K3K_VAL_NNULI                  (6)
#define AGX2K3K_VAL_NSYNC                  (7)
#define AGX2K3K_VAL_ALL_FRAME              (8)

/*- Defined values for attribute AGX2K3K_ATTR_DEMO_FUNCTION -*/
#define AGX2K3K_VAL_FUNC_SINUSOID          (1)
#define AGX2K3K_VAL_FUNC_NOISY             (2)
#define AGX2K3K_VAL_FUNC_PHASE             (3)
#define AGX2K3K_VAL_FUNC_RINGING           (4)
#define AGX2K3K_VAL_FUNC_SINGLE            (5)
#define AGX2K3K_VAL_FUNC_AM                (6)
#define AGX2K3K_VAL_FUNC_CLK               (7)
#define AGX2K3K_VAL_FUNC_GLITCH            (8)
#define AGX2K3K_VAL_FUNC_BURST             (9)
#define AGX2K3K_VAL_FUNC_MSO               (10)
#define AGX2K3K_VAL_FUNC_RUNT              (11)
#define AGX2K3K_VAL_FUNC_TRANSITION        (12)
#define AGX2K3K_VAL_FUNC_RFBURST           (13)
#define AGX2K3K_VAL_FUNC_SHOLD             (14)
#define AGX2K3K_VAL_FUNC_LFSINE            (15)
#define AGX2K3K_VAL_FUNC_FMBURST           (16)
#define AGX2K3K_VAL_FUNC_ETE               (17)
#define AGX2K3K_VAL_FUNC_CAN               (18)
#define AGX2K3K_VAL_FUNC_LIN               (19)
#define AGX2K3K_VAL_FUNC_UART              (20)
#define AGX2K3K_VAL_FUNC_I2C               (21)
#define AGX2K3K_VAL_FUNC_SPI               (22)
#define AGX2K3K_VAL_FUNC_I2S               (23)
#define AGX2K3K_VAL_FUNC_CANLIN            (24)
#define AGX2K3K_VAL_FUNC_ARRINC            (25)
#define AGX2K3K_VAL_FUNC_FLEXRAY           (26)
#define AGX2K3K_VAL_FUNC_MIL               (27)
#define AGX2K3K_VAL_FUNC_MIL2              (28)
#define AGX2K3K_VAL_FUNC_USB               (29)
#define AGX2K3K_VAL_FUNC_NMONOTONIC        (30)
#define AGX2K3K_VAL_FUNC_DCMOTOR           (31)
#define AGX2K3K_VAL_FUNC_HARMONICS         (32)
#define AGX2K3K_VAL_FUNC_COUPLING          (33)
#define AGX2K3K_VAL_FUNC_CFD               (34)
#define AGX2K3K_VAL_FUNC_SENT              (35)
#define AGX2K3K_VAL_FUNC_KEYSIGHT          (36)


/*- Defined values for attribute AGX2K3K_ATTR_FUNCTION_OPERATION -*/
#define AGX2K3K_VAL_MAGNIFY           (22)
#define AGX2K3K_VAL_ABSOLUTE          (23)
#define AGX2K3K_VAL_SQUARE            (24)
#define AGX2K3K_VAL_LN                (11)
#define AGX2K3K_VAL_LOG               (12)
#define AGX2K3K_VAL_EXP               (13)
#define AGX2K3K_VAL_TEN               (14)
#define AGX2K3K_VAL_LOWPASS           (15)
#define AGX2K3K_VAL_HIGHPASS          (16)
#define AGX2K3K_VAL_DIVIDE            (17)
#define AGX2K3K_VAL_LINEAR            (18)
#define AGX2K3K_VAL_TREND             (19)
#define AGX2K3K_VAL_BTIMING           (20)
#define AGX2K3K_VAL_BSTATE            (21)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_SIGNALS_AUTOSETUP_TYPE -*/
#define AGX2K3K_VAL_HARMONICS         (1)
#define AGX2K3K_VAL_EFFICIENCY        (2)
#define AGX2K3K_VAL_RIPPLE            (3)
#define AGX2K3K_VAL_MODULATION        (4)
#define AGX2K3K_VAL_QUALITY           (5)
#define AGX2K3K_VAL_SLEW              (6)
#define AGX2K3K_VAL_SWITCH            (7)
#define AGX2K3K_VAL_RDSVCE            (8)

/*- Defined values for attribute AGX2K3K_ATTR_M1553_TRIGGER_TYPE -*/
#define AGX2K3K_VAL_DSTART            (1)
#define AGX2K3K_VAL_DSTOP             (2)
#define AGX2K3K_VAL_CSTART            (3)
#define AGX2K3K_VAL_CSTOP             (4)
#define AGX2K3K_VAL_RTA               (5)
#define AGX2K3K_VAL_SERROR            (7)
#define AGX2K3K_VAL_MERROR            (8)
#define AGX2K3K_VAL_RTA11             (9)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_MODE -*/
#define AGX2K3K_VAL_A429              (1)
#define AGX2K3K_VAL_M1553             (2)
#define AGX2K3K_VAL_FLEXRAY           (3)
#define AGX2K3K_VAL_CAN               (4)
#define AGX2K3K_VAL_I2S               (5)
#define AGX2K3K_VAL_IIC               (6)
#define AGX2K3K_VAL_LIN               (7)
#define AGX2K3K_VAL_SPI               (8)
#define AGX2K3K_VAL_URAT              (9)
#define AGX2K3K_VAL_USB               (10)                        /*- This is not applicable for 3000T X-Seriean and 3000 X-Series.  -*/
#define AGX2K3K_VAL_SENT              (11)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_A429_MODE -*/
#define AGX2K3K_VAL_A429_LABEL        (1)
#define AGX2K3K_VAL_A429_LBITS        (2)
#define AGX2K3K_VAL_A429_PERROR       (3)
#define AGX2K3K_VAL_A429_WERROR       (4)
#define AGX2K3K_VAL_A429_GERROR       (5)
#define AGX2K3K_VAL_A429_WGERRORS     (6)
#define AGX2K3K_VAL_A429_ALLERRORS    (7)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_FLEXRAY_MODE -*/
#define AGX2K3K_VAL_FLEXRAY_FRAME     (1)
#define AGX2K3K_VAL_FLEXRAY_CYCLE     (2)
#define AGX2K3K_VAL_FLEXRAY_DATA      (3)
#define AGX2K3K_VAL_FLEXRAY_HERROR    (4)
#define AGX2K3K_VAL_FLEXRAY_FERROR    (5)
#define AGX2K3K_VAL_FLEXRAY_AERROR    (6)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_M1553_MODE -*/
#define AGX2K3K_VAL_M1553_DSTART      (1)
#define AGX2K3K_VAL_M1553_CSTART      (2)
#define AGX2K3K_VAL_M1553_RTA         (3)
#define AGX2K3K_VAL_M1553_PERROR      (4)
#define AGX2K3K_VAL_M1553_SERROR      (5)
#define AGX2K3K_VAL_M1553_MERROR      (6)
#define AGX2K3K_VAL_M1553_RTA11       (7)

#define AGX2K3K_VAL_GENERIC           (4)
#define AGX2K3K_VAL_P480L60HZ         (6)
#define AGX2K3K_VAL_P720L60HZ         (7)
#define AGX2K3K_VAL_P1080L24HZ        (8)
#define AGX2K3K_VAL_P1080L25HZ        (9)
#define AGX2K3K_VAL_P1080L50HZ        (10)
#define AGX2K3K_VAL_P1080L60HZ        (11)
#define AGX2K3K_VAL_I1080L50HZ        (12)
#define AGX2K3K_VAL_I1080L60HZ        (13)

/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_PATTERN_FORMAT -*/
#define AGX2K3K_VAL_PATTERN_ASCII     (1)
#define AGX2K3K_VAL_PATTERN_HEX       (2)

#define AGX2K3K_VAL_F50               (1)
#define AGX2K3K_VAL_F60               (2)
#define AGX2K3K_VAL_F400              (3)

/* New values added in version 3.0 */
/*- Defined values for attribute AGX2K3K_ATTR_REFERENCE_SIGNAL_MODE -*/
#define AGX2K3K_VAL_REFERENCE_SIGNAL_MODE_OFF                (1)
#define AGX2K3K_VAL_REFERENCE_SIGNAL_MODE_OUT                (2)
#define AGX2K3K_VAL_REFERENCE_SIGNAL_MODE_IN                 (3)

/*- Defined values for attribute AGX2K3K_ATTR_DVM_MODE -*/
#define AGX2K3K_VAL_DVM_MODE_ACRMS                           (1)
#define AGX2K3K_VAL_DVM_MODE_DC                              (2)
#define AGX2K3K_VAL_DVM_MODE_DCRMS                           (3)
#define AGX2K3K_VAL_DVM_MODE_FREQUENCY                       (4)

/*- Defined values for attribute AGX2K3K_ATTR_DVM_ANALOG_SOURCE -*/
#define AGX2K3K_VAL_DVM_ANALOG_CHANNEL_1                     (1)
#define AGX2K3K_VAL_DVM_ANALOG_CHANNEL_2                     (2)
#define AGX2K3K_VAL_DVM_ANALOG_CHANNEL_3                     (3)
#define AGX2K3K_VAL_DVM_ANALOG_CHANNEL_4                     (4)

/*- Defined values for attribute AGX2K3K_ATTR_MEASURE_INFORMATION_TYPE -*/
#define AGX2K3K_VAL_MEASURE_STATISTICS_MODE_ON               (1)
#define AGX2K3K_VAL_MEASURE_STATISTICS_MODE_CURRENT          (2)
#define AGX2K3K_VAL_MEASURE_STATISTICS_MODE_MINIMUM          (3)
#define AGX2K3K_VAL_MEASURE_STATISTICS_MODE_MAXIMUM          (4)
#define AGX2K3K_VAL_MEASURE_STATISTICS_MODE_MEAN             (5)
#define AGX2K3K_VAL_MEASURE_STATISTICS_MODE_STDDEV           (6)
#define AGX2K3K_VAL_MEASURE_STATISTICS_MODE_COUNT            (7)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_HARMONICS_FAIL_PASS_STATUS -*/
#define AGX2K3K_VAL_POWER_HARMONICS_STATUS_PASS              (1)
#define AGX2K3K_VAL_POWER_HARMONICS_STATUS_FAIL              (2)
#define AGX2K3K_VAL_POWER_HARMONICS_STATUS_UNTESTED          (3)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_FLEX_EVENT_TRIGGER_MODE -*/
#define AGX2K3K_VAL_SBUS_FLEX_EVENT_TRIGGER_MODE_WAKEUP      (1)
#define AGX2K3K_VAL_SBUS_FLEX_EVENT_TRIGGER_MODE_TSS         (2)
#define AGX2K3K_VAL_SBUS_FLEX_EVENT_TRIGGER_MODE_FES         (3)
#define AGX2K3K_VAL_SBUS_FLEX_EVENT_TRIGGER_MODE_BSS         (4)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_BASE -*/
#define AGX2K3K_VAL_SBUS_USB_BASE_HEX                        (1)
#define AGX2K3K_VAL_SBUS_USB_BASE_ASCII                      (2)
#define AGX2K3K_VAL_SBUS_USB_BASE_BINARY                     (3)
#define AGX2K3K_VAL_SBUS_USB_BASE_DECIMAL                    (4)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_DIFFERENTIAL_SOURCE -*/
#define AGX2K3K_VAL_SBUS_USB_DIFF_SOURCE_CHANNEL_1           (1)
#define AGX2K3K_VAL_SBUS_USB_DIFF_SOURCE_CHANNEL_2           (2)
#define AGX2K3K_VAL_SBUS_USB_DIFF_SOURCE_CHANNEL_3           (3)
#define AGX2K3K_VAL_SBUS_USB_DIFF_SOURCE_CHANNEL_4           (4)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_D_MINUS_SOURCE -*/
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_CHANNEL_1        (1)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_CHANNEL_2        (2)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_CHANNEL_3        (3)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_CHANNEL_4        (4)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_0        (5)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_1        (6)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_2        (7)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_3        (8)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_4        (9)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_5        (10)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_6        (11)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_7        (12)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_8        (13)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_9        (14)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_10       (15)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_11       (16)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_12       (17)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_13       (18)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_14       (19)
#define AGX2K3K_VAL_SBUS_USB_D_MINUS_SOURCE_DIGITAL_15       (20)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_D_PLUS_SOURCE -*/
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_CHANNEL_1         (1)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_CHANNEL_2         (2)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_CHANNEL_3         (3)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_CHANNEL_4         (4)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_0         (5)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_1         (6)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_2         (7)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_3         (8)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_4         (9)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_5         (10)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_6         (11)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_7         (12)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_8         (13)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_9         (14)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_10        (15)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_11        (16)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_12        (17)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_13        (18)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_14        (19)
#define AGX2K3K_VAL_SBUS_USB_D_PLUS_SOURCE_DIGITAL_15        (20)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_SPEED -*/
#define AGX2K3K_VAL_SBUS_USB_SPEED_LOW                       (1)
#define AGX2K3K_VAL_SBUS_USB_SPEED_FULL                      (2)
#define AGX2K3K_VAL_SBUS_USB_SPEED_HIGH                      (3)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_TRIGGER_MODE -*/
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_SOP                     (1)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_EOP                     (2)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_ENTERSUSPEND            (3)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_EXITSUSPEND             (4)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_RESET                   (5)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_TOKEN                   (6)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_DATA                    (7)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_HANDSHAKE               (8)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_SPECIAL                 (9)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_ALLERRORS               (10)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PIDERROR                (11)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_CRC5ERROR               (12)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_CRC16ERROR              (13)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_GLITCHERROR             (14)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_STUFFERROR              (15)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_SE1ERROR                (16)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_TRIGGER_DATA_PID -*/
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_DATA0               (1)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_DATA1               (2)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_DATA2               (3)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_MDATA               (4)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_TRIGGER_HANDSHAKE_PID -*/
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_HANDSHAKE_ACK       (1)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_HANDSHAKE_NAK       (2)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_HANDSHAKE_STALL     (3)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_HANDSHAKE_NYET      (4)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_TRIGGER_SPECIAL_PID -*/
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_SPECIAL_PID_PING        (1)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_SPECIAL_PID_PRE         (2)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_SPECIAL_PID_ERR         (3)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_SPECIAL_PID_SPLIT       (4)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_USB_TRIGGER_TOKEN_PID -*/
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_TOKEN_OUT           (1)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_TOKEN_IN            (2)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_TOKEN_SETUP         (3)
#define AGX2K3K_VAL_SBUS_USB_TRIGGER_PID_TOKEN_SOF           (4)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_USB_SEARCH_MODE -*/
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_TOKEN             (1)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_DATA              (2)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_HANDSHAKE         (3)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_SPECIAL           (4)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_ALLERRORS         (5)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_PIDERROR          (6)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_CRC5ERROR         (7)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_CRC16ERROR        (8)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_GLITCHERROR       (9)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_STUFFERROR        (10)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_MODE_SE1ERROR          (11)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_USB_DATA_PID -*/
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_DATA0              (1)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_DATA1              (2)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_DATA2              (3)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_MDATA              (4)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_USB_HANDSHAKE_PID -*/
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_HANDSHAKE_ACK      (1)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_HANDSHAKE_NAK      (2)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_HANDSHAKE_STALL    (3)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_HANDSHAKE_NYET     (4)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_USB_SPECIAL_PID -*/
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_SPECIAL_PID_PING       (1)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_SPECIAL_PID_PRE        (2)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_SPECIAL_PID_ERR        (3)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_SPECIAL_PID_SPLIT      (4)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_USB_TOKEN_PID -*/
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_TOKEN_OUT          (1)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_TOKEN_IN           (2)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_TOKEN_SETUP        (3)
#define AGX2K3K_VAL_SEARCH_SERIAL_USB_PID_TOKEN_SOF          (4)

/*- Defined values for attribute AGX2K3K_ATTR_INTERPOLATION -*/
#define AGX2K3K_VAL_NO_INTERPOLATION                         IVISCOPE_VAL_NO_INTERPOLATION
#define AGX2K3K_VAL_LINEAR_INTERPOLATION                     IVISCOPE_VAL_LINEAR

/*- Defined values for attribute AGX2K3K_ATTR_WAVEGEN_STORE_ARB_WAVEFORM -*/
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_CHANNEL_1             (1)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_CHANNEL_2             (2)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_CHANNEL_3             (3)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_CHANNEL_4             (4)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_WMENORY_1             (5)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_WMENORY_2             (6)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_FUNCTION_1            (7)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_FUNCTION_2            (8)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_FUNCTION_3            (9)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_FUNCTION_4            (10)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_MATH_1                (11)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_MATH_2                (12)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_MATH_3                (13)
#define AGX2K3K_VAL_STORE_ARB_WAVEFORM_MATH_4                (14)

/*- Defined values for attribute AGX2K3K_ATTR_WAVEGEN_MODULATION_SIGNAL_SHAPE -*/
#define AGX2K3K_VAL_WAVEGEN_MODULATION_SINEUSOID             (1)
#define AGX2K3K_VAL_WAVEGEN_MODULATION_SQUARE                (2)
#define AGX2K3K_VAL_WAVEGEN_MODULATION_RAMP                  (3)

/*- Defined values for attribute AGX2K3K_ATTR_WAVEGEN_MODULATION_TYPE -*/
#define AGX2K3K_VAL_WAVEGEN_MODULATION_TYPE_AM               (1)
#define AGX2K3K_VAL_WAVEGEN_MODULATION_TYPE_FM               (2)
#define AGX2K3K_VAL_WAVEGEN_MODULATION_TYPE_FSK              (3)

/*- Define values for attribute AGX2K3K_ATTR_WAVEGEN_OUTPUT_POLARITY_INVERTED -*/
#define AGX2K3K_VAL_GENERATOR_INVERTED                       (1)
#define AGX2K3K_VAL_GENERATOR_NORMAL                         (2)

/* INFINITE */
#define AGX2K3K_VAL_INFINITE                                 IVISCOPE_VAL_MAX_TIME_INFINITE



/*- Defined values for attribute AGX2K3K_ATTR_CHANNEL_PROBE_TEK_MODEL -*/
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P5205     (1)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P5210     (2)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6205     (3)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6241     (4)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6243     (5)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6245     (6)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6246     (7)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6247     (8)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6248     (9)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6249     (10)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6250     (11)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P6251     (12)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P670X     (13)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_P671X     (14)
#define AGX2K3K_VAL_CHANNEL_PROBE_TEK_MODEL_TCP202    (15)

/*- Defined values for attribute AGX2K3K_ATTR_COUNTER_TOTALIZE_GATE_SOURCE -*/
/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_ZONE_SOURCE -*/
/*- Defined values for attribute AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_ADJACENT -*/
/*- Defined values for attribute AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_DIFFERENTIAL -*/
/*- Defined values for attribute AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_DMINUS -*/
/*- Defined values for attribute AGX2K3K_ATTR_COMPLIANCE_USB_SOURCE_DPLUS -*/

#define AGX2K3K_VAL_SOURCE_CHANNEL1                                (1)
#define AGX2K3K_VAL_SOURCE_CHANNEL2                                (2)
#define AGX2K3K_VAL_SOURCE_CHANNEL3                                (3)
#define AGX2K3K_VAL_SOURCE_CHANNEL4                                (4)

/*- Defined values for attribute AGX2K3K_ATTR_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE -*/
#define AGX2K3K_VAL_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE_DHSS        (1)
#define AGX2K3K_VAL_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE_HHSS        (2)
#define AGX2K3K_VAL_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE_DLSS        (3)
#define AGX2K3K_VAL_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE_HLSS        (4)
#define AGX2K3K_VAL_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE_DFSS        (5)
#define AGX2K3K_VAL_COMPLIANCE_USB_SIGNAL_QUALITY_TYPE_HFSS        (6)

/*- Defined values for attribute AGX2K3K_ATTR_COMPLIANCE_USB_TEST_CONNECTION -*/
#define AGX2K3K_VAL_COMPLIANCE_USB_TEST_CONNECTION_SINGLEENDED     (1)
#define AGX2K3K_VAL_COMPLIANCE_USB_TEST_CONNECTION_DIFFERENTIAL    (2)

/*- Defined values for attribute AGX2K3K_ATTR_COMPLIANCE_USB_TEST_TYPE -*/
#define AGX2K3K_VAL_COMPLIANCE_USB_TEST_TYPE_NEAREND               (1)
#define AGX2K3K_VAL_COMPLIANCE_USB_TEST_TYPE_FAREND                (2)

/*- Defined values for attribute AGX2K3K_ATTR_COUNTER_MODE -*/
#define AGX2K3K_VAL_COUNTER_MODE_FREQUENCY                         (1)
#define AGX2K3K_VAL_COUNTER_MODE_PERIOD                            (2)
#define AGX2K3K_VAL_COUNTER_MODE_TOTALIZE                          (3)

/*- Defined values for attribute AGX2K3K_ATTR_COUNTER_SOURCE -*/
#define AGX2K3K_VAL_COUNTER_SOURCE_CHANNEL1                        (1)
#define AGX2K3K_VAL_COUNTER_SOURCE_CHANNEL2                        (2)
#define AGX2K3K_VAL_COUNTER_SOURCE_CHANNEL3                        (3)
#define AGX2K3K_VAL_COUNTER_SOURCE_CHANNEL4                        (4)
#define AGX2K3K_VAL_COUNTER_SOURCE_TQEVENT                         (5)

/*- Defined values for attribute AGX2K3K_ATTR_COUNTER_TOTALIZE_GATE_POLARITY -*/
/*- Defined values for attribute AGX2K3K_ATTR_COUNTER_TOTALIZE_SLOPE -*/
#define AGX2K3K_VAL_COUNTER_TOTALIZER_EDGES_NEGATIVE              (1)
#define AGX2K3K_VAL_COUNTER_TOTALIZER_EDGES_POSITIVE              (2)

/*- Defined values for attribute AGX2K3K_ATTR_FFT_DISPLAY_MODE -*/
#define AGX2K3K_VAL_FFT_DISPLAY_MODE_NORMAL                       (1)
#define AGX2K3K_VAL_FFT_DISPLAY_MODE_AVERAGE                      (2)
#define AGX2K3K_VAL_FFT_DISPLAY_MODE_MAXHOLD                      (3)
#define AGX2K3K_VAL_FFT_DISPLAY_MODE_MINHOLD                      (4)

/*- Defined values for attribute AGX2K3K_ATTR_FFT_SOURCE1 -*/
#define AGX2K3K_VAL_FFT_SOURCE_CHANNEL1                           (1)
#define AGX2K3K_VAL_FFT_SOURCE_CHANNEL2                           (2)
#define AGX2K3K_VAL_FFT_SOURCE_CHANNEL3                           (3)
#define AGX2K3K_VAL_FFT_SOURCE_CHANNEL4                           (4)
#define AGX2K3K_VAL_FFT_SOURCE_FUNCTION1                          (5)
#define AGX2K3K_VAL_FFT_SOURCE_FUNCTION2                          (6)
#define AGX2K3K_VAL_FFT_SOURCE_MATH1                              (7)
#define AGX2K3K_VAL_FFT_SOURCE_MATH2                              (8)

/*- Defined values for attribute AGX2K3K_ATTR_FFT_VERTICAL_UNIT -*/
#define AGX2K3K_VAL_FFT_VERTICAL_UNIT_DECIBEL                     (1)
#define AGX2K3K_VAL_FFT_VERTICAL_UNIT_VRMS                        (2)

/*- Defined values for attribute AGX2K3K_ATTR_FFT_WINDOW -*/
#define AGX2K3K_VAL_FFT_WINDOW_RECTANGULAR                        (1)
#define AGX2K3K_VAL_FFT_WINDOW_HANNING                            (2)
#define AGX2K3K_VAL_FFT_WINDOW_FLATTOP                            (3)
#define AGX2K3K_VAL_FFT_WINDOW_BHARRIS                            (4)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_EFFICIENCY_TYPE -*/
#define AGX2K3K_VAL_POWER_EFFICIENCY_TYPE_DCDC                    (1)
#define AGX2K3K_VAL_POWER_EFFICIENCY_TYPE_DCAC                    (2)
#define AGX2K3K_VAL_POWER_EFFICIENCY_TYPE_ACDC                    (3)
#define AGX2K3K_VAL_POWER_EFFICIENCY_TYPE_ACAC                    (4)

/*- Defined values for attribute AGX2K3K_ATTR_POWER_HARMONICS_REAL_POWER_TYPE -*/
#define AGX2K3K_VAL_POWER_HARMONICS_REAL_POWER_SOURCE_MEASURED    (1)
#define AGX2K3K_VAL_POWER_HARMONICS_REAL_POWER_SOURCE_USER        (2)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_CAN_DISPLAY -*/
#define AGX2K3K_VAL_SBUS_CAN_DISPLAY_TYPE_HEXADECIMAL             (1)
#define AGX2K3K_VAL_SBUS_CAN_DISPLAY_TYPE_SYMBOLIC                (2)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_LIN_DISPLAY -*/
#define AGX2K3K_VAL_SBUS_LIN_DISPLAY_TYPE_HEXADECIMAL             (1)
#define AGX2K3K_VAL_SBUS_LIN_DISPLAY_TYPE_SYMBOLIC                (2)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_CRC -*/
#define AGX2K3K_VAL_SBUS_SENT_CRC_LEGACY                          (1)
#define AGX2K3K_VAL_SBUS_SENT_CRC_RECOMMENDED                     (2)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_DISPLAY -*/
#define AGX2K3K_VAL_SBUS_SENT_DISPLAY_BASE_HEX                    (1)
#define AGX2K3K_VAL_SBUS_SENT_DISPLAY_BASE_DECIMAL                (2)
#define AGX2K3K_VAL_SBUS_SENT_DISPLAY_BASE_SYMBOLIC               (3)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_IDLE -*/
#define AGX2K3K_VAL_SBUS_SENT_IDLE_STATE_LOW                      (1)
#define AGX2K3K_VAL_SBUS_SENT_IDLE_STATE_HIGH                     (2)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_FORMAT -*/
#define AGX2K3K_VAL_SBUS_SENT_MESSAGE_FORMAT_NIBBLES              (1)
#define AGX2K3K_VAL_SBUS_SENT_MESSAGE_FORMAT_FSIGNAL              (2)
#define AGX2K3K_VAL_SBUS_SENT_MESSAGE_FORMAT_FSSERIAL             (3)
#define AGX2K3K_VAL_SBUS_SENT_MESSAGE_FORMAT_FESERIAL             (4)
#define AGX2K3K_VAL_SBUS_SENT_MESSAGE_FORMAT_SSERIAL              (5)
#define AGX2K3K_VAL_SBUS_SENT_MESSAGE_FORMAT_ESERIAL              (6)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_SIGNAL_ORDER -*/
#define AGX2K3K_VAL_SBUS_SENT_SIGNAL_ORDER_MSNFIRST               (1)
#define AGX2K3K_VAL_SBUS_SENT_SIGNAL_ORDER_LSNFIRST               (2)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_SOURCE -*/
#define AGX2K3K_VAL_SBUS_SENT_SOURCE_CHANNEL1                     (1)
#define AGX2K3K_VAL_SBUS_SENT_SOURCE_CHANNEL2                     (2)
#define AGX2K3K_VAL_SBUS_SENT_SOURCE_CHANNEL3                     (3)
#define AGX2K3K_VAL_SBUS_SENT_SOURCE_CHANNEL4                     (4)
#define AGX2K3K_VAL_SBUS_SENT_SOURCE_DIGITAL0                     (5)
#define AGX2K3K_VAL_SBUS_SENT_SOURCE_DIGITAL1                     (6)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_TRIGGER -*/
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_SFCMESSAGE             (1)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_SSCMESSAGE             (2)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_FCDATA                 (3)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_SCMID                  (4)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_SCDATA                 (5)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_TOLERROR               (6)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_FCCERROR               (7)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_SCCERROR               (8)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_CRCERROR               (9)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_PPERROR                (10)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_MODE_SSPERROR               (11)

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_SENT_TRIGGER_SLOW_LENGTH -*/
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_SLOW_LENGTH_SHORT           (1)
#define AGX2K3K_VAL_SBUS_SENT_TRIGGER_SLOW_LENGTH_LONG            (2)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_PEAK_SOURCE -*/
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_FUNCTION1                  (1)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_FUNCTION2                  (2)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_FUNCTION3                  (3)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_FUNCTION4                  (4)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_MATH1                      (5)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_MATH2                      (6)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_MATH3                      (7)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_MATH4                      (8)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_NONE                       (9)
#define AGX2K3K_VAL_SEARCH_PEAK_SOURCE_FFT                        (10)

/*- Defined values for attribute AGX2K3K_ATTR_SEARCH_SERIAL_SENT_MODE -*/
#define AGX2K3K_VAL_SEARCH_SERIAL_SENT_MODE_FCDATA                (1)
#define AGX2K3K_VAL_SEARCH_SERIAL_SENT_MODE_SCMID                 (2)
#define AGX2K3K_VAL_SEARCH_SERIAL_SENT_MODE_SCDATA                (3)
#define AGX2K3K_VAL_SEARCH_SERIAL_SENT_MODE_CRCERROR              (4)
#define AGX2K3K_VAL_SEARCH_SERIAL_SENT_MODE_PPERROR               (5)

/*- Defined values for attribute AGX2K3K_ATTR_TRIGGER_ZONE_MODE -*/
#define AGX2K3K_VAL_TRIGGER_ZONE_MODE_INTERSECT                   (1)
#define AGX2K3K_VAL_TRIGGER_ZONE_MODE_NOTINTERSECT                (2)

/*- Defined values for attribute AGX2K3K_ATTR_WAVEGEN_OUTPUT_MODE -*/
#define AGX2K3K_VAL_WAVEGEN_OUTPUT_MODE_NORMAL                    (1)
#define AGX2K3K_VAL_WAVEGEN_OUTPUT_MODE_SINGLE                    (2)

/*- used in agx2k3k_ConfigureFFTFrequency as input parameter -*/
#define AGX2K3K_VAL_FFT_FREQUENCY_TYPE_START_STOP                 (1)
#define AGX2K3K_VAL_FFT_FREQUENCY_TYPE_SPAN_CENTER                (2)

/* used in RecallSerialBusDFataFromFile as input parameter */
#define AGX2K3K_VAL_SBUS1                                         (1)
#define AGX2K3K_VAL_SBUS2                                         (2)
#define AGX2K3K_VAL_SBUS_NONE                                     (3)

#define AGX2K3K_VAL_RECALL_FILE_DBC                               (1)
#define AGX2K3K_VAL_RECALL_FILE_LDF                               (2)

/*---------------------------------------------------------------------------
 *------------------------ Attribute Values Defined in version 3.2 ----------*
 * ---------------------------------------------------------------------------*/

/*- Defined values for attribute AGX2K3K_ATTR_SBUS_CAN_FDSTANDARD -*/
#define AGX2K3K_VAL_ISO                      (1)
#define AGX2K3K_VAL_NISO                     (2)

/*- Defined values for attribute AGX2K3K_ATTR_HISTOGRAM_AXIS -*/
#define AGX2K3K_VAL_VERTICAL                 (1)
#define AGX2K3K_VAL_HORIZONTAL               (2)

/*- Defined values for attribute AGX2K3K_ATTR_HISTOGRAM_MODE -*/
#define AGX2K3K_VAL_HISTOGRAM_OFF            (1)
#define AGX2K3K_VAL_HISTOGRAM_WAVEFORM       (2)
#define AGX2K3K_VAL_HISTOGRAM_MEASUREMENT    (3)

/*- Defined values for attribute AGX2K3K_ATTR_JITTER_MEASUREMENT -*/
#define AGX2K3K_VAL_MEAS1                    (1)
#define AGX2K3K_VAL_MEAS2                    (2)
#define AGX2K3K_VAL_MEAS3                    (3)
#define AGX2K3K_VAL_MEAS4                    (4)
#define AGX2K3K_VAL_MEAS5                    (5)
#define AGX2K3K_VAL_MEAS6                    (6)
#define AGX2K3K_VAL_MEAS7                    (7)
#define AGX2K3K_VAL_MEAS8                    (8)
#define AGX2K3K_VAL_MEAS9                    (9)
#define AGX2K3K_VAL_MEAS10                   (10)

/*- Defined values for attribute AGX2K3K_ATTR_WAVEFORM_SOURCE_SUBSOURCE -*/
#define AGX2K3K_VAL_SUB0                     (1)
#define AGX2K3K_VAL_SUB1                     (2)

/****************************************************************************
 *----------------------- Functions Value Define ---------------------------*
 ****************************************************************************/
/*- ConfigureWaevform -*/
#define AGX2K3K_VAL_WGEN_STYLE_FREQUENCY     0
#define AGX2K3K_VAL_WGEN_STYLE_PERIOD        1

/*- ConfigureGeneratorAmplitude -*/
#define AGX2K3K_VAL_WGEN_AMPLITUDE_OFFSET    0
#define AGX2K3K_VAL_WGEN_HIGH_LOW            1

#define AGX2K3K_VAL_ACQ_MODE_MREALTIME       3
#define AGX2K3K_VAL_ACQ_MODE_EQUITIME        4

#define AGX2K3K_VAL_IIC_TYPE_ADDRESS         11
#define AGX2K3K_VAL_IIC_TYPE_WRITE_NACK      12

#define AGX2K3K_VAL_MATH_OPERATION_CLOCK     30

/*---------------------------------------------------------------------------
 *------------------------ Function Values Defined in version 3.2 ----------*
 * ---------------------------------------------------------------------------*/

/*- ReadCycleToCycleWidth -*/
#define AGX2K3K_VAL_CTCW_POSITIVE     (1)
#define AGX2K3K_VAL_CTCW_NEGATIVE     (2)

/*- ReadNCycleToNCycleJitter & ReadNPeriodJitter -*/
#define AGX2K3K_VAL_RISING            (1)
#define AGX2K3K_VAL_FALLING           (2)
#define AGX2K3K_VAL_BOTH              (3)

/*- ConfigureClockRecoveryFixed -*/
#define AGX2K3K_VAL_FIXED_AUTO        (1)
#define AGX2K3K_VAL_FIXED_SEMI        (2)
#define AGX2K3K_VAL_FIXED_DATARATE    (3)

/*- ReadHistogramMeasurement -*/
#define AGX2K3K_VAL_BWIDTH            (1)
#define AGX2K3K_VAL_HITS              (2)
#define AGX2K3K_VAL_M1S               (3)
#define AGX2K3K_VAL_M2S               (4)
#define AGX2K3K_VAL_M3S               (5)
#define AGX2K3K_VAL_MAX               (6)
#define AGX2K3K_VAL_MEAN              (7)
#define AGX2K3K_VAL_MED               (8)
#define AGX2K3K_VAL_MIN               (9)
#define AGX2K3K_VAL_MODE              (10)
#define AGX2K3K_VAL_PEAK              (11)
#define AGX2K3K_VAL_PPEAK             (12)
#define AGX2K3K_VAL_SDEV              (13)

/*- ReadRTEyeMeasurement -*/
#define AGX2K3K_VAL_EYE_HEIGHT        (1)
#define AGX2K3K_VAL_EYE_WIDTH         (2)

/*- ReadDataTIEJitter & ReadClockTIEJitter -*/
#define AGX2K3K_VAL_SECOND            (1)
#define AGX2K3K_VAL_UNITINTERVAL      (2)

/****************************************************************************
 *---------------- Instrument Driver Function Declarations -----------------*
 ****************************************************************************/

/*- Init and Close Functions -------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_init (ViRsrc resourceName, ViBoolean IDQuery,
                                 ViBoolean resetDevice, ViSession *vi);

DllExport ViStatus _VI_FUNC  agx2k3k_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                            ViBoolean resetDevice, ViConstString optionString,
                                            ViSession *newVi);

DllExport ViStatus _VI_FUNC  agx2k3k_close (ViSession vi);

DllExport ViStatus _VI_FUNC agx2k3k_DeviceClear (ViSession instrumentHandle);

/*- Coercion Info Functions --------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_GetNextCoercionRecord (ViSession vi,
                                                  ViInt32 bufferSize,
                                                  ViChar record[]);

/*- Locking Functions --------------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_LockSession (ViSession vi, ViBoolean *callerHasLock);

DllExport ViStatus _VI_FUNC  agx2k3k_UnlockSession (ViSession vi, ViBoolean *callerHasLock);

/*- Channel Info Functions ---------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_GetChannelName (ViSession vi, ViInt32 index,
                                          ViInt32 bufferSize, ViChar name[]);

/*- Acquisition Subsystem Configuration ---------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureAcquisitionType (ViSession vi,
                                                     ViInt32 acquisitionType);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureAcquisitionMode (ViSession vi,
                                                    ViInt32 acquisitionMode);

DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureAcquisitionRecord (ViSession vi,
                                                       ViReal64 timePerRecord,
                                                       ViInt32 minimumRecordLength,
                                                       ViReal64 acqStartTime);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureRecordMode (ViSession vi,
                                               ViInt32 horizontalRecordMode);

DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureNumAverages (ViSession vi,
                                                 ViInt32 numberOfAverages);

DllExport ViStatus _VI_FUNC  agx2k3k_SampleMode (ViSession vi, ViInt32 *sampleMode);

DllExport ViStatus _VI_FUNC  agx2k3k_SampleRate (ViSession vi, ViReal64 *sampleRate);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunBus (ViSession vi,
                                           ViInt32 busClock, ViInt32 busSlope,
                                           ViReal64 busIncrementY,
                                           ViReal64 busOriginY, ViInt32 busUnitY);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunBusMultiChan (ViSession vi, ViChar FunctionChannel[],
                                                    ViInt32 busClock, ViInt32 busSlope,
                                                    ViReal64 busIncrementY,
                                                    ViReal64 busOriginY, ViInt32 busUnitY);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunTrendMeas (ViSession vi,
                                                 ViInt32 trendMeasurement);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTrendMeasMultiChan (ViSession vi, ViChar FunctionChannel[],
                                                       ViInt32 trendMeasurement);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunLinear (ViSession vi,
                                              ViReal64 linearGain,
                                              ViReal64 linearOffset);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunLinearMultiChan (ViSession vi, ViChar Function_Channel[],
                                                       ViReal64 linearGain,
                                                       ViReal64 linearOffset);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunFrequency (ViSession vi,
                                                 ViReal64 highPass,
                                                 ViReal64 lowPass);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunFreqMultiChan (ViSession vi, ViChar Function_Channel[],
                                                     ViReal64 highPass,
                                                     ViReal64 lowPass);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunFFTUnit (ViSession vi,
                                               ViInt32 unit);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFFTUnitMultiChan (ViSession vi, ViChar Function_Channel[],
                                                     ViInt32 unit);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunInputOffset (ViSession vi,
                                                   ViReal64 inputOffset);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureInputOffsMultiChan (ViSession vi, ViChar Function_Channel[],
                                                       ViReal64 inputOffset);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathChannelEnabled (ViSession vi,
                                                       ViBoolean channelEnabled);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathChanMultiChan (ViSession vi, ViChar Function_Channel[],
                                                      ViBoolean channelEnabled);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathFunction (ViSession vi,
                                                 ViConstString source1,
                                                 ViConstString source2,
                                                 ViInt32 operation);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathSettings (ViSession vi,
                                                 ViReal64 scale, ViReal64 offset);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathMultiChan (ViSession vi, ViChar Function_Channel[],
                                                  ViReal64 scale, ViReal64 offset);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathFFT (ViSession vi,
                                            ViReal64 span, ViReal64 center,
                                            ViInt32 window);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathFFTMultiChan (ViSession vi,
                                                     ViChar FunctionChannel[],
                                                     ViReal64 span,
                                                     ViReal64 center,
                                                     ViInt32 window);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathSmoothPointsNumber (ViSession vi, ViConstString FunctionChannels, ViInt32 PointsNumber);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMathFFTFrequency (ViSession vi, ViConstString FunctionChannels, ViReal64 StartFrequency,
                                                     ViReal64 StopFrequency);


DllExport ViStatus _VI_FUNC agx2k3k_ActualRecordLength (ViSession vi,
                                              ViInt32 *actualRecordLength);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureInitiateContinuous (ViSession vi,
                                                       ViBoolean continuous);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureAcquisitionSegment (ViSession vi,
                                                       ViBoolean segmentAnalyzeEnable,
                                                       ViInt32 segmentCount,
                                                       ViInt32 segmentIndex);

/*- Channel Subsystem Configuration -------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureChannel (ViSession vi,
                                             ViConstString channel,
                                             ViReal64 range,
                                             ViReal64 offset,
                                             ViInt32 coupling,
                                             ViReal64 probeAttenuation,
                                             ViBoolean enabled);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureDigitalChannel (ViSession vi,
                                                   ViConstString channelName,
                                                   ViInt32 size,
                                                   ViReal64 threshold,
                                                   ViBoolean channelEnabled);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureChanCharacteristics (ViSession vi,
                                                        ViConstString channel,
                                                        ViReal64 inputImpedance,
                                                        ViReal64 maxInputFrequency);

DllExport ViStatus _VI_FUNC agx2k3k_AutoProbeSenseValue (ViSession vi,
                                               ViConstString channel,
                                               ViReal64 *autoProbeSenseValue);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureChannelInvert (ViSession vi,
                                                  ViConstString channel,
                                                  ViBoolean invert);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureChannelVernier (ViSession vi,
                                                   ViConstString channel,
                                                   ViBoolean vernier);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureChannelProbe (ViSession vi,
                                                 ViConstString channel,
                                                 ViReal64 skew, ViInt32 units);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureChannelProbeTek (ViSession vi, ViConstString ChannelName, ViInt32 ProbeModel);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureChannelLabel (ViSession vi,
                                                 ViConstString channel,
                                                 ViConstString label);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureDigitalPosition (ViSession vi,
                                                    ViConstString channelName,
                                                    ViInt32 position);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTimebaseMode (ViSession vi,
                                                 ViInt32 timebaseMode);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTimebase (ViSession vi,
                                             ViInt32 reference,
                                             ViBoolean vernier);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureWindowTimebase (ViSession vi,
                                                   ViReal64 position,
                                                   ViReal64 range,
                                                   ViReal64 scale);

/*- Triggering Subsystem Configuration ----------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureTrigger (ViSession vi,
                                             ViInt32 triggerType,
                                             ViReal64 holdoff);

DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureTriggerCoupling (ViSession vi,
                                                     ViInt32 coupling);

DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureTriggerModifier (ViSession vi,
                                                     ViInt32 modifier);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerReject (ViSession vi,
                                                  ViBoolean noiseReject,
                                                  ViBoolean HFReject);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerZone (ViSession vi, ViConstString Zones, ViInt32 ZoneSource,
                                                ViBoolean ZoneStateEnable, ViInt32 ZoneMode, ViBoolean MultiChanZoneStateEnabled);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerZonePlacement (ViSession vi, ViConstString Zones,
                                                         ViReal64 ZoneWidth, ViReal64 ZoneHeight, ViReal64 ZoneXCenter,
                                                         ViReal64 ZoneYCenter);

DllExport ViStatus _VI_FUNC agx2k3k_ReadTriggerZoneValidity (ViSession vi, ViConstString Zones, ViChar ZoneValidity[256]);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerLevel (ViSession vi,
                                                 ViReal64 levelHigh,
                                                 ViReal64 levelLow);

DllExport ViStatus _VI_FUNC agx2k3k_AutoSetupTriggerLevel (ViSession vi);

DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureEdgeTriggerSource (ViSession vi,
                                                       ViConstString source,
                                                       ViReal64 level,
                                                       ViInt32 slope);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureRuntTriggerSource (ViSession vi,
                                                      ViConstString triggerSource,
                                                      ViReal64 runtLowThreshold,
                                                      ViReal64 runtHighThreshold,
                                                      ViInt32 runtPolarity);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureRuntTriggerWidth (ViSession vi,
                                                     ViInt32 triggerCondition,
                                                     ViReal64 runtTime);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTransitionTrigger (ViSession vi,
                                                      ViConstString triggerSource,
                                                      ViInt32 slope,
                                                      ViInt32 triggerCondition,
                                                      ViReal64 transitionTime);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSetupHoldTrigger (ViSession vi,
                                                     ViConstString clockSource,
                                                     ViConstString dataSource,
                                                     ViInt32 slope,
                                                     ViReal64 holdTime,
                                                     ViReal64 setupTime);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTVTriggerSource (ViSession vi,
                                                    ViConstString source,
                                                    ViInt32 TVSignalFormat,
                                                    ViInt32 TVEvent,
                                                    ViInt32 TVPolarity);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTVTriggerLineNumber (ViSession vi,
                                                        ViInt32 lineNumber);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureIICBusSource (ViSession vi,
                                                 ViConstString busName,
                                                 ViConstString serialClockSource,
                                                 ViConstString serialDataSource,
                                                 ViInt32 type);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureIICEEPROMQualifier (ViSession vi,
                                                       ViConstString busName,
                                                       ViInt32 qualifier);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureIICData (ViSession vi,
                                            ViConstString busName,
                                            ViInt32 address,
                                            ViInt32 addressSize,
                                            ViInt32 data,
                                            ViInt32 data2);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureI2SBusSource (ViSession vi,
                                                 ViConstString busName,
                                                 ViConstString clockSource,
                                                 ViConstString dataSource,
                                                 ViConstString wsSource);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureI2SBus (ViSession vi,
                                           ViConstString busName,
                                           ViInt32 rxWidth,
                                           ViInt32 txWidth,
                                           ViInt32 wsPolarity,
                                           ViInt32 clockSlope,
                                           ViInt32 alignment);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureI2STrigger (ViSession vi,
                                               ViConstString busName,
                                               ViInt32 triggerCondition,
                                               ViInt32 triggerAudio,
                                               ViConstString patternData,
                                               ViInt32 rangeLow,
                                               ViInt32 rangeHigh);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureLINBus (ViSession vi,
                                           ViConstString busName,
                                           ViConstString LINSource,
                                           ViInt32 baudRate,
                                           ViInt32 standard,
                                           ViInt32 syncBreak,
                                           ViBoolean parity);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureLINTrigger (ViSession vi,
                                               ViConstString busName,
                                               ViInt32 triggerCondition,
                                               ViInt32 patternDataLength,
                                               ViConstString patternData,
                                               ViInt32 patternID);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureLINTriggerSymbolic (ViSession vi, ViConstString BusName, ViConstString Message,
                                                       ViConstString SignalName, ViReal64 SignalValue);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureLINDisplay (ViSession vi, ViConstString BusName, ViInt32 DisplayType);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSENT (ViSession vi, ViConstString BusName, ViReal64 NormalClockPeriod,
                                         ViInt32 CRCFormat, ViInt32 NumberBase, ViInt32 MessageFormat,
                                         ViInt32 IdleState, ViInt32 NibblesLength, ViBoolean PausePulseEnabled,
                                         ViInt32 InputSource, ViInt32 Tolerance);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSENTTrigger (ViSession vi, ViConstString BusName, ViInt32 TriggerMode,
                                                ViConstString FastData, ViInt32 SlowLength, ViInt32 SlowData, ViInt32 SlowID,
                                                ViInt32 Tolerance);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSENTSignal (ViSession vi, ViConstString BusName, ViConstString SignalName,
                                               ViBoolean DisplayEnabled, ViInt32 SignalLength, ViReal64 Multiplier,
                                               ViReal64 Offset, ViInt32 Order, ViInt32 StartBit);


DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSPITrigger (ViSession vi,
                                               ViConstString busName,
                                               ViInt32 triggerType,
                                               ViInt32 MISODataWidth,
                                               ViConstString MISODataValue,
                                               ViInt32 MOSIDataWidth,
                                               ViConstString MOSIDataValue);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSBusFlexrayTrigger (ViSession vi,
                                                       ViChar busName[],
                                                       ViInt32 trigger,
                                                       ViInt32 triggerEventBssID,
                                                       ViInt32 triggerEventType,
                                                       ViInt32 triggerFrameCCBase,
                                                       ViInt32 triggerFrameCCRepetition,
                                                       ViInt32 triggerFrameID,
                                                       ViInt32 triggerFrameType);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSPIBus (ViSession vi,
                                           ViConstString busName,
                                           ViConstString clockSource,
                                           ViConstString dataSource,
                                           ViConstString frameSource,
                                           ViInt32 frameBy,
                                           ViInt32 slope,
                                           ViInt32 bitOrder,
                                           ViInt32 wordWidth);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSPITriggerTimeout (ViSession vi,
                                                      ViConstString busName,
                                                      ViReal64 timeout);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureEdgeTriggerReject (ViSession vi,
                                                      ViInt32 triggerRejectFilter);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureEdgeBurstTrigger (ViSession vi,
                                                     ViConstString triggerSource,
                                                     ViInt32 slope,
                                                     ViInt32 count,
                                                     ViReal64 idleTime);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGlitchTriggerSource (ViSession vi,
                                                        ViConstString source,
                                                        ViReal64 level,
                                                        ViReal64 glitchWidth,
                                                        ViInt32 glitchPolarity,
                                                        ViInt32 glitchCondition);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGlitchWidth (ViSession vi,
                                                ViReal64 minWidth);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGlitchRange (ViSession vi,
                                                ViReal64 greaterThanTime,
                                                ViReal64 lessThanTime);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePatternTrigger (ViSession vi,
                                                   ViConstString patternValue,
                                                   ViInt32 triggerCondition,
                                                   ViReal64 greaterThanTime,
                                                   ViReal64 lessThanTime);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCANBus (ViSession vi,
                                           ViConstString busName,
                                           ViConstString source,
                                           ViInt32 baudRate,
                                           ViInt32 signal);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCANFDBus (ViSession vi, ViConstString BusName, ViReal64 SamplePoint,
                                             ViInt32 BaudRate);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCANTrigger (ViSession vi,
                                               ViConstString busName,
                                               ViInt32 trigger,
                                               ViInt32 dataLength,
                                               ViConstString patternData,
                                               ViInt32 idType,
                                               ViConstString idValue);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCANDisplay (ViSession vi, ViConstString BusName, ViInt32 DisplayType);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCANTriggerPattern (ViSession vi, ViConstString BusName, ViInt32 DataLengthCode,
                                                      ViInt32 StartPosition);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCANTriggerSymbolic (ViSession vi, ViConstString BusName, ViConstString Message,
                                                       ViConstString SignalName, ViReal64 SignalValue);
DllExport ViStatus _VI_FUNC agx2k3k_EnableCANTriggerIDFilter (ViSession vi, ViConstString BusName, ViBoolean IDFilterEnabled);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGeneratorAmplitude (ViSession vi,
                                                       ViInt32 settingStyle,
                                                       ViReal64 amplitude,
                                                       ViReal64 offset,
                                                       ViReal64 highLevel,
                                                       ViReal64 lowLevel,
                                                       ViInt32 loadImpendance);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGenAmpMultiChan (ViSession vi,
                                                    ViChar WaveGenChannel[],
                                                    ViInt32 settingStyle,
                                                    ViReal64 amplitude,
                                                    ViReal64 offset,
                                                    ViReal64 highLevel,
                                                    ViReal64 lowLevel,
                                                    ViInt32 loadImpendance);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureWaveform (ViSession vi,
                                             ViInt32 functionType,
                                             ViInt32 settingSytle,
                                             ViReal64 period,
                                             ViReal64 frequency,
                                             ViReal64 symmetry,
                                             ViReal64 dutyCycle,
                                             ViReal64 pulseWidth);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureWaveformMultiChan (ViSession vi,
                                                      ViChar WaveGenChannel[],
                                                      ViInt32 functionType,
                                                      ViInt32 settingSytle,
                                                      ViReal64 period,
                                                      ViReal64 frequency,
                                                      ViReal64 symmetry,
                                                      ViReal64 dutyCycle,
                                                      ViReal64 pulseWidth);

DllExport ViStatus _VI_FUNC agx2k3k_EnableGeneratorOutput (ViSession vi,
                                                 ViBoolean enableOutput);
DllExport ViStatus _VI_FUNC agx2k3k_EnableGenOutputMultiChan (ViSession vi, ViChar Wave_Gen_Channel[],
                                                    ViBoolean enableOutput);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGeneratorOutputMode (ViSession vi, ViConstString WaveGenChannel, ViInt32 OutputMode, ViBoolean SingleCycleOutputEnabled);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGeneratorTrackingPhase (ViSession vi, ViConstString WaveGenChannel, ViInt32 PhaseRelationship);

/*- Waveform Acquisition ------------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_ReadWaveform (ViSession vi,
                                         ViConstString channel,
                                         ViInt32 waveformSize,
                                         ViInt32 maxTime,
                                         ViReal64 waveform[],
                                         ViInt32 *actualPoints,
                                         ViReal64 *initialX,
                                         ViReal64 *xIncrement);

DllExport ViStatus _VI_FUNC  agx2k3k_InitiateAcquisition (ViSession vi);

DllExport ViStatus _VI_FUNC  agx2k3k_AcquisitionStatus (ViSession vi, ViInt32 *status);

DllExport ViStatus _VI_FUNC  agx2k3k_Abort (ViSession vi);

DllExport ViStatus _VI_FUNC agx2k3k_FetchEfficiency (ViSession instrumentHandle,
                                           ViReal64 *measurement);

DllExport ViStatus _VI_FUNC agx2k3k_FetchRipple (ViSession instrumentHandle,
                                       ViChar channelName[],
                                       ViReal64 *measurement);

DllExport ViStatus _VI_FUNC  agx2k3k_FetchWaveform (ViSession vi,
                                          ViConstString channel,
                                          ViInt32 waveformSize,
                                          ViReal64 waveform[],
                                          ViInt32 *actualPoints,
                                          ViReal64 *initialX,
                                          ViReal64 *xIncrement);

DllExport ViStatus _VI_FUNC agx2k3k_ReadEfficiency (ViSession instrumentHandle,
                                          ViReal64 *measurement);

DllExport ViStatus _VI_FUNC agx2k3k_ReadRipple (ViSession instrumentHandle,
                                      ViChar channelName[],
                                      ViReal64 *measurement);

/* Used when AGX2K3K_ATTR_ACQUISTION_TYPE is either */
/* AGX2K3K_VAL_PEAK_DETECT */
DllExport ViStatus _VI_FUNC  agx2k3k_ReadMinMaxWaveform (ViSession vi,
                                               ViConstString channel,
                                               ViInt32 waveformSize,
                                               ViInt32 maxTime,
                                               ViReal64 minWaveform[],
                                               ViReal64 maxWaveform[],
                                               ViInt32 *actualPoints,
                                               ViReal64 *initialX,
                                               ViReal64 *xIncrement);

/* Used when AGX2K3K_ATTR_ACQUISTION_TYPE is either */
/* AGX2K3K_VAL_PEAK_DETECT */
DllExport ViStatus _VI_FUNC  agx2k3k_FetchMinMaxWaveform (ViSession vi,
                                                ViConstString channel,
                                                ViInt32 waveformSize,
                                                ViReal64 minWaveform[],
                                                ViReal64 maxWaveform[],
                                                ViInt32 *actualPoints,
                                                ViReal64 *initialX,
                                                ViReal64 *xIncrement);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBTriggerSource (ViSession vi,
                                                     ViInt32 trigger, ViInt32 speed,
                                                     ViConstString dPlus, ViConstString dMinus);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMeasSource (ViSession vi,
                                               ViConstString source1,
                                               ViConstString source2);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMeasurementDelay (ViSession vi,
                                                     ViInt32 edgeSpec1,
                                                     ViInt32 edgeSpec2);

/*- Waveform Measurements ----------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_ConfigureRefLevels (ViSession vi,
                                               ViReal64 lowRef,
                                               ViReal64 midRef,
                                               ViReal64 highRef);

DllExport ViStatus _VI_FUNC  agx2k3k_ReadWaveformMeasurement (ViSession vi,
                                                    ViConstString channel,
                                                    ViInt32 measurementFunction,
                                                    ViInt32 maxTime,
                                                    ViReal64 *measurement);

DllExport ViStatus _VI_FUNC  agx2k3k_FetchWaveformMeasurement (ViSession vi,
                                                     ViConstString channel,
                                                     ViInt32 measurementFunction,
                                                     ViReal64 *measurement);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUARTBus (ViSession vi,
                                            ViConstString busName,
                                            ViInt32 baudRate,
                                            ViInt32 bitOrder,
                                            ViInt32 frameTerminateValue,
                                            ViInt32 parity,
                                            ViInt32 polarity,
                                            ViInt32 bitCount);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUARTBusSource (ViSession vi,
                                                  ViConstString busName,
                                                  ViConstString rxSource,
                                                  ViConstString txSource);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUARTTrigger (ViSession vi,
                                                ViConstString busName,
                                                ViInt32 triggerType,
                                                ViInt32 triggerCondition,
                                                ViInt32 triggerData,
                                                ViInt32 burstValue,
                                                ViReal64 idlePeriod);

/*- Auto Setup ----------------------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_AutoSetup (ViSession vi);

/*- Utility -------------------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_IsInvalidWfmElement (ViSession vi, ViReal64 elementValue,
                                               ViBoolean *isInvalid);

/*- Error Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_error_query (ViSession vi, ViInt32 *errorCode,
                                        ViChar errorMessage[]);

DllExport ViStatus _VI_FUNC  agx2k3k_GetError (ViSession vi, ViStatus *code,
                                     ViInt32 bufferSize, ViChar description[]);

DllExport ViStatus _VI_FUNC  agx2k3k_ClearError (ViSession vi);

DllExport ViStatus _VI_FUNC agx2k3k_error_message (ViSession vi, ViStatus errorCode,
                                         ViChar errorMessage[256]);

/*- Interchangeability Checking Functions ------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_GetNextInterchangeWarning (ViSession vi,
                                                     ViInt32 bufferSize,
                                                     ViChar warnString[]);

DllExport ViStatus _VI_FUNC agx2k3k_ResetInterchangeCheck (ViSession vi);

DllExport ViStatus _VI_FUNC agx2k3k_ClearInterchangeWarnings (ViSession vi);


/*- System Functions ---------------------------------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_viWrite (ViSession vi, ViByte buffer[], ViInt64 count, ViInt64 *returnCount);
DllExport ViStatus _VI_FUNC  agx2k3k_viRead (ViSession vi, ViInt64 bufferSize, ViByte buffer[], ViInt64 *returnCount);

/*- Utility Functions --------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_InvalidateAllAttributes (ViSession vi);

DllExport ViStatus _VI_FUNC  agx2k3k_reset (ViSession vi);

DllExport ViStatus _VI_FUNC  agx2k3k_ResetWithDefaults (ViSession vi);

DllExport ViStatus _VI_FUNC  agx2k3k_self_test (ViSession vi, ViInt16 *selfTestResult,
                                      ViChar selfTestMessage[]);

DllExport ViStatus _VI_FUNC  agx2k3k_revision_query (ViSession vi,
                                           ViChar instrumentDriverRevision[],
                                           ViChar firmwareRevision[]);

DllExport ViStatus _VI_FUNC agx2k3k_Disable (ViSession vi);

/* Obsolete Direct IO Functions. */
DllExport ViStatus _VI_FUNC  agx2k3k_WriteInstrData (ViSession vi, ViConstString writeBuffer);

DllExport ViStatus _VI_FUNC  agx2k3k_ReadInstrData (ViSession vi, ViInt32 numBytes,
                                          ViChar rdBuf[], ViInt32 *bytesRead);

/*- Set, Get, and Check Attribute Functions ----------------------------*/
DllExport ViStatus _VI_FUNC  agx2k3k_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);

DllExport ViStatus _VI_FUNC  agx2k3k_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);

DllExport ViStatus _VI_FUNC  agx2k3k_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]);

DllExport ViStatus _VI_FUNC  agx2k3k_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);

DllExport ViStatus _VI_FUNC  agx2k3k_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

DllExport ViStatus _VI_FUNC  agx2k3k_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);

DllExport ViStatus _VI_FUNC  agx2k3k_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);

DllExport ViStatus _VI_FUNC  agx2k3k_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value);

DllExport ViStatus _VI_FUNC  agx2k3k_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);

DllExport ViStatus _VI_FUNC  agx2k3k_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

DllExport ViStatus _VI_FUNC  agx2k3k_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);

DllExport ViStatus _VI_FUNC  agx2k3k_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);

DllExport ViStatus _VI_FUNC  agx2k3k_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value);

DllExport ViStatus _VI_FUNC  agx2k3k_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);

DllExport ViStatus _VI_FUNC  agx2k3k_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

/*********************************************************
 *      Functions reserved for class driver use only.
 *      End-users should not call these functions.
 *********************************************************/
DllExport ViStatus _VI_FUNC  agx2k3k_IviInit (ViRsrc resourceName, ViBoolean IDQuery,
                                    ViBoolean reset, ViSession vi);

DllExport ViStatus _VI_FUNC  agx2k3k_IviClose (ViSession vi);

/*---------------------------------------------------------------------------
 *------------------------ New Function Defined in version 2.0 ----------*
 * ---------------------------------------------------------------------------*/

/*- Power Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSignalSource (ViSession vi, ViInt32 CurrentFirstSource, ViInt32 CurrentSecondSource,
                                                 ViInt32 VoltageFirstSource, ViInt32 VoltageSecondSource);
DllExport ViStatus _VI_FUNC agx2k3k_AutoSetupPowerSignal (ViSession vi, ViInt32 PowerAnalysisType);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerQuality (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerSLEW (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerControlResponse (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerSLEWSource (ViSession vi, ViInt32 Source);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerRipple (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerQuality (ViSession vi, ViInt32 QualityType);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerOnOff (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerPSRR (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerPSRR (ViSession vi, ViInt32 FreqMax, ViInt32 FreqMin, ViInt32 MaxRatioValue);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerSignal (ViSession vi, ViInt32 SignalCycles, ViReal64 SignalDuration,
                                                ViReal64 SignalInrushExpected, ViInt32 SignalOvershoot, ViReal64 MaxExpectedVoltage,
                                                ViReal64 SteadyStateVoltage);
DllExport ViStatus _VI_FUNC agx2k3k_StopPowerOnOff (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_NextPowerOnOff (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerInrush (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerModulation (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerModulation (ViSession vi, ViInt32 ModulationSource, ViInt32 ModulationType);
DllExport ViStatus _VI_FUNC agx2k3k_StopPowerInrush (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_NextPowerInrush (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerHarmonics (ViSession vi, ViInt32 DisplayStyle, ViInt32 FreqSettingLine, ViInt32 Standard);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerEfficiency (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerHarmonics (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_EnablePowerAnalysis (ViSession vi, ViBoolean PowerAnalysisEnabled);
DllExport ViStatus _VI_FUNC agx2k3k_LaunchAutoDeskew (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerSwitch (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerSwitch (ViSession vi, ViInt32 SwitchConduction, ViInt32 CurrentSwitchPercent,
                                                ViInt32 VoltageSwitchPercent, ViReal64 SwitchRDS, ViReal64 SwitchVCE);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyPowerTransient (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_StopPowerTransient (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_NextPowerTransient (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerTransient (ViSession vi, ViReal64 InitCurrent, ViReal64 NewCurrent);

DllExport ViStatus _VI_FUNC agx2k3k_ReadPowerHarmonics (ViSession vi, ViInt32 DataLength, ViChar TableData[]);
DllExport ViStatus _VI_FUNC agx2k3k_ReadHarmonicsPowerAnalysis (ViSession vi, ViInt32 *FailCount, ViReal64 *PowerFactor,
                                                      ViInt32 *IterationCount, ViReal64 *TotalHarmonicsDistortion, ViInt32 *OverallStatus);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerCycleCount (ViSession vi, ViInt32 HarmonicsCycleCount, ViInt32 QualityCycleCount);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerDuration (ViSession vi, ViReal64 EfficiencyDuration, ViReal64 ModulationDuration,
                                                  ViReal64 TurnOffDuration, ViReal64 TurnOnDuration, ViReal64 RippleDuration,
                                                  ViReal64 TransientDuration);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerVoltage (ViSession vi, ViReal64 InrushMaxVoltage, ViReal64 TurnOffMaxVoltage,
                                                 ViReal64 TurnOnMaxVoltage, ViReal64 TurnOffSteadyVoltage, ViReal64 TurnOnSteadyVoltage,
                                                 ViReal64 TransientSteadyVoltage);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerInrushCurrent (ViSession vi, ViReal64 InrushCurrentAmplitude);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerControlResponse (ViSession vi, ViInt32 StartFrequency, ViInt32 StopFrequency,
                                                         ViInt32 YMaximum, ViInt32 YMinimum);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerAnalysisEfficiency (ViSession vi, ViInt32 PowerEfficiencyType);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigurePowerAnalysisHarmonics (ViSession vi, ViInt32 PowerHarmonicsType, ViBoolean UserDefinedPowerEnabled,
                                                           ViReal64 UserDefinedPower);
/*- Search Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchSerialA429 (ViSession vi, ViInt32 Label, ViInt32 Mode,
                                                     ViConstString PatternData, ViConstString PatternSDI, ViConstString PatternSSM);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchSerialFlexray (ViSession vi, ViInt32 Cycle, ViInt32 Mode,
                                                        ViConstString Data, ViInt32 FrameID, ViInt32 DataLength);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchSerialM1553 (ViSession vi, ViInt32 Mode, ViConstString PatternData,
                                                      ViInt32 RemoteTerminalAddress);

/*- Recall Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_RecallDataFromFile (ViSession vi, ViConstString FileName, ViInt32 StartColumn);
DllExport ViStatus _VI_FUNC agx2k3k_RecallSerialBusDataFromFile (ViSession vi, ViInt32 FileType, ViConstString FileName,
                                                       ViInt32 SerialBus);

/*- Measurement Goup Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMeasureStatistics (ViSession vi, ViBoolean DeviationEnabled, ViInt32 MaxStatisticsNumber);

/*- Demo Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureDemoFunction (ViSession vi, ViInt32 DemoFunction);

/*- Trigger Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ForceTrigger (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerDelay (ViSession vi, ViInt32 ArmSlope, ViInt32 ArmSource,
                                                 ViReal64 DelayTime, ViInt32 TriggerCount, ViInt32 TriggerSlope,
                                                 ViInt32 TriggerSource);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerPattern (ViSession vi, ViInt32 Format, ViInt32 Qualifier,
                                                   ViReal64 GreaterThan, ViReal64 LessThan);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerTV (ViSession vi,
                                              ViInt32 UDTVEnumber,
                                              ViBoolean UDTVHsyncEnabled,
                                              ViReal64 UDTVHtime,
                                              ViReal64 UDTVPGHtime);

/*- Function Group Group Functions ----------------------------------------------------*/
/*- Save Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_SaveArbitraryWave (ViSession vi, ViConstString fileName);
DllExport ViStatus _VI_FUNC agx2k3k_SavePowerAnalysis (ViSession vi, ViConstString fileName);
DllExport ViStatus _VI_FUNC agx2k3k_EnableSaveMaxWave (ViSession vi, ViBoolean SaveMaxWaveEnabled);
DllExport ViStatus _VI_FUNC agx2k3k_SaveAnalysisResults (ViSession vi, ViConstString OutputFile, ViBoolean CursorEnabled,
                                               ViBoolean MaskEnabled, ViBoolean MeasurementEnabled, ViBoolean SearchEnabled,
                                               ViBoolean SegmentedEnabled);
DllExport ViStatus _VI_FUNC agx2k3k_SaveUSBSignalQualityTestResults (ViSession vi, ViConstString OutputFile);
DllExport ViStatus _VI_FUNC agx2k3k_SaveMultiChanWaveformData (ViSession vi, ViConstString OutputFile);


/*- SBUS Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_AutoSetupA429 (ViSession vi, ViConstString busName);
DllExport ViStatus _VI_FUNC agx2k3k_AutoSetupFlexray (ViSession vi, ViConstString busName);
DllExport ViStatus _VI_FUNC agx2k3k_AutoSetupM1553 (ViSession vi, ViConstString busName);
DllExport ViStatus _VI_FUNC agx2k3k_ResetSBusA429 (ViSession vi, ViConstString busName);
DllExport ViStatus _VI_FUNC agx2k3k_ResetSBusFlexray (ViSession vi, ViConstString busName);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSBusA429 (ViSession vi, ViConstString BusName, ViInt32 Base,
                                             ViInt32 Format, ViInt32 Signal, ViInt32 Source, ViInt32 SignalSpeed);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSBusFlexray (ViSession vi, ViConstString BusName, ViInt32 Baudrate,
                                                ViInt32 Channel, ViInt32 Source);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSBusA429Trigger (ViSession vi,
                                                    ViChar busName[],
                                                    ViInt32 triggerLabel,
                                                    ViChar triggerPatternData[],
                                                    ViChar triggerPatternSDI[],
                                                    ViChar triggerPatternSSM[],
                                                    ViInt32 triggerType,
                                                    ViInt32 triggerRangeMin,
                                                    ViInt32 triggerRangeMax);

DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSBusM1553 (ViSession vi, ViConstString BusName, ViInt32 Base,
                                              ViInt32 Source, ViInt32 TriggerRTA, ViConstString TriggerPatternData,
                                              ViInt32 TriggerType);


/*- Marker Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMarkerXCursorUnit (ViSession vi, ViInt32 XCursorUnit, ViBoolean ResetXCursorLocation);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMarkerYCursorUnit (ViSession vi, ViInt32 YCursorUnit, ViBoolean ResetYCursorLocation);

/*- WGEN Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModulationNoise (ViSession vi, ViInt32 byteOrder, ViBoolean interpolateEnabled, ViInt32 noisePercent);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModNoiseMultiChan (ViSession vi, ViChar WaveGenChannel[], ViInt32 ByteOrder, ViInt32 NoisePercent);

/* New functions added in version 3.0 */

/*- Acquisition Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureRefSignalType (ViSession vi, ViInt32 ReferenceSignalMode);

/*- Digital Voltmeter Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureDigitalVoltmeter (ViSession vi, ViBoolean AutoRange, ViBoolean EnableDVM,
                                                     ViInt32 DVMMode, ViInt32 DVMAnalogSource);
DllExport ViStatus _VI_FUNC agx2k3k_ReadDigitalVoltmeter (ViSession vi, ViReal64* DVMFrequency, ViReal64* DVMCurrent);

/*- Function Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFunAverageNumber (ViSession vi, ViChar FunctionChannel[], ViInt32 NumberAverages);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSelectedFunction (ViSession vi, ViChar FunctionChannel[], ViReal64 VerticalAxis, ViReal64 ReferenceLevel);

/*- Marker Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureMarkerUnits (ViSession vi, ViInt32 Dimension);
DllExport ViStatus _VI_FUNC agx2k3k_ReadMarkerDYDX (ViSession vi, ViPReal64 DYDX);

/*- Measurement Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ReadQualityPowerAnalysis (ViSession vi, ViReal64 *PhaseAngle, ViReal64 *ApparentPower,
                                                    ViReal64 *CrestFactor, ViReal64 *PowerFactor, ViReal64 *ReactivePower, ViReal64 *RealPower);
DllExport ViStatus _VI_FUNC agx2k3k_ReadCurrentValue (ViSession vi, ViReal64 *Area);
DllExport ViStatus _VI_FUNC agx2k3k_ClearMeasurementsMarkers (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ReadSwitchLossPowerAnalysis (ViSession vi, ViReal64 *PowerLossPerCycle, ViReal64 *EnergyLoss,
                                                       ViReal64 *PowerLoss);
DllExport ViStatus _VI_FUNC agx2k3k_ReadEfficiencyPowerAnalysis (ViSession vi, ViReal64 *InputPower, ViReal64 *OutputPower);
DllExport ViStatus _VI_FUNC agx2k3k_ReadOnOffTime (ViSession vi, ViReal64 *TurnOffTime, ViReal64 *TurnOnTime);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureStatisticsType (ViSession vi, ViInt32 MeasureInformationType);
DllExport ViStatus _VI_FUNC agx2k3k_ReadDisplayedMeasurements (ViSession vi, ViChar ContinuouslyDisplayedMeasurements[], ViInt32 ResultLength);



/*- Serial Bus Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ReadSBusA429 (ViSession vi, ViChar BusName[], ViInt32 *A429ErrorCount, ViInt32 *A429WordCount);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureA429TriggerRange (ViSession vi, ViChar BusName[], ViConstString A429TriggerRange);
DllExport ViStatus _VI_FUNC agx2k3k_ReadSBusFlexray (ViSession vi, ViChar BusName[], ViInt32 *FlexSyncFrameCount, ViInt32 *FlexTotalFrameCount);
DllExport ViStatus _VI_FUNC agx2k3k_ReadSBusErrorCount (ViSession vi, ViChar BusName[], ViPInt32 SpecErrorCount);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFlexTriggerType (ViSession vi, ViChar BusName[], ViInt32 FlexrayTriggerMode);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBTriggerSource2 (ViSession vi, ViChar BusName[], ViInt32 Speed, ViInt32 D_Plus, ViInt32 D_Minus);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBTriggerType (ViSession vi, ViChar BusName[], ViInt32 TriggerType);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBPortion (ViSession vi, ViChar BusName[], ViConstString AddressPortion,
                                               ViConstString CRCPortion, ViConstString DataPortion, ViInt32 DataLength, ViConstString EndpointPortion);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBPortionAdvanced (ViSession vi, ViChar BusName[], ViConstString ETPortion,
                                                       ViConstString FramePortion, ViConstString HubAddressPortion, ViConstString PidCheckPortion,
                                                       ViConstString PortPortion, ViConstString SCPortion, ViConstString SEUPortion);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBPID (ViSession vi, ViChar BusName[], ViInt32 DataPid,
                                           ViInt32 HandshakePid, ViInt32 SpecialPid, ViInt32 TokenPid);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBNormal (ViSession vi, ViChar BusName[], ViInt32 UsbBase, ViInt32 UsbDifferentialSource);

/*- Search Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchUSBType (ViSession vi, ViInt32 SearchType);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchUSBPortion (ViSession vi, ViConstString AddressPortion, ViConstString CRCPortion,
                                                     ViConstString DataPortion, ViInt32 DataLength, ViConstString EndpointPortion);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchUSBPortionAd (ViSession vi, ViConstString ETPortion, ViConstString FramePortion,
                                                       ViConstString HubAddressPortion, ViConstString PortPortion, ViConstString SCPortion,
                                                       ViConstString SEUPortion);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchUSBPID (ViSession vi, ViInt32 DataPid, ViInt32 HandshakePid,
                                                 ViInt32 SpecialPid, ViInt32 TokenPid);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchEventNumber (ViSession vi, ViInt32 EventNumber);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchPeak (ViSession vi, ViReal64 Excursion, ViInt32 MaxPeaks,
                                               ViReal64 Threshold, ViInt32 PeakSource);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchSerialCAN (ViSession vi, ViConstString Message, ViConstString SignalName,
                                                    ViReal64 SignalValue);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchSerialLIN (ViSession vi, ViConstString Message, ViConstString SignalName,
                                                    ViReal64 SignalValue);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSearchSerialSENT (ViSession vi, ViConstString FastData, ViInt32 Mode,
                                                     ViInt32 SlowData, ViInt32 SlowID);

/*- WGEN Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureArbWaveformData (ViSession vi, ViChar WaveGenChannel[], ViConstString ArbData,
                                                    ViConstString ArbDacData, ViInt32 PointsCount);
DllExport ViStatus _VI_FUNC agx2k3k_ClearArbWaveformData (ViSession vi, ViChar WaveGenChannel[]);
DllExport ViStatus _VI_FUNC agx2k3k_StoreArbWaveform (ViSession vi, ViChar WaveGenChannel[], ViInt32 Source);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModulationAM (ViSession vi, ViChar WaveGenChannel[], ViInt32 DepthPercent, ViReal64 WaveformFrequency);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModulationFM (ViSession vi, ViChar WaveGenChannel[], ViReal64 Deviation, ViReal64 Frequency);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModulationFSKey (ViSession vi, ViChar WaveGenChannel[], ViReal64 Frequency, ViReal64 Rate);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModulationFunction (ViSession vi, ViChar WaveGenChannel[], ViInt32 SignalShape, ViInt32 RampSymmetry);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModulationState (ViSession vi, ViChar WaveGenChannel[], ViBoolean EnableModulatedWaveform);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureModulationType (ViSession vi, ViChar WaveGenChannel[], ViInt32 ModulationType);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGeneratorTracking (ViSession vi, ViChar WaveGenChannel[], ViBoolean EnableTracking,
                                                      ViBoolean EnableAmplitudeTracking, ViBoolean EnableFrequencyTracking, ViBoolean SetIdentical);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureGeneratorInverted (ViSession vi, ViChar WaveGenChannel[], ViInt32 OutputPolarity);

/*- Interpolation Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureInterpolation (ViSession vi, ViInt32 InterpolationMethod);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureInterpolationByChannel (ViSession vi, ViChar waveGenChannel[], ViInt32 InterpolationMethod);


/*- Compliance Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_RunUSBComplianceTest (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_AutoSetupUSBComplianceTest (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBComplianceTest (ViSession vi, ViInt32 SignleQualityTestType, ViInt32 ConnectionType,
                                                      ViInt32 TestEndType, ViInt32 HubsNumber);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureUSBComplianceSource (ViSession vi, ViInt32 SourceAdjacent, ViInt32 SourceDifferential,
                                                        ViInt32 SourceDMinus, ViInt32 SourceDPlus);
/*- FFT Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ClearFFTEvaluatedWaveforms (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFFTAverageCount (ViSession vi, ViInt32 FFTWaveformCount);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFFT (ViSession vi, ViReal64 FFTVerticalOffset, ViReal64 FFTVerticalRange,
                                        ViReal64 FFTReferenceLevel, ViReal64 FFTVerticalScale, ViInt32 FFTVerticalScaleUnits,
                                        ViBoolean FFTDisplayEnabled);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFFTFrequency (ViSession vi, ViInt32 FrequencyType, ViReal64 StartFrequency,
                                                 ViReal64 StopFrequency, ViReal64 FFTFrequencySpan, ViReal64 FFTFrequencyCenter);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFFTSource (ViSession vi, ViInt32 FFTSource);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFFTWindowing (ViSession vi, ViInt32 FFTWindow);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureFFTDisplayMode (ViSession vi, ViInt32 FFTDisplayMode);
/*- Counter Group Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_EnableCounter (ViSession vi, ViBoolean CounterEnabled);
DllExport ViStatus _VI_FUNC agx2k3k_ResetTotalizerCount (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCounter (ViSession vi, ViInt32 CounterSource, ViInt32 CounterMode,
                                            ViInt32 CounterDigits);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCounterTotalizer (ViSession vi, ViInt32 CounterSource, ViBoolean TotalizerGatingEnabled,
                                                     ViInt32 GatingSignalPolarity, ViInt32 TotalizerSlope);
DllExport ViStatus _VI_FUNC agx2k3k_ReadCounterValue (ViSession vi, ViPReal64 CounterValue);

/*---------------------------------------------------------------------------
 *------------------------ New Function Defined in version 3.2 ----------*
 * ---------------------------------------------------------------------------*/

/*-  Channel Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureChannelBandwidth (ViSession vi, ViChar channelName[], ViReal64 bandwidth);

/*-  Clock Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureClockRecoveryFixed (ViSession vi, ViInt32 option, ViReal64 dataRate);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureClockRecoveryFOPLL (ViSession vi, ViReal64 dataRate, ViReal64 loopBandwidth);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureClockRecoverySOPLL (ViSession vi, ViReal64 dataRate, ViReal64 loopBandwidth,
                                                       ViReal64 dampingFactor);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureClockRecoveryExplicit (ViSession vi, ViConstString source, ViInt32 direction,
                                                          ViInt32 multiplier);

/*-  Histogram Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureHistogram (ViSession vi, ViInt32 mode);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureHistogramMeasurement (ViSession vi, ViInt32 measurement);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureHistogramWaveform (ViSession vi, ViInt32 axis, ViReal64 leftLimit,
                                                      ViReal64 rightLimit, ViReal64 topLimit, ViReal64 bottomLimit,
                                                      ViConstString source);
DllExport ViStatus _VI_FUNC agx2k3k_ResetHistogramCounters (ViSession vi);

/*-  Jitter Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ApplyJitterHistogram (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyJitterSpectrum (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureJitterMeasurement (ViSession vi, ViInt32 measurement);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureJitterAnalysisEnabled (ViSession vi, ViBoolean enabled);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyJitterTrend (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ApplyJitterTrendSmooth (ViSession vi);

/*-  Measurement Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ReadNCycleToNCycleJitter (ViSession vi, ViConstString source, ViInt32 direction,
                                                    ViInt32 numPeriods, ViReal64* jitter);
DllExport ViStatus _VI_FUNC agx2k3k_ReadNPeriodJitter (ViSession vi, ViConstString source, ViInt32 slope,
                                             ViInt32 numPeriods, ViReal64* jitter);
DllExport ViStatus _VI_FUNC agx2k3k_ReadDataTIEJitter (ViSession vi, ViConstString source, ViInt32 unit,
                                             ViReal64* jitter);
DllExport ViStatus _VI_FUNC agx2k3k_ReadClockTIEJitter (ViSession vi, ViConstString source, ViInt32 unit,
                                              ViInt32 direction, ViReal64* jitter);
DllExport ViStatus _VI_FUNC agx2k3k_ReadHistogramMeasurement (ViSession vi, ViInt32 measurement, ViReal64* value);
DllExport ViStatus _VI_FUNC agx2k3k_ReadRTEyeMeasurement (ViSession vi, ViInt32 measurement, ViReal64* value);
DllExport ViStatus _VI_FUNC agx2k3k_ReadCycleToCycleWidth (ViSession vi, ViInt32 polarity, ViConstString source, ViReal64* width);

/*-  Real-Time Eye Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_BuildRealTimeEye (ViSession vi);
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureRealTimeEye (ViSession vi, ViConstString source, ViBoolean enabled);
DllExport ViStatus _VI_FUNC agx2k3k_QueryUIIntervals (ViSession vi, ViInt32* intervals);

/*-  CAN Bus Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureCANFDStandard (ViSession vi, ViInt32 standard);

/*-  System Precision Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureSystemPrecision (ViSession vi, ViBoolean enabled, ViInt32 length);

/*-  Trigger Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureTriggerJFreeEnabled (ViSession vi, ViBoolean enabled);

/*-  Waveform Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC agx2k3k_ConfigureWaveformSubsource (ViSession vi, ViInt32 subsource);

/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/

#define AGX2K3K_ERROR_CHANNEL_NOT_ENABLED    IVISCOPE_ERROR_CHANNEL_NOT_ENABLED
#define AGX2K3K_ERROR_MAX_TIME_EXCEEDED      IVISCOPE_ERROR_MAX_TIME_EXCEEDED

/****************************************************************************
 *---------------------------- End Include File ----------------------------*
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __AGX2K3K_HEADER */




