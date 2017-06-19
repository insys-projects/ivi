/****************************************************************************
 *            ITSCOPE -  Digital Oscilloscope
 *---------------------------------------------------------------------------
 * (c) 2017, Instrumental Systems, Corporation.  All Rights Reserved.         
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:    itscope.h                                        
 * Purpose:  ITSCOPE - Digital Oscilloscope
 *           instrument driver declarations.
 *                                                                          
 ****************************************************************************/

#ifndef __ITSCOPE_HEADER
#define __ITSCOPE_HEADER

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
//#define DllExport	__declspec( DllExport )
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

#define MAXDEV	128
#define MAXCHAN	8

/****************************************************************************
 *--------------------- Instrument Driver Information ----------------------*
 ****************************************************************************/
#define ITSCOPE_MAJOR_VERSION                1     /* Instrument driver major version   */
#define ITSCOPE_MINOR_VERSION                0     /* Instrument driver minor version   */
                                                                
#define ITSCOPE_CLASS_SPEC_MAJOR_VERSION     4     /* Class specification major version */
#define ITSCOPE_CLASS_SPEC_MINOR_VERSION     1     /* Class specification minor version */

#define ITSCOPE_SUPPORTED_INSTRUMENT_MODELS  "PE510K1, PE520K1"
                    
#define ITSCOPE_DRIVER_VENDOR                "Instrumental Systems"
    
#ifdef _IVI_mswin64_
#define ITSCOPE_DRIVER_DESCRIPTION           "InSys ITSCOPE Series Oscilloscopes [Compiled for 64-bit]"
#else
#define ITSCOPE_DRIVER_DESCRIPTION           "InSys ITSCOPE Series Oscilloscopes"
#endif

/**************************************************************************** 
 *------------------------------ Useful Macros -----------------------------* 
 ****************************************************************************/

    /* Defined values for the status parameter of the ITSCOPE_Acquisition_Status function */
#define ITSCOPE_VAL_ACQ_COMPLETE              IVISCOPE_VAL_ACQ_COMPLETE       
#define ITSCOPE_VAL_ACQ_IN_PROGRESS           IVISCOPE_VAL_ACQ_IN_PROGRESS    

    /* Define Channel Names */
#define ITSCOPE_VAL_CH_1                      "CH1"
#define ITSCOPE_VAL_CH_2                      "CH2"
#define ITSCOPE_VAL_CH_3                      "CH3"
#define ITSCOPE_VAL_CH_4                      "CH4"
#define ITSCOPE_VAL_CH_5                      "CH5"
#define ITSCOPE_VAL_CH_6                      "CH6"
#define ITSCOPE_VAL_CH_7                      "CH7"
#define ITSCOPE_VAL_CH_8                      "CH8"
#define ITSCOPE_VAL_MATH                      "MATH"
#define ITSCOPE_VAL_REF_1                     "REF1"
#define ITSCOPE_VAL_REF_2                     "REF2"
#define ITSCOPE_VAL_REF_3                     "REF3"
#define ITSCOPE_VAL_REF_4                     "REF4"
#define ITSCOPE_VAL_REF_5                     "REF5"
#define ITSCOPE_VAL_REF_6                     "REF6"
#define ITSCOPE_VAL_REF_7                     "REF7"
#define ITSCOPE_VAL_REF_8                     "REF8"

#define ITSCOPE_VAL_HISTOGRAM                 "HIS"

     /*
      *The following macros are used in function 
      *itscope_ReadFreqDomainMeasurement.
      */
#define ITSCOPE_VAL_ADJ_LA1                   "LA1"
#define ITSCOPE_VAL_ADJ_LA2                   "LA2"
#define ITSCOPE_VAL_ADJ_LA3                   "LA3"
#define ITSCOPE_VAL_ADJ_UA1                   "UA1"
#define ITSCOPE_VAL_ADJ_UA2                   "UA2"
#define ITSCOPE_VAL_ADJ_UA3                   "UA3"
/**************************************************************************** 
 *---------------------------- Attribute Defines ---------------------------* 
 ****************************************************************************/

    /*- IVI Inherent Instrument Attributes ---------------------------------*/    

        /* User Options */
#define ITSCOPE_ATTR_RANGE_CHECK                   IVI_ATTR_RANGE_CHECK                    /* ViBoolean */
#define ITSCOPE_ATTR_QUERY_INSTRUMENT_STATUS       IVI_ATTR_QUERY_INSTRUMENT_STATUS        /* ViBoolean */
#define ITSCOPE_ATTR_CACHE                         IVI_ATTR_CACHE                          /* ViBoolean */
#define ITSCOPE_ATTR_SIMULATE                      IVI_ATTR_SIMULATE                       /* ViBoolean */
#define ITSCOPE_ATTR_RECORD_COERCIONS              IVI_ATTR_RECORD_COERCIONS               /* ViBoolean */

#define ITSCOPE_ATTR_DRIVER_SETUP                  IVI_ATTR_DRIVER_SETUP
#define ITSCOPE_ATTR_INTERCHANGE_CHECK             IVI_ATTR_INTERCHANGE_CHECK 

        /* Instrument Capabilities */
#define ITSCOPE_ATTR_CHANNEL_COUNT                 IVI_ATTR_CHANNEL_COUNT                  /* ViInt32,  read-only  */
#define ITSCOPE_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only */

        /* Driver Information  */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_PREFIX        IVI_ATTR_SPECIFIC_DRIVER_PREFIX         /* ViString, read-only  */
#define ITSCOPE_ATTR_SUPPORTED_INSTRUMENT_MODELS   IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS    /* ViString, read-only  */
#define ITSCOPE_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only  */
#define ITSCOPE_ATTR_INSTRUMENT_MANUFACTURER       IVI_ATTR_INSTRUMENT_MANUFACTURER        /* ViString, read-only  */
#define ITSCOPE_ATTR_INSTRUMENT_MODEL              IVI_ATTR_INSTRUMENT_MODEL               /* ViString, read-only  */
#define ITSCOPE_ATTR_INSTRUMENT_FIRMWARE_REVISION  IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION   /* ViString, read-only  */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_REVISION      IVI_ATTR_SPECIFIC_DRIVER_REVISION       /* ViString, read-only  */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_VENDOR        IVI_ATTR_SPECIFIC_DRIVER_VENDOR         /* ViString, read-only  */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_DESCRIPTION   IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION    /* ViString, read-only  */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

        /* Advanced Session Information */
#define ITSCOPE_ATTR_LOGICAL_NAME                  IVI_ATTR_LOGICAL_NAME                   /* ViString, read-only  */
#define ITSCOPE_ATTR_IO_RESOURCE_DESCRIPTOR        IVI_ATTR_IO_RESOURCE_DESCRIPTOR         /* ViString, read-only  */
    
    /*- Channel Sub-system ------------------------------------------------*/                    
#define ITSCOPE_ATTR_CHANNEL_ENABLED               IVISCOPE_ATTR_CHANNEL_ENABLED           /* ViBoolean, multi-channel         */
#define ITSCOPE_ATTR_PROBE_ATTENUATION             IVISCOPE_ATTR_PROBE_ATTENUATION         /* ViReal64,  multi-channel         */
#define ITSCOPE_ATTR_PROBE_SENSE_VALUE             IVISCOPE_ATTR_PROBE_SENSE_VALUE         /* ViReal64,  multi-channel         */
#define ITSCOPE_ATTR_VERTICAL_RANGE                IVISCOPE_ATTR_VERTICAL_RANGE            /* ViReal64,  multi-channel, volts  */
#define ITSCOPE_ATTR_VERTICAL_OFFSET               IVISCOPE_ATTR_VERTICAL_OFFSET           /* ViReal64,  multi-channel, volts  */
#define ITSCOPE_ATTR_VERTICAL_COUPLING             IVISCOPE_ATTR_VERTICAL_COUPLING         /* ViInt32,   multi-channel         */
#define ITSCOPE_ATTR_MAX_INPUT_FREQUENCY           IVISCOPE_ATTR_MAX_INPUT_FREQUENCY       /* ViReal64,  multi-Channel, hertz  */
#define ITSCOPE_ATTR_INPUT_IMPEDANCE               IVISCOPE_ATTR_INPUT_IMPEDANCE           /* ViReal64,  multi-channel, ohms   */

    
    /*- Trigger Sub-system -------------------------------------------------*/
#define ITSCOPE_ATTR_TRIGGER_TYPE                  IVISCOPE_ATTR_TRIGGER_TYPE              /* ViInt32                          */
#define ITSCOPE_ATTR_TRIGGER_SOURCE                IVISCOPE_ATTR_TRIGGER_SOURCE            /* ViString                         */
#define ITSCOPE_ATTR_TRIGGER_LEVEL                 IVISCOPE_ATTR_TRIGGER_LEVEL             /* ViReal64,  volts                 */
#define ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 94L)   /* ViReal64,  multi-channel, volts  */  
#define ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 95L)   /* ViReal64,  multi-channel, volts  */
#define ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 96L)   /* ViReal64,  multi-channel, volts  */
#define ITSCOPE_ATTR_TRIGGER_HOLDOFF               IVISCOPE_ATTR_TRIGGER_HOLDOFF           /* ViReal64,  seconds               */
#define ITSCOPE_ATTR_TRIGGER_MODIFIER              IVISCOPE_ATTR_TRIGGER_MODIFIER          /* ViInt32                          */

  
        /* Edge Trigger */
