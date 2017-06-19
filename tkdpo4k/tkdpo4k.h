/****************************************************************************
 *            TKDPO4K 2(4)-CH Digital Phosphor Oscilloscope
 *---------------------------------------------------------------------------
 * (c) 2005 - 2016, National Instruments, Corporation.  All Rights Reserved.         
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:    tkdpo4k.h                                        
 * Purpose:  TKDPO4K 2(4)-CH Digital Phosphor Oscilloscope
 *           instrument driver declarations.
 *                                                                          
 ****************************************************************************/

#ifndef __TKDPO4K_HEADER
#define __TKDPO4K_HEADER

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

/****************************************************************************
 *--------------------- Instrument Driver Information ----------------------*
 ****************************************************************************/
#define TKDPO4K_MAJOR_VERSION                4     /* Instrument driver major version   */
#define TKDPO4K_MINOR_VERSION                4     /* Instrument driver minor version   */
                                                                
#define TKDPO4K_CLASS_SPEC_MAJOR_VERSION     4     /* Class specification major version */
#define TKDPO4K_CLASS_SPEC_MINOR_VERSION     1     /* Class specification minor version */

#define TKDPO4K_SUPPORTED_INSTRUMENT_MODELS  "DPO4104, DPO4054, DPO4034, DPO4032, MSO4104, MSO4054, "\
                                             "MSO4034, MSO4032, MSO3012, MSO3014, MSO3032, MSO3034, "\
                                             "MSO3052, MSO3054 DPO3012, DPO3014, DPO3032, DPO3034, "\
                                             "DPO3052, DPO3054, DPO4104B, DPO4054B, DPO4034B, MSO4104B, "\
                                             "MSO4054B, MSO4034B, MDO4104-6, MDO4104-3, MDO4054-6, MDO4054-3, "\
                                             "DPO4102B, DPO4102B-L, DPO4104B, MSO4102B, MSO4102B-L, MSO4104B, "\
                                             "DPO4014B, MSO4014B, MDO4014-3, MDO4034-3, MDO4014B-3, MDO4034B-3, "\
                                             "MDO4054B-3, MDO4054B-6, MDO4104B-3, MDO4104B-6, MDO3012, MDO3014, "\
                                             "MDO3022, MDO3024, MDO3032, MDO3034, MDO3052, MDO3054, MDO3102, MDO3104"\
											 "MDO4024C, MDO4034C, MDO4054C, MDO4104C"
                    
#define TKDPO4K_DRIVER_VENDOR                "National Instruments"
    
#ifdef _IVI_mswin64_
#define TKDPO4K_DRIVER_DESCRIPTION           "Tektronix DPO4000 Series Oscilloscopes [Compiled for 64-bit]"
#else
#define TKDPO4K_DRIVER_DESCRIPTION           "Tektronix DPO4000 Series Oscilloscopes"
#endif

/**************************************************************************** 
 *------------------------------ Useful Macros -----------------------------* 
 ****************************************************************************/

    /* Defined values for the status parameter of the TKDPO4K_Acquisition_Status function */
#define TKDPO4K_VAL_ACQ_COMPLETE              IVISCOPE_VAL_ACQ_COMPLETE       
#define TKDPO4K_VAL_ACQ_IN_PROGRESS           IVISCOPE_VAL_ACQ_IN_PROGRESS    

    /* Define Channel Names */
#define TKDPO4K_VAL_CH_1                      "CH1"
#define TKDPO4K_VAL_CH_2                      "CH2"
#define TKDPO4K_VAL_CH_3                      "CH3"
#define TKDPO4K_VAL_CH_4                      "CH4"
#define TKDPO4K_VAL_MATH                      "MATH"
#define TKDPO4K_VAL_REF_1                     "REF1"
#define TKDPO4K_VAL_REF_2                     "REF2"
#define TKDPO4K_VAL_REF_3                     "REF3"
#define TKDPO4K_VAL_REF_4                     "REF4"
#define TKDPO4K_VAL_D_0                       "D0"
#define TKDPO4K_VAL_D_1                       "D1"
#define TKDPO4K_VAL_D_2                       "D2"
#define TKDPO4K_VAL_D_3                       "D3"
#define TKDPO4K_VAL_D_4                       "D4"
#define TKDPO4K_VAL_D_5                       "D5"
#define TKDPO4K_VAL_D_6                       "D6"
#define TKDPO4K_VAL_D_7                       "D7"
#define TKDPO4K_VAL_D_8                       "D8"
#define TKDPO4K_VAL_D_9                       "D9"
#define TKDPO4K_VAL_D_10                      "D10"
#define TKDPO4K_VAL_D_11                      "D11"
#define TKDPO4K_VAL_D_12                      "D12"
#define TKDPO4K_VAL_D_13                      "D13"
#define TKDPO4K_VAL_D_14                      "D14"
#define TKDPO4K_VAL_D_15                      "D15"  

#define TKDPO4K_VAL_RF_CHANNEL                "RF" 
#define TKDPO4K_VAL_RF_AMPLITUDE              "RF_AMP"
#define TKDPO4K_VAL_RF_FREQUENCY              "RF_FREQ"
#define TKDPO4K_VAL_RF_PHASE                  "RF_PHAS"
#define TKDPO4K_VAL_RF_NORMAL                 "RF_NORM"
#define TKDPO4K_VAL_RF_AVERAGE                "RF_AVE"
#define TKDPO4K_VAL_RF_MAXHOLD                "RF_MAXH"
#define TKDPO4K_VAL_RF_MINHOLD                "RF_MINH"
    
#define TKDPO4K_VAL_RF_BB_IQ                  "RF_BB_IQ"
#define TKDPO4K_VAL_HISTOGRAM                 "HIS"

     /*
      *The following macros are used in function 
      *tkdpo4k_ReadFreqDomainMeasurement.
      */
#define TKDPO4K_VAL_ADJ_LA1                   "LA1"
#define TKDPO4K_VAL_ADJ_LA2                   "LA2"
#define TKDPO4K_VAL_ADJ_LA3                   "LA3"
#define TKDPO4K_VAL_ADJ_UA1                   "UA1"
#define TKDPO4K_VAL_ADJ_UA2                   "UA2"
#define TKDPO4K_VAL_ADJ_UA3                   "UA3"
/**************************************************************************** 
 *---------------------------- Attribute Defines ---------------------------* 
 ****************************************************************************/

    /*- IVI Inherent Instrument Attributes ---------------------------------*/    

        /* User Options */
#define TKDPO4K_ATTR_RANGE_CHECK                   IVI_ATTR_RANGE_CHECK                    /* ViBoolean */
#define TKDPO4K_ATTR_QUERY_INSTRUMENT_STATUS       IVI_ATTR_QUERY_INSTRUMENT_STATUS        /* ViBoolean */
#define TKDPO4K_ATTR_CACHE                         IVI_ATTR_CACHE                          /* ViBoolean */
#define TKDPO4K_ATTR_SIMULATE                      IVI_ATTR_SIMULATE                       /* ViBoolean */
#define TKDPO4K_ATTR_RECORD_COERCIONS              IVI_ATTR_RECORD_COERCIONS               /* ViBoolean */

#define TKDPO4K_ATTR_DRIVER_SETUP                  IVI_ATTR_DRIVER_SETUP
#define TKDPO4K_ATTR_INTERCHANGE_CHECK             IVI_ATTR_INTERCHANGE_CHECK 

        /* Instrument Capabilities */
#define TKDPO4K_ATTR_CHANNEL_COUNT                 IVI_ATTR_CHANNEL_COUNT                  /* ViInt32,  read-only  */
#define TKDPO4K_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only */

        /* Driver Information  */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_PREFIX        IVI_ATTR_SPECIFIC_DRIVER_PREFIX         /* ViString, read-only  */
#define TKDPO4K_ATTR_SUPPORTED_INSTRUMENT_MODELS   IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS    /* ViString, read-only  */
#define TKDPO4K_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only  */
#define TKDPO4K_ATTR_INSTRUMENT_MANUFACTURER       IVI_ATTR_INSTRUMENT_MANUFACTURER        /* ViString, read-only  */
#define TKDPO4K_ATTR_INSTRUMENT_MODEL              IVI_ATTR_INSTRUMENT_MODEL               /* ViString, read-only  */
#define TKDPO4K_ATTR_INSTRUMENT_FIRMWARE_REVISION  IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION   /* ViString, read-only  */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_REVISION      IVI_ATTR_SPECIFIC_DRIVER_REVISION       /* ViString, read-only  */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_VENDOR        IVI_ATTR_SPECIFIC_DRIVER_VENDOR         /* ViString, read-only  */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_DESCRIPTION   IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION    /* ViString, read-only  */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

        /* Advanced Session Information */
#define TKDPO4K_ATTR_LOGICAL_NAME                  IVI_ATTR_LOGICAL_NAME                   /* ViString, read-only  */
#define TKDPO4K_ATTR_IO_RESOURCE_DESCRIPTOR        IVI_ATTR_IO_RESOURCE_DESCRIPTOR         /* ViString, read-only  */
    
    /*- Channel Sub-system ------------------------------------------------*/                    
#define TKDPO4K_ATTR_CHANNEL_ENABLED               IVISCOPE_ATTR_CHANNEL_ENABLED           /* ViBoolean, multi-channel         */
#define TKDPO4K_ATTR_PROBE_ATTENUATION             IVISCOPE_ATTR_PROBE_ATTENUATION         /* ViReal64,  multi-channel         */
#define TKDPO4K_ATTR_PROBE_SENSE_VALUE             IVISCOPE_ATTR_PROBE_SENSE_VALUE         /* ViReal64,  multi-channel         */
#define TKDPO4K_ATTR_VERTICAL_RANGE                IVISCOPE_ATTR_VERTICAL_RANGE            /* ViReal64,  multi-channel, volts  */
#define TKDPO4K_ATTR_VERTICAL_OFFSET               IVISCOPE_ATTR_VERTICAL_OFFSET           /* ViReal64,  multi-channel, volts  */
#define TKDPO4K_ATTR_VERTICAL_COUPLING             IVISCOPE_ATTR_VERTICAL_COUPLING         /* ViInt32,   multi-channel         */
#define TKDPO4K_ATTR_MAX_INPUT_FREQUENCY           IVISCOPE_ATTR_MAX_INPUT_FREQUENCY       /* ViReal64,  multi-Channel, hertz  */
#define TKDPO4K_ATTR_INPUT_IMPEDANCE               IVISCOPE_ATTR_INPUT_IMPEDANCE           /* ViReal64,  multi-channel, ohms   */
#define TKDPO4K_ATTR_DIGITAL_THRESHOLD             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 152L)  /* ViReal64,  multi-channel, volts  */                                  
#define TKDPO4K_ATTR_UPPER_THRESHOLD               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 198L)  /* ViReal64,  multi-channel, volts  */   
#define TKDPO4K_ATTR_LOWER_THRESHOLD               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 199L)  /* ViReal64                         */               

    /*- RF Channel Sub-system ------------------------------------------------*/
#define TKDPO4K_ATTR_RF_NUM_AVERAGES                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 239L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_AVERAGE_TRACE_COUNT          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 240L)  /* ViInt32, read-only               */
#define TKDPO4K_ATTR_RF_DETECTION_METHOD             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 241L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_DETECTION_MODE               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 242L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_SQUELCH_STATE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 243L)  /* ViBoolean                        */  
#define TKDPO4K_ATTR_RF_SQUELCH_THRESHOLD            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 244L)  /* ViReal64, volts                  */
#define TKDPO4K_ATTR_RF_SPECTRUM_MODE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 245L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_RBW                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 246L)  /* ViReal64                         */
#define TKDPO4K_ATTR_RF_RBW_MODE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 247L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_SPAN_RBW_RATIO               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 248L)  /* ViReal64                         */
#define TKDPO4K_ATTR_RF_FREQ_START                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 249L)  /* ViReal64, hertz                  */
#define TKDPO4K_ATTR_RF_FREQ_STOP                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 250L)  /* ViReal64, hertz                  */
#define TKDPO4K_ATTR_RF_FREQ_CENTER                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 263L)  /* ViReal64, hertz                  */
#define TKDPO4K_ATTR_RF_FREQ_SPAN                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 264L)  /* ViReal64, hertz                  */
#define TKDPO4K_ATTR_RF_REF_LEVEL                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 251L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_UNITS                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 252L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_FFT_WINDOW                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 253L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_MEASURE_TYPE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 254L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_MEASURE_ACPR_ADJACENTPAIRS   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 255L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RF_MEASURE_ACPR_CHANNEL_SPACING (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 256L)  /* ViReal64                         */
#define TKDPO4K_ATTR_RF_MEASURE_CHANNEL_BANDWIDTH    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 257L)  /* ViReal64                         */
#define TKDPO4K_ATTR_RF_MEASURE_CHANNEL_POWER        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 258L)  /* ViReal64                         */
#define TKDPO4K_ATTR_RF_MEASURE_ACPR_RATIO           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 259L)  /* ViReal64, read-only              */
#define TKDPO4K_ATTR_RF_MEASURE_OBW_PERCENT_DOWN     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 260L)  /* ViReal64                         */
#define TKDPO4K_ATTR_RF_MEASURE_OBW_LOW_FREQ         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 261L)  /* ViReal64, read-only, hertz       */
#define TKDPO4K_ATTR_RF_MEASURE_OBW_HIGH_FREQ        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 262L)  /* ViReal64, read-only, hertz       */
#define TKDPO4K_ATTR_RF_PHASE_IN_DEGREES             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1006L) /* ViInt32                          */
#define TKDPO4K_ATTR_RF_PHASE_WRAP_DEGREES_NUM       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1007L) /* ViInt32                          */
#define TKDPO4K_ATTR_RF_PHASE_WRAP_CONTROL_STATE     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1008L) /* ViBoolean                        */
#define TKDPO4K_ATTR_RF_VERSUS_TIME_BANDWIDTH        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1009L) /* ViReal64, read-only              */
#define TKDPO4K_ATTR_RF_SPECTROGRAM_TIME_STAMP       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1010L) /* ViInt32,  read-only              */
#define TKDPO4K_ATTR_RF_SPECTROGRAM_SECONDS_NUM      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1011L) /* ViInt32,  read-only              */
#define TKDPO4K_ATTR_RF_IQ_DATA_FILE_FORMAT          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1012L) /* ViInt32                          */
    
    /*- Trigger Sub-system -------------------------------------------------*/
#define TKDPO4K_ATTR_TRIGGER_TYPE                  IVISCOPE_ATTR_TRIGGER_TYPE              /* ViInt32                          */
#define TKDPO4K_ATTR_TRIGGER_SOURCE                IVISCOPE_ATTR_TRIGGER_SOURCE            /* ViString                         */
#define TKDPO4K_ATTR_TRIGGER_LEVEL                 IVISCOPE_ATTR_TRIGGER_LEVEL             /* ViReal64,  volts                 */
#define TKDPO4K_ATTR_TRIGGER_LEVEL_BY_CH           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 94L)   /* ViReal64,  multi-channel, volts  */  
#define TKDPO4K_ATTR_TRIGGER_UPPERTHRES_BY_CH      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 95L)   /* ViReal64,  multi-channel, volts  */
#define TKDPO4K_ATTR_TRIGGER_LOWERTHRES_BY_CH      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 96L)   /* ViReal64,  multi-channel, volts  */
#define TKDPO4K_ATTR_TRIGGER_HOLDOFF               IVISCOPE_ATTR_TRIGGER_HOLDOFF           /* ViReal64,  seconds               */
#define TKDPO4K_ATTR_TRIGGER_MODIFIER              IVISCOPE_ATTR_TRIGGER_MODIFIER          /* ViInt32                          */
#define TKDPO4K_ATTR_SERIAL_TRIGGER_SOURCE         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 151L)  /* ViInt32                          */

#define TKDPO4K_ATTR_TRIGGER_LEVEL_ON_AUXIN        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1002L) /* ViReal64                         */
    
        /* Edge Trigger */
#define TKDPO4K_ATTR_TRIGGER_COUPLING              IVISCOPE_ATTR_TRIGGER_COUPLING          /* ViInt32                          */
#define TKDPO4K_ATTR_TRIGGER_SLOPE                 IVISCOPE_ATTR_TRIGGER_SLOPE             /* ViInt32                          */
                                                                                                                                            
        /* TV Trigger */
#define TKDPO4K_ATTR_TV_TRIGGER_POLARITY           IVISCOPE_ATTR_TV_TRIGGER_POLARITY       /* ViInt32                          */
#define TKDPO4K_ATTR_TV_TRIGGER_SIGNAL_FORMAT      IVISCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT  /* ViInt32                          */
#define TKDPO4K_ATTR_TV_TRIGGER_EVENT              IVISCOPE_ATTR_TV_TRIGGER_EVENT          /* ViInt32                          */
#define TKDPO4K_ATTR_TV_TRIGGER_LINE_NUMBER        IVISCOPE_ATTR_TV_TRIGGER_LINE_NUMBER    /* ViInt32                          */
#define TKDPO4K_ATTR_TV_TRIGGER_HDTV_FORMAT        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 180L)  /* ViInt32                          */
#define TKDPO4K_ATTR_TV_TRIGGER_FIELD_HOLDOFF      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 181L)  /* ViReal64                         */
#define TKDPO4K_ATTR_TV_TRIGGER_CUSTOM_FORMAT      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 182L)  /* ViInt32                          */
#define TKDPO4K_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 183L)  /* ViInt32                          */
#define TKDPO4K_ATTR_TV_TRIGGER_CUSTOM_LINE_PERIOD (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 196L)  /* ViReal64                         */
#define TKDPO4K_ATTR_TV_TRIGGER_CUSTOM_SYNC_INTERVAL (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 197L) /* ViReal64                        */
    
    /* Runt Trigger */
#define TKDPO4K_ATTR_RUNT_HIGH_THRESHOLD           IVISCOPE_ATTR_RUNT_HIGH_THRESHOLD       /* ViReal64,  volts                 */
#define TKDPO4K_ATTR_RUNT_LOW_THRESHOLD            IVISCOPE_ATTR_RUNT_LOW_THRESHOLD        /* ViReal64,  volts                 */
#define TKDPO4K_ATTR_RUNT_POLARITY                 IVISCOPE_ATTR_RUNT_POLARITY             /* ViInt32                          */
#define TKDPO4K_ATTR_RUNT_WHEN                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8L)    /* ViInt32                          */
#define TKDPO4K_ATTR_RUNT_WIDTH                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9L)    /* ViReal64                         */

        /*- Glitch Trigger Attributes -*/
#define TKDPO4K_ATTR_GLITCH_POLARITY               IVISCOPE_ATTR_GLITCH_POLARITY           /* ViInt32                          */
#define TKDPO4K_ATTR_GLITCH_CONDITION              IVISCOPE_ATTR_GLITCH_CONDITION          /* ViInt32                          */
#define TKDPO4K_ATTR_GLITCH_WIDTH                  IVISCOPE_ATTR_GLITCH_WIDTH              /* ViReal64, seconds                */  

        /* AC Line Trigger */ 
#define TKDPO4K_ATTR_AC_LINE_TRIGGER_SLOPE         IVISCOPE_ATTR_AC_LINE_TRIGGER_SLOPE     /* ViInt32                          */

        /* Delay Trigger */
#define TKDPO4K_ATTR_DELAY_TRIGGER_MODE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 11L)   /* ViInt32                          */
#define TKDPO4K_ATTR_DELAY_TRIGGER_EVENTS          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12L)   /* ViInt32                          */
#define TKDPO4K_ATTR_DELAY_TRIGGER_SOURCE          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13L)   /* ViString                         */
#define TKDPO4K_ATTR_DELAY_TRIGGER_COUPLING        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14L)   /* ViInt32                          */
#define TKDPO4K_ATTR_DELAY_TRIGGER_SLOPE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 15L)   /* ViInt32                          */
#define TKDPO4K_ATTR_DELAY_TRIGGER_LEVEL           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16L)   /* ViReal64                         */
#define TKDPO4K_ATTR_DELAY_TRIGGER_TIME            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 148L)  /* ViReal64                         */

        /* Transition Trigger */
#define TKDPO4K_ATTR_TRANSITION_POLARITY           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17L)   /* ViInt32                          */
#define TKDPO4K_ATTR_TRANSITION_WHEN               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18L)   /* ViInt32                          */
#define TKDPO4K_ATTR_TRANSITION_DELTA_TIME         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19L)   /* ViReal64                         */
#define TKDPO4K_ATTR_TRANSITION_HIGH_THRESHOLD     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20L)   /* ViReal64                         */
#define TKDPO4K_ATTR_TRANSITION_LOW_THRESHOLD      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 21L)   /* ViReal64                         */
                                                                                
        /* Logic Trigger */
#define TKDPO4K_ATTR_LOGIC_THRES_BY_CH             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 97L)   /* ViReal64, multi-channel, volts   */       

        /* Pattern Trigger */
#define TKDPO4K_ATTR_PATTERN_FUNCTION              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 26L)   /* ViInt32                          */
#define TKDPO4K_ATTR_PATTERN_WHEN                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 27L)   /* ViInt32                          */
#define TKDPO4K_ATTR_PATTERN_INPUT                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 98L)
#define TKDPO4K_ATTR_PATTERN_DELTATIME             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 28L)   /* ViInt32                          */

        /* State Trigger */
#define TKDPO4K_ATTR_STATE_FUNCTION                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 99L)   /* ViInt32                          */
#define TKDPO4K_ATTR_STATE_WHEN                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 36L)   /* ViInt32                          */
#define TKDPO4K_ATTR_STATE_INPUT                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 100L)  /* ViInt32                          */  
#define TKDPO4K_ATTR_STATE_CLOCK_EDGE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 101L)  /* ViInt32                          */
#define TKDPO4K_ATTR_STATE_CLOCK_SOURCE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 92L)   /* ViInt32                          */

        /* SetHold Trigger */
#define TKDPO4K_ATTR_SETHOLD_CLOCK_EDGE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 102L)  /* ViInt32                          */
#define TKDPO4K_ATTR_SETHOLD_CLOCK_SOURCE          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 103L)  /* ViInt32                          */
#define TKDPO4K_ATTR_SETHOLD_DATA_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 104L)  /* ViInt32                          */   
#define TKDPO4K_ATTR_SETHOLD_HOLDTIME              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 105L)  /* ViReal64                         */
#define TKDPO4K_ATTR_SETHOLD_SETTIME               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 106L)  /* ViReal64                         */ 
    
        /* CAN Trigger */
#define TKDPO4K_ATTR_CAN_CONDITION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 107L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_CAN_DATA_VALUE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 111L)  /* ViString                         */
#define TKDPO4K_ATTR_CAN_DATA_SIZE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 110L)  /* ViInt32                          */
#define TKDPO4K_ATTR_CAN_ID_VALUE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 109L)  /* ViString                         */
#define TKDPO4K_ATTR_CAN_ID_MODE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 108L)  /* ViInt32                          */
#define TKDPO4K_ATTR_CAN_FRAMETYPE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 112L)  /* ViInt32                          */

        /* I2C Trigger */
#define TKDPO4K_ATTR_I2C_CONDITION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 113L)  /* ViInt32                          */
#define TKDPO4K_ATTR_I2C_ADDR_VALUE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 115L)  /* ViString                         */
#define TKDPO4K_ATTR_I2C_ADDR_MODE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 114L)  /* ViInt32                          */
#define TKDPO4K_ATTR_I2C_DATA_VALUE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 117L)  /* ViString                         */
#define TKDPO4K_ATTR_I2C_DATA_SIZE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 116L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_I2C_DATA_DIRECTION            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 118L)  /* ViInt32                          */

        /* SPI Trigger */
#define TKDPO4K_ATTR_SPI_CONDITION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 119L)  /* ViInt32                          */
#define TKDPO4K_ATTR_SPI_DATA_SIZE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 120L)  /* ViInt32                          */
#define TKDPO4K_ATTR_SPI_MOSI_VALUE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 121L)  /* ViString                         */
#define TKDPO4K_ATTR_SPI_MISO_VALUE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 122L)  /* ViString                         */   

        /* Parallel Trigger */
#define TKDPO4K_ATTR_PARALLEL_VALUE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 195L)
    
        /* RS232 Trigger */
#define TKDPO4K_ATTR_RS232_CONDITION               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 190L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RS232_RX_DATA_VALUE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 191L)  /* ViString                         */
#define TKDPO4K_ATTR_RS232_RX_DATA_SIZE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 192L)  /* ViInt32                          */
#define TKDPO4K_ATTR_RS232_TX_DATA_VALUE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 193L)  /* ViString                         */
#define TKDPO4K_ATTR_RS232_TX_DATA_SIZE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 194L)  /* ViInt32                          */
    
        /* USB Trigger */
#define TKDPO4K_ATTR_USB_TRAN_FIELD                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 211L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_USB_TRAN_START_END            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 212L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_USB_TRAN_END_POINT            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 213L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_USB_HUB_ADDRESS               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 214L)  /* ViString                         */     
#define TKDPO4K_ATTR_USB_PORT_ADDRESS              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 215L)  /* ViString                         */     
        
        /* Ethernet Trigger */
#define TKDPO4K_ATTR_ENET_BASIC_FRAME_TYPE         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 216L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_ENET_BASIC_CONDITION          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 217L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_ENET_BASIC_QUALIFIER          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 218L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_ENET_DATA_SIZE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 219L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_ENET_DATA_OFFSET              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 220L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_ENET_DATA_UPPER_LIMIT         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 221L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_DATA                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 222L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_IP_SRC_ADDRESS           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 223L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_IP_DST_ADDRESS           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 224L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_MAC_SRC_ADDRESS          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 225L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_MAC_DST_ADDRESS          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 226L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_MAC_UPPER_LENGTH         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 227L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_MAC_LENGTH               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 228L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_QTAG                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 229L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_TCP_SEQUENCE_NUMBER      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 230L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_TCP_ACQ_NUMBER           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 231L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_TCP_SRC_ADDRESS          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 232L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_TCP_DST_ADDRESS          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 233L)  /* ViString                         */     
#define TKDPO4K_ATTR_ENET_IP_PROTOCOL              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 234L)  /* ViString                         */     
    
    /*- BUS Sub-system -----------------------------------------------------*/
    /*- Basic -*/
#define TKDPO4K_ATTR_BUS_SELECT                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 93L)   /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_TYPE                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 132L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_STATE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 133L)  /* ViBoolean                        */

    /*- CAN -*/
#define TKDPO4K_ATTR_BUS_CAN_BITRATE               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 134L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_CAN_PROBE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 135L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_BUS_CAN_SOURCE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 136L)  /* ViInt32                          */

    /*- I2C -*/
#define TKDPO4K_ATTR_BUS_I2C_CLOCK_SOURCE          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 138L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_I2C_DATA_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 139L)  /* ViInt32                          */

    /*- SPI -*/
#define TKDPO4K_ATTR_BUS_SPI_MISO_POLARITY         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 140L)  /* ViInt32                          */   
#define TKDPO4K_ATTR_BUS_SPI_MISO_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 144L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_SPI_MOSI_POLARITY         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 141L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_SPI_MOSI_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 145L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_SPI_SCLK_POLARITY         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 142L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_BUS_SPI_SCLK_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 146L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_SPI_SS_POLARITY           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 143L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_SPI_SS_SOURCE             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 147L)  /* ViInt32                          */

    /*- Parallel -*/
#define TKDPO4K_ATTR_BUS_PARALLEL_BITS_SOURCE      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 160L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_PARALLEL_CLOCK_EDGE       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 161L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_PARALLEL_CLOCK_LOCKED     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 162L)  /* ViBoolean                        */
#define TKDPO4K_ATTR_BUS_PARALLEL_CLOCK_SOURCE     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 163L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_PARALLEL_WIDTH            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 164L)  /* ViInt32                          */
    
    /*- RS232 -*/
#define TKDPO4K_ATTR_BUS_RS232_BITRATE             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 170L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_RS232_DATABITS            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 171L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_BUS_RS232_DELIMITER           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 172L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_BUS_RS232_PARITY              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 173L)  /* ViInt32                          */          
#define TKDPO4K_ATTR_BUS_RS232_RX_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 174L)  /* ViInt32                          */
#define TKDPO4K_ATTR_BUS_RS232_TX_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 175L)  /* ViInt32                          */

    /*- Ethernet -*/
#define TKDPO4K_ATTR_BUS_ETHERNET_TYPE             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 205L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_BUS_ETHERNET_PROBE_TYPE       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 206L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_BUS_ETHERNET_PLUS_DATA_SRC    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 207L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_BUS_ETHERNET_DIFF_INPUT_DATA_SRC (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 208L)  /* ViInt32                       */     
#define TKDPO4K_ATTR_BUS_ETHERNET_MINUS_DATA_SRC   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 209L)  /* ViInt32                          */     
#define TKDPO4K_ATTR_BUS_ETHERNET_PROTOCOL         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 210L)  /* ViInt32                          */     
    
    /*- Acquisition Sub-system ---------------------------------------------*/
#define TKDPO4K_ATTR_ACQUISITION_TYPE              IVISCOPE_ATTR_ACQUISITION_TYPE          /* ViInt32                          */
#define TKDPO4K_ATTR_INITIATE_CONTINUOUS           IVISCOPE_ATTR_INITIATE_CONTINUOUS       /* ViBoolean                        */
#define TKDPO4K_ATTR_NUM_AVERAGES                  IVISCOPE_ATTR_NUM_AVERAGES              /* ViInt32                          */
#define TKDPO4K_ATTR_NUM_ENVELOPES                 IVISCOPE_ATTR_NUM_ENVELOPES             /* ViInt32                          */
#define TKDPO4K_ATTR_HORZ_TIME_PER_RECORD          IVISCOPE_ATTR_HORZ_TIME_PER_RECORD      /* ViReal64                         */
#define TKDPO4K_ATTR_HORZ_MIN_NUM_PTS              IVISCOPE_ATTR_HORZ_MIN_NUM_PTS          /* ViInt32                          */
#define TKDPO4K_ATTR_HORZ_RECORD_LENGTH            IVISCOPE_ATTR_HORZ_RECORD_LENGTH        /* ViInt32,   read-only             */
#define TKDPO4K_ATTR_ACQUISITION_START_TIME        IVISCOPE_ATTR_ACQUISITION_START_TIME    /* ViReal64,  percentage of record length  */
#define TKDPO4K_ATTR_HORZ_SAMPLE_RATE              IVISCOPE_ATTR_HORZ_SAMPLE_RATE          /* ViReal64,  read-only             */
#define TKDPO4K_ATTR_SAMPLE_MODE                   IVISCOPE_ATTR_SAMPLE_MODE               /* ViReal64,  read-only             */
#define TKDPO4K_ATTR_MAGNIVU_ENABLED               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 153L)  /* ViBoolean                        */ 
#define TKDPO4K_ATTR_FASTACQ_WAVEFORM_GRADING      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1003L) /* ViInt32                          */
#define TKDPO4K_ATTR_FASTACQ_STATE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1004L) /* ViBoolean                        */
    
    
    /*- Waveform Measurement -----------------------------------------------*/