#define ITSCOPE_ATTR_TRIGGER_COUPLING              IVISCOPE_ATTR_TRIGGER_COUPLING          /* ViInt32                          */
#define ITSCOPE_ATTR_TRIGGER_SLOPE                 IVISCOPE_ATTR_TRIGGER_SLOPE             /* ViInt32                          */
                                                                                                                                            
        /* TV Trigger */
#define ITSCOPE_ATTR_TV_TRIGGER_POLARITY           IVISCOPE_ATTR_TV_TRIGGER_POLARITY       /* ViInt32                          */
#define ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT      IVISCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT  /* ViInt32                          */
#define ITSCOPE_ATTR_TV_TRIGGER_EVENT              IVISCOPE_ATTR_TV_TRIGGER_EVENT          /* ViInt32                          */
#define ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER        IVISCOPE_ATTR_TV_TRIGGER_LINE_NUMBER    /* ViInt32                          */
#define ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 180L)  /* ViInt32                          */
#define ITSCOPE_ATTR_TV_TRIGGER_FIELD_HOLDOFF      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 181L)  /* ViReal64                         */
#define ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_FORMAT      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 182L)  /* ViInt32                          */
#define ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 183L)  /* ViInt32                          */
#define ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_LINE_PERIOD (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 196L)  /* ViReal64                         */
#define ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SYNC_INTERVAL (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 197L) /* ViReal64                        */
    
    /* Runt Trigger */
#define ITSCOPE_ATTR_RUNT_HIGH_THRESHOLD           IVISCOPE_ATTR_RUNT_HIGH_THRESHOLD       /* ViReal64,  volts                 */
#define ITSCOPE_ATTR_RUNT_LOW_THRESHOLD            IVISCOPE_ATTR_RUNT_LOW_THRESHOLD        /* ViReal64,  volts                 */
#define ITSCOPE_ATTR_RUNT_POLARITY                 IVISCOPE_ATTR_RUNT_POLARITY             /* ViInt32                          */
#define ITSCOPE_ATTR_RUNT_WHEN                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8L)    /* ViInt32                          */
#define ITSCOPE_ATTR_RUNT_WIDTH                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9L)    /* ViReal64                         */

        /*- Glitch Trigger Attributes -*/
#define ITSCOPE_ATTR_GLITCH_POLARITY               IVISCOPE_ATTR_GLITCH_POLARITY           /* ViInt32                          */
#define ITSCOPE_ATTR_GLITCH_CONDITION              IVISCOPE_ATTR_GLITCH_CONDITION          /* ViInt32                          */
#define ITSCOPE_ATTR_GLITCH_WIDTH                  IVISCOPE_ATTR_GLITCH_WIDTH              /* ViReal64, seconds                */  

        /* AC Line Trigger */ 
#define ITSCOPE_ATTR_AC_LINE_TRIGGER_SLOPE         IVISCOPE_ATTR_AC_LINE_TRIGGER_SLOPE     /* ViInt32                          */

        /* Delay Trigger */
#define ITSCOPE_ATTR_DELAY_TRIGGER_MODE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 11L)   /* ViInt32                          */
#define ITSCOPE_ATTR_DELAY_TRIGGER_EVENTS          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12L)   /* ViInt32                          */
#define ITSCOPE_ATTR_DELAY_TRIGGER_SOURCE          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13L)   /* ViString                         */
#define ITSCOPE_ATTR_DELAY_TRIGGER_COUPLING        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14L)   /* ViInt32                          */
#define ITSCOPE_ATTR_DELAY_TRIGGER_SLOPE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 15L)   /* ViInt32                          */
#define ITSCOPE_ATTR_DELAY_TRIGGER_LEVEL           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16L)   /* ViReal64                         */
#define ITSCOPE_ATTR_DELAY_TRIGGER_TIME            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 148L)  /* ViReal64                         */

        /* Transition Trigger */
#define ITSCOPE_ATTR_TRANSITION_POLARITY           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17L)   /* ViInt32                          */
#define ITSCOPE_ATTR_TRANSITION_WHEN               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18L)   /* ViInt32                          */
#define ITSCOPE_ATTR_TRANSITION_DELTA_TIME         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19L)   /* ViReal64                         */
#define ITSCOPE_ATTR_TRANSITION_HIGH_THRESHOLD     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20L)   /* ViReal64                         */
#define ITSCOPE_ATTR_TRANSITION_LOW_THRESHOLD      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 21L)   /* ViReal64                         */
                                                                                
        /* Logic Trigger */
#define ITSCOPE_ATTR_LOGIC_THRES_BY_CH             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 97L)   /* ViReal64, multi-channel, volts   */       

        /* Pattern Trigger */
#define ITSCOPE_ATTR_PATTERN_FUNCTION              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 26L)   /* ViInt32                          */
#define ITSCOPE_ATTR_PATTERN_WHEN                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 27L)   /* ViInt32                          */
#define ITSCOPE_ATTR_PATTERN_INPUT                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 98L)
#define ITSCOPE_ATTR_PATTERN_DELTATIME             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 28L)   /* ViInt32                          */

        /* State Trigger */
#define ITSCOPE_ATTR_STATE_FUNCTION                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 99L)   /* ViInt32                          */
#define ITSCOPE_ATTR_STATE_WHEN                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 36L)   /* ViInt32                          */
#define ITSCOPE_ATTR_STATE_INPUT                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 100L)  /* ViInt32                          */  
#define ITSCOPE_ATTR_STATE_CLOCK_EDGE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 101L)  /* ViInt32                          */
#define ITSCOPE_ATTR_STATE_CLOCK_SOURCE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 92L)   /* ViInt32                          */

        /* SetHold Trigger */
#define ITSCOPE_ATTR_SETHOLD_CLOCK_EDGE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 102L)  /* ViInt32                          */
#define ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 103L)  /* ViInt32                          */
#define ITSCOPE_ATTR_SETHOLD_DATA_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 104L)  /* ViInt32                          */   
#define ITSCOPE_ATTR_SETHOLD_HOLDTIME              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 105L)  /* ViReal64                         */
#define ITSCOPE_ATTR_SETHOLD_SETTIME               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 106L)  /* ViReal64                         */ 
    
    
    
    /*- Acquisition Sub-system ---------------------------------------------*/
#define ITSCOPE_ATTR_ACQUISITION_TYPE              IVISCOPE_ATTR_ACQUISITION_TYPE          /* ViInt32                          */
#define ITSCOPE_ATTR_INITIATE_CONTINUOUS           IVISCOPE_ATTR_INITIATE_CONTINUOUS       /* ViBoolean                        */
#define ITSCOPE_ATTR_NUM_AVERAGES                  IVISCOPE_ATTR_NUM_AVERAGES              /* ViInt32                          */
#define ITSCOPE_ATTR_NUM_ENVELOPES                 IVISCOPE_ATTR_NUM_ENVELOPES             /* ViInt32                          */
#define ITSCOPE_ATTR_HORZ_TIME_PER_RECORD          IVISCOPE_ATTR_HORZ_TIME_PER_RECORD      /* ViReal64                         */
#define ITSCOPE_ATTR_HORZ_MIN_NUM_PTS              IVISCOPE_ATTR_HORZ_MIN_NUM_PTS          /* ViInt32                          */
#define ITSCOPE_ATTR_HORZ_RECORD_LENGTH            IVISCOPE_ATTR_HORZ_RECORD_LENGTH        /* ViInt32,   read-only             */
#define ITSCOPE_ATTR_ACQUISITION_START_TIME        IVISCOPE_ATTR_ACQUISITION_START_TIME    /* ViReal64,  percentage of record length  */
#define ITSCOPE_ATTR_HORZ_SAMPLE_RATE              IVISCOPE_ATTR_HORZ_SAMPLE_RATE          /* ViReal64,  read-only             */
#define ITSCOPE_ATTR_SAMPLE_MODE                   IVISCOPE_ATTR_SAMPLE_MODE               /* ViReal64,  read-only             */
    
    
    /*- Waveform Measurement -----------------------------------------------*/
#define ITSCOPE_ATTR_MEAS_HIGH_REF                 IVISCOPE_ATTR_MEAS_HIGH_REF             /* ViReal64,  percentage            */
#define ITSCOPE_ATTR_MEAS_LOW_REF                  IVISCOPE_ATTR_MEAS_LOW_REF              /* ViReal64,  percentage            */
#define ITSCOPE_ATTR_MEAS_MID_REF                  IVISCOPE_ATTR_MEAS_MID_REF              /* ViReal64,  percentage            */
#define ITSCOPE_ATTR_MEAS_REF_CALC_METH            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 52L)   /* ViInt32                          */
#define ITSCOPE_ATTR_MEAS_SOURCE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 53L)   /* ViString                         */ 
#define ITSCOPE_ATTR_MEAS_FUNCTION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 54L)   /* ViInt32                          */ 
#define ITSCOPE_ATTR_MEAS_GATING                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 55L)   /* ViBoolean                        */ 
#define ITSCOPE_ATTR_MEAS_MID2_REF                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 56L)   /* ViReal64,  percentage            */
#define ITSCOPE_ATTR_MEAS_SOURCE2                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 57L)   /* ViString                         */
#define ITSCOPE_ATTR_MEAS_DEL_DIRECTION            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 58L)   /* ViInt32                          */
#define ITSCOPE_ATTR_MEAS_DEL_EDGE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 59L)   /* ViInt32                          */
#define ITSCOPE_ATTR_MEAS_DEL_EDGE2                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 60L)   /* ViInt32                          */

    /*- Math ---------------------------------------------------------------*/