#define TKDPO4K_ATTR_MEAS_HIGH_REF                 IVISCOPE_ATTR_MEAS_HIGH_REF             /* ViReal64,  percentage            */
#define TKDPO4K_ATTR_MEAS_LOW_REF                  IVISCOPE_ATTR_MEAS_LOW_REF              /* ViReal64,  percentage            */
#define TKDPO4K_ATTR_MEAS_MID_REF                  IVISCOPE_ATTR_MEAS_MID_REF              /* ViReal64,  percentage            */
#define TKDPO4K_ATTR_MEAS_REF_CALC_METH            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 52L)   /* ViInt32                          */
#define TKDPO4K_ATTR_MEAS_SOURCE                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 53L)   /* ViString                         */ 
#define TKDPO4K_ATTR_MEAS_FUNCTION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 54L)   /* ViInt32                          */ 
#define TKDPO4K_ATTR_MEAS_GATING                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 55L)   /* ViBoolean                        */ 
#define TKDPO4K_ATTR_MEAS_MID2_REF                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 56L)   /* ViReal64,  percentage            */
#define TKDPO4K_ATTR_MEAS_SOURCE2                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 57L)   /* ViString                         */
#define TKDPO4K_ATTR_MEAS_DEL_DIRECTION            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 58L)   /* ViInt32                          */
#define TKDPO4K_ATTR_MEAS_DEL_EDGE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 59L)   /* ViInt32                          */
#define TKDPO4K_ATTR_MEAS_DEL_EDGE2                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 60L)   /* ViInt32                          */
#define TKDPO4K_ATTR_SNAPSHOT_MEAS_VALUE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1001L) /* ViReal64   read-only             */

    /*- Math ---------------------------------------------------------------*/
#define TKDPO4K_ATTR_MATH_TYPE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 123L)  /* ViInt32                          */   
#define TKDPO4K_ATTR_MATH_FUNCTION                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 124L)  /* ViInt32                          */
#define TKDPO4K_ATTR_MATH_SOURCE_1ST               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 125L)  /* ViInt32                          */
#define TKDPO4K_ATTR_MATH_SOURCE_2ND               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 126L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_FFT_VERT_SCALE                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 128L)  /* ViInt32                          */
#define TKDPO4K_ATTR_FFT_WINDOW                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 127L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_ADV_VAR1                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 129L)  /* ViReal64                         */
#define TKDPO4K_ATTR_ADV_VAR2                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 130L)  /* ViReal64                         */
#define TKDPO4K_ATTR_ADV_DEFINE                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 131L)  /* ViString                         */
#define TKDPO4K_ATTR_MATH_EXPRESSION_DISPLAY       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1005L) /* ViBoolean                        */
    
    /*- Models and Options -------------------------------------------------*/
#define TKDPO4K_ATTR_MODEL                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 61L)   /* ViInt32                          */ 

    /*- Miscellaneous ------------------------------------------------------*/
#define TKDPO4K_ATTR_ID_QUERY_RESPONSE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1L)    /* ViString,  read-only             */
#define TKDPO4K_ATTR_FETCH_BUFFER_SIZE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 149L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_WAVEFORM_SIZE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 150L)  /* ViInt32                          */ 
#define TKDPO4K_ATTR_MATH_RF_SOURCE_1ST             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 235L)
#define TKDPO4K_ATTR_MATH_RF_SOURCE_2ND             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 236L)

    /*- Arbitrary Function Generator Group Attributes ---------------*/
#define TKDPO4K_ATTR_FUNC_WAVEFORM                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6001L) /* ViInt32      */
#define TKDPO4K_ATTR_FUNC_AMPLITUDE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6002L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_FREQUENCY                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6003L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_START_PHASE               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6004L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_HIGH_LEVEL                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6005L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_LOW_LEVEL                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6006L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_DC_OFFSET                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6007L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_DUTY_CYCLE_HIGH           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6008L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_PERIOD                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6009L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_PRESET_LEVEL              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6010L) /* ViInt32      */
#define TKDPO4K_ATTR_FUNC_NOISE_LEVEL               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6011L) /* ViInt32      */
#define TKDPO4K_ATTR_FUNC_NOISE_ENABLED             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6012L) /* ViBoolean    */
#define TKDPO4K_ATTR_FUNC_PULSE_WIDTH               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6013L) /* ViReal64     */
#define TKDPO4K_ATTR_FUNC_RAMP_SYMMETRY             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6014L) /* ViReal64     */
#define TKDPO4K_ATTR_OUTPUT_ENABLED                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6015L) /* ViBoolean    */
#define TKDPO4K_ATTR_OUTPUT_IMPEDANCE               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6016L) /* ViInt32      */

/*- Arbitrary Waveform Generator Group Attributes ---------------*/
#define TKDPO4K_ATTR_ARB_WAVEFORM_DATE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2001L)     /* ViString         */
#define TKDPO4K_ATTR_ARB_WAVEFORM_TIME              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2002L)     /* ViString         */
#define TKDPO4K_ATTR_ARB_WAVEFORM_LABEL             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2003L)     /* ViString         */
#define TKDPO4K_ATTR_ARB_WAVEFORM_PREDEFINED_FUNCTION  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2004L)  /* ViString         */
#define TKDPO4K_ATTR_ARB_WAVEFORM_POINT_NUMBER      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2005L)     /* ViInt32          */
#define TKDPO4K_ATTR_ARB_WAVEFORM_POINTS            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2006L)     /* ViString         */
#define TKDPO4K_ATTR_WAVEFORM_QUANTUM               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2009L)     /* ViInt32          */
#define TKDPO4K_ATTR_MIN_WAVEFORM_SIZE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2010L)     /* ViInt32          */
#define TKDPO4K_ATTR_MAX_WAVEFORM_SIZE              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2011L)     /* ViInt32          */
#define TKDPO4K_ATTR_MAX_NUM_WAVEFORMS              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2012L)     /* ViInt32          */

/*- Digital Voltmeter Group Attributes ---------------*/
#define TKDPO4K_ATTR_DVM_RANGE_AUTO_ENABLED         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3001L)     /* ViBoolean        */
#define TKDPO4K_ATTR_DVM_TYPE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3002L)     /* ViInt32          */
#define TKDPO4K_ATTR_DVM_SOURCE                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3003L)     /* ViInt32          */
#define TKDPO4K_ATTR_DVM_FREQUENCY                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3004L)     /* ViReal64         */
#define TKDPO4K_ATTR_DVM_HISTORY_AVERAGE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3005L)     /* ViReal64         */
#define TKDPO4K_ATTR_DVM_HISTORY_MAXIMUM            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3006L)     /* ViReal64         */
#define TKDPO4K_ATTR_DVM_HISTORY_MINIMUM            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3007L)     /* ViReal64         */
#define TKDPO4K_ATTR_DVM_MAX_RESULE                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3008L)     /* ViReal64         */
#define TKDPO4K_ATTR_DVM_MIN_RESULT                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3009L)     /* ViReal64         */
#define TKDPO4K_ATTR_DVM_RESULT                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3010L)     /* ViReal64         */

/*- Action Event Group Attributes ---------------*/
#define TKDPO4K_ATTR_AUX_OUT_EVENT_ENABLED          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4001L)     /* ViBoolean        */
#define TKDPO4K_ATTR_PRINT_EVENT_ENABLED            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4002L)     /* ViBoolean        */
#define TKDPO4K_ATTR_SAVE_IMAGE_EVENT_ENABLED       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4003L)     /* ViBoolean        */
#define TKDPO4K_ATTR_SAVE_WAVEFORM_EVENT_ENABLED    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4004L)     /* ViBoolean        */
#define TKDPO4K_ATTR_SRQ_EVENT_ENABLED              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4005L)     /* ViBoolean        */
#define TKDPO4K_ATTR_STOP_ACQUISITION_EVENT_ENABLED (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4006L)     /* ViBoolean        */
#define TKDPO4K_ATTR_EVENT_REPEAT_COUNT             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4007L)     /* ViInt32          */
#define TKDPO4K_ATTR_EVENT_TRIGGER_TYPE             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4008L)     /* ViInt32          */
#define TKDPO4K_ATTR_ACQUISITION_NUMBER             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4009L)     /* ViInt32          */
                                                                                                
/*- Configuration Group Attributes ---------------*/
#define TKDPO4K_ATTR_AFG_ENABLED                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5001L)     /* ViBoolean        */
#define TKDPO4K_ATTR_ARB_ENABLED                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5002L)     /* ViBoolean        */
#define TKDPO4K_ATTR_DVM_ENABLED                    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5003L)     /* ViBoolean        */  
#define TKDPO4K_ATTR_CUSTOM_MASK_ENABLED            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5004L)     /* ViBoolean        */
#define TKDPO4K_ATTR_STANDARD_MASK_ENABLED          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5005L)     /* ViBoolean        */
#define TKDPO4K_ATTR_ARB_WAVEFORM_HANDLE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6017L)
     
                                                                                          
/**************************************************************************** 
 *------------------------ Attribute Value Defines -------------------------* 
 ****************************************************************************/
    /*- Trigger Source or Input Source -*/
#define TKDPO4K_VAL_SOUR_CH1                        1
#define TKDPO4K_VAL_SOUR_CH2                        2
#define TKDPO4K_VAL_SOUR_CH3                        3
#define TKDPO4K_VAL_SOUR_CH4                        4
#define TKDPO4K_VAL_SOUR_REF1                       6
#define TKDPO4K_VAL_SOUR_REF2                       7
#define TKDPO4K_VAL_SOUR_REF3                       8
#define TKDPO4K_VAL_SOUR_REF4                       9
#define TKDPO4K_VAL_SOUR_EXT                        21
#define TKDPO4K_VAL_SOUR_NONE                       30
#define TKDPO4K_VAL_SOUR_D0                         50
#define TKDPO4K_VAL_SOUR_D1                         51
#define TKDPO4K_VAL_SOUR_D2                         52
#define TKDPO4K_VAL_SOUR_D3                         53
#define TKDPO4K_VAL_SOUR_D4                         54
#define TKDPO4K_VAL_SOUR_D5                         55
#define TKDPO4K_VAL_SOUR_D6                         56
#define TKDPO4K_VAL_SOUR_D7                         57
#define TKDPO4K_VAL_SOUR_D8                         58
#define TKDPO4K_VAL_SOUR_D9                         59
#define TKDPO4K_VAL_SOUR_D10                        60
#define TKDPO4K_VAL_SOUR_D11                        61
#define TKDPO4K_VAL_SOUR_D12                        62
#define TKDPO4K_VAL_SOUR_D13                        63  
#define TKDPO4K_VAL_SOUR_D14                        64
#define TKDPO4K_VAL_SOUR_D15                        65 
    
    /*- Ref Source-*/
#define TKDPO4K_VAL_RFSOUR_NORMAL                   1
#define TKDPO4K_VAL_RFSOUR_AVERAGE                  2
#define TKDPO4K_VAL_RFSOUR_MAXHOLD                  3
#define TKDPO4K_VAL_RFSOUR_MINHOLD                  4

    /*- Defined values for attribute TKDPO4K_ATTR_RF_DETECTION_METHOD -*/
#define TKDPO4K_VAL_RF_DETECTION_POS_PEAK                           0
#define TKDPO4K_VAL_RF_DETECTION_NEG_PEAK                           1
#define TKDPO4K_VAL_RF_DETECTION_SAMPLE                             2
#define TKDPO4K_VAL_RF_DETECTION_AVERAGE                            3

    /*- Defined values for attribute TKDPO4K_ATTR_RF_DETECTION_MODE -*/     
#define TKDPO4K_VAL_RF_DETECT_MODE_AUTO                             0
#define TKDPO4K_VAL_RF_DETECT_MODE_MANUAL                           1
    /*- Defined values for attribute TKDPO4K_ATTR_RF_REF_LEVEL -*/
#define TKDPO4K_VAL_RF_REF_LEVEL_AUTO                               1000
    /*- Defined values for attribute TKDPO4K_ATTR_BUS_SELECT -*/
#define TKDPO4K_VAL_BUS1                            1
#define TKDPO4K_VAL_BUS2                            2
#define TKDPO4K_VAL_BUS3                            3
#define TKDPO4K_VAL_BUS4                            4

    /*- Minimum and Maximum fetch waveform buffer size -*/
#define MIN_FETCH_BUFFER_SIZE                       1000L
#define MAX_FETCH_BUFFER_SIZE                       1000000L    
    
    /*- Defined values for attribute TKDPO4K_ATTR_PROBE_ATTENUATION -*/
#define TKDPO4K_VAL_PROBE_SENSE_ON                  IVISCOPE_VAL_PROBE_SENSE_ON

    /*- Defined values for attribute TKDPO4K_ATTR_VERTICAL_COUPLING -*/
#define TKDPO4K_VAL_AC                              IVISCOPE_VAL_AC
#define TKDPO4K_VAL_DC                              IVISCOPE_VAL_DC
#define TKDPO4K_VAL_GND                             IVISCOPE_VAL_GND
#define TKDPO4K_VAL_DCREJ							(IVISCOPE_VAL_VERTICAL_COUPLING_SPECIFIC_EXT_BASE+1L)
    /*- Defined values for attribute TKDPO4K_ATTR_TRIGGER_TYPE -*/
    /*- Edge Trigger -*/    
#define TKDPO4K_VAL_EDGE_TRIGGER                    IVISCOPE_VAL_EDGE_TRIGGER   
#define TKDPO4K_VAL_AC_LINE_TRIGGER                 IVISCOPE_VAL_AC_LINE_TRIGGER 
    /*- Video Trigger -*/
#define TKDPO4K_VAL_TV_TRIGGER                      IVISCOPE_VAL_TV_TRIGGER 
    /*- Pulse Trigger -*/
#define TKDPO4K_VAL_RUNT_TRIGGER                    IVISCOPE_VAL_RUNT_TRIGGER
#define TKDPO4K_VAL_GLITCH_TRIGGER                  IVISCOPE_VAL_GLITCH_TRIGGER
#define TKDPO4K_VAL_TRANSITION_TRIGGER              (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 1L)
    /*- Logic Trigger -*/
#define TKDPO4K_VAL_PATTERN_TRIGGER                 (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 2L)      
#define TKDPO4K_VAL_STATE_TRIGGER                   (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 3L)
#define TKDPO4K_VAL_SETHOLD_TRIGGER                 (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 4L)
    /*- Serial Trigger -*/
#define TKDPO4K_VAL_CAN_TRIGGER                     (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 5L)
#define TKDPO4K_VAL_I2C_TRIGGER                     (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 6L)
#define TKDPO4K_VAL_SPI_TRIGGER                     (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 7L)
#define TKDPO4K_VAL_RS232_TRIGGER                   (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 11L)
#define TKDPO4K_VAL_PARALLEL_TRIGGER                (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 12L)    
    /*- Main Trigger Type -*/
#define TKDPO4K_VAL_PULSE_TRIGGER                   (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 8L)
#define TKDPO4K_VAL_LOGIC_TRIGGER                   (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 9L)
#define TKDPO4K_VAL_SERIAL_TRIGGER                  (IVISCOPE_VAL_TRIGGER_TYPE_SPECIFIC_EXT_BASE + 10L)

    /*- Defined values for attribute TKDPO4K_ATTR_TRIGGER_SOURCE -*/
#define TKDPO4K_VAL_EXTERNAL                        IVISCOPE_VAL_EXTERNAL
#define TKDPO4K_VAL_AC_LINE                         "VAL_AC_LINE"
    
    /*- Defined values for attribute TKDPO4K_ATTR_TRIGGER_MODIFIER -*/
#define TKDPO4K_VAL_NO_TRIGGER_MOD                  IVISCOPE_VAL_NO_TRIGGER_MOD
#define TKDPO4K_VAL_AUTO                            IVISCOPE_VAL_AUTO

    /*- Defined values for attribute TKDPO4K_ATTR_TRIGGER_COUPLING -*/          
#define TKDPO4K_VAL_HF_REJECT                       IVISCOPE_VAL_HF_REJECT
#define TKDPO4K_VAL_LF_REJECT                       IVISCOPE_VAL_LF_REJECT
#define TKDPO4K_VAL_NOISE_REJECT                    IVISCOPE_VAL_NOISE_REJECT

    /*- Defined values for attribute TKDPO4K_ATTR_TRIGGER_SLOPE -*/
#define TKDPO4K_VAL_POSITIVE                        IVISCOPE_VAL_POSITIVE
#define TKDPO4K_VAL_NEGATIVE                        IVISCOPE_VAL_NEGATIVE
#define TKDPO4K_VAL_EITHER                          (2L)  

    /*- Defined values for attribute TKDPO4K_ATTR_TV_TRIGGER_SIGNAL_FORMAT -*/
#define TKDPO4K_VAL_NTSC                            IVISCOPE_VAL_NTSC
#define TKDPO4K_VAL_PAL                             IVISCOPE_VAL_PAL
#define TKDPO4K_VAL_SECAM                           IVISCOPE_VAL_SECAM
#define TKDPO4K_VAL_CUSTOM                          (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +1L)
#define TKDPO4K_VAL_HDTV                            (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +2L)
#define TKDPO4K_VAL_BILEVELCUSTOM                   (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +3L)
#define TKDPO4K_VAL_TRILEVELCUSTOM                  (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +4L)
#define TKDPO4K_VAL_HD480P60                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +5L)
#define TKDPO4K_VAL_HD576P50                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +6L)
#define TKDPO4K_VAL_HD720P30                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +7L)
#define TKDPO4K_VAL_HD720P50                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +8L)
#define TKDPO4K_VAL_HD720P60                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +9L)
#define TKDPO4K_VAL_HD875I60                        (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +10L)
#define TKDPO4K_VAL_HD1080P24                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +11L)
#define TKDPO4K_VAL_HD1080SF24                      (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +12L)
#define TKDPO4K_VAL_HD1080I50                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +13L)
#define TKDPO4K_VAL_HD1080I60                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +14L)
#define TKDPO4K_VAL_HD1080P25                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +15L)
#define TKDPO4K_VAL_HD1080P30                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +16L)
#define TKDPO4K_VAL_HD1080P50                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +17L)
#define TKDPO4K_VAL_HD1080P60                       (IVISCOPE_VAL_TV_SIGNAL_FORMAT_SPECIFIC_EXT_BASE +18L)

    /*- Defined values for attribute TKDPO4K_ATTR_TV_TRIGGER_EVENT -*/
#define TKDPO4K_VAL_TV_EVENT_FIELD1                 IVISCOPE_VAL_TV_EVENT_FIELD1
#define TKDPO4K_VAL_TV_EVENT_FIELD2                 IVISCOPE_VAL_TV_EVENT_FIELD2
#define TKDPO4K_VAL_TV_EVENT_ANY_FIELD              IVISCOPE_VAL_TV_EVENT_ANY_FIELD
#define TKDPO4K_VAL_TV_EVENT_ANY_LINE               IVISCOPE_VAL_TV_EVENT_ANY_LINE
#define TKDPO4K_VAL_TV_EVENT_LINE_NUMBER            IVISCOPE_VAL_TV_EVENT_LINE_NUMBER

    /*- Defined values for attribute TKDPO4K_ATTR_TV_TRIGGER_POLARITY -*/
#define TKDPO4K_VAL_TV_POSITIVE                     IVISCOPE_VAL_TV_POSITIVE
#define TKDPO4K_VAL_TV_NEGATIVE                     IVISCOPE_VAL_TV_NEGATIVE

    /*- Defined values for attribute TKDPO4K_ATTR_TV_TRIGGER_HDTV_FORMAT -*/
#define TKDPO4K_VAL_HDTV_1080P24                1
#define TKDPO4K_VAL_HDTV_720P60                 2
#define TKDPO4K_VAL_HDTV_480P60                 3
#define TKDPO4K_VAL_HDTV_1080I50                4
#define TKDPO4K_VAL_HDTV_1080P25                5
#define TKDPO4K_VAL_HDTV_1080I60                6
#define TKDPO4K_VAL_HDTV_1080PSF24              7
#define TKDPO4K_VAL_HDTV_576P50                 8
#define TKDPO4K_VAL_HDTV_720P30                 9
#define TKDPO4K_VAL_HDTV_720P50                 10
#define TKDPO4K_VAL_HDTV_875I60                 11
#define TKDPO4K_VAL_HDTV_1080P30                12
#define TKDPO4K_VAL_HDTV_1080P50                13
#define TKDPO4K_VAL_HDTV_1080P60                14
    
    /*- Defined values for attribute TKDPO4K_ATTR_TV_TRIGGER_CUSTOM_FORMAT -*/
#define TKDPO4K_VAL_TV_CUSTOM_INTERLACED        1
#define TKDPO4K_VAL_TV_CUSTOM_PROGRESSIVE       2
    
    /*- Defined values for attribute TKDPO4K_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE -*/
#define TKDPO4K_VAL_TV_RATE15K                  1
#define TKDPO4K_VAL_TV_RATE20K                  2
#define TKDPO4K_VAL_TV_RATE25K                  3
#define TKDPO4K_VAL_TV_RATE35K                  4
#define TKDPO4K_VAL_TV_RATE50K                  5
    
    /*- Defined values for attribute TKDPO4K_ATTR_AC_LINE_SLOPE -*/
#define TKDPO4K_VAL_AC_LINE_POSITIVE                IVISCOPE_VAL_AC_LINE_POSITIVE
#define TKDPO4K_VAL_AC_LINE_NEGATIVE                IVISCOPE_VAL_AC_LINE_NEGATIVE
#define TKDPO4K_VAL_AC_LINE_EITHER                  IVISCOPE_VAL_AC_LINE_EITHER  

    /*- Defined values for attribute TKDPO4K_ATTR_ACQUISITION_TYPE -*/
#define TKDPO4K_VAL_NORMAL                          IVISCOPE_VAL_NORMAL 
#define TKDPO4K_VAL_HI_RES                          IVISCOPE_VAL_HI_RES
#define TKDPO4K_VAL_PEAK_DETECT                     IVISCOPE_VAL_PEAK_DETECT
#define TKDPO4K_VAL_ENVELOPE                        IVISCOPE_VAL_ENVELOPE
#define TKDPO4K_VAL_AVERAGE                         IVISCOPE_VAL_AVERAGE

    /*- TKDPO4K_ATTR_SAMPLE_MODE   -*/
#define TKDPO4K_VAL_REAL_TIME                       IVISCOPE_VAL_REAL_TIME

    /*- TKDPO4K_NUM_ENVELOPES -*/
#define TKDPO4K_VAL_INFINITE                        0
    
    /*- Defined values for attribute TKDPO4K_ATTR_RUNT_WHEN -*/
#define TKDPO4K_VAL_RUNT_WHEN_OCCURS                1
#define TKDPO4K_VAL_RUNT_WHEN_WIDER_THAN            2
#define TKDPO4K_VAL_RUNT_WHEN_LESS_THAN             3
#define TKDPO4K_VAL_RUNT_WHEN_EQUAL                 4
#define TKDPO4K_VAL_RUNT_WHEN_NOTEQUAL              5

    /*- Defined values for attribute TKDPO4K_ATTR_GLITCH_POLARITY -*/
#define TKDPO4K_VAL_GLITCH_POSITIVE                 IVISCOPE_VAL_GLITCH_POSITIVE
#define TKDPO4K_VAL_GLITCH_NEGATIVE                 IVISCOPE_VAL_GLITCH_NEGATIVE

    /*- Defined values for attribute TKDPO4K_ATTR_GLITCH_CONDITION -*/
#define TKDPO4K_VAL_GLITCH_GREATER_THAN             IVISCOPE_VAL_GLITCH_GREATER_THAN
#define TKDPO4K_VAL_GLITCH_LESS_THAN                IVISCOPE_VAL_GLITCH_LESS_THAN
#define TKDPO4K_VAL_GLITCH_WITHIN_5_PERCENT         101
#define TKDPO4K_VAL_GLITCH_OUTSIDE_5_PERCENT        102
    
    /*- Defined values for attribute TKDPO4K_ATTR_MODEL -*/
#define TKDPO4K_VAL_DPO_4104                        1
#define TKDPO4K_VAL_DPO_4054                        2
#define TKDPO4K_VAL_DPO_4034                        3
#define TKDPO4K_VAL_DPO_4032                        4
#define TKDPO4K_VAL_MSO_4104                        10
#define TKDPO4K_VAL_MSO_4054                        11
#define TKDPO4K_VAL_MSO_4034                        12
#define TKDPO4K_VAL_MSO_4032                        13
#define TKDPO4K_VAL_DPO_3012                        20
#define TKDPO4K_VAL_DPO_3014                        21
#define TKDPO4K_VAL_DPO_3032                        22
#define TKDPO4K_VAL_DPO_3034                        23
#define TKDPO4K_VAL_DPO_3052                        24
#define TKDPO4K_VAL_DPO_3054                        25
#define TKDPO4K_VAL_MSO_3012                        30
#define TKDPO4K_VAL_MSO_3014                        31
#define TKDPO4K_VAL_MSO_3032                        32
#define TKDPO4K_VAL_MSO_3034                        33
#define TKDPO4K_VAL_MSO_3052                        34
#define TKDPO4K_VAL_MSO_3054                        35
#define TKDPO4K_VAL_DPO_4034B                       36    
#define TKDPO4K_VAL_DPO_4054B                       37
#define TKDPO4K_VAL_DPO_4104B                       38  
#define TKDPO4K_VAL_MSO_4034B                       39
#define TKDPO4K_VAL_MSO_4054B                       40
#define TKDPO4K_VAL_MSO_4104B                       41
#define TKDPO4K_VAL_MDO_41046                       42 
#define TKDPO4K_VAL_MDO_41043                       43 
#define TKDPO4K_VAL_MDO_40546                       44 
#define TKDPO4K_VAL_MDO_40543                       45 
#define TKDPO4K_VAL_DPO_4102B                       46  
#define TKDPO4K_VAL_DPO_4102B_L                     47  
#define TKDPO4K_VAL_DPO_4104B_L                     48 
#define TKDPO4K_VAL_MSO_4102B                       49 
#define TKDPO4K_VAL_MSO_4102B_L                     50 
#define TKDPO4K_VAL_MSO_4104B_L                     51
#define TKDPO4K_VAL_DPO_4014B                       52
#define TKDPO4K_VAL_MSO_4014B                       53
#define TKDPO4K_VAL_MDO_4014_3                      54
#define TKDPO4K_VAL_MDO_4034_3                      55
#define TKDPO4K_VAL_MDO_4014B_3                     56
#define TKDPO4K_VAL_MDO_4034B_3                     57
#define TKDPO4K_VAL_MDO_4054B_3                     58
#define TKDPO4K_VAL_MDO_4054B_6                     59
#define TKDPO4K_VAL_MDO_4104B_3                     60
#define TKDPO4K_VAL_MDO_4104B_6                     61
#define TKDPO4K_VAL_MDO_3012                        62
#define TKDPO4K_VAL_MDO_3014                        63
#define TKDPO4K_VAL_MDO_3022                        64
#define TKDPO4K_VAL_MDO_3024                        65
#define TKDPO4K_VAL_MDO_3032                        66
#define TKDPO4K_VAL_MDO_3034                        67
#define TKDPO4K_VAL_MDO_3052                        68    
#define TKDPO4K_VAL_MDO_3054                        69
#define TKDPO4K_VAL_MDO_3102                        70  
#define TKDPO4K_VAL_MDO_3104                        71
#define TKDPO4K_VAL_MDO_4024C						72
#define TKDPO4K_VAL_MDO_4034C						73
#define TKDPO4K_VAL_MDO_4054C						74
#define TKDPO4K_VAL_MDO_4104C						75

     /*- Defined values for attribute TKDPO4K_ATTR_RUNT_POLARITY -*/
#define TKDPO4K_VAL_RUNT_POSITIVE                   IVISCOPE_VAL_RUNT_POSITIVE
#define TKDPO4K_VAL_RUNT_NEGATIVE                   IVISCOPE_VAL_RUNT_NEGATIVE
#define TKDPO4K_VAL_RUNT_EITHER                     IVISCOPE_VAL_RUNT_EITHER  

    /*- Defined values for attribute TKDPO4K_ATTR_TRANSITION_POLARITY -*/
#define TKDPO4K_VAL_TRANSITION_POSITIVE             1
#define TKDPO4K_VAL_TRANSITION_NEGATIVE             2
#define TKDPO4K_VAL_TRANSITION_EITHER               3

    /*- Defined values for attribute TKDPO4K_ATTR_TRANSITION_WHEN Values-*/
#define TKDPO4K_VAL_TRANSITION_FASTER_THAN          1
#define TKDPO4K_VAL_TRANSITION_SLOWER_THAN          2
#define TKDPO4K_VAL_TRANSITION_EQUAL                3
#define TKDPO4K_VAL_TRANSITION_UNEQUAL              4 

    /*- Defined values for attribute TKDPO4K_ATTR_DELAY_TRIGGER_MODE -*/
#define TKDPO4K_VAL_TRIG_AFTER_TIME                 1
#define TKDPO4K_VAL_TRIG_AFTER_EVENTS               2

    /*- Defined values for attribute TKDPO4K_ATTR_PATTERN_FUNCTION   -*/
    /*-                         &&   TKDPO4K_ATTR_STATE_FUNCTION     -*/
#define TKDPO4K_VAL_LOGIC_AND                       1
#define TKDPO4K_VAL_LOGIC_OR                        2
#define TKDPO4K_VAL_LOGIC_NAND                      3
#define TKDPO4K_VAL_LOGIC_NOR                       4

    /*- Defined values for attribute TKDPO4K_ATTR_PATTERN_WHEN   -*/
    /*-                          &&  TKDPO4K_ATTR_STATE_WHEN     -*/
#define TKDPO4K_VAL_LOGIC_TRUE                      1
#define TKDPO4K_VAL_LOGIC_FALSE                     2
#define TKDPO4K_VAL_LOGIC_LESSTHAN                  3
#define TKDPO4K_VAL_LOGIC_MORETHAN                  4
#define TKDPO4K_VAL_LOGIC_EQUAL                     5
#define TKDPO4K_VAL_LOGIC_UNEQUAL                   6

    /*- Defined values for attribute TKDPO4K_ATTR_PATTERN_INPUT  -*/
    /*-                         &&   TKDPO4K_ATTR_STATE_INPUT    -*/