#define ITSCOPE_ATTR_MATH_TYPE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 123L)  /* ViInt32                          */   
#define ITSCOPE_ATTR_MATH_FUNCTION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 124L)  /* ViInt32                          */
#define ITSCOPE_ATTR_MATH_SOURCE_1ST               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 125L)  /* ViInt32                          */
#define ITSCOPE_ATTR_MATH_SOURCE_2ND               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 126L)  /* ViInt32                          */ 
#define ITSCOPE_ATTR_FFT_VERT_SCALE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 128L)  /* ViInt32                          */
#define ITSCOPE_ATTR_FFT_WINDOW                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 127L)  /* ViInt32                          */ 
#define ITSCOPE_ATTR_ADV_VAR1                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 129L)  /* ViReal64                         */
#define ITSCOPE_ATTR_ADV_VAR2                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 130L)  /* ViReal64                         */
#define ITSCOPE_ATTR_ADV_DEFINE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 131L)  /* ViString                         */
    
    /*- Models and Options -------------------------------------------------*/
#define ITSCOPE_ATTR_MODEL                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 61L)   /* ViInt32                          */ 

    /*- Miscellaneous ------------------------------------------------------*/
#define ITSCOPE_ATTR_ID_QUERY_RESPONSE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1L)    /* ViString,  read-only             */
#define ITSCOPE_ATTR_FETCH_BUFFER_SIZE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 149L)  /* ViInt32                          */ 
#define ITSCOPE_ATTR_WAVEFORM_SIZE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 150L)  /* ViInt32                          */ 

                                                                                          
/**************************************************************************** 
 *------------------------ Attribute Value Defines -------------------------* 
 ****************************************************************************/
    /*- Trigger Source or Input Source -*/
#define ITSCOPE_VAL_SOUR_CH1                        1
#define ITSCOPE_VAL_SOUR_CH2                        2
#define ITSCOPE_VAL_SOUR_CH3                        3
#define ITSCOPE_VAL_SOUR_CH4                        4
#define ITSCOPE_VAL_SOUR_CH5                        5
#define ITSCOPE_VAL_SOUR_CH6                        6
#define ITSCOPE_VAL_SOUR_CH7                        7
#define ITSCOPE_VAL_SOUR_CH8                        8
#define ITSCOPE_VAL_SOUR_REF1                       10
#define ITSCOPE_VAL_SOUR_REF2                       11
#define ITSCOPE_VAL_SOUR_REF3                       12
#define ITSCOPE_VAL_SOUR_REF4                       13
#define ITSCOPE_VAL_SOUR_EXT                        21
#define ITSCOPE_VAL_SOUR_NONE                       30
#define ITSCOPE_VAL_SOUR_D0                         50
#define ITSCOPE_VAL_SOUR_D1                         51
#define ITSCOPE_VAL_SOUR_D2                         52
#define ITSCOPE_VAL_SOUR_D3                         53
#define ITSCOPE_VAL_SOUR_D4                         54
#define ITSCOPE_VAL_SOUR_D5                         55
#define ITSCOPE_VAL_SOUR_D6                         56
#define ITSCOPE_VAL_SOUR_D7                         57
#define ITSCOPE_VAL_SOUR_D8                         58
#define ITSCOPE_VAL_SOUR_D9                         59
#define ITSCOPE_VAL_SOUR_D10                        60
#define ITSCOPE_VAL_SOUR_D11                        61
#define ITSCOPE_VAL_SOUR_D12                        62
#define ITSCOPE_VAL_SOUR_D13                        63  
#define ITSCOPE_VAL_SOUR_D14                        64
#define ITSCOPE_VAL_SOUR_D15                        65 
    
    /*- Ref Source-*/
#define ITSCOPE_VAL_RFSOUR_NORMAL                   1
#define ITSCOPE_VAL_RFSOUR_AVERAGE                  2
#define ITSCOPE_VAL_RFSOUR_MAXHOLD                  3
#define ITSCOPE_VAL_RFSOUR_MINHOLD                  4

    /*- Defined values for attribute ITSCOPE_ATTR_BUS_SELECT -*/
#define ITSCOPE_VAL_BUS1                            1
#define ITSCOPE_VAL_BUS2                            2
#define ITSCOPE_VAL_BUS3                            3
#define ITSCOPE_VAL_BUS4                            4

    /*- Minimum and Maximum fetch waveform buffer size -*/
#define MIN_FETCH_BUFFER_SIZE                       1000L
#define MAX_FETCH_BUFFER_SIZE                       1000000L    
    
    /*- Defined values for attribute ITSCOPE_ATTR_PROBE_ATTENUATION -*/
#define ITSCOPE_VAL_PROBE_SENSE_ON                  IVISCOPE_VAL_PROBE_SENSE_ON

    /*- Defined values for attribute ITSCOPE_ATTR_VERTICAL_COUPLING -*/
#define ITSCOPE_VAL_AC                              IVISCOPE_VAL_AC
#define ITSCOPE_VAL_DC                              IVISCOPE_VAL_DC
#define ITSCOPE_VAL_GND                             IVISCOPE_VAL_GND
#define ITSCOPE_VAL_DCREJ							(IVISCOPE_VAL_VERTICAL_COUPLING_SPECIFIC_EXT_BASE+1L)
    /*- Defined values for attribute ITSCOPE_ATTR_TRIGGER_TYPE -*/
    /*- Edge Trigger -*/    
#define ITSCOPE_VAL_EDGE_TRIGGER                    IVISCOPE_VAL_EDGE_TRIGGER   
#define ITSCOPE_VAL_AC_LINE_TRIGGER                 IVISCOPE_VAL_AC_LINE_TRIGGER 
    /*- Video Trigger -*/
#define ITSCOPE_VAL_TV_TRIGGER                      IVISCOPE_VAL_TV_TRIGGER 
    /*- Pulse Trigger -*/
#define ITSCOPE_VAL_RUNT_TRIGGER                    IVISCOPE_VAL_RUNT_TRIGGER
#define ITSCOPE_VAL_GLITCH_TRIGGER                  IVISCOPE_VAL_GLITCH_TRIGGER
#define ITSCOPE_VAL_TRANSITION_TRIGGER              (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 1L)
    /*- Logic Trigger -*/
#define ITSCOPE_VAL_PATTERN_TRIGGER                 (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 2L)      
#define ITSCOPE_VAL_STATE_TRIGGER                   (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 3L)
#define ITSCOPE_VAL_SETHOLD_TRIGGER                 (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 4L)

	/*- Main Trigger Type -*/
#define ITSCOPE_VAL_PULSE_TRIGGER                   (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 8L)
#define ITSCOPE_VAL_LOGIC_TRIGGER                   (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 9L)
#define ITSCOPE_VAL_SERIAL_TRIGGER                  (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 10L)

    /*- Defined values for attribute ITSCOPE_ATTR_TRIGGER_SOURCE -*/
#define ITSCOPE_VAL_EXTERNAL                        IVISCOPE_VAL_EXTERNAL
#define ITSCOPE_VAL_AC_LINE                         "VAL_AC_LINE"
#define ITSCOPE_VAL_CH1		                        "CH1"
#define ITSCOPE_VAL_CH2		                        "CH2"
#define ITSCOPE_VAL_CH3		                        "CH3"
#define ITSCOPE_VAL_CH4		                        "CH4"
#define ITSCOPE_VAL_CH5		                        "CH5"
#define ITSCOPE_VAL_CH6		                        "CH6"
#define ITSCOPE_VAL_CH7		                        "CH7"
#define ITSCOPE_VAL_CH8		                        "CH8"

	/*- Defined values for attribute ITSCOPE_ATTR_TRIGGER_MODIFIER -*/
#define ITSCOPE_VAL_NO_TRIGGER_MOD                  IVISCOPE_VAL_NO_TRIGGER_MOD
#define ITSCOPE_VAL_AUTO                            IVISCOPE_VAL_AUTO

    /*- Defined values for attribute ITSCOPE_ATTR_TRIGGER_COUPLING -*/          
#define ITSCOPE_VAL_HF_REJECT                       IVISCOPE_VAL_HF_REJECT
#define ITSCOPE_VAL_LF_REJECT                       IVISCOPE_VAL_LF_REJECT
#define ITSCOPE_VAL_NOISE_REJECT                    IVISCOPE_VAL_NOISE_REJECT

    /*- Defined values for attribute ITSCOPE_ATTR_TRIGGER_SLOPE -*/
#define ITSCOPE_VAL_POSITIVE                        IVISCOPE_VAL_POSITIVE
#define ITSCOPE_VAL_NEGATIVE                        IVISCOPE_VAL_NEGATIVE
#define ITSCOPE_VAL_EITHER                          (2L)  

    /*- Defined values for attribute ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT -*/