#define TKDPO4K_VAL_LOGIC_HIGH                      1
#define TKDPO4K_VAL_LOGIC_LOW                       2
#define TKDPO4K_VAL_LOGIC_X                         3

    /*- Defined values for attribute TKDPO4K_ATTR_STATE_CLOCK_EDGE   -*/
    /*-                         &&   TKDPO4K_ATTR_SETHOLD_CLOCK_EDGE -*/
#define TKDPO4K_VAL_CLOCK_RISE                      1
#define TKDPO4K_VAL_CLOCK_FALL                      2

    /*- Defined values for attribute TKDPO4K_ATTR_RS232_CONDITION -*/
#define TKDPO4K_VAL_RS232_RX_START                  1
#define TKDPO4K_VAL_RS232_RX_DATA                   2
#define TKDPO4K_VAL_RS232_RX_END_PACKET             3
#define TKDPO4K_VAL_RS232_TX_START                  4
#define TKDPO4K_VAL_RS232_TX_DATA                   5
#define TKDPO4K_VAL_RS232_TX_END_PACKET             6

    /*- Default value for attribute TKDPO4K_ATTR_RS232_RX_DATA_VALUE -*/
#define TKDPO4K_VAL_RS232_RX_DATA_VALUE             "XXXXXXXX"   

    /*- Default value for attribute TKDPO4K_ATTR_PARALLEL_VALUE -*/
#define TKDPO4K_VAL_PARALLEL_VALUE                  "XXXXXXXX"

    /*- Defined values for attribute TKDPO4K_ATTR_CAN_CONDITION -*/
#define TKDPO4K_VAL_CAN_COND_SOF                    1
#define TKDPO4K_VAL_CAN_COND_FRAMETYPE              2
#define TKDPO4K_VAL_CAN_COND_IDENTIFIER             3
#define TKDPO4K_VAL_CAN_COND_DATA                   4
#define TKDPO4K_VAL_CAN_COND_IDANDDATA              5
#define TKDPO4K_VAL_CAN_COND_EOF                    6
#define TKDPO4K_VAL_CAN_COND_ACKMISS                7

    /*- Default value for attribute TKDPO4K_ATTR_CAN_DATA_VALUE -*/
#define TKDPO4K_VAL_CAN_DATA_VALUE                  "XXXXXXXX"

    /*- Default value for attribute TKDPO4K_ATTR_CAN_ID_VALUE -*/
#define TKDPO4K_VAL_CAN_ID_VALUE                    "XXXXXXXXXXX"   

    /*- Defined values for attribute TKDPO4K_ATTR_CAN_ID_MODE -*/
#define TKDPO4K_VAL_CAN_ID_STANDARD                 1
#define TKDPO4K_VAL_CAN_ID_EXTENDED                 2

    /*- Defined values for attribute TKDPO4K_ATTR_CAN_FRAMETYPE -*/
#define TKDPO4K_VAL_CAN_FRAME_DATA                  1
#define TKDPO4K_VAL_CAN_FRAME_REMOTE                2
#define TKDPO4K_VAL_CAN_FRAME_ERROR                 3
#define TKDPO4K_VAL_CAN_FRAME_OVERLOAD              4

    /*- Defined values for attribute TKDPO4K_ATTR_I2C_CONDITION -*/
#define TKDPO4K_VAL_I2C_COND_START                  1
#define TKDPO4K_VAL_I2C_COND_STOP                   2
#define TKDPO4K_VAL_I2C_COND_REPEATSTART            3
#define TKDPO4K_VAL_I2C_COND_ACKMISS                4
#define TKDPO4K_VAL_I2C_COND_ADDRESS                5
#define TKDPO4K_VAL_I2C_COND_DATA                   6
#define TKDPO4K_VAL_I2C_COND_ADDRANDDATA            7

    /*- Default value for attribute TKDPO4K_ATTR_I2C_ADDR_VALUE -*/
#define TKDPO4K_VAL_I2C_ADDR_VALUE                  "XXXXXXX"
    
    /*- Defined values for attribute TKDPO4K_ATTR_I2C_ADDR_MODE -*/
#define TKDPO4K_VAL_I2C_ADDR7                       1
#define TKDPO4K_VAL_I2C_ADDR10                      2

    /*- Default value for attribute TKDPO4K_ATTR_I2C_DATA_VALUE -*/
#define TKDPO4K_VAL_I2C_DATA_VALUE                  "XXXXXXXX"      

    /*- Defined values for attribute TKDPO4K_ATTR_I2C_DATA_DIRECTION -*/
#define TKDPO4K_VAL_I2C_READ                        1
#define TKDPO4K_VAL_I2C_WRITE                       2
#define TKDPO4K_VAL_I2C_NOCARE                      3

    /*- Defined values for attribute TKDPO4K_ATTR_SPI_CONDITION -*/
#define TKDPO4K_VAL_SPI_SS                          1
#define TKDPO4K_VAL_SPI_MOSI                        2  
#define TKDPO4K_VAL_SPI_MISO                        3
#define TKDPO4K_VAL_SPI_MISOMOSI                    4

    /*- Default value for attribute TKDPO4K_ATTR_SPI_MOSI_VALUE -*/
#define TKDPO4K_VAL_SPI_MOSI_VALUE                  "XXXXXXXX"  

    /*- Default value for attribute TKDPO4K_ATTR_SPI_MISO_VALUE -*/
#define TKDPO4K_VAL_SPI_MISO_VALUE                  "XXXXXXXX"

    /*- Defined values for attribute TKDPO4K_ATTR_MEAS_FUNCTION -*/
#define TKDPO4K_VAL_RISE_TIME                      IVISCOPE_VAL_RISE_TIME
#define TKDPO4K_VAL_FALL_TIME                      IVISCOPE_VAL_FALL_TIME
#define TKDPO4K_VAL_FREQUENCY                      IVISCOPE_VAL_FREQUENCY
#define TKDPO4K_VAL_PERIOD                         IVISCOPE_VAL_PERIOD
#define TKDPO4K_VAL_VOLTAGE_RMS                    IVISCOPE_VAL_VOLTAGE_RMS
#define TKDPO4K_VAL_VOLTAGE_PEAK_TO_PEAK           IVISCOPE_VAL_VOLTAGE_PEAK_TO_PEAK
#define TKDPO4K_VAL_VOLTAGE_MAX                    IVISCOPE_VAL_VOLTAGE_MAX
#define TKDPO4K_VAL_VOLTAGE_MIN                    IVISCOPE_VAL_VOLTAGE_MIN 
#define TKDPO4K_VAL_VOLTAGE_HIGH                   IVISCOPE_VAL_VOLTAGE_HIGH
#define TKDPO4K_VAL_VOLTAGE_LOW                    IVISCOPE_VAL_VOLTAGE_LOW
#define TKDPO4K_VAL_VOLTAGE_AVERAGE                IVISCOPE_VAL_VOLTAGE_AVERAGE 
#define TKDPO4K_VAL_WIDTH_NEG                      IVISCOPE_VAL_WIDTH_NEG
#define TKDPO4K_VAL_WIDTH_POS                      IVISCOPE_VAL_WIDTH_POS
#define TKDPO4K_VAL_DUTY_CYCLE_NEG                 IVISCOPE_VAL_DUTY_CYCLE_NEG
#define TKDPO4K_VAL_DUTY_CYCLE_POS                 IVISCOPE_VAL_DUTY_CYCLE_POS
#define TKDPO4K_VAL_AMPLITUDE                      IVISCOPE_VAL_AMPLITUDE 
#define TKDPO4K_VAL_VOLTAGE_CYCLE_RMS              IVISCOPE_VAL_VOLTAGE_CYCLE_RMS
#define TKDPO4K_VAL_VOLTAGE_CYCLE_AVERAGE          IVISCOPE_VAL_VOLTAGE_CYCLE_AVERAGE
#define TKDPO4K_VAL_POSITIVE_OVERSHOOT             IVISCOPE_VAL_OVERSHOOT
#define TKDPO4K_VAL_NEGATIVE_OVERSHOOT             (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  1)
#define TKDPO4K_VAL_TIMING_BURST_WIDTH             (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  2)
#define TKDPO4K_VAL_PHASE                          (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  3)    
#define TKDPO4K_VAL_DELAY                          (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  4)   
#define TKDPO4K_VAL_AREA                           (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  5) 
#define TKDPO4K_VAL_CYCLE_AREA                     (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  6)
#define TKDPO4K_VAL_CROSS_NEG                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  7)
#define TKDPO4K_VAL_CROSS_POS                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  8)
#define TKDPO4K_VAL_EDGE_POS                       (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  9)
#define TKDPO4K_VAL_EDGE_NEG                       (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  10)
#define TKDPO4K_VAL_PULSE_POS                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  11)
#define TKDPO4K_VAL_PULSE_NEG                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  12)
#define TKDPO4K_VAL_HISTOGRAM_HITS                 (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  13)
#define TKDPO4K_VAL_MEDIAN                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  14)
#define TKDPO4K_VAL_SIGMA1                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  15)
#define TKDPO4K_VAL_SIGMA2                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  16)
#define TKDPO4K_VAL_SIGMA3                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  17)
#define TKDPO4K_VAL_STDDEV                         (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  18)
#define TKDPO4K_VAL_WAVEFORMS                      (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  19)
#define TKDPO4K_VAL_TOVERSHOOT                     (IVISCOPE_VAL_MEASUREMENT_FUNCTION_SPECIFIC_EXT_BASE +  20)


/*- Defined values for attribute TKDPO4K_ATTR_MEAS_DEL_DIRECTION -*/
#define TKDPO4K_VAL_DEL_DIR_BACKWARD                1
#define TKDPO4K_VAL_DEL_DIR_FORWARD                 2

/*- Defined values for attribute TKDPO4K_ATTR_MEAS_DEL_EDGE and TKDPO4K_ATTR_MEAS_DEL_EDGE2 -*/
#define TKDPO4K_VAL_DEL_EDGE_RISE                   1
#define TKDPO4K_VAL_DEL_EDGE_FALL                   2

/*- Defined values for attribute TKDPO4K_ATTR_MEAS_GATING -*/
#define TKDPO4K_VAL_MEAS_GATING_OFF                 1
#define TKDPO4K_VAL_MEAS_GATING_SCREEN              2
#define TKDPO4K_VAL_MEAS_GATING_CURSOR              3
                            
    /*- Defined values for attribute TKDPO4K_ATTR_MEAS_REF_CALC_METH -*/
#define TKDPO4K_VAL_MEAS_HISTOGRAM                  1
#define TKDPO4K_VAL_MEAS_MIN_MAX                    2
#define TKDPO4K_VAL_MEAS_AUTO                       3

    /*- Defined values for attribute TKDPO4K_ATTR_MATH_TYPE -*/
#define TKDPO4K_VAL_MATH_TYPE_DUAL                  1
#define TKDPO4K_VAL_MATH_TYPE_FFT                   2
#define TKDPO4K_VAL_MATH_TYPE_ADVANCED              3
#define TKDPO4K_VAL_MATH_TYPE_SPECTRUM              4
    
    /*- Defined values for attribute TKDPO4K_ATTR_MATH_FUNCTION -*/
#define TKDPO4K_VAL_MATH_ADD                        1
#define TKDPO4K_VAL_MATH_SUBTRACT                   2
#define TKDPO4K_VAL_MATH_MULTIPLY                   3
#define TKDPO4K_VAL_MATH_DIVIDE                     4
#define TKDPO4K_VAL_MATH_FFT                        5

    /*- Defined values for attribute TKDPO4K_ATTR_FFT_VERT_SCALE -*/
#define TKDPO4K_VAL_MATH_DBV_RMS                    1
#define TKDPO4K_VAL_MATH_LINEAR_RMS                 2

    /*- Defined values for attribute TKDPO4K_ATTR_FFT_WINDOW -*/
#define TKDPO4K_VAL_MATH_HANNING                    1 
#define TKDPO4K_VAL_MATH_RECTANGULAR                2
#define TKDPO4K_VAL_MATH_HAMMING                    3
#define TKDPO4K_VAL_MATH_BLACK_HARRIS               4

    /*- Default value for attribute TKDPO4K_ATTR_ADV_DEFINE -*/
#define TKDPO4K_VAL_ADV_DEFINE                      "CH1+CH2"   

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_TYPE -*/
#define TKDPO4K_VAL_BUS_I2C                         1
#define TKDPO4K_VAL_BUS_SPI                         2
#define TKDPO4K_VAL_BUS_CAN                         3
#define TKDPO4K_VAL_BUS_RS232                       4
#define TKDPO4K_VAL_BUS_PARALLEL                    5

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_CAN_BITRATE -*/
#define TKDPO4K_VAL_CAN_RATE10K                     1
#define TKDPO4K_VAL_CAN_RATE20K                     2
#define TKDPO4K_VAL_CAN_RATE33K                     3
#define TKDPO4K_VAL_CAN_RATE37K                     13
#define TKDPO4K_VAL_CAN_RATE50K                     4
#define TKDPO4K_VAL_CAN_RATE62K                     5
#define TKDPO4K_VAL_CAN_RATE83K                     6
#define TKDPO4K_VAL_CAN_RATE92K                     14
#define TKDPO4K_VAL_CAN_RATE100K                    7
#define TKDPO4K_VAL_CAN_RATE125K                    8
#define TKDPO4K_VAL_CAN_RATE250K                    9
#define TKDPO4K_VAL_CAN_RATE500K                    10
#define TKDPO4K_VAL_CAN_RATE800K                    11
#define TKDPO4K_VAL_CAN_RATE1M                      12

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_CAN_PROBE -*/
#define TKDPO4K_VAL_CAN_CANH                        1
#define TKDPO4K_VAL_CAN_CANL                        2
#define TKDPO4K_VAL_CAN_RX                          3
#define TKDPO4K_VAL_CAN_TX                          4
#define TKDPO4K_VAL_CAN_DIFF                        5

    /*- Defined value for attribute TKDPO4K_ATTR_SPI_SCLK_POLARITY -*/
#define TKDPO4K_VAL_SPI_SCLK_RISE                   1
#define TKDPO4K_VAL_SPI_SCLK_FALL                   2

    /*- Defined value for attribute TKDPO4K_ATTR_SPI_MISO_POLARITY -*/
    /*-                             TKDPO4K_ATTR_SPI_MOSI_POLARITY -*/
    /*-                             TKDPO4K_ATTR_SPI_SS_POLARITY   -*/