#define ITSCOPE_VAL_NTSC                            IVISCOPE_VAL_NTSC
#define ITSCOPE_VAL_PAL                             IVISCOPE_VAL_PAL
#define ITSCOPE_VAL_SECAM                           IVISCOPE_VAL_SECAM
#define ITSCOPE_VAL_CUSTOM                          (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +1L)
#define ITSCOPE_VAL_HDTV                            (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +2L)
#define ITSCOPE_VAL_BILEVELCUSTOM                   (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +3L)
#define ITSCOPE_VAL_TRILEVELCUSTOM                  (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +4L)
#define ITSCOPE_VAL_HD480P60                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +5L)
#define ITSCOPE_VAL_HD576P50                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +6L)
#define ITSCOPE_VAL_HD720P30                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +7L)
#define ITSCOPE_VAL_HD720P50                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +8L)
#define ITSCOPE_VAL_HD720P60                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +9L)
#define ITSCOPE_VAL_HD875I60                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +10L)
#define ITSCOPE_VAL_HD1080P24                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +11L)
#define ITSCOPE_VAL_HD1080SF24                      (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +12L)
#define ITSCOPE_VAL_HD1080I50                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +13L)
#define ITSCOPE_VAL_HD1080I60                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +14L)
#define ITSCOPE_VAL_HD1080P25                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +15L)
#define ITSCOPE_VAL_HD1080P30                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +16L)
#define ITSCOPE_VAL_HD1080P50                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +17L)
#define ITSCOPE_VAL_HD1080P60                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +18L)

    /*- Defined values for attribute ITSCOPE_ATTR_TV_TRIGGER_EVENT -*/
#define ITSCOPE_VAL_TV_EVENT_FIELD1                 IVISCOPE_VAL_TV_EVENT_FIELD1
#define ITSCOPE_VAL_TV_EVENT_FIELD2                 IVISCOPE_VAL_TV_EVENT_FIELD2
#define ITSCOPE_VAL_TV_EVENT_ANY_FIELD              IVISCOPE_VAL_TV_EVENT_ANY_FIELD
#define ITSCOPE_VAL_TV_EVENT_ANY_LINE               IVISCOPE_VAL_TV_EVENT_ANY_LINE
#define ITSCOPE_VAL_TV_EVENT_LINE_NUMBER            IVISCOPE_VAL_TV_EVENT_LINE_NUMBER

    /*- Defined values for attribute ITSCOPE_ATTR_TV_TRIGGER_POLARITY -*/
#define ITSCOPE_VAL_TV_POSITIVE                     IVISCOPE_VAL_TV_POSITIVE
#define ITSCOPE_VAL_TV_NEGATIVE                     IVISCOPE_VAL_TV_NEGATIVE

    /*- Defined values for attribute ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT -*/
#define ITSCOPE_VAL_HDTV_1080P24                1
#define ITSCOPE_VAL_HDTV_720P60                 2
#define ITSCOPE_VAL_HDTV_480P60                 3
#define ITSCOPE_VAL_HDTV_1080I50                4
#define ITSCOPE_VAL_HDTV_1080P25                5
#define ITSCOPE_VAL_HDTV_1080I60                6
#define ITSCOPE_VAL_HDTV_1080PSF24              7
#define ITSCOPE_VAL_HDTV_576P50                 8
#define ITSCOPE_VAL_HDTV_720P30                 9
#define ITSCOPE_VAL_HDTV_720P50                 10
#define ITSCOPE_VAL_HDTV_875I60                 11
#define ITSCOPE_VAL_HDTV_1080P30                12
#define ITSCOPE_VAL_HDTV_1080P50                13
#define ITSCOPE_VAL_HDTV_1080P60                14
    
    /*- Defined values for attribute ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_FORMAT -*/
#define ITSCOPE_VAL_TV_CUSTOM_INTERLACED        1
#define ITSCOPE_VAL_TV_CUSTOM_PROGRESSIVE       2
    
    /*- Defined values for attribute ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE -*/
#define ITSCOPE_VAL_TV_RATE15K                  1
#define ITSCOPE_VAL_TV_RATE20K                  2
#define ITSCOPE_VAL_TV_RATE25K                  3
#define ITSCOPE_VAL_TV_RATE35K                  4
#define ITSCOPE_VAL_TV_RATE50K                  5
    
    /*- Defined values for attribute ITSCOPE_ATTR_AC_LINE_SLOPE -*/
#define ITSCOPE_VAL_AC_LINE_POSITIVE                IVISCOPE_VAL_AC_LINE_POSITIVE
#define ITSCOPE_VAL_AC_LINE_NEGATIVE                IVISCOPE_VAL_AC_LINE_NEGATIVE
#define ITSCOPE_VAL_AC_LINE_EITHER                  IVISCOPE_VAL_AC_LINE_EITHER  

    /*- Defined values for attribute ITSCOPE_ATTR_ACQUISITION_TYPE -*/
#define ITSCOPE_VAL_NORMAL                          IVISCOPE_VAL_NORMAL 
#define ITSCOPE_VAL_HI_RES                          IVISCOPE_VAL_HI_RES
#define ITSCOPE_VAL_PEAK_DETECT                     IVISCOPE_VAL_PEAK_DETECT
#define ITSCOPE_VAL_ENVELOPE                        IVISCOPE_VAL_ENVELOPE
#define ITSCOPE_VAL_AVERAGE                         IVISCOPE_VAL_AVERAGE

	/*- Defined values for ACQUISITION STATE -*/
#define ITSCOPE_VAL_ACQUISITION_RUN                 1
#define ITSCOPE_VAL_ACQUISITION_STOP                0

    /*- ITSCOPE_ATTR_SAMPLE_MODE   -*/
#define ITSCOPE_VAL_REAL_TIME                       IVISCOPE_VAL_REAL_TIME

    /*- ITSCOPE_NUM_ENVELOPES -*/
#define ITSCOPE_VAL_INFINITE                        0
    
    /*- Defined values for attribute ITSCOPE_ATTR_RUNT_WHEN -*/
#define ITSCOPE_VAL_RUNT_WHEN_OCCURS                1
#define ITSCOPE_VAL_RUNT_WHEN_WIDER_THAN            2
#define ITSCOPE_VAL_RUNT_WHEN_LESS_THAN             3
#define ITSCOPE_VAL_RUNT_WHEN_EQUAL                 4
#define ITSCOPE_VAL_RUNT_WHEN_NOTEQUAL              5

    /*- Defined values for attribute ITSCOPE_ATTR_GLITCH_POLARITY -*/
#define ITSCOPE_VAL_GLITCH_POSITIVE                 IVISCOPE_VAL_GLITCH_POSITIVE
#define ITSCOPE_VAL_GLITCH_NEGATIVE                 IVISCOPE_VAL_GLITCH_NEGATIVE

    /*- Defined values for attribute ITSCOPE_ATTR_GLITCH_CONDITION -*/
#define ITSCOPE_VAL_GLITCH_GREATER_THAN             IVISCOPE_VAL_GLITCH_GREATER_THAN
#define ITSCOPE_VAL_GLITCH_LESS_THAN                IVISCOPE_VAL_GLITCH_LESS_THAN
#define ITSCOPE_VAL_GLITCH_WITHIN_5_PERCENT         101
#define ITSCOPE_VAL_GLITCH_OUTSIDE_5_PERCENT        102
    
    /*- Defined values for attribute ITSCOPE_ATTR_MODEL -*/
#define ITSCOPE_VAL_PE510K1	                        1
#define ITSCOPE_VAL_PE520K1	                        2


     /*- Defined values for attribute ITSCOPE_ATTR_RUNT_POLARITY -*/
#define ITSCOPE_VAL_RUNT_POSITIVE                   IVISCOPE_VAL_RUNT_POSITIVE
#define ITSCOPE_VAL_RUNT_NEGATIVE                   IVISCOPE_VAL_RUNT_NEGATIVE
#define ITSCOPE_VAL_RUNT_EITHER                     IVISCOPE_VAL_RUNT_EITHER  

    /*- Defined values for attribute ITSCOPE_ATTR_TRANSITION_POLARITY -*/
#define ITSCOPE_VAL_TRANSITION_POSITIVE             1
#define ITSCOPE_VAL_TRANSITION_NEGATIVE             2
#define ITSCOPE_VAL_TRANSITION_EITHER               3

    /*- Defined values for attribute ITSCOPE_ATTR_TRANSITION_WHEN Values-*/
#define ITSCOPE_VAL_TRANSITION_FASTER_THAN          1
#define ITSCOPE_VAL_TRANSITION_SLOWER_THAN          2
#define ITSCOPE_VAL_TRANSITION_EQUAL                3
#define ITSCOPE_VAL_TRANSITION_UNEQUAL              4 

    /*- Defined values for attribute ITSCOPE_ATTR_DELAY_TRIGGER_MODE -*/
#define ITSCOPE_VAL_TRIG_AFTER_TIME                 1
#define ITSCOPE_VAL_TRIG_AFTER_EVENTS               2

    /*- Defined values for attribute ITSCOPE_ATTR_PATTERN_FUNCTION   -*/
    /*-                         &&   ITSCOPE_ATTR_STATE_FUNCTION     -*/
#define ITSCOPE_VAL_LOGIC_AND                       1
#define ITSCOPE_VAL_LOGIC_OR                        2
#define ITSCOPE_VAL_LOGIC_NAND                      3
#define ITSCOPE_VAL_LOGIC_NOR                       4

    /*- Defined values for attribute ITSCOPE_ATTR_PATTERN_WHEN   -*/
    /*-                          &&  ITSCOPE_ATTR_STATE_WHEN     -*/
#define ITSCOPE_VAL_LOGIC_TRUE                      1
#define ITSCOPE_VAL_LOGIC_FALSE                     2
#define ITSCOPE_VAL_LOGIC_LESSTHAN                  3
#define ITSCOPE_VAL_LOGIC_MORETHAN                  4
#define ITSCOPE_VAL_LOGIC_EQUAL                     5
#define ITSCOPE_VAL_LOGIC_UNEQUAL                   6

    /*- Defined values for attribute ITSCOPE_ATTR_PATTERN_INPUT  -*/
    /*-                         &&   ITSCOPE_ATTR_STATE_INPUT    -*/
#define ITSCOPE_VAL_LOGIC_HIGH                      1
#define ITSCOPE_VAL_LOGIC_LOW                       2
#define ITSCOPE_VAL_LOGIC_X                         3

    /*- Defined values for attribute ITSCOPE_ATTR_STATE_CLOCK_EDGE   -*/
    /*-                         &&   ITSCOPE_ATTR_SETHOLD_CLOCK_EDGE -*/
#define ITSCOPE_VAL_CLOCK_RISE                      1
#define ITSCOPE_VAL_CLOCK_FALL                      2

    /*- Defined values for attribute ITSCOPE_ATTR_MEAS_FUNCTION -*/
#define ITSCOPE_VAL_RISE_TIME                      IVISCOPE_VAL_RISE_TIME
#define ITSCOPE_VAL_FALL_TIME                      IVISCOPE_VAL_FALL_TIME
#define ITSCOPE_VAL_FREQUENCY                      IVISCOPE_VAL_FREQUENCY
#define ITSCOPE_VAL_PERIOD                         IVISCOPE_VAL_PERIOD
#define ITSCOPE_VAL_VOLTAGE_RMS                    IVISCOPE_VAL_VOLTAGE_RMS
#define ITSCOPE_VAL_VOLTAGE_PEAK_TO_PEAK           IVISCOPE_VAL_VOLTAGE_PEAK_TO_PEAK
#define ITSCOPE_VAL_VOLTAGE_MAX                    IVISCOPE_VAL_VOLTAGE_MAX
#define ITSCOPE_VAL_VOLTAGE_MIN                    IVISCOPE_VAL_VOLTAGE_MIN 
#define ITSCOPE_VAL_VOLTAGE_HIGH                   IVISCOPE_VAL_VOLTAGE_HIGH
#define ITSCOPE_VAL_VOLTAGE_LOW                    IVISCOPE_VAL_VOLTAGE_LOW
#define ITSCOPE_VAL_VOLTAGE_AVERAGE                IVISCOPE_VAL_VOLTAGE_AVERAGE 
#define ITSCOPE_VAL_WIDTH_NEG                      IVISCOPE_VAL_WIDTH_NEG
#define ITSCOPE_VAL_WIDTH_POS                      IVISCOPE_VAL_WIDTH_POS
#define ITSCOPE_VAL_DUTY_CYCLE_NEG                 IVISCOPE_VAL_DUTY_CYCLE_NEG
#define ITSCOPE_VAL_DUTY_CYCLE_POS                 IVISCOPE_VAL_DUTY_CYCLE_POS
#define ITSCOPE_VAL_AMPLITUDE                      IVISCOPE_VAL_AMPLITUDE 
#define ITSCOPE_VAL_VOLTAGE_CYCLE_RMS              IVISCOPE_VAL_VOLTAGE_CYCLE_RMS
#define ITSCOPE_VAL_VOLTAGE_CYCLE_AVERAGE          IVISCOPE_VAL_VOLTAGE_CYCLE_AVERAGE
#define ITSCOPE_VAL_POSITIVE_OVERSHOOT             IVISCOPE_VAL_OVERSHOOT
#define ITSCOPE_VAL_NEGATIVE_OVERSHOOT             (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  1)
#define ITSCOPE_VAL_TIMING_BURST_WIDTH             (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  2)
#define ITSCOPE_VAL_PHASE                          (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  3)    
#define ITSCOPE_VAL_DELAY                          (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  4)   
#define ITSCOPE_VAL_AREA                           (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  5) 
#define ITSCOPE_VAL_CYCLE_AREA                     (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  6)
#define ITSCOPE_VAL_CROSS_NEG                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  7)
#define ITSCOPE_VAL_CROSS_POS                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  8)
#define ITSCOPE_VAL_EDGE_POS                       (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  9)
#define ITSCOPE_VAL_EDGE_NEG                       (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  10)
#define ITSCOPE_VAL_PULSE_POS                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  11)
#define ITSCOPE_VAL_PULSE_NEG                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  12)
#define ITSCOPE_VAL_HISTOGRAM_HITS                 (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  13)
#define ITSCOPE_VAL_MEDIAN                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  14)
#define ITSCOPE_VAL_SIGMA1                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  15)
#define ITSCOPE_VAL_SIGMA2                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  16)
#define ITSCOPE_VAL_SIGMA3                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  17)
#define ITSCOPE_VAL_STDDEV                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  18)
#define ITSCOPE_VAL_WAVEFORMS                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  19)
#define ITSCOPE_VAL_TOVERSHOOT                     (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  20)


/*- Defined values for attribute ITSCOPE_ATTR_MEAS_DEL_DIRECTION -*/
#define ITSCOPE_VAL_DEL_DIR_BACKWARD                1
#define ITSCOPE_VAL_DEL_DIR_FORWARD                 2

/*- Defined values for attribute ITSCOPE_ATTR_MEAS_DEL_EDGE and ITSCOPE_ATTR_MEAS_DEL_EDGE2 -*/
#define ITSCOPE_VAL_DEL_EDGE_RISE                   1
#define ITSCOPE_VAL_DEL_EDGE_FALL                   2

/*- Defined values for attribute ITSCOPE_ATTR_MEAS_GATING -*/
#define ITSCOPE_VAL_MEAS_GATING_OFF                 1
#define ITSCOPE_VAL_MEAS_GATING_SCREEN              2
#define ITSCOPE_VAL_MEAS_GATING_CURSOR              3
                            
    /*- Defined values for attribute ITSCOPE_ATTR_MEAS_REF_CALC_METH -*/
#define ITSCOPE_VAL_MEAS_HISTOGRAM                  1
#define ITSCOPE_VAL_MEAS_MIN_MAX                    2
#define ITSCOPE_VAL_MEAS_AUTO                       3

    /*- Defined values for attribute ITSCOPE_ATTR_MATH_TYPE -*/
#define ITSCOPE_VAL_MATH_TYPE_DUAL                  1
#define ITSCOPE_VAL_MATH_TYPE_FFT                   2
#define ITSCOPE_VAL_MATH_TYPE_ADVANCED              3
#define ITSCOPE_VAL_MATH_TYPE_SPECTRUM              4
    
    /*- Defined values for attribute ITSCOPE_ATTR_MATH_FUNCTION -*/
#define ITSCOPE_VAL_MATH_ADD                        1
#define ITSCOPE_VAL_MATH_SUBTRACT                   2
#define ITSCOPE_VAL_MATH_MULTIPLY                   3
#define ITSCOPE_VAL_MATH_DIVIDE                     4
#define ITSCOPE_VAL_MATH_FFT                        5

    /*- Defined values for attribute ITSCOPE_ATTR_FFT_VERT_SCALE -*/
#define ITSCOPE_VAL_MATH_DBV_RMS                    1
#define ITSCOPE_VAL_MATH_LINEAR_RMS                 2

    /*- Defined values for attribute ITSCOPE_ATTR_FFT_WINDOW -*/
#define ITSCOPE_VAL_MATH_HANNING                    1 
#define ITSCOPE_VAL_MATH_RECTANGULAR                2
#define ITSCOPE_VAL_MATH_HAMMING                    3
#define ITSCOPE_VAL_MATH_BLACK_HARRIS               4

    /*- Default value for attribute ITSCOPE_ATTR_ADV_DEFINE -*/
#define ITSCOPE_VAL_ADV_DEFINE                      "CH1+CH2"   


#define ITSCOPE_VAL_FFT_WINDOW_RECTANGULAR                          0
#define ITSCOPE_VAL_FFT_WINDOW_HAMMING                              1
#define ITSCOPE_VAL_FFT_WINDOW_HANNING                              2
#define ITSCOPE_VAL_FFT_WINDOW_BLACKMANHARRIS                       3
#define ITSCOPE_VAL_FFT_WINDOW_KAISER                               4

#define ITSCOPE_VAL_FFT_WINDOW_FLATTOP                              5



/*- Defined values for attribute ITSCOPE_ATTR_FASTACQ_WAVEFORM_GRADING -*/
#define ITSCOPE_VAL_WAVEFORM_GRADING_NORMAL                         1
#define ITSCOPE_VAL_WAVEFORM_GRADING_TEMPERATURE                    2
#define ITSCOPE_VAL_WAVEFORM_GRADING_SPECTRAL                       3
#define ITSCOPE_VAL_WAVEFORM_GRADING_INVERTED                       4

/*- Defined values for attribute ITSCOPE_ATTR_RF_IQ_DATA_FILE_FORMAT -*/
#define ITSCOPE_VAL_FILE_FORMAT_TIQ                                 1
#define ITSCOPE_VAL_FILE_FORMAT_MATLAB                              2