#define TKDPO4K_VAL_SPI_POLARITY_LOW                1
#define TKDPO4K_VAL_SPI_POLARITY_HIGH               2

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_PARALLEL_CLOCK_EDGE -*/
#define TKDPO4K_VAL_PARALLEL_CLOCK_RISE             1
#define TKDPO4K_VAL_PARALLEL_CLOCK_FALL             2
#define TKDPO4K_VAL_PARALLEL_CLOCK_EITHER           3

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_RS232_DELIMITER -*/
#define TKDPO4K_VAL_RS232_DELIMITER_NULL            1
#define TKDPO4K_VAL_RS232_DELIMITER_LF              2
#define TKDPO4K_VAL_RS232_DELIMITER_CR              3
#define TKDPO4K_VAL_RS232_DELIMITER_SPACE           4
#define TKDPO4K_VAL_RS232_DELIMITER_XFF             5

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_RS232_PARITY -*/
#define TKDPO4K_VAL_RS232_PARITY_NONE               1
#define TKDPO4K_VAL_RS232_PARITY_EVEN               2
#define TKDPO4K_VAL_RS232_PARITY_ODD                3

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_ETHERNET_TYPE -*/   
#define TKDPO4K_VAL_ENET_ENET10BASET                                0
#define TKDPO4K_VAL_ENET_ENET100BASETX                              1

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_ETHERNET_PROBE_TYPE -*/   
#define TKDPO4K_VAL_ENET_PROBE_DIFF                                 0
#define TKDPO4K_VAL_ENET_PROBE_SING                                 1

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_ETHERNET_DIFF_INPUT_DATA_SRC -*/   
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_CH1                    0
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_CH2                    1
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_CH3                    2
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_CH4                    3
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_MATH                   4
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_REF1                   5
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_REF2                   6
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_REF3                   7
#define TKDPO4K_VAL_ENET_DIFF_INPUT_DATA_SRC_REF4                   8

    /*- Defined value for attribute TKDPO4K_BUS_ETHERNET_PROTOCOL -*/   
#define TKDPO4K_VAL_ENET_PROT_IPV4                                  0
#define TKDPO4K_VAL_ENET_PROT_OTHER                                 1

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_ETHERNET_MINUS_DATA_SRC -*/   
#define TKDPO4K_VAL_ENET_MINUS_DATA_SRC_CH1                         0
#define TKDPO4K_VAL_ENET_MINUS_DATA_SRC_CH2                         1
#define TKDPO4K_VAL_ENET_MINUS_DATA_SRC_CH3                         2
#define TKDPO4K_VAL_ENET_MINUS_DATA_SRC_CH4                         3

   /*- Defined value for attribute TKDPO4K_ATTR_BUS_ETHERNET_PLUS_DATA_SRC -*/   
#define TKDPO4K_VAL_ENET_PLUS_DATA_CH1                              0
#define TKDPO4K_VAL_ENET_PLUS_DATA_CH2                              1
#define TKDPO4K_VAL_ENET_PLUS_DATA_CH3                              2
#define TKDPO4K_VAL_ENET_PLUS_DATA_CH4                              3

    /*- Defined value for attribute TKDPO4K_ATTR_USB_TRAN_FIELD -*/   
#define TKDPO4K_VAL_USB_NOCARE                                      0
#define TKDPO4K_VAL_USB_SSPLIT                                      1
#define TKDPO4K_VAL_USB_CSPLIT                                      2

    /*- Defined value for attribute TKDPO4K_ATTR_USB_TRAN_START_END -*/   
#define TKDPO4K_VAL_USB_START_END_NOCARE                            0
#define TKDPO4K_VAL_USB_START_END_FULLSP                            1
#define TKDPO4K_VAL_USB_START_END_LOWSP                             2
#define TKDPO4K_VAL_USB_START_END_ISOSTART                          3
#define TKDPO4K_VAL_USB_START_END_ISOMID                            4
#define TKDPO4K_VAL_USB_START_END_ISOEND                            5
#define TKDPO4K_VAL_USB_START_END_ISOALL                            6

    /*- Defined value for attribute TKDPO4K_ATTR_USB_TRAN_END_POINT -*/   
#define TKDPO4K_VAL_USB_END_NOCARE                                  0
#define TKDPO4K_VAL_USB_END_CONTR                                   1
#define TKDPO4K_VAL_USB_END_ISO                                     2
#define TKDPO4K_VAL_USB_END_BULK                                    3
#define TKDPO4K_VAL_USB_END_INTERRUPT                               4

    /*- Defined value for attribute TKDPO4K_ATTR_BUS_ETHERNET_PROBE_TYPE -*/   
#define TKDPO4K_VAL_ENET_PROBE_DIFFERENTIAL                         0
#define TKDPO4K_VAL_ENET_PROBE_SINGLEENDED                          1



    /*- Defined value for attribute TKDPO4K_ATTR_BUS_ENET_BASIC_CONDITION -*/   
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_SFD                        0
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_MACADDR                    1
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_MACLEN                     2
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_DAT                        3
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_EOP                        4
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_IDL                        5
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_FCSE                       6
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_IPH                        7
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_TCPH                       8
#define TKDPO4K_VAL_ENET_BASIC_CONDITION_QTAG                       9


    /*- Defined value for attribute TKDPO4K_ATTR_BUS_ENET_BASIC_FRAME_TYPE -*/   
#define TKDPO4K_VAL_ENET_BASIC_FRAME_BAS                            0 
#define TKDPO4K_VAL_ENET_BASIC_FRAME_QTAG                           1

    /*- Defined value for attribute TKDPO4K_ATTR_ENET_BASIC_QUALIFIER -*/   
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_LESST                      0
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_MORET                      1
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_EQ                         2
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_UNEQ                       3
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_LESSEQ                     4
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_MOREEQ                     5
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_IN                         6
#define TKDPO4K_VAL_ENET_BASIC_QUALIFIER_OUT                        7

#define TKDPO4K_VAL_RF_SPECTRUM_TRIGGERED                           0

#define TKDPO4K_VAL_RF_RBW_MODE_MANUAL                              0
#define TKDPO4K_VAL_RF_RBW_MODE_AUTO                                1


#define TKDPO4K_VAL_RF_SPECTRUM_FREERUN                             1

#define TKDPO4K_VAL_RF_UNITS_DBM                                    0
#define TKDPO4K_VAL_RF_UNITS_DBUW                                   1
#define TKDPO4K_VAL_RF_UNITS_DBMV                                   2
#define TKDPO4K_VAL_RF_UNITS_DMUV                                   3  
#define TKDPO4K_VAL_RF_UNITS_DBMA                                   4  
#define TKDPO4K_VAL_RF_UNITS_DBUA                                   5
#define TKDPO4K_VAL_RF_UNITS_DBUV                                   6


#define TKDPO4K_VAL_FFT_WINDOW_RECTANGULAR                          0
#define TKDPO4K_VAL_FFT_WINDOW_HAMMING                              1
#define TKDPO4K_VAL_FFT_WINDOW_HANNING                              2
#define TKDPO4K_VAL_FFT_WINDOW_BLACKMANHARRIS                       3
#define TKDPO4K_VAL_FFT_WINDOW_KAISER                               4

#define TKDPO4K_VAL_RF_MEASURE_TYPE_NONE                            0
#define TKDPO4K_VAL_RF_MEASURE_TYPE_CP                              1
#define TKDPO4K_VAL_RF_MEASURE_TYPE_ACPR                            2
#define TKDPO4K_VAL_RF_MEASURE_TYPE_OBW                             3

#define TKDPO4K_VAL_FFT_WINDOW_FLATTOP                              5



/*- Defined values for attribute TKDPO4K_ATTR_FASTACQ_WAVEFORM_GRADING -*/
#define TKDPO4K_VAL_WAVEFORM_GRADING_NORMAL                         1
#define TKDPO4K_VAL_WAVEFORM_GRADING_TEMPERATURE                    2
#define TKDPO4K_VAL_WAVEFORM_GRADING_SPECTRAL                       3
#define TKDPO4K_VAL_WAVEFORM_GRADING_INVERTED                       4

/*- Defined values for attribute TKDPO4K_ATTR_RF_IQ_DATA_FILE_FORMAT -*/
#define TKDPO4K_VAL_FILE_FORMAT_TIQ                                 1
#define TKDPO4K_VAL_FILE_FORMAT_MATLAB                              2

/*- Defined values for attribute TKDPO4K_ATTR_FUNC_WAVEFORM -*/
#define TKDPO4K_VAL_WFM_SINE                                (1)
#define TKDPO4K_VAL_WFM_SQUARE                              (2)
#define TKDPO4K_VAL_WFM_PULSE                               (3)
#define TKDPO4K_VAL_WFM_RAMP                                (4)
#define TKDPO4K_VAL_WFM_NOISE                               (5)
#define TKDPO4K_VAL_WFM_DC                                  (6)
#define TKDPO4K_VAL_WFM_SINC                                (7)
#define TKDPO4K_VAL_WFM_GAUSSIAN                            (8)
#define TKDPO4K_VAL_WFM_LORENTZ                             (9)
#define TKDPO4K_VAL_WFM_ERISE                               (10)
#define TKDPO4K_VAL_WFM_EDECAY                              (11)
#define TKDPO4K_VAL_WFM_HAVERSINE                           (12)
#define TKDPO4K_VAL_WFM_CARDIAC                             (13)
#define TKDPO4K_VAL_WFM_ARBITRARY                           (14)

/*- Extended arbitrary waveform -*/ 
#define TKDPO4K_VAL_FIRST_WAVEFORM_HANDLE                   (10000L)

#define TKDPO4K_VAL_WFM_USER                                (TKDPO4K_VAL_FIRST_WAVEFORM_HANDLE)

/*- Defined values for Arbitrary Waveform Handles ---------------*/
#define TKDPO4K_VAL_ALL_WAVEFORMS                           (-1L)    

/*- Defined values for attribute TKDPO4K_ATTR_FUNC_PRESET_LEVEL -*/
#define TKDPO4K_VAL_FUNC_LEVEL_CMOS_5_0V                    (1)
#define TKDPO4K_VAL_FUNC_LEVEL_CMOS_3_3V                    (2)
#define TKDPO4K_VAL_FUNC_LEVEL_CMOS_2_5V                    (3)
#define TKDPO4K_VAL_FUNC_LEVEL_USER                         (4)
#define TKDPO4K_VAL_FUNC_LEVEL_ECL                          (5)
#define TKDPO4K_VAL_FUNC_LEVEL_TTL                          (6)

/*- Defined values for attribute TKDPO4K_ATTR_OUTPUT_IMPEDANCE -*/
#define TKDPO4K_VAL_FUNC_OUTPUT_IMPEDANCE_FIFTY             (1)
#define TKDPO4K_VAL_FUNC_OUTPUT_IMPEDANCE_HIGHZ             (2)

/*- Defined values for attribute TKDPO4K_ATTR_DVM_TYPE -*/
#define TKDPO4K_VAL_DVM_TYPE_ACRMS                          (1)
#define TKDPO4K_VAL_DVM_TYPE_ACDCRMS                        (2)
#define TKDPO4K_VAL_DVM_TYPE_DC                             (3)
#define TKDPO4K_VAL_DVM_TYPE_FREQUENCY                      (4)
#define TKDPO4K_VAL_DVM_TYPE_OFF                            (5)

/*- Defined values for attribute TKDPO4K_ATTR_DVM_SOURCE -*/
#define TKDPO4K_VAL_DVM_SOURCE_CH1                          (1)
#define TKDPO4K_VAL_DVM_SOURCE_CH2                          (2)
#define TKDPO4K_VAL_DVM_SOURCE_CH3                          (3)
#define TKDPO4K_VAL_DVM_SOURCE_CH4                          (4)

/*- Defined values for attribute TKDPO4K_ATTR_EVENT_TRIGGER_TYPE -*/
#define TKDPO4K_VAL_EVENT_TRIGGER_TYPE_NONE                 (1)
#define TKDPO4K_VAL_EVENT_TRIGGER_TYPE_TRIGGER               (2)
#define TKDPO4K_VAL_EVENT_TRIGGER_TYPE_ACQCOMPLETE          (3)

/*- Defined values for Event Action -*/ 
#define TKDPO4K_VAL_EVENT_ACTION_AUXOUT                     (1)
#define TKDPO4K_VAL_EVENT_ACTION_PRINT                      (2)
#define TKDPO4K_VAL_EVENT_ACTION_SAVE_IMAGE                 (3)      
#define TKDPO4K_VAL_EVENT_ACTION_SAVE_WAVEFORM              (4)
#define TKDPO4K_VAL_EVENT_ACTION_SRQ                        (5)
#define TKDPO4K_VAL_EVENT_ACTION_STOP_ACQ                   (6)