/*- Defined values for attribute ITSCOPE_ATTR_FUNC_WAVEFORM -*/
#define ITSCOPE_VAL_WFM_SINE                                (1)
#define ITSCOPE_VAL_WFM_SQUARE                              (2)
#define ITSCOPE_VAL_WFM_PULSE                               (3)
#define ITSCOPE_VAL_WFM_RAMP                                (4)
#define ITSCOPE_VAL_WFM_NOISE                               (5)
#define ITSCOPE_VAL_WFM_DC                                  (6)
#define ITSCOPE_VAL_WFM_SINC                                (7)
#define ITSCOPE_VAL_WFM_GAUSSIAN                            (8)
#define ITSCOPE_VAL_WFM_LORENTZ                             (9)
#define ITSCOPE_VAL_WFM_ERISE                               (10)
#define ITSCOPE_VAL_WFM_EDECAY                              (11)
#define ITSCOPE_VAL_WFM_HAVERSINE                           (12)
#define ITSCOPE_VAL_WFM_CARDIAC                             (13)
#define ITSCOPE_VAL_WFM_ARBITRARY                           (14)

/*- Extended arbitrary waveform -*/ 
#define ITSCOPE_VAL_FIRST_WAVEFORM_HANDLE                   (10000L)

#define ITSCOPE_VAL_WFM_USER                                (ITSCOPE_VAL_FIRST_WAVEFORM_HANDLE)

/*- Defined values for Arbitrary Waveform Handles ---------------*/
#define ITSCOPE_VAL_ALL_WAVEFORMS                           (-1L)    

/*- Defined values for attribute ITSCOPE_ATTR_FUNC_PRESET_LEVEL -*/
#define ITSCOPE_VAL_FUNC_LEVEL_CMOS_5_0V                    (1)
#define ITSCOPE_VAL_FUNC_LEVEL_CMOS_3_3V                    (2)
#define ITSCOPE_VAL_FUNC_LEVEL_CMOS_2_5V                    (3)
#define ITSCOPE_VAL_FUNC_LEVEL_USER                         (4)
#define ITSCOPE_VAL_FUNC_LEVEL_ECL                          (5)
#define ITSCOPE_VAL_FUNC_LEVEL_TTL                          (6)

/*- Defined values for attribute ITSCOPE_ATTR_OUTPUT_IMPEDANCE -*/
#define ITSCOPE_VAL_FUNC_OUTPUT_IMPEDANCE_FIFTY             (1)
#define ITSCOPE_VAL_FUNC_OUTPUT_IMPEDANCE_HIGHZ             (2)

/*- Defined values for attribute ITSCOPE_ATTR_DVM_TYPE -*/
#define ITSCOPE_VAL_DVM_TYPE_ACRMS                          (1)
#define ITSCOPE_VAL_DVM_TYPE_ACDCRMS                        (2)
#define ITSCOPE_VAL_DVM_TYPE_DC                             (3)
#define ITSCOPE_VAL_DVM_TYPE_FREQUENCY                      (4)
#define ITSCOPE_VAL_DVM_TYPE_OFF                            (5)

/*- Defined values for attribute ITSCOPE_ATTR_DVM_SOURCE -*/
#define ITSCOPE_VAL_DVM_SOURCE_CH1                          (1)
#define ITSCOPE_VAL_DVM_SOURCE_CH2                          (2)
#define ITSCOPE_VAL_DVM_SOURCE_CH3                          (3)
#define ITSCOPE_VAL_DVM_SOURCE_CH4                          (4)

/*- Defined values for attribute ITSCOPE_ATTR_EVENT_TRIGGER_TYPE -*/
#define ITSCOPE_VAL_EVENT_TRIGGER_TYPE_NONE                 (1)
#define ITSCOPE_VAL_EVENT_TRIGGER_TYPE_TRIGGER               (2)
#define ITSCOPE_VAL_EVENT_TRIGGER_TYPE_ACQCOMPLETE          (3)

/*- Defined values for Event Action -*/ 
#define ITSCOPE_VAL_EVENT_ACTION_AUXOUT                     (1)
#define ITSCOPE_VAL_EVENT_ACTION_PRINT                      (2)
#define ITSCOPE_VAL_EVENT_ACTION_SAVE_IMAGE                 (3)      
#define ITSCOPE_VAL_EVENT_ACTION_SAVE_WAVEFORM              (4)
#define ITSCOPE_VAL_EVENT_ACTION_SRQ                        (5)
#define ITSCOPE_VAL_EVENT_ACTION_STOP_ACQ                   (6)




/**************************************************************************** 
 *---------------- Instrument Driver Function Declarations -----------------* 
 ****************************************************************************/

    /*- Init and Close Functions -------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_init (ViRsrc resourceName, ViBoolean IDQuery,
                                  ViBoolean resetDevice, ViSession *vi);
DllExport ViStatus _VI_FUNC  itscope_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                             ViBoolean resetDevice, ViConstString optionString, 
                                             ViSession *newVi);
DllExport ViStatus _VI_FUNC  itscope_close (ViSession vi);   

    /*- Coercion Functions --------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_GetNextCoercionRecord (ViSession vi, ViInt32 bufferSize, ViChar record[]);

    /*- Locking Functions --------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_LockSession (ViSession vi, ViBoolean *callerHasLock);   
DllExport ViStatus _VI_FUNC  itscope_UnlockSession (ViSession vi, ViBoolean *callerHasLock);

    /*- Channel Subsystem Configuration -----------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_ConfigureChannel (ViSession vi,
                                               ViConstString channel,
                                               ViReal64 range,
                                               ViReal64 offset, 
                                               ViInt32 coupling,
                                               ViReal64 probeAttenuation,
                                               ViBoolean enabled);


DllExport ViStatus _VI_FUNC itscope_ConfigureChanCharacteristics (ViSession vi,
                                                         ViConstString channel,
                                                         ViReal64 inputImpedance,
                                                         ViReal64 maxInputFrequency);
DllExport ViStatus _VI_FUNC itscope_AutoProbeSenseValue (ViSession vi,
                                                ViConstString channel,
                                                ViReal64 *autoProbeSenseValue);

    /*- Triggering Subsystem Configuration ---------------------------------*/
    /*- IviScopeBase Functions -*/    
DllExport ViStatus _VI_FUNC itscope_ConfigureTrigger (ViSession vi, ViInt32 triggerType, 
                                                        ViReal64 holdoff);
DllExport ViStatus _VI_FUNC itscope_ConfigureTriggerCoupling (ViSession vi,
                                                        ViInt32 coupling);
DllExport ViStatus _VI_FUNC itscope_ConfigureTriggerLevel (ViSession vi,
                                                    ViConstString channel,
                                                    ViReal64 level);
DllExport ViStatus _VI_FUNC itscope_ConfigureTriggerThreshold(ViSession vi,
                                                        ViConstString channel,
                                                        ViReal64 upperThreshold,
                                                        ViReal64 lowerThreshold);
DllExport ViStatus _VI_FUNC itscope_ConfigureEdgeTriggerSource (ViSession vi, 
                                                        ViConstString source,
                                                        ViReal64 level,
                                                        ViInt32 slope);
    /*- IviScopeACLineTrigger Function -*/                                                        
DllExport ViStatus _VI_FUNC itscope_ConfigureAcLineTriggerSlope (ViSession vi,
                                                        ViInt32 acLineSlope);
                                                        
    /*- Delay Trigger Functions -*/
DllExport ViStatus _VI_FUNC itscope_ConfigureDelayTriggerSource ( ViSession vi,
                                                        ViConstString source,
                                                        ViInt32 coupling,
                                                        ViReal64 level,
                                                        ViInt32 slope);
DllExport ViStatus _VI_FUNC itscope_ConfigureDelayTriggerMode (   ViSession vi,
                                                        ViInt32 mode);
DllExport ViStatus _VI_FUNC itscope_ConfigureDelayTriggerEvents ( ViSession vi,
                                                        ViInt32 events);
DllExport ViStatus _VI_FUNC itscope_ConfigureDelayTriggerTime (   ViSession vi,
                                                        ViReal64 time);
                                                        
    /*- IviScopeTVTrigger Functions -*/
DllExport ViStatus _VI_FUNC itscope_ConfigureTVTriggerSource (ViSession vi, 
                                                        ViConstString source,
                                                        ViInt32 TVSignalFormat, 
                                                        ViInt32 TVEvent,
                                                        ViInt32 TVPolarity);
DllExport ViStatus _VI_FUNC itscope_ConfigureTVTriggerLineNumber (ViSession vi,
                                                        ViInt32 tvLineNumber);
DllExport ViStatus _VI_FUNC itscope_ConfigureTVTriggerFieldHoldoff (ViSession vi,
                                                          ViReal64 holdoff);
DllExport ViStatus _VI_FUNC itscope_ConfigureTVTriggerHDTV (ViSession vi,
                                                  ViInt32 format);
DllExport ViStatus _VI_FUNC itscope_ConfigureTVTriggerCustom (ViSession vi,
                                                    ViInt32 format,
                                                    ViInt32 scanRate,
                                                    ViReal64 linePeriod,
                                                    ViReal64 syncInterval);
    /*- IviScopeRuntTrigger Function -*/                                                        
DllExport ViStatus _VI_FUNC itscope_ConfigureRuntTriggerSource (ViSession vi, 
                                                        ViConstString source,
                                                        ViReal64 runtLowThreshold, 
                                                        ViReal64 runtHighThreshold, 
                                                        ViInt32 runtPolarity);
                                                        
DllExport ViStatus _VI_FUNC itscope_ConfigureRuntTriggerCondition (ViSession vi,
                                                            ViReal64 runtWidth,
                                                            ViInt32 runtCondition);
    /*- IviScopeGlitchTrigger Function -*/                                                        
DllExport ViStatus _VI_FUNC itscope_ConfigureGlitchTriggerSource (ViSession vi, 
                                                        ViConstString source, 
                                                        ViReal64 level, 
                                                        ViReal64 glitchWidth, 
                                                        ViInt32 glitchPolarity, 
                                                        ViInt32 glitchCondition);

    /*- Transition Trigger Function -*/                                                        
DllExport ViStatus _VI_FUNC itscope_ConfigureTransitTriggerSource (ViSession vi,
                                                     ViConstString source,
                                                     ViInt32 polarity,
                                                     ViInt32 when,
                                                     ViReal64 deltaTime,
                                                     ViReal64 lowThreshold,
                                                     ViReal64 highThreshold);
    /*- Logic Basic -*/                                                     
DllExport ViStatus _VI_FUNC itscope_ConfigureLogicTriggerThreshold (ViSession vi,
                                                            ViConstString channel,
                                                            ViReal64 threshold);

    /*- Logic Pattern Trigger Function -*/
DllExport ViStatus _VI_FUNC itscope_ConfigurePatternTrigger (ViSession vi,
                                                            ViInt32 function,
                                                            ViReal64 deltaTime,
                                                            ViInt32 condition);
DllExport ViStatus _VI_FUNC itscope_ConfigurePatternTriggerInput (ViSession vi,
                                                                ViConstString channel,
                                                                ViInt32 logicInput);
                                                                
    /*- Logic State Trigger Function -*/
DllExport ViStatus _VI_FUNC itscope_ConfigureStateTrigger (ViSession vi,
                                                        ViInt32 function);
DllExport ViStatus _VI_FUNC itscope_ConfigureStateTriggerDataInput (ViSession vi,
                                                                ViConstString channel,
                                                                ViInt32 logicInput);                                                        
DllExport ViStatus _VI_FUNC itscope_ConfigureStateTriggerClockInput (ViSession vi,
                                                                    ViInt32 clockSource,
                                                                    ViInt32 edge);
                                                                    
    /*- Logic Sethold Trigger Function -*/
DllExport ViStatus _VI_FUNC itscope_ConfigureSetholdTriggerSource( ViSession vi,
                                                            ViInt32 dataSource,
                                                            ViInt32 clockSource,
                                                            ViInt32 clockEdge,
                                                            ViReal64 setTime,
                                                            ViReal64 holdTime);


    /*- Waveform Acquisition Subsystem -------------------------------------*/
DllExport ViStatus _VI_FUNC  itscope_ConfigureAcquisitionRecord (ViSession vi, 
                                                 ViReal64 timePerRecord, 
                                                 ViInt32 minimumRecordLength,
                                                 ViReal64 acquisitionStartTime);
DllExport ViStatus _VI_FUNC  itscope_ActualRecordLength (ViSession vi, 
                                               ViInt32 *actualRecordLength);
DllExport ViStatus _VI_FUNC  itscope_ConfigureAcquisitionType (ViSession vi,
                                                  ViInt32 acquisitionType);
DllExport ViStatus _VI_FUNC  itscope_ConfigureNumAverages (ViSession vi,
                                                  ViInt32 numberOfAverages);
DllExport ViStatus _VI_FUNC  itscope_ConfigureNumEnvelopes (ViSession vi,
                                                   ViInt32 numberOfEnvelopes);
DllExport ViStatus _VI_FUNC  itscope_ForceTrigger (ViSession vi);

DllExport ViStatus _VI_FUNC  itscope_ConfigureWfmBufferSize(ViSession vi, 
                                                ViInt32 bufferSize);
                                                
DllExport ViStatus _VI_FUNC  itscope_ReadWaveform (ViSession vi, 
                                          ViConstString channel,
                                          ViInt32 waveformSize,
                                          ViInt32 maxTime,
                                          ViReal64 waveform[],
                                          ViInt32 *actualPoints, 
                                          ViReal64 *initialX,
                                          ViReal64 *xIncrement);

DllExport ViStatus _VI_FUNC  itscope_InitiateAcquisition (ViSession vi);

DllExport ViStatus _VI_FUNC  itscope_AcquisitionStatus (ViSession vi, ViInt32 *status);

DllExport ViStatus _VI_FUNC  itscope_Abort (ViSession vi);

DllExport ViStatus _VI_FUNC  itscope_FetchWaveform (ViSession vi,
                                           ViConstString channel,
                                           ViInt32 waveformSize,
                                           ViReal64 waveform[],
                                           ViInt32 *actualPoints,
                                           ViReal64 *initialX,
                                           ViReal64 *xIncrement);   

DllExport ViStatus _VI_FUNC itscope_ConfigureFastAcquisition(ViSession vi, 
                                                   ViInt32 WaveformGrading, 
                                                   ViBoolean FastAcqusitionEnable);

    /*additional functions needed to be spec compliant*/
DllExport ViStatus _VI_FUNC itscope_SampleRate (ViSession vi, ViReal64 *rate);
DllExport ViStatus _VI_FUNC itscope_SampleMode (ViSession vi, ViInt32 *mode);
DllExport ViStatus _VI_FUNC itscope_ConfigureTriggerModifier (ViSession vi,
                                                        ViInt32 triggerModifier);
DllExport ViStatus _VI_FUNC itscope_ConfigureInitiateContinuous (ViSession vi,
                                                        ViBoolean continuousAcquisition);

    /* 
        Used when ITSCOPE_ATTR_ACQUISTION_TYPE is either 
        ITSCOPE_VAL_PEAK_DETECT or ITSCOPE_VAL_ENVELOPE 
    */
DllExport ViStatus _VI_FUNC  itscope_ReadMinMaxWaveform (ViSession vi,
                                                ViConstString channel, 
                                                ViInt32 waveformSize,
                                                ViInt32 maxTime,
                                                ViReal64 minWaveform[],
                                                ViReal64 maxWaveform[],
                                                ViInt32 *actualPoints,
                                                ViReal64 *initialX, 
                                                ViReal64 *xIncrement);

    /* 
        Used when ITSCOPE_ATTR_ACQUISTION_TYPE is either
        ITSCOPE_VAL_PEAK_DETECT or ITSCOPE_VAL_ENVELOPE 
    */
DllExport ViStatus _VI_FUNC  itscope_FetchMinMaxWaveform (ViSession vi,
                                                 ViConstString channel, 
                                                 ViInt32 waveformSize,
                                                 ViReal64 minWaveform[], 
                                                 ViReal64 maxWaveform[],
                                                 ViInt32 *actualPoints, 
                                                 ViReal64 *initialX,
                                                 ViReal64 *xIncrement);
                                                 
DllExport ViStatus _VI_FUNC  itscope_InitiateContinuousAcquisition (ViSession vi, 
                                            ViInt32 waveformSize);
                                            
DllExport ViStatus _VI_FUNC itscope_FetchContinuousWaveform (ViSession vi, 
                                                    ViConstString channel,
                                                    ViReal64 waveform[],
                                                    ViInt32 *actualPoints,
                                                    ViReal64 *initialX,
                                                    ViReal64 *xIncrement);
                                                    
DllExport ViStatus _VI_FUNC itscope_FetchContinuousMinMaxWaveform (ViSession vi, 
                                                    ViConstString channel,
                                                    ViReal64 minWaveform[], 
                                                    ViReal64 maxWaveform[], 
                                                    ViInt32 *actualPoints, 
                                                    ViReal64 *initialX, 
                                                    ViReal64 *xIncrement);

DllExport ViStatus _VI_FUNC  itscope_StopContinuousAcquisition (ViSession vi);   

    /*- Waveform Measurements ----------------------------------------------*/
DllExport ViStatus _VI_FUNC  itscope_ConfigureMidRef (ViSession vi, ViReal64 mid); 


DllExport ViStatus _VI_FUNC  itscope_ConfigureRefLevels (ViSession vi, 
                                                ViReal64 low, 
                                                ViReal64 mid, 
                                                ViReal64 high);
DllExport ViStatus _VI_FUNC  itscope_ConfigureMeasurement (ViSession vi,
                                                  ViInt32 referenceLevelCalcMethod,
                                                  ViInt32 gating);
DllExport ViStatus _VI_FUNC  itscope_ReadWaveformMeasurement (ViSession vi,
                                                     ViConstString channel, 
                                                     ViInt32 measurementFunction,
                                                     ViInt32 maxTime, 
                                                     ViReal64 *measurement);
DllExport ViStatus _VI_FUNC  itscope_ReadPhaseMeasurement (ViSession vi,
                                                     ViConstString measureSource1,
                                                     ViConstString measureSource2,
                                                     ViInt32 maxTime,
                                                     ViReal64 *measurement);


DllExport ViStatus _VI_FUNC  itscope_ReadDelayMeasurement (ViSession vi,
                                                     ViConstString measureSource1,
                                                     ViConstString measureSource2,
                                                     ViInt32 delayEdge1,
                                                     ViInt32 delayEdge2,
                                                     ViInt32 delayDirection,
                                                     ViInt32 maxTime,
                                                     ViReal64 *measurement);