/**************************************************************************** 
 *---------------- Instrument Driver Function Declarations -----------------* 
 ****************************************************************************/

    /*- Init and Close Functions -------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_init (ViRsrc resourceName, ViBoolean IDQuery,
                                  ViBoolean resetDevice, ViSession *vi);
DllExport ViStatus _VI_FUNC  tkdpo4k_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                             ViBoolean resetDevice, ViConstString optionString, 
                                             ViSession *newVi);
DllExport ViStatus _VI_FUNC  tkdpo4k_close (ViSession vi);   

    /*- Coercion Functions --------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_GetNextCoercionRecord (ViSession vi, ViInt32 bufferSize, ViChar record[]);

    /*- Locking Functions --------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_LockSession (ViSession vi, ViBoolean *callerHasLock);   
DllExport ViStatus _VI_FUNC  tkdpo4k_UnlockSession (ViSession vi, ViBoolean *callerHasLock);

    /*- Channel Subsystem Configuration -----------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureChannel (ViSession vi,
                                               ViConstString channel,
                                               ViReal64 range,
                                               ViReal64 offset, 
                                               ViInt32 coupling,
                                               ViReal64 probeAttenuation,
                                               ViBoolean enabled);

DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureDigitalChannel (ViSession vi,
                                                    ViConstString channel,
                                                    ViReal64 threshold,
                                                    ViBoolean enabled);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureChanCharacteristics (ViSession vi,
                                                         ViConstString channel,
                                                         ViReal64 inputImpedance,
                                                         ViReal64 maxInputFrequency);
DllExport ViStatus _VI_FUNC tkdpo4k_AutoProbeSenseValue (ViSession vi,
                                                ViConstString channel,
                                                ViReal64 *autoProbeSenseValue);

DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureBusThreshold (ViSession vi,
                                                    ViConstString channel,
                                                    ViReal64 upperThreshold,
                                                    ViReal64 lowerThreshold);
    /*- Triggering Subsystem Configuration ---------------------------------*/
    /*- IviScopeBase Functions -*/    
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTrigger (ViSession vi, ViInt32 triggerType, 
                                                        ViReal64 holdoff);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTriggerCoupling (ViSession vi,
                                                        ViInt32 coupling);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTriggerLevel (ViSession vi,
                                                    ViConstString channel,
                                                    ViReal64 level);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTriggerThreshold(ViSession vi,
                                                        ViConstString channel,
                                                        ViReal64 upperThreshold,
                                                        ViReal64 lowerThreshold);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEdgeTriggerSource (ViSession vi, 
                                                        ViConstString source,
                                                        ViReal64 level,
                                                        ViInt32 slope);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureAuxInTriggerLevel(ViSession vi, 
                                                     ViReal64 TriggerLevel);
    /*- IviScopeACLineTrigger Function -*/                                                        
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureAcLineTriggerSlope (ViSession vi,
                                                        ViInt32 acLineSlope);
                                                        
    /*- Delay Trigger Functions -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureDelayTriggerSource ( ViSession vi,
                                                        ViConstString source,
                                                        ViInt32 coupling,
                                                        ViReal64 level,
                                                        ViInt32 slope);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureDelayTriggerMode (   ViSession vi,
                                                        ViInt32 mode);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureDelayTriggerEvents ( ViSession vi,
                                                        ViInt32 events);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureDelayTriggerTime (   ViSession vi,
                                                        ViReal64 time);
                                                        
    /*- IviScopeTVTrigger Functions -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTVTriggerSource (ViSession vi, 
                                                        ViConstString source,
                                                        ViInt32 TVSignalFormat, 
                                                        ViInt32 TVEvent,
                                                        ViInt32 TVPolarity);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTVTriggerLineNumber (ViSession vi,
                                                        ViInt32 tvLineNumber);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTVTriggerFieldHoldoff (ViSession vi,
                                                          ViReal64 holdoff);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTVTriggerHDTV (ViSession vi,
                                                  ViInt32 format);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTVTriggerCustom (ViSession vi,
                                                    ViInt32 format,
                                                    ViInt32 scanRate,
                                                    ViReal64 linePeriod,
                                                    ViReal64 syncInterval);
    /*- IviScopeRuntTrigger Function -*/                                                        
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRuntTriggerSource (ViSession vi, 
                                                        ViConstString source,
                                                        ViReal64 runtLowThreshold, 
                                                        ViReal64 runtHighThreshold, 
                                                        ViInt32 runtPolarity);
                                                        
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRuntTriggerCondition (ViSession vi,
                                                            ViReal64 runtWidth,
                                                            ViInt32 runtCondition);
    /*- IviScopeGlitchTrigger Function -*/                                                        
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureGlitchTriggerSource (ViSession vi, 
                                                        ViConstString source, 
                                                        ViReal64 level, 
                                                        ViReal64 glitchWidth, 
                                                        ViInt32 glitchPolarity, 
                                                        ViInt32 glitchCondition);

    /*- Transition Trigger Function -*/                                                        
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTransitTriggerSource (ViSession vi,
                                                     ViConstString source,
                                                     ViInt32 polarity,
                                                     ViInt32 when,
                                                     ViReal64 deltaTime,
                                                     ViReal64 lowThreshold,
                                                     ViReal64 highThreshold);
    /*- Logic Basic -*/                                                     
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureLogicTriggerThreshold (ViSession vi,
                                                            ViConstString channel,
                                                            ViReal64 threshold);

    /*- Logic Pattern Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigurePatternTrigger (ViSession vi,
                                                            ViInt32 function,
                                                            ViReal64 deltaTime,
                                                            ViInt32 condition);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigurePatternTriggerInput (ViSession vi,
                                                                ViConstString channel,
                                                                ViInt32 logicInput);
                                                                
    /*- Logic State Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureStateTrigger (ViSession vi,
                                                        ViInt32 function);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureStateTriggerDataInput (ViSession vi,
                                                                ViConstString channel,
                                                                ViInt32 logicInput);                                                        
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureStateTriggerClockInput (ViSession vi,
                                                                    ViInt32 clockSource,
                                                                    ViInt32 edge);
                                                                    
    /*- Logic Sethold Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureSetholdTriggerSource( ViSession vi,
                                                            ViInt32 dataSource,
                                                            ViInt32 clockSource,
                                                            ViInt32 clockEdge,
                                                            ViReal64 setTime,
                                                            ViReal64 holdTime);

    /*- Serial Basic Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureSerialTriggerSource( ViSession vi, 
                                                            ViInt32 busSource);

    /*- Serial CAN Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureCANTrigger( ViSession vi,
                                                ViInt32 busNumber,
                                                ViInt32 condition);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureCANTriggerID( ViSession vi,
                                                    ViInt32 busNumber,
                                                    ViInt32 mode,
                                                    ViConstString value);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureCANTriggerData( ViSession vi,
                                                    ViInt32 busNumber,
                                                    ViInt32 size,
                                                    ViConstString value);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureCANTriggerFrameType( ViSession vi,
                                                        ViInt32 busNumber,
                                                        ViInt32 frameType);
                                                        
    /*- Serial I2C Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureI2CTrigger( ViSession vi,
                                                ViInt32 busNumber,
                                                ViInt32 condition,
                                                ViInt32 direction);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureI2CTriggerAddress( ViSession vi,
                                                        ViInt32 busNumber,
                                                        ViInt32 mode,
                                                        ViConstString value);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureI2CTriggerData( ViSession vi,
                                                    ViInt32 busNumber,
                                                    ViInt32 size,
                                                    ViConstString value);                                                       

    /*- Serial SPI Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureSPITrigger( ViSession vi,
                                                ViInt32 busNumber,
                                                ViInt32 condition,
                                                ViInt32 dataSize,
                                                ViConstString MISOValue,
                                                ViConstString MOSIValue);
    /*- Serial Parallel Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureParallelTrigger( ViSession vi,
                                                    ViInt32 busNumber,
                                                    ViString value);

    /*- USB Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureUSBTrigger( ViSession vi,
                                                    ViInt32 busNumber,
                                                    ViString hubAddress,
                                                    ViString portAddress,
                                                    ViInt32 splitTranStartEnd,
                                                    ViInt32 splitTranField,
                                                    ViInt32 splitTranEnd);                                

    /*- Ethernet Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEthernetTriggerBasic( ViSession vi,
                                                            ViInt32 busNumber,
                                                            ViInt32 frameType,
                                                            ViInt32 condition,
                                                            ViInt32 qualifier); 

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEthernetTriggerData( ViSession vi,
                                                            ViInt32 busNumber,
                                                            ViInt32 size,
                                                            ViInt32 offset,
                                                            ViString upperLimit,
                                                            ViString data);                               

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEthernetTriggerIPH( ViSession vi,
                                                                ViInt32 busNumber,
                                                                ViString srcAddress,
                                                                ViString dstAddress,
                                                                ViString protocol);                               

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEthernetTriggerMAC( ViSession vi,
                                                            ViInt32 busNumber,
                                                            ViString srcAddress,
                                                            ViString dstAddress,
                                                            ViString upperLength,
                                                            ViString length);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEthernetTriggerQTAG( ViSession vi,
                                                            ViInt32 busNumber,
                                                            ViString QTAG); 

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEthernetTriggerTCPH( ViSession vi,
                                                            ViInt32 busNumber,
                                                            ViString srcAddress,
                                                            ViString dstAddress,
                                                            ViString seqNumber,
                                                            ViString ackNumber);    

    /*- Serial RS232 Trigger Function -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRS232Trigger(ViSession vi,
                                                ViInt32 busNumber,
                                                ViInt32 condition,
                                                ViInt32 RxSize,
                                                ViString RxValue,
                                                ViInt32 TxSize,
                                                ViString TxValue);

    /*- Waveform Acquisition Subsystem -------------------------------------*/
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureAcquisitionRecord (ViSession vi, 
                                                 ViReal64 timePerRecord, 
                                                 ViInt32 minimumRecordLength,
                                                 ViReal64 acquisitionStartTime);
DllExport ViStatus _VI_FUNC  tkdpo4k_ActualRecordLength (ViSession vi, 
                                               ViInt32 *actualRecordLength);
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureAcquisitionType (ViSession vi,
                                                  ViInt32 acquisitionType);
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureNumAverages (ViSession vi,
                                                  ViInt32 numberOfAverages);
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureNumEnvelopes (ViSession vi,
                                                   ViInt32 numberOfEnvelopes);
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureMagnivuEnabled (ViSession vi,
                                                    ViBoolean magnivuEnabled);

DllExport ViStatus _VI_FUNC  tkdpo4k_ForceTrigger (ViSession vi);

DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureWfmBufferSize(ViSession vi, 
                                                ViInt32 bufferSize);
                                                
DllExport ViStatus _VI_FUNC  tkdpo4k_ReadWaveform (ViSession vi, 
                                          ViConstString channel,
                                          ViInt32 waveformSize,
                                          ViInt32 maxTime,
                                          ViReal64 waveform[],
                                          ViInt32 *actualPoints, 
                                          ViReal64 *initialX,
                                          ViReal64 *xIncrement);

DllExport ViStatus _VI_FUNC  tkdpo4k_InitiateAcquisition (ViSession vi);

DllExport ViStatus _VI_FUNC  tkdpo4k_AcquisitionStatus (ViSession vi, ViInt32 *status);

DllExport ViStatus _VI_FUNC  tkdpo4k_Abort (ViSession vi);

DllExport ViStatus _VI_FUNC  tkdpo4k_FetchWaveform (ViSession vi,
                                           ViConstString channel,
                                           ViInt32 waveformSize,
                                           ViReal64 waveform[],
                                           ViInt32 *actualPoints,
                                           ViReal64 *initialX,
                                           ViReal64 *xIncrement);   

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureFastAcquisition(ViSession vi, 
                                                   ViInt32 WaveformGrading, 
                                                   ViBoolean FastAcqusitionEnable);

    /*additional functions needed to be spec compliant*/
DllExport ViStatus _VI_FUNC tkdpo4k_SampleRate (ViSession vi, ViReal64 *rate);
DllExport ViStatus _VI_FUNC tkdpo4k_SampleMode (ViSession vi, ViInt32 *mode);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTriggerModifier (ViSession vi,
                                                        ViInt32 triggerModifier);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureInitiateContinuous (ViSession vi,
                                                        ViBoolean continuousAcquisition);

    /* 
        Used when TKDPO4K_ATTR_ACQUISTION_TYPE is either 
        TKDPO4K_VAL_PEAK_DETECT or TKDPO4K_VAL_ENVELOPE 
    */
DllExport ViStatus _VI_FUNC  tkdpo4k_ReadMinMaxWaveform (ViSession vi,
                                                ViConstString channel, 
                                                ViInt32 waveformSize,
                                                ViInt32 maxTime,
                                                ViReal64 minWaveform[],
                                                ViReal64 maxWaveform[],
                                                ViInt32 *actualPoints,
                                                ViReal64 *initialX, 
                                                ViReal64 *xIncrement);

    /* 
        Used when TKDPO4K_ATTR_ACQUISTION_TYPE is either
        TKDPO4K_VAL_PEAK_DETECT or TKDPO4K_VAL_ENVELOPE 
    */
DllExport ViStatus _VI_FUNC  tkdpo4k_FetchMinMaxWaveform (ViSession vi,
                                                 ViConstString channel, 
                                                 ViInt32 waveformSize,
                                                 ViReal64 minWaveform[], 
                                                 ViReal64 maxWaveform[],
                                                 ViInt32 *actualPoints, 
                                                 ViReal64 *initialX,
                                                 ViReal64 *xIncrement);
                                                 
DllExport ViStatus _VI_FUNC  tkdpo4k_InitiateContinuousAcquisition (ViSession vi, 
                                            ViInt32 waveformSize);
                                            
DllExport ViStatus _VI_FUNC tkdpo4k_FetchContinuousWaveform (ViSession vi, 
                                                    ViConstString channel,
                                                    ViReal64 waveform[],
                                                    ViInt32 *actualPoints,
                                                    ViReal64 *initialX,
                                                    ViReal64 *xIncrement);
                                                    
DllExport ViStatus _VI_FUNC tkdpo4k_FetchContinuousMinMaxWaveform (ViSession vi, 
                                                    ViConstString channel,
                                                    ViReal64 minWaveform[], 
                                                    ViReal64 maxWaveform[], 
                                                    ViInt32 *actualPoints, 
                                                    ViReal64 *initialX, 
                                                    ViReal64 *xIncrement);

DllExport ViStatus _VI_FUNC  tkdpo4k_StopContinuousAcquisition (ViSession vi);   

    /*- Waveform Measurements ----------------------------------------------*/
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureMidRef (ViSession vi, ViReal64 mid); 


DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureRefLevels (ViSession vi, 
                                                ViReal64 low, 
                                                ViReal64 mid, 
                                                ViReal64 high);
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureMeasurement (ViSession vi,
                                                  ViInt32 referenceLevelCalcMethod,
                                                  ViInt32 gating);
DllExport ViStatus _VI_FUNC  tkdpo4k_ReadWaveformMeasurement (ViSession vi,
                                                     ViConstString channel, 
                                                     ViInt32 measurementFunction,
                                                     ViInt32 maxTime, 
                                                     ViReal64 *measurement);
DllExport ViStatus _VI_FUNC  tkdpo4k_ReadPhaseMeasurement (ViSession vi,
                                                     ViConstString measureSource1,
                                                     ViConstString measureSource2,
                                                     ViInt32 maxTime,
                                                     ViReal64 *measurement);

DllExport ViStatus _VI_FUNC tkdpo4k_ReadFreqDomainMeasurement (ViSession vi,
                                                     ViReal64 *RFChannelPower,
                                                     ViInt32 *unit,
                                                     ViReal64 *ACPR,
                                                     ViConstString ACPRAdjacentChannel);

DllExport ViStatus _VI_FUNC  tkdpo4k_ReadDelayMeasurement (ViSession vi,
                                                     ViConstString measureSource1,
                                                     ViConstString measureSource2,
                                                     ViInt32 delayEdge1,
                                                     ViInt32 delayEdge2,
                                                     ViInt32 delayDirection,
                                                     ViInt32 maxTime,
                                                     ViReal64 *measurement);

DllExport ViStatus _VI_FUNC tkdpo4k_ReadSnapshotMeasurement(ViSession vi,
                                                  ViReal64 *SnapshotMeasValue);

DllExport ViStatus _VI_FUNC tkdpo4k_ReadRFVersusBandwidth(ViSession vi, 
                                                ViReal64 *VersusTimeBandwidth);

DllExport ViStatus _VI_FUNC tkdpo4k_ReadSpectrogramTime(ViSession vi, 
                                              ViInt32 *SecondsNum, 
                                              ViInt32 *TimeStamp);   
 
DllExport ViStatus _VI_FUNC  tkdpo4k_FetchWaveformMeasurement (ViSession vi,
                                                      ViConstString channel, 
                                                      ViInt32 measurementFunction, 
                                                      ViReal64 *measurement);

DllExport ViStatus _VI_FUNC  tkdpo4k_FetchPhaseMeasurement (ViSession vi,
                                                      ViConstString measureSource1,
                                                      ViConstString measureSource2,
                                                      ViReal64 *measurement);

DllExport ViStatus _VI_FUNC  tkdpo4k_FetchDelayMeasurement (ViSession vi,
                                                      ViConstString measureSource1,
                                                      ViConstString measureSource2,
                                                      ViInt32 delayEdge1,
                                                      ViInt32 delayEdge2,
                                                      ViInt32 delayDirection,
                                                      ViReal64 *measurement);

    /*- BUS Configuration --------------------------------------------------*/
    /*- BUS Basic -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureBUSType (ViSession vi,
                                            ViInt32 busNumber,
                                            ViInt32 type);
                                            
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureBUSState (ViSession vi,
                                                ViInt32 busNumber,
                                                ViBoolean state);
                                                
    /*- CAN -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureCAN (ViSession vi,
                                        ViInt32 busNumber,
                                        ViInt32 bitRate,
                                        ViInt32 probeType,
                                        ViInt32 source);
                                        
    /*- I2C -*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureI2C (ViSession vi,
                                        ViInt32 busNumber,
                                        ViInt32 clockSource,
                                        ViInt32 dataSource);
                                        
    /*- SPI -*/                                     
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureSPI (ViSession vi,
                                        ViInt32 busNumber,
                                        ViInt32 SCLKSource,
                                        ViInt32 SCLKPolarity,   
                                        ViInt32 SSSource,
                                        ViInt32 SSPolarity, 
                                        ViInt32 MOSISource,
                                        ViInt32 MOSIPolarity,                                       
                                        ViInt32 MISOSource,
                                        ViInt32 MISOPolarity);                                      

    /*- Parallel -*/                                                    
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureParallel (ViSession     vi,
                                              ViInt32       busNumber,
                                              ViConstString bitsSource,
                                              ViInt32       clockSource,
                                              ViInt32       clockEdge,
                                              ViBoolean     clockLocked,
                                              ViInt32       width);

    /*- RS232 -*/
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureRS232 (ViSession    vi,
                                           ViInt32      busNumber,
                                           ViInt32      bitRate,
                                           ViInt32      dataBits,
                                           ViInt32      delimiter,
                                           ViInt32      parity,
                                           ViInt32      RxSource,
                                           ViInt32      TxSource);
    /*- Ethernet -*/
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureEthernet (ViSession vi,
                                           ViInt32      busNumber, 
                                           ViInt32      ethernetType,
                                           ViInt32      probeType,
                                           ViInt32      diffInputDataSrc,
                                           ViInt32      plusDataSrc,
                                           ViInt32      minusDataSrc,
                                           ViInt32      protocol);
    /*- Auto Setup ---------------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_AutoSetup (ViSession vi);

    /*- Math Channels ------------------------------------------------------*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureMathChannel (ViSession vi,
                                                ViInt32 mathSource1,
                                                ViInt32 mathFunction,
                                                ViInt32 mathSource2);
DllExport ViStatus _VI_FUNC  tkdpo4k_ConfigureMathChannelFFT (ViSession vi, 
                                                    ViInt32 mathSource1,
                                                    ViInt32 fftWindow, 
                                                    ViInt32 fftScale);
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureMathChannelAdvanced (ViSession vi,
                                                        ViReal64 var1,
                                                        ViReal64 var2,
                                                        ViConstString funcDefinition);                                                    

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureMathChannelSpectrum (ViSession vi,
                                                        ViInt32 mathRFSource1,
                                                        ViInt32 mathRFSource2,
                                                        ViInt32 mathFunction);

DllExport ViStatus _VI_FUNC  tkdpo4k_EnableMathChannel (ViSession vi, 
                                              ViBoolean enableMathChannel);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureMathExpDisplay(ViSession vi, 
                                                  ViBoolean AutoScale);
                                                
                                                    
    /*- Reference Channels -------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_StoreWfmToRefChannel (ViSession vi, 
                                                 ViConstString sourceChannel,
                                                 ViConstString refChannel);
                                                 
DllExport ViStatus _VI_FUNC  tkdpo4k_EnableReferenceChannel (ViSession vi, 
                                                   ViConstString refChannel,
                                                   ViBoolean enableRefChannel);                                                 
 

     /*- RF Channels -------------------------------------------------*/    

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRFRbw (ViSession vi,
                                          ViReal64 spanBandwidth,
                                          ViBoolean autoResolution,
                                          ViReal64 manualResolution,
                                          ViReal64 manualReference,
                                          ViBoolean autoReference);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRFSquelch (ViSession vi,
                                              ViBoolean enableSquelch,
                                              ViReal64 squelchThreshold); 

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRFSpectrum (ViSession vi,
                                               ViInt32 spectrumMode,
                                               ViInt32 frequencySettingMode,
                                               ViReal64 centerFrequency,
                                               ViReal64 spanFrequency,
                                               ViReal64 startFrequency,
                                               ViReal64 stopFrequency);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRFDetection (ViSession vi,
                                                ViBoolean enableAutoDetectionMode,
                                                ViConstString RFChannel,
                                                ViInt32 detectionMethod);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRFNumAverages (ViSession vi,
                                                  ViInt32 numberOfAverages);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRFMeasurement (ViSession vi,
                                                  ViInt32 measureType,
                                                  ViReal64 channelBandwidth,
                                                  ViInt32 ACPRAdjacentPairCount,
                                                  ViReal64 ACPRChannelSpacing,
                                                  ViReal64 OBWPowerPercentage);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRFPhaseTrace(ViSession vi, 
                                                ViInt32 PhaseInDegrees, 
                                                ViInt32 DegreesNumber,
                                                ViBoolean EnablePhaseWrap);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureTimeDomainWinFunc (ViSession vi,
                                                      ViInt32 RFWindowFunction);

DllExport ViStatus _VI_FUNC tkdpo4k_EnableRFChannel (ViSession vi,
                                           ViConstString refChannel,
                                           ViBoolean enable); 

DllExport ViStatus _VI_FUNC tkdpo4k_SetRFIQDataFileFormat(ViSession vi, 
                                                ViInt32 FileFormat);

DllExport ViStatus _VI_FUNC tkdpo4k_ResetSpectrumTraces(ViSession vi);

DllExport ViStatus _VI_FUNC tkdpo4k_ClearSpectrogram(ViSession vi);

/*- Arbitrary Function Generator Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureStandardWaveform(ViSession vi, 
                                                    ViInt32 Waveform,
                                                    ViReal64 Amplitude, 
                                                    ViReal64 DCOffset, 
                                                    ViReal64 Frequency,
                                                    ViReal64 StartPhase);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigurePresetLevel(ViSession vi, 
                                               ViInt32 PresetLevel);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureNoise(ViSession vi, 
                                         ViBoolean EnableNoise, 
                                         ViInt32 NoiseLevel);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureOutputEnabled(ViSession vi, 
                                                 ViBoolean Enabled);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureOutputImpedance(ViSession vi, 
                                                   ViInt32 Impedance);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigurePeriod(ViSession vi, 
                                          ViReal64 Period);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureSquareDutyCycle(ViSession vi, 
                                                   ViReal64 DutyCycle);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigurePulseWidth(ViSession vi, 
                                              ViReal64 PulseWidth);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureRampSymmetry(ViSession vi, 
                                                ViReal64 RampSymmetry);

/*- Arbitrary Waveform Generator Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC tkdpo4k_QueryArbitraryWaveform(ViSession vi, 
                                                 ViChar WaveformSlot[], 
                                                 ViChar WaveformDate[],
                                                 ViChar WaveformTime[]);
                                                     
DllExport ViStatus _VI_FUNC tkdpo4k_QueryEditedArbitraryWaveform(ViSession vi, 
                                                       ViChar Function[], 
                                                       ViInt32 *PointNumber);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureWaveformLabel(ViSession vi, 
                                                 ViChar WaveformSlot[], 
                                                 ViChar Label[]);

DllExport ViStatus _VI_FUNC tkdpo4k_GenerateArbWaveformFunction(ViSession vi, 
                                                      ViInt32 Function, 
                                                      ViInt32 PointNumber);

DllExport ViStatus _VI_FUNC tkdpo4k_CreateArbWaveform (ViSession vi, 
                                             ViInt32 wfmSize, 
                                             ViReal64 wfmData[], 
                                             ViInt32* wfmHandle);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureArbWaveform(ViSession vi, 
                                               ViInt32 Handle, 
                                               ViReal64 Amplitude, 
                                               ViReal64 Offset, 
                                               ViReal64 Frequency);

DllExport ViStatus _VI_FUNC tkdpo4k_ClearArbWaveform(ViSession vi, 
                                           ViInt32 wfmHandle);

/*- General Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC tkdpo4k_SaveWaveform(ViSession vi, 
                                       ViChar Source[], 
                                       ViChar Destination[]);

DllExport ViStatus _VI_FUNC tkdpo4k_RecallWaveform(ViSession vi, 
                                         ViChar Source[], 
                                         ViChar Destination[]);

/*- Digital Voltmeter Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC tkdpo4k_ResetDigitalVoltmeter(ViSession vi);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureDigitalVoltmeter(ViSession vi, 
                                                    ViInt32 Type, 
                                                    ViInt32 Source, 
                                                    ViBoolean AutoRange);

DllExport ViStatus _VI_FUNC tkdpo4k_ReadDigitalVoltmeter(ViSession vi, 
                                               ViReal64 *Value, 
                                               ViReal64 *Frequency);

DllExport ViStatus _VI_FUNC tkdpo4k_ReadHistoryDVM(ViSession vi, 
                                         ViReal64 *Average, 
                                         ViReal64 *Maximum, 
                                         ViReal64 *Minimum);

DllExport ViStatus _VI_FUNC tkdpo4k_ReadStatisticsDVM(ViSession vi, 
                                            ViReal64 *Maximum, 
                                            ViReal64 *Minimum);

/*- Action Event Group Functions ----------------------------------------------------*/
DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEvent(ViSession vi, 
                                         ViInt32 EventType, 
                                         ViInt32 AcquisitionNumber,  
                                         ViInt32 RepeatCount);

DllExport ViStatus _VI_FUNC tkdpo4k_ConfigureEventEnabled(ViSession vi, 
                                                ViInt32 EventAction, 
                                                ViBoolean EventEnabled);

    /*- Error Functions ----------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_error_query (ViSession vi, ViInt32 *errorCode,
                                         ViChar errorMessage[]);
DllExport ViStatus _VI_FUNC  tkdpo4k_error_message (ViSession vi, ViStatus errorCode,
                                           ViChar errorMessage[256]);
DllExport ViStatus _VI_FUNC  tkdpo4k_GetError (ViSession vi, 
                                     ViStatus *errorCode, 
                                     ViInt32 bufferSize,
                                     ViChar description[]);
DllExport ViStatus _VI_FUNC  tkdpo4k_ClearError (ViSession vi);

        /*- Channel Info Functions ---------------------------------------------*/
        
DllExport ViStatus _VI_FUNC  tkdpo4k_GetChannelName (ViSession vi, ViInt32 index,
                                           ViInt32 bufferSize,
                                           ViChar name[]);

    /*- Interchangeability Checking Functions ------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[]);
DllExport ViStatus _VI_FUNC  tkdpo4k_ResetInterchangeCheck (ViSession vi);
DllExport ViStatus _VI_FUNC  tkdpo4k_ClearInterchangeWarnings (ViSession vi);

    /*- Utility Functions --------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_reset (ViSession vi);
DllExport ViStatus _VI_FUNC  tkdpo4k_ResetWithDefaults (ViSession vi);
DllExport ViStatus _VI_FUNC  tkdpo4k_Disable (ViSession vi);
DllExport ViStatus _VI_FUNC  tkdpo4k_InvalidateAllAttributes (ViSession vi);

DllExport ViStatus _VI_FUNC  tkdpo4k_self_test (ViSession vi, ViInt16 *selfTestResult,
                                       ViChar selfTestMessage[]);
DllExport ViStatus _VI_FUNC  tkdpo4k_revision_query (ViSession vi, 
                                            ViChar instrumentDriverRevision[],
                                            ViChar firmwareRevision[]);

DllExport ViStatus _VI_FUNC  tkdpo4k_viWrite (ViSession vi, ViByte buffer[], ViInt64 count, ViInt64 *returnCount);
DllExport ViStatus _VI_FUNC  tkdpo4k_viRead (ViSession vi, ViInt64 bufferSize, ViByte buffer[], ViInt64 *returnCount);
/* Obsolete Direct IO Functions. */
DllExport ViStatus _VI_FUNC  tkdpo4k_WriteInstrData (ViSession vi, ViConstString writeBuffer); 
DllExport ViStatus _VI_FUNC  tkdpo4k_ReadInstrData  (ViSession vi, ViInt32 numBytes, 
                                            ViChar rdBuf[], ViInt32 *bytesRead);
    
    /*- Utility -------------------------------------------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_IsInvalidWfmElement (ViSession vi,
                                                 ViReal64 elementValue,
                                                 ViBoolean *isInvalid);

    /*- Set, Get, and Check Attribute Functions ----------------------------*/
    
DllExport ViStatus _VI_FUNC  tkdpo4k_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
DllExport ViStatus _VI_FUNC  tkdpo4k_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
DllExport ViStatus _VI_FUNC  tkdpo4k_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]); 
DllExport ViStatus _VI_FUNC  tkdpo4k_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
DllExport ViStatus _VI_FUNC  tkdpo4k_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

DllExport ViStatus _VI_FUNC  tkdpo4k_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
DllExport ViStatus _VI_FUNC  tkdpo4k_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
DllExport ViStatus _VI_FUNC  tkdpo4k_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
DllExport ViStatus _VI_FUNC  tkdpo4k_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
DllExport ViStatus _VI_FUNC  tkdpo4k_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

DllExport ViStatus _VI_FUNC  tkdpo4k_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
DllExport ViStatus _VI_FUNC  tkdpo4k_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
DllExport ViStatus _VI_FUNC  tkdpo4k_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
DllExport ViStatus _VI_FUNC  tkdpo4k_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
DllExport ViStatus _VI_FUNC  tkdpo4k_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

    /*********************************************************
        Functions reserved for class driver use only.
        End-users should not call these functions.  
     *********************************************************/

DllExport ViStatus _VI_FUNC  tkdpo4k_IviInit (ViRsrc resourceName, ViBoolean IDQuery, 
                                     ViBoolean reset, ViSession vi);
DllExport ViStatus _VI_FUNC  tkdpo4k_IviClose (ViSession vi);   
                                         
/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/
#define TKDPO4K_ERROR_MAX_TIME_EXCEEDED               IVISCOPE_ERROR_MAX_TIME_EXCEEDED
#define TKDPO4K_ERROR_NOT_VALID_FOR_MODEL             (IVI_SPECIFIC_ERROR_BASE + 1)
#define TKDPO4K_ERROR_OPTION_NOT_INSTALLED            (IVI_SPECIFIC_ERROR_BASE + 2)
#define TKDPO4K_ERROR_NO_WFMS_AVAILABLE               (IVI_SPECIFIC_ERROR_BASE + 3)  

/****************************************************************************
 *--------- Obsolete Inherent Instrument Attributes and functions ----------*
 ****************************************************************************/
    /*- These attributes and functions have been deprecated and may not
        be supported in future versions of this driver. -*/
 
         /* Driver Information  */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  /* ViInt32,  read-only  */
#define TKDPO4K_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  /* ViInt32,  read-only  */

        /* Error Info */
#define TKDPO4K_ATTR_PRIMARY_ERROR                  IVI_ATTR_PRIMARY_ERROR                  /* ViInt32   */
#define TKDPO4K_ATTR_SECONDARY_ERROR                IVI_ATTR_SECONDARY_ERROR                /* ViInt32   */
#define TKDPO4K_ATTR_ERROR_ELABORATION              IVI_ATTR_ERROR_ELABORATION              /* ViString  */

        /* Advanced Session Information */
#define TKDPO4K_ATTR_IO_SESSION_TYPE                IVI_ATTR_IO_SESSION_TYPE                /* ViString, read-only  */
#define TKDPO4K_ATTR_IO_SESSION                     IVI_ATTR_IO_SESSION                     /* ViSession, read-only */

        /*- Deprecated Attribute Identifiers for Renamed Attributes -*/
#define TKDPO4K_ATTR_NUM_CHANNELS                   TKDPO4K_ATTR_CHANNEL_COUNT             /* ViInt32,  read-only  */
#define TKDPO4K_ATTR_QUERY_INSTR_STATUS             TKDPO4K_ATTR_QUERY_INSTRUMENT_STATUS   /* ViBoolean */
#define TKDPO4K_ATTR_RESOURCE_DESCRIPTOR            TKDPO4K_ATTR_IO_RESOURCE_DESCRIPTOR    /* ViString, read-only  */


        /*- Deprecated Error Information functions -*/
DllExport ViStatus _VI_FUNC  tkdpo4k_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                          ViStatus *secondaryError, 
                                          ViChar errorElaboration[256]);
DllExport ViStatus _VI_FUNC  tkdpo4k_ClearErrorInfo (ViSession vi);

/**************************************************************************** 
 *---------------------------- End Include File ----------------------------* 
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __TKDPO4K_HEADER */