DllExport ViStatus _VI_FUNC  itscope_FetchWaveformMeasurement (ViSession vi,
                                                      ViConstString channel, 
                                                      ViInt32 measurementFunction, 
                                                      ViReal64 *measurement);

DllExport ViStatus _VI_FUNC  itscope_FetchPhaseMeasurement (ViSession vi,
                                                      ViConstString measureSource1,
                                                      ViConstString measureSource2,
                                                      ViReal64 *measurement);

DllExport ViStatus _VI_FUNC  itscope_FetchDelayMeasurement (ViSession vi,
                                                      ViConstString measureSource1,
                                                      ViConstString measureSource2,
                                                      ViInt32 delayEdge1,
                                                      ViInt32 delayEdge2,
                                                      ViInt32 delayDirection,
                                                      ViReal64 *measurement);


/*- Auto Setup ---------------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_AutoSetup (ViSession vi);

    /*- Math Channels ------------------------------------------------------*/
DllExport ViStatus _VI_FUNC itscope_ConfigureMathChannel (ViSession vi,
                                                ViInt32 mathSource1,
                                                ViInt32 mathFunction,
                                                ViInt32 mathSource2);
DllExport ViStatus _VI_FUNC  itscope_ConfigureMathChannelFFT (ViSession vi, 
                                                    ViInt32 mathSource1,
                                                    ViInt32 fftWindow, 
                                                    ViInt32 fftScale);
DllExport ViStatus _VI_FUNC itscope_ConfigureMathChannelAdvanced (ViSession vi,
                                                        ViReal64 var1,
                                                        ViReal64 var2,
                                                        ViConstString funcDefinition);                                                    

DllExport ViStatus _VI_FUNC  itscope_EnableMathChannel (ViSession vi, 
                                              ViBoolean enableMathChannel);

                                                   
    /*- Reference Channels -------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_StoreWfmToRefChannel (ViSession vi, 
                                                 ViConstString sourceChannel,
                                                 ViConstString refChannel);
                                                 
DllExport ViStatus _VI_FUNC  itscope_EnableReferenceChannel (ViSession vi, 
                                                   ViConstString refChannel,
                                                   ViBoolean enableRefChannel);                                                 
 


/*- Arbitrary Function Generator Group Functions ----------------------------------------------------*/
/*- Arbitrary Waveform Generator Group Functions ----------------------------------------------------*/

/*- General Group Functions ----------------------------------------------------*/

/*- Action Event Group Functions ----------------------------------------------------*/

    /*- Error Functions ----------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_error_query (ViSession vi, ViInt32 *errorCode,
                                         ViChar errorMessage[]);
DllExport ViStatus _VI_FUNC  itscope_error_message (ViSession vi, ViStatus errorCode,
                                           ViChar errorMessage[256]);
DllExport ViStatus _VI_FUNC  itscope_GetError (ViSession vi, 
                                     ViStatus *errorCode, 
                                     ViInt32 bufferSize,
                                     ViChar description[]);
DllExport ViStatus _VI_FUNC  itscope_ClearError (ViSession vi);

        /*- Channel Info Functions ---------------------------------------------*/
        
DllExport ViStatus _VI_FUNC  itscope_GetChannelName (ViSession vi, ViInt32 index,
                                           ViInt32 bufferSize,
                                           ViChar name[]);

    /*- Interchangeability Checking Functions ------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[]);
DllExport ViStatus _VI_FUNC  itscope_ResetInterchangeCheck (ViSession vi);
DllExport ViStatus _VI_FUNC  itscope_ClearInterchangeWarnings (ViSession vi);

    /*- Utility Functions --------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_reset (ViSession vi);
DllExport ViStatus _VI_FUNC  itscope_ResetWithDefaults (ViSession vi);
DllExport ViStatus _VI_FUNC  itscope_Disable (ViSession vi);
DllExport ViStatus _VI_FUNC  itscope_InvalidateAllAttributes (ViSession vi);

DllExport ViStatus _VI_FUNC  itscope_self_test (ViSession vi, ViInt16 *selfTestResult,
                                       ViChar selfTestMessage[]);
DllExport ViStatus _VI_FUNC  itscope_revision_query (ViSession vi, 
                                            ViChar instrumentDriverRevision[],
                                            ViChar firmwareRevision[]);

DllExport ViStatus _VI_FUNC  itscope_viWrite (ViSession vi, ViByte buffer[], ViInt64 count, ViInt64 *returnCount);
DllExport ViStatus _VI_FUNC  itscope_viRead (ViSession vi, ViInt64 bufferSize, ViByte buffer[], ViInt64 *returnCount);
/* Obsolete Direct IO Functions. */
DllExport ViStatus _VI_FUNC  itscope_WriteInstrData (ViSession vi, ViConstString writeBuffer); 
DllExport ViStatus _VI_FUNC  itscope_ReadInstrData  (ViSession vi, ViInt32 numBytes, 
                                            ViChar rdBuf[], ViInt32 *bytesRead);
    
    /*- Utility -------------------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_IsInvalidWfmElement (ViSession vi,
                                                 ViReal64 elementValue,
                                                 ViBoolean *isInvalid);

    /*- Set, Get, and Check Attribute Functions ----------------------------*/
    
DllExport ViStatus _VI_FUNC  itscope_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
DllExport ViStatus _VI_FUNC  itscope_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
DllExport ViStatus _VI_FUNC  itscope_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]); 
DllExport ViStatus _VI_FUNC  itscope_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
DllExport ViStatus _VI_FUNC  itscope_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

DllExport ViStatus _VI_FUNC  itscope_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
DllExport ViStatus _VI_FUNC  itscope_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
DllExport ViStatus _VI_FUNC  itscope_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
DllExport ViStatus _VI_FUNC  itscope_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
DllExport ViStatus _VI_FUNC  itscope_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

DllExport ViStatus _VI_FUNC  itscope_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
DllExport ViStatus _VI_FUNC  itscope_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
DllExport ViStatus _VI_FUNC  itscope_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
DllExport ViStatus _VI_FUNC  itscope_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
DllExport ViStatus _VI_FUNC  itscope_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

    /*********************************************************
        Functions reserved for class driver use only.
        End-users should not call these functions.  
     *********************************************************/

DllExport ViStatus _VI_FUNC  itscope_IviInit (ViRsrc resourceName, ViBoolean IDQuery, 
                                     ViBoolean reset, ViSession vi);
DllExport ViStatus _VI_FUNC  itscope_IviClose (ViSession vi);   
                                         
/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/
#define ITSCOPE_ERROR_MAX_TIME_EXCEEDED               IVISCOPE_ERROR_MAX_TIME_EXCEEDED
#define ITSCOPE_ERROR_NOT_VALID_FOR_MODEL             (IVI_SPECIFIC_ERROR_BASE + 1)
#define ITSCOPE_ERROR_OPTION_NOT_INSTALLED            (IVI_SPECIFIC_ERROR_BASE + 2)
#define ITSCOPE_ERROR_NO_WFMS_AVAILABLE               (IVI_SPECIFIC_ERROR_BASE + 3)  

/****************************************************************************
 *--------- Obsolete Inherent Instrument Attributes and functions ----------*
 ****************************************************************************/
    /*- These attributes and functions have been deprecated and may not
        be supported in future versions of this driver. -*/
 
         /* Driver Information  */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  /* ViInt32,  read-only  */
#define ITSCOPE_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  /* ViInt32,  read-only  */

        /* Error Info */
#define ITSCOPE_ATTR_PRIMARY_ERROR                  IVI_ATTR_PRIMARY_ERROR                  /* ViInt32   */
#define ITSCOPE_ATTR_SECONDARY_ERROR                IVI_ATTR_SECONDARY_ERROR                /* ViInt32   */
#define ITSCOPE_ATTR_ERROR_ELABORATION              IVI_ATTR_ERROR_ELABORATION              /* ViString  */

        /* Advanced Session Information */
#define ITSCOPE_ATTR_IO_SESSION_TYPE                IVI_ATTR_IO_SESSION_TYPE                /* ViString, read-only  */
#define ITSCOPE_ATTR_IO_SESSION                     IVI_ATTR_IO_SESSION                     /* ViSession, read-only */

        /*- Deprecated Attribute Identifiers for Renamed Attributes -*/
#define ITSCOPE_ATTR_NUM_CHANNELS                   ITSCOPE_ATTR_CHANNEL_COUNT             /* ViInt32,  read-only  */
#define ITSCOPE_ATTR_QUERY_INSTR_STATUS             ITSCOPE_ATTR_QUERY_INSTRUMENT_STATUS   /* ViBoolean */
#define ITSCOPE_ATTR_RESOURCE_DESCRIPTOR            ITSCOPE_ATTR_IO_RESOURCE_DESCRIPTOR    /* ViString, read-only  */


        /*- Deprecated Error Information functions -*/
DllExport ViStatus _VI_FUNC  itscope_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                          ViStatus *secondaryError, 
                                          ViChar errorElaboration[256]);
DllExport ViStatus _VI_FUNC  itscope_ClearErrorInfo (ViSession vi);

/**************************************************************************** 
 *---------------------------- End Include File ----------------------------* 
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __ITSCOPE_HEADER */
