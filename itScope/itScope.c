/*****************************************************************************
* (c) 2017, Instrumental Systems, Corporation.  All Rights Reserved. *
*****************************************************************************/

/*****************************************************************************
*  ITSCOPE - Digital Oscilloscope Instrument Driver
*  Original Release: March, 2017
*  Based on TKDPO4K Tektronix IVI-C driver
*  By: Do, InSys
*      do.insys@gmail.com
*
*  Modification History:
*
*       March, 2017 - Instrument Driver Created.
*
*****************************************************************************/

#include <string.h>
#include <stdio.h>  
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "brd.h"
#include "extn.h"
#include "utypes.h"
#include "gipcy.h"
#include "ctrladc.h"
#include "ctrlstrm.h"

#include "itScope.h"
#include "pe_scope.h"

#ifdef _CVI
#include <formatio.h>
#include <utility.h>
#else
#ifdef _WIN64
extern int	Scan(void *, const char *, ...);
extern int	Fmt(void *, const char *, ...);
extern int	NumFmtdBytes(void);
#else
#define CVIFUNC         __stdcall
#define CVIFUNC_C       __cdecl
extern int	CVIFUNC_C	Scan(void *, const char *, ...);
extern int	CVIFUNC_C	Fmt(void *, const char *, ...);
extern int	CVIFUNC		NumFmtdBytes(void);
#endif
#endif

static	int	is_error(void)
{
	printf("Err\n");
	return -1;
}

static	int	is_vi_error(void)
{
	printf("Err\n");
	return -1;
}

static	int	is_vi_error_elab(void)
{
	printf("Err\n");
	return -1;
}

static	int	is_vi_error_parm(void)
{
	printf("Err\n");
	return -1;
}

#ifndef _viCheckParm
#define _viCheckParm(fCall, parameterPosition, parameterName) \
                             if (error = (fCall), (error = (error < 0) ? (error) : VI_SUCCESS)) \
                                 {is_vi_error_parm(); \
										Ivi_SetErrorInfo(vi, VI_FALSE, error, Ivi_ParamPositionError(parameterPosition), parameterName); goto Error;}  else error = error
#endif

#ifndef _checkErr
#define _checkErr(fCall)      if (error = (fCall), (error = (error < 0) ? error : VI_SUCCESS)) \
                                 {  is_error(); \
									goto Error;}  else error = error
#endif

#ifndef _viCheckErr
#define _viCheckErr(fCall)    if (error = (fCall), (error = (error < 0) ? error : VI_SUCCESS)) \
						         {  is_vi_error(); \
									Ivi_SetErrorInfo(vi, VI_FALSE, error, 0, VI_NULL); goto Error;}  else error = error
#endif

#ifndef _viCheckErrElab
#define _viCheckErrElab(fCall, elab) \
                             if (error = (fCall), (error = (error < 0) ? error : VI_SUCCESS)) \
                                 {  is_vi_error_elab(); \
									Ivi_SetErrorInfo(vi, VI_FALSE, error, 0, elab); goto Error;}  else error = error
#endif


/*****************************************************************************
 *--------------------- Hidden Attribute Declarations -----------------------*
 *****************************************************************************/
#define ITSCOPE_ATTR_OPC_TIMEOUT            (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 1L)       /* ViInt32   */
#define ITSCOPE_ATTR_INPUT_DATA_SOURCE      (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 2L)       /* ViString  */

    /*- Trigger Type -*/
#define ITSCOPE_ATTR_MAIN_TRIGGER_TYPE      (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 4L)       /* ViInt32   */
#define ITSCOPE_ATTR_EDGE_TRIGGER_CLASS     (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 5L)       /* ViInt32   */
#define ITSCOPE_ATTR_PULSE_TRIGGER_CLASS    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 6L)       /* ViInt32   */
#define ITSCOPE_ATTR_LOGIC_TRIGGER_CLASS    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 7L)       /* ViInt32   */
#define ITSCOPE_ATTR_SERIAL_TRIGGER_CLASS   (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 8L)       /* ViInt32   */

#define ITSCOPE_ATTR_MEAS_VALUE             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 10L)      /* ViReal64  */
#define ITSCOPE_ATTR_PROBE_SENSE            (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 11L)      /* ViBoolean */
#define ITSCOPE_ATTR_PROBE_GAIN             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 24L)      /* ViReal64  */
#define ITSCOPE_ATTR_TOTAL_POINTS           (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 12L)      /* ViInt32   */

    /*- Logic Basic Attributes -*/
#define ITSCOPE_ATTR_LOGIC_FUNCTION         (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 13L)      /* ViInt32   */
#define ITSCOPE_ATTR_LOGIC_WHEN             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 14L)      /* ViInt32   */
#define ITSCOPE_ATTR_LOGIC_INPUT_BY_CH      (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 15L)      /* ViInt32   */

#define ITSCOPE_ATTR_DATA_WIDTH             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 16L)      /* ViInt32   */
#define ITSCOPE_ATTR_DATA_START_POS         (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 17L)      /* ViInt32   */ 
#define ITSCOPE_ATTR_DATA_STOP_POS          (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 18L)      /* ViInt32   */ 
#define ITSCOPE_ATTR_DATA_YZERO             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 19L)      /* ViReal64  */
#define ITSCOPE_ATTR_DATA_YOFF              (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 23L)      /* ViReal64  */
#define ITSCOPE_ATTR_DATA_YMULT             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 20L)      /* ViReal64  */
#define ITSCOPE_ATTR_DATA_XINCR             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 21L)      /* ViReal64  */
#define ITSCOPE_ATTR_DATA_XZERO             (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 22L)      /* ViReal64  */

#define ITSCOPE_ATTR_INSTR_PARALLEL_BIT_SOURCE  (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 30L)  /* ViInt32   */
    
#define ITSCOPE_ATTR_WFM_STRUCTURE          (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 124L)     /* ViAddr    */

/*****************************************************************************
 *---------------------------- Useful Macros --------------------------------*
 *****************************************************************************/

    /*- I/O buffer size -----------------------------------------------------*/
#define BUFFER_SIZE                             512L

#define ITSCOPE_IO_SESSION_TYPE                 IVI_VAL_VISA_SESSION_TYPE
    
#define MAX_TABLE_SIZE                          39
                                                                                                          

    /*- List of channels ----------------------------------------------------*/ 
#define ANALOG_CHANNEL_LIST_1    "CH1"
#define ANALOG_CHANNEL_LIST_2    "CH1,CH2"
#define ANALOG_CHANNEL_LIST_4    "CH1,CH2,CH3,CH4"
#define ANALOG_CHANNEL_LIST_8    "CH1,CH2,CH3,CH4,CH5,CH6,CH7,CH8"
#define VIRTUAL_CHANNEL_LIST_2   "MATH,REF1,REF2"
#define VIRTUAL_CHANNEL_LIST_4   "MATH,REF1,REF2,REF3,REF4"
#define VIRTUAL_CHANNEL_LIST_8   "MATH,REF1,REF2,REF3,REF4,REF5,REF6,REF7,REF8"

    /*- Replicate capabilities ----------------------------------------------*/
#define BITSOURCE_REP_CAP_NAME   "BitSource" 
#define BITSOURCE_LIST           "BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11,BIT12,BIT13,BIT14,BIT15"
    
#define WAVEFORMSLOT_REP_CAP_NAME   "WaveformSlot"
#define WAVEFORMSLOT_LIST           "1,2,3,4"


    /*- Instrument model macros ---------------------------------------------*/
#define IS_PEK1(model) \
    ((model == ITSCOPE_VAL_PE510K1) || (model == ITSCOPE_VAL_PE520K1))
	
#define IS_PEA1(model) \
    ((model == ITSCOPE_VAL_PE510A1) || (model == ITSCOPE_VAL_PE520A1))

    
    /*- Macros for access to enum range table -----------------------------------*/
#define itscope_GetCmdFromIntValue(value, table, cmd) \
        Ivi_GetViInt32EntryFromValue (value, table, VI_NULL, VI_NULL,\
                                      VI_NULL, VI_NULL, cmd, VI_NULL)

#define itscope_GetValueFromCmd(buffer, table, value) \
        Ivi_GetViInt32EntryFromString (buffer, table, value, VI_NULL,\
        VI_NULL, VI_NULL, VI_NULL)

    /*- Macros for the wrapper of empty channel name ---------------------------*/
#define VALID_CHANNELNAME(channelName) \
        (VI_NULL == channelName ? "" : channelName)

    
    /*- Waveform Quantum ----------------------------------------------------*/
#define ITSCOPE_VAL_WAVEFORMS_QUANTUM                     1  
    
    /*- Minimum Waveform Size -----------------------------------------------*/
#define ITSCOPE_VAL_MIN_WAVEFORM_SIZE                     2
    
    /*- Maximum Waveform Size -----------------------------------------------*/
#define ITSCOPE_VAL_MAX_WAVEFORM_SIZE                     131072
    
   
static IviRangeTableEntry attrPEK1AdvVarRangeTableEntries[] =
	{
		{-1.0E15, 1.0E15, 0, "", 0},
		{IVI_RANGE_TABLE_LAST_ENTRY}
	};

static IviRangeTable attrPEK1AdvVarRangeTable =
	{
		IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        "MATHVAR:VAR1",
        attrPEK1AdvVarRangeTableEntries,
	};


    
    /*-Macro for instrument commands -------------------------------------------*/
typedef struct { ViInt32       value;        /* Attribute ID */ 
                 ViConstString string;       /* Command */
                 ViConstString repCommand;   /* Extension for Reapted Command */
} itscopeStringValueEntry;

typedef itscopeStringValueEntry itscopeStringValueTable[];

static IviRangeTablePtr attrMeasFunctionRangeTable = VI_NULL;

/*****************************************************************************
 *--- Typedefs for string/string tables used in trigger source callbacks ----*
 *****************************************************************************/

typedef struct
{
    ViString col1;
    ViString col2;
	ViString col3;
	ViString col4;
	ViString col5;
	ViString col6;
	ViString col7;
	ViString col8;
	ViString col9;
	ViString col10;
} itscopeStringTableEntries;

typedef itscopeStringTableEntries itscopeStringTable[];

/*****************************************************************************
 *------------------------------- Global Data -------------------------------*
 *****************************************************************************/
    
    /*- ITSCOPE_ATTR_MODEL -*/
static IviRangeTableEntry attrModelRangeTableEntries[] =
    {
        {ITSCOPE_VAL_PE510K1, 0, 0, "PE510K1", 500},
		{ITSCOPE_VAL_PE520K1, 0, 0, "PE520K1", 250 },
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
   
static IviRangeTable attrModelRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrModelRangeTableEntries,
    };
    /*- Identify instrument model from IDN return string -*/
static IviStringValueTable modelIdentificationTable =
{
    {ITSCOPE_VAL_PE510K1,      "INSYS,PE510K1" },
	{ITSCOPE_VAL_PE520K1,      "INSYS,PE520K1" },
    { VI_NULL,                  VI_NULL }   
};

    /*- ITSCOPE_ATTR_TRIGGER_SOURCE -*/
static itscopeStringTable triggerSourceStringTable =
{
    { ITSCOPE_VAL_EXTERNAL,    "AUX"    },
    { ITSCOPE_VAL_AC_LINE,     "LINE"   },
	{ ITSCOPE_VAL_CH1,		   "CH1"	},
	{ ITSCOPE_VAL_CH2,		   "CH2"	},
	{ ITSCOPE_VAL_CH3,		   "CH3"	},
	{ ITSCOPE_VAL_CH4,		   "CH4"	},
	{ ITSCOPE_VAL_CH5,		   "CH5"	},
	{ ITSCOPE_VAL_CH6,		   "CH6"	},
	{ ITSCOPE_VAL_CH7,		   "CH7"	},
	{ ITSCOPE_VAL_CH8,		   "CH8"	},
	{ VI_NULL,                 VI_NULL	}
};                               

    /*- ITSCOPE_ATTR_MEAS_SOURCE*/
static itscopeStringTable measurementSourceStringTable = 
{
    { ITSCOPE_VAL_HISTOGRAM,    "HIS"    },
    { VI_NULL,                  VI_NULL }
};
/*****************************************************************************
 *-------------- Utility Function Declarations (Non-Exported) ---------------*
 *****************************************************************************/
static ViStatus itscope_InitAttributes (ViSession vi, ViString idnString, ViInt32 instrModelNo);
static ViStatus itscope_DefaultInstrSetup (ViSession openInstrSession);
static ViStatus itscope_NonInvasiveInstrSetup (ViSession openInstrSession); 
static ViStatus itscope_CheckStatus (ViSession vi);
static ViStatus itscope_WaitForOPC (ViSession vi, ViInt32 maxTime);
static ViStatus itscope_GetStringFromTable( itscopeStringValueTable table, ViInt32 value, ViConstString* string );
static ViStatus itscope_GetRepCommandFromTable( itscopeStringValueTable table, ViInt32 value,ViConstString* repCommand );
static ViStatus itscope_BuildChannelList (ViInt32 model, ViChar channelList[]);  
static ViStatus itscope_GetProbeAttenuation (ViSession vi, 
                                              ViConstString channelName, 
                                              ViReal64 *value);
static ViStatus itscope_ChangeOneAttributeFlag (ViSession vi, 
                                                 ViAttr attributeId, 
                                                 ViInt32 flagBit, 
                                                 ViBoolean flagState);
static ViStatus itscope_SetCachingOnProbeDependentAttributes (ViSession vi, 
                                                               ViBoolean state);
static ViStatus itscope_FetchRawWaveform (ViSession vi, ViSession io, 
                                           ViConstString dataSource,
                                           ViReal64 *yZero,
                                           ViReal64 *yOffset, 
                                           ViReal64 *yMultiplier,
                                           ViReal64 *xIncrement, 
                                           ViReal64 *initialX, 
                                           ViInt32 waveformSize, 
                                           ViReal64 waveform[],
                                           ViInt32 *actualPoints);
static ViStatus itscope_FetchRawMinMaxWaveform (ViSession vi, ViSession io,
                                                 ViConstString dataSource,
                                                 ViReal64 *yZero,
                                                 ViReal64 *yOffset, 
                                                 ViReal64 *yMultiplier,
                                                 ViReal64 *xIncrement, 
                                                 ViReal64 *initialX, 
                                                 ViInt32 waveformSize,
                                                 ViReal64 minWaveform[],
                                                 ViReal64 maxWaveform[],
                                                 ViInt32 *actualPoints);
static ViStatus itscope_FetchContinuousRawWaveform (ViSession vi, ViSession io, 
                                                       ViConstString dataSource,
                                                       ViReal64 *yZero,
                                                       ViReal64 *yOffset, 
                                                       ViReal64 *yMultiplier,
                                                       ViReal64 *xIncrement, 
                                                       ViReal64 *initialX, 
                                                       ViReal64 waveform[],
                                                       ViInt32 *actualPoints);
static ViStatus itscope_FetchContinuousRawMinMaxWaveform (ViSession vi, ViSession io,
                                                             ViConstString dataSource,
                                                             ViReal64 *yZero,
                                                             ViReal64 *yOffset, 
                                                             ViReal64 *yMultiplier,
                                                             ViReal64 *xIncrement, 
                                                             ViReal64 *initialX, 
                                                             ViReal64 minWaveform[],
                                                             ViReal64 maxWaveform[],
                                                             ViInt32 *actualPoints);                                                       
static void itscope_ScaleRawWaveform (ViInt32 waveformSize, ViReal64 waveform[],
                                       ViReal64 yZero, ViReal64 yOffset, ViReal64 yMultiplier);
static ViStatus itscope_CoerceStringSourceToCmd (ViSession vi, ViConstString source, 
                                                  ViConstString *sourceCmdString);
static ViStatus itscope_CoerceToResolution (ViSession vi, ViReal64 value, 
                                             ViReal64 resolution, ViInt32 comparePrecision, 
                                             ViReal64 *coercedValue);
static ViStatus itscope_GetStr1PtrFromStr2 (itscopeStringTable strTable, 
                                             ViConstString *str1, ViConstString str2);
static ViStatus itscope_GetStr2BufFromStr1 (itscopeStringTable strTable, 
                                             ViConstString str1, ViChar str2[]);
static ViStatus itscope_GetStr2PtrFromStr1 (itscopeStringTable strTable, 
                                             ViConstString *str2, ViConstString str1);
static ViStatus itscope_CheckRangeWithPrecision (ViSession vi, ViAttr attributeId, 
                                                  ViReal64 value, ViReal64 min, 
                                                  ViReal64 max);
static ViStatus itscope_GetDriverSetupOption (ViSession vi,
                                               ViConstString optionTag,
                                               ViInt32 optionValueSize,
                                               ViChar optionValue[]);
                                               
static ViStatus _VI_FUNC itscope_CheckStatusCallback (ViSession vi, ViSession io);
static ViStatus _VI_FUNC itscope_WaitForOPCCallback (ViSession vi, ViSession io);
static ViStatus _VI_FUNC itscopeStdViInt32_WriteCallback (ViSession vi, ViSession io, 
                                                           ViConstString channelName, 
                                                           ViAttr attributeId, ViInt32 value);
static ViStatus _VI_FUNC itscopeStdViInt32_ReadCallback (ViSession vi, ViSession io, 
                                                          ViConstString channelName, 
                                                          ViAttr attributeId, ViInt32 *value);

static ViStatus itscope_TestifyPEK1Model(ViSession vi);

/*****************************************************************************
 *----------------- Callback Declarations (Non-Exported) --------------------*
 *****************************************************************************/ 
static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_ReadCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_CheckCallback (ViSession vi, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_CoerceCallback (ViSession vi, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value, 
                                                                      ViReal64 *coercedValue);
static ViStatus _VI_FUNC itscopeAttrHorzMinNumPts_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value);                                                                     
static ViStatus _VI_FUNC itscopeAttrHorzRecordLength_ReadCallback (ViSession vi, 
                                                                    ViSession io, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrHorzSampleRate_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrHorzTimePerRecord_ReadCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrHorzTimePerRecord_WriteCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrHorzTimePerRecord_RangeTableCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           IviRangeTablePtr *rangeTablePtr); 
static ViStatus _VI_FUNC itscopeAttrInitiateContinuous_ReadCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViBoolean *value);
static ViStatus _VI_FUNC itscopeAttrInitiateContinuous_WriteCallback (ViSession vi, 
                                                                       ViSession io, 
                                                                       ViConstString channelName, 
                                                                       ViAttr attributeId, 
                                                                       ViBoolean value);    
    /*- Channel Sub-system --------------------------------------------*/                                                                     
static ViStatus _VI_FUNC itscopeAttrChannelEnabled_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViBoolean *value);
static ViStatus _VI_FUNC itscopeAttrChannelEnabled_WriteCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViBoolean value);                                                                     

static ViStatus _VI_FUNC itscopeAttrMaxInputFrequency_ReadCallback (ViSession vi, 
                                                             ViSession io, 
                                                             ViConstString channelName, 
                                                             ViAttr attributeId, 
                                                             ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrMaxInputFrequency_WriteCallback (ViSession vi, 
                                                              ViSession io, 
                                                              ViConstString channelName, 
                                                              ViAttr attributeId, 
                                                              ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrChannelImpedance_ReadCallback (ViSession vi, 
                                                                    ViSession io, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrChannelImpedance_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrDelayTriggerLevel_CheckCallback (ViSession vi, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrDelayTriggerLevel_ReadCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrDelayTriggerLevel_WriteCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrDelayTriggerMode_CheckCallback (ViSession vi, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViInt32 value);
                                                                     
static ViStatus _VI_FUNC itscopeAttrIdQueryResponse_ReadCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   const ViConstString cacheValue);

static ViStatus _VI_FUNC itscopeAttrMeasureValue_ReadCallback (ViSession vi, 
                                                                ViSession io, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrNumEnvelopes_ReadCallback (ViSession vi, 
                                                                ViSession io,
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViInt32 *value); 
static ViStatus _VI_FUNC itscopeAttrNumEnvelopes_WriteCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrPatternDeltatime_CoerceCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value,
                                                                      ViReal64 *coercedValue);
static ViStatus _VI_FUNC itscopeAttrProbeAttenuation_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrProbeSenseValue_ReadCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrRuntHighThreshold_CheckCallback (ViSession vi, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrRuntHighThreshold_ReadCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrRuntHighThreshold_WriteCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrRuntLowThreshold_CheckCallback (ViSession vi, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrRuntLowThreshold_ReadCallback (ViSession vi, 
                                                                    ViSession io, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrRuntLowThreshold_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrRuntWidth_CoerceCallback (ViSession vi, 
                                                               ViConstString channelName, 
                                                               ViAttr attributeId, 
                                                               ViReal64 value, 
                                                               ViReal64 *coercedValue);
static ViStatus _VI_FUNC itscopeAttrTransitionDeltaTime_CoerceCallback (ViSession vi,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 value,
                                                                         ViReal64 *coercedValue);
static ViStatus _VI_FUNC itscopeAttrTransitionHighThreshold_CheckCallback (ViSession vi,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTransitionHighThreshold_ReadCallback (ViSession vi,
                                                                           ViSession io,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrTransitionHighThreshold_WriteCallback (ViSession vi,
                                                                            ViSession io,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTransitionLowThreshold_CheckCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTransitionLowThreshold_ReadCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrTransitionLowThreshold_WriteCallback (ViSession vi,
                                                                           ViSession io,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTriggerHoldoff_CoerceCallback (ViSession vi, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViReal64 value, 
                                                                    ViReal64 *coercedValue);
static ViStatus _VI_FUNC itscopeAttrTriggerLevel_CheckCallback (ViSession vi, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTriggerLevel_ReadCallback (ViSession vi, 
                                                                ViSession io, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrTriggerLevel_WriteCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTriggerSource_CheckCallback (ViSession vi, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value);
static ViStatus _VI_FUNC itscopeAttrTriggerSource_ReadCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 const ViConstString cacheValue);
static ViStatus _VI_FUNC itscopeAttrTriggerSource_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value);
static ViStatus _VI_FUNC itscopeAttrTriggerType_CheckCallback (ViSession vi, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTriggerType_RangeTableCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    IviRangeTablePtr *rangeTablePtr);
static ViStatus _VI_FUNC itscopeAttrTriggerType_ReadCallback (ViSession vi, 
                                                               ViSession io, 
                                                               ViConstString channelName, 
                                                               ViAttr attributeId, 
                                                               ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrTriggerType_WriteCallback (ViSession vi, 
                                                                ViSession io, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerEvent_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerEvent_WriteCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerLineNumber_CheckCallback (ViSession vi, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerLineNumber_ReadCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerLineNumber_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerSignalFormat_ReadCallback (ViSession vi, 
                                                                       ViSession io, 
                                                                       ViConstString channelName, 
                                                                       ViAttr attributeId, 
                                                                       ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerSignalFormat_WriteCallback (ViSession vi, 
                                                                        ViSession io, 
                                                                        ViConstString channelName, 
                                                                        ViAttr attributeId, 
                                                                        ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerHdtvFormat_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrTvTriggerHdtvFormat_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerFieldHoldoff_ReadCallback (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 *value);

static ViStatus _VI_FUNC itscopeAttrTvTriggerFieldHoldoff_WriteCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomFormat_ReadCallback (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomFormat_WriteCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomScanRate_ReadCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomScanRate_WriteCallback (ViSession vi,
                                                                           ViSession io,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrVerticalCoupling_ReadCallback (ViSession vi, 
                                                                    ViSession io, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrVerticalCoupling_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViInt32 value);
static ViStatus _VI_FUNC itscopeAttrVerticalOffset_CheckCallback (ViSession vi, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrVerticalOffset_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrVerticalOffset_WriteCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViReal64 value);
                                                                   
static ViStatus _VI_FUNC itscopeAttrVerticalRange_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrVerticalRange_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue);  
static ViStatus _VI_FUNC itscopeAttrProbeGain_ReadCallback (ViSession vi, 
                                                            ViSession io, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrProbeGain_WriteCallback (ViSession vi, 
                                                             ViSession io, 
                                                             ViConstString channelName, 
                                                             ViAttr attributeId, 
                                                             ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrVerticalRange_ReadCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrVerticalRange_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViReal64 value);
                                                                  
static ViStatus _VI_FUNC itscopeResolutionOne_CoerceCallback (ViSession vi, 
                                                               ViConstString channelName, 
                                                               ViAttr attributeId, 
                                                               ViReal64 value, 
                                                               ViReal64 *coercedValue);
static ViStatus _VI_FUNC itscopeStdViReal64_ReadCallback (ViSession vi, 
                                                           ViSession io, 
                                                           ViConstString channelName, 
                                                           ViAttr attributeId, 
                                                           ViReal64 *value);
static ViStatus _VI_FUNC itscopeStdViReal64_WriteCallback (ViSession vi, 
                                                            ViSession io, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViReal64 value);
static ViStatus _VI_FUNC itscopeStdViStringSource_CheckCallback (ViSession vi, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value);
static ViStatus _VI_FUNC itscopeStdViStringSource_ReadCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 const ViConstString cacheValue);
static ViStatus _VI_FUNC itscopeStdViStringSource_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value);
                                                                
static ViStatus _VI_FUNC itscopeAttrProbeAttenuation_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrMaxInputFrequency_RangeTableCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   IviRangeTablePtr *rangeTablePtr);
static ViStatus _VI_FUNC itscopeAttrGlitchWidth_CoerceCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value,
                                                           ViReal64 *coercedValue);

static ViStatus _VI_FUNC itscopeAttrStateWhen_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrStateWhen_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrTotalPoints_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrTotalPoints_WriteCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrAcLineTriggerSlope_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrAcLineTriggerSlope_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrEdgeTriggerClass_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrEdgeTriggerClass_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrLogicTriggerClass_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrLogicTriggerClass_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrStateClockSource_RangeTableCallback (ViSession vi,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrStateClockSource_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value);
static ViStatus _VI_FUNC itscopeAttrStateClockSource_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrTriggerHoldoff_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrTriggerHoldoff_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrTriggerLevelByCh_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrTriggerLevelByCh_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 value);
static ViStatus _VI_FUNC itscopeAttrTriggerLevelByCh_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 value);
                                                                     
static ViStatus _VI_FUNC itscopeAttrTriggerUpperthresByCh_ReadCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrTriggerUpperthresByCh_WriteCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViReal64 value);
                                                                          
static ViStatus _VI_FUNC itscopeAttrTriggerLowerthresByCh_ReadCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 *value);
static ViStatus _VI_FUNC itscopeAttrTriggerLowerthresByCh_WriteCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrTriggerThresholdByCh_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrLogicInputByCh_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrLogicInputByCh_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrLogicThresByCh_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC itscopeAttrLogicThresByCh_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrLogicThresByCh_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrPatternFunction_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrPatternFunction_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrPatternWhen_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrPatternWhen_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrPatternInput_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrPatternInput_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrStateFunction_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrStateFunction_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrStateInput_ReadCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrStateInput_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_RangeTableCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrSetholdHoldtime_CoerceCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value,
                                                                    ViReal64 *coercedValue);

static ViStatus _VI_FUNC itscopeAttrSetholdSettime_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue);



static ViStatus _VI_FUNC itscopeAttrMathFunction_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrMathFunction_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrAdvDefine_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            const ViConstString cacheValue);

static ViStatus _VI_FUNC itscopeAttrAdvDefine_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViConstString value);

static ViStatus _VI_FUNC itscopeAttrDelayTriggerTime_CoerceCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 value,
                                                                     ViReal64 *coercedValue);

static ViStatus _VI_FUNC itscopeAttrDelayTriggerSource_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     const ViConstString cacheValue);

static ViStatus _VI_FUNC itscopeAttrDelayTriggerSource_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViConstString value);

static ViStatus _VI_FUNC itscopeAttrDelayTriggerSource_CheckCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViConstString value);

static ViStatus _VI_FUNC itscopeAttrDataStartPos_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrDataStartPos_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrDataStopPos_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrDataStopPos_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrDataWidth_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViInt32 *value);

static ViStatus _VI_FUNC itscopeAttrDataWidth_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrDataYZero_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);
                                                              
static ViStatus _VI_FUNC itscopeAttrDataYoff_ReadCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 *value);

                                                            
static ViStatus _VI_FUNC itscopeAttrDataYmult_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value);

static ViStatus _VI_FUNC itscopeAttrDataXincr_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value);

static ViStatus _VI_FUNC itscopeAttrDataXzero_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value);

static ViStatus _VI_FUNC itscopeAttrSetholdHoldtime_RangeTableCallback (ViSession vi,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrSetholdSettime_RangeTableCallback (ViSession vi,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrHorzMinNumPts_RangeTableCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrTotalPoints_RangeTableCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrInputImpedance_RangeTableCallback (ViSession vi,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrTriggerCoupling_RangeTableCallback (ViSession vi,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrDelayTriggerCoupling_RangeTableCallback (ViSession vi,
                                                                             ViConstString channelName,
                                                                             ViAttr attributeId,
                                                                             IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrTvTriggerHdtvFormat_RangeTableCallback (ViSession vi,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrTvTriggerSignalFormat_RangeTableCallback (ViSession vi,
                                                                              ViConstString channelName,
                                                                              ViAttr attributeId,
                                                                              IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomLinePeriod_ReadCallback (ViSession vi,
                                                                            ViSession io,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            ViReal64 *value);

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomLinePeriod_WriteCallback (ViSession vi,
                                                                             ViSession io,
                                                                             ViConstString channelName,
                                                                             ViAttr attributeId,
                                                                             ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomSyncInterval_ReadCallback (ViSession vi,
                                                                              ViSession io,
                                                                              ViConstString channelName,
                                                                              ViAttr attributeId,
                                                                              ViReal64 *value);

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomSyncInterval_WriteCallback (ViSession vi,
                                                                               ViSession io,
                                                                               ViConstString channelName,
                                                                               ViAttr attributeId,
                                                                               ViReal64 value);

static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_RangeTableCallback (ViSession vi,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_CheckCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 value);

static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                    ViInt32 value);

                                                                             
 /*-------------------- STD callbacks -------------------------------------------------*/
static ViStatus _VI_FUNC itscopeAttrAdvVar1_RangeTableCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC itscopeAttrAdvVar2_RangeTableCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                IviRangeTablePtr *rangeTablePtr);



/*****************************************************************************
 *------------ User-Callable Functions (Exportable Functions) ---------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: itscope_init   
 * Purpose:  VXIplug&play required function.  Calls the   
 *           itscope_InitWithOptions function.   
 *****************************************************************************/
ViStatus _VI_FUNC itscope_init (ViRsrc resourceName, ViBoolean IDQuery, 
                                 ViBoolean resetDevice, ViSession *newVi)
{   
    ViStatus    error = VI_SUCCESS;

    if (newVi == VI_NULL)
    {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER, 
                          VI_ERROR_PARAMETER4, "Null address for Instrument Handle");
       _checkErr( IVI_ERROR_INVALID_PARAMETER);
    }
   _checkErr( itscope_InitWithOptions (resourceName, IDQuery, resetDevice,   
                                        "", newVi));
Error:  
    return error;
}

/*****************************************************************************
 * Function: itscope_InitWithOptions                                       
 * Purpose:  This function creates a new IVI session and calls the 
 *           IviInit function.                                     
 *****************************************************************************/
ViStatus _VI_FUNC itscope_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery, 
                                            ViBoolean resetDevice, ViConstString optionString, 
                                            ViSession *newVi)
{   
    ViStatus    error = VI_SUCCESS;
    ViSession   vi = VI_NULL;
    ViChar      newResourceName[IVI_MAX_MESSAGE_BUF_SIZE];
    ViChar      newOptionString[IVI_MAX_MESSAGE_BUF_SIZE];
    ViBoolean   isLogicalName;
   
    if (newVi == VI_NULL)
    {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER, 
                          VI_ERROR_PARAMETER5, "Null address for Instrument Handle");
       _checkErr( IVI_ERROR_INVALID_PARAMETER);
    }

    *newVi = VI_NULL;
    
   _checkErr( Ivi_GetInfoFromResourceName (resourceName, (ViString)optionString, newResourceName,
                                           newOptionString, &isLogicalName));
    
        /* create a new interchangeable driver */
   _checkErr( Ivi_SpecificDriverNew ("itscope", newOptionString, &vi));  
    if (!isLogicalName)
    {
        ViInt32 oldFlag = 0;
        
       _checkErr (Ivi_GetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, &oldFlag));
       _checkErr (Ivi_SetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, oldFlag & 0xfffb | 0x0010));
       _checkErr (Ivi_SetAttributeViString (vi, "", IVI_ATTR_IO_RESOURCE_DESCRIPTOR, 0, newResourceName));
    }
        /* init the driver */
   _checkErr( itscope_IviInit (newResourceName, IDQuery, resetDevice, vi)); 
    
    if (isLogicalName)
    {
       _checkErr( Ivi_ApplyDefaultSetup (vi));
    }
    *newVi = vi;
Error:
    if (error < VI_SUCCESS) 
    {
        Ivi_Dispose (vi);
    }
        
    return error;
}


/*****************************************************************************
 * Function: itscope_IviInit                                                       
 * Purpose:  This function is called by itscope_InitWithOptions  
 *           or by an IVI class driver.  This function initializes the I/O 
 *           interface, optionally resets the device, optionally performs an
 *           ID query, and sends a default setup to the instrument.                
 *****************************************************************************/
ViStatus _VI_FUNC itscope_IviInit (ViRsrc resourceName, ViBoolean IDQuery, 
                                    ViBoolean reset, ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    ViSession   io = VI_NULL;
    ViInt32     modelNumber = ITSCOPE_VAL_PE510K1;
    ViChar      idnString[256] = "INSYS,PE510K1";	// default
    ViChar      modelStr[BUFFER_SIZE] = "";
    ViChar      channelList[BUFFER_SIZE] = {0};
    ViChar      setup[BUFFER_SIZE] = ""; 
    ViInt32     modelStrLen = 1;
    ViChar      nonInvasiveInitStr[BUFFER_SIZE] = "";     
    ViBoolean   nonInvasiveInit = VI_FALSE;

	_checkErr( itscope_GetDriverSetupOption (vi, "Model", 20, modelStr));
	
	if (!Ivi_Simulating(vi))
	{	// Hardware Initialization
		_checkErr(scpi_hwInit(vi,resourceName));
	}

    /* Identification Query */
    if (IDQuery)                               
    {
        ViBoolean idQueryResult = VI_FALSE;
        ViChar corp[BUFFER_SIZE] = {0};
        ViChar idnModelStr[BUFFER_SIZE] = {0};
        ViInt32 index;

		if (!Ivi_Simulating(vi)) 
			//_viCheckErr( viQueryf (io, "*IDN?", "%256[^\n]", idnString));
			_viCheckErr(_viQueryf(vi, "*IDN?", "%256[^\n]", idnString));

		sscanf (idnString, "%[^,]", corp);
        index = sprintf(idnModelStr, "%s,", corp);              
        sscanf (idnString, "%*[^,],%[^,]", idnModelStr+index);

        error = Ivi_GetValueFromTable(modelIdentificationTable, idnModelStr, &modelNumber);
        
        if (error < VI_SUCCESS)
        {
            error = VI_ERROR_FAIL_ID_QUERY;
            _viCheckErrElab (error, "Failed to Recognize Model"); 
        }
        
        idQueryResult = VI_TRUE; 
    } 
    else
    {
        /* Parse ID query string for model number only if the */
        /* user did not specify the model via option string   */
        if (!*modelStr)
            sscanf (idnString, "%*[^,],%[^,]", modelStr);
        
        modelStrLen = (ViInt32)strlen(modelStr)-1;
    
        if (modelStr[modelStrLen] == 'B') {
            modelStr[modelStrLen]= '\0';
        }
        error = Ivi_GetViInt32EntryFromString (modelStr, &attrModelRangeTable, &modelNumber, 
                                           VI_NULL, VI_NULL, VI_NULL, VI_NULL);
        if (error < VI_SUCCESS)
            _viCheckErrElab( VI_ERROR_FAIL_ID_QUERY, "Failed to Recognize Model");
    }
        /* Building channel table */
   _checkErr( itscope_BuildChannelList (modelNumber, channelList) );   

   _checkErr( Ivi_BuildRepCapTable (vi, BITSOURCE_REP_CAP_NAME, BITSOURCE_LIST));
   _checkErr( Ivi_BuildRepCapTable (vi, WAVEFORMSLOT_REP_CAP_NAME, WAVEFORMSLOT_LIST));
    
   _checkErr( Ivi_BuildChannelTable (vi, channelList, VI_FALSE, VI_NULL));
    
    /* Check NonInvasiveInit */
   _checkErr( Ivi_GetAttributeViString (vi, "", IVI_ATTR_DRIVER_SETUP, 0, BUFFER_SIZE, setup));
    if(strstr (setup, "NonInvasiveInit"))
    {
       _checkErr( itscope_GetDriverSetupOption (vi, "NonInvasiveInit", 20, nonInvasiveInitStr));            
        if(strncmp (nonInvasiveInitStr, "TRUE", strlen("FALSE")) == 0)
        {
            nonInvasiveInit = VI_TRUE;      
        }
        else if(strncmp (nonInvasiveInitStr, "FALSE", strlen("FALSE")) == 0)
        {
            nonInvasiveInit = VI_FALSE;         
        }
        else
        {
           _viCheckErr( IVI_ERROR_INVALID_CONFIGURATION );      
        }
    }
    
    /* Add attributes */
   _checkErr (itscope_InitAttributes (vi, idnString, modelNumber));
    
    /* Reset instrument */
    if (reset) 
    {
       _checkErr (itscope_reset (vi));
    }
    else  /* Send Default Instrument Setup */
    {   
        if(nonInvasiveInit == VI_FALSE)
        {
           _checkErr (itscope_DefaultInstrSetup (vi));
        }
        else
        {
           _checkErr (itscope_NonInvasiveInstrSetup (vi));
        }
    }

   _checkErr (itscope_CheckStatus (vi));

Error:
    if (error < VI_SUCCESS)
    {
        if (!Ivi_Simulating (vi) && io)
        {
            //viClose (io);
			_viClose(vi);
        }
    }
    return error;
}

/*****************************************************************************
 * Function: itscope_close                                                           
 * Purpose:  This function closes the instrument.                            
 *
 *           Note:  This function must unlock the session before calling
 *           Ivi_Dispose.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_close(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	_checkErr( pe_close(vi));

   _checkErr( Ivi_LockSession (vi, VI_NULL));

   _checkErr (itscope_IviClose (vi));

Error:    
    Ivi_UnlockSession (vi, VI_NULL);
    Ivi_Dispose (vi);  
    return error;
}

/*****************************************************************************
 * Function: itscope_IviClose                                                        
 * Purpose:  This function performs all of the drivers clean-up operations   
 *           except for closing the IVI session.  This function is called by 
 *           itscope_close or by an IVI class driver. 
 *
 *           Note:  This function must close the I/O session and set 
 *           IVI_ATTR_IO_SESSION to 0.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_IviClose (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    //ViSession   io = VI_NULL;

        /* Do not lock here.  Either the class or specific driver will manage the lock. */
	/*
	_checkErr( Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, &io));

Error:
    Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, VI_NULL);
    if(io)
    {
        viClose (io);
    }
	*/
    return error;   
}

/*****************************************************************************
 * Function: itscope_reset                                                         
 * Purpose:  This function resets the instrument.                          
 *****************************************************************************/
ViStatus _VI_FUNC itscope_reset (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

	if (!Ivi_Simulating(vi))                /* call only when locked */
	{
		ViSession   io = Ivi_IOSession(vi); /* call only when locked */
		_checkErr(Ivi_SetNeedToCheckStatus(vi, VI_TRUE));
		//_viCheckErr(viPrintf(io, "*RST"));
		_viCheckErr(_viPrintf(vi, "*RST"));
	}

	_checkErr (itscope_DefaultInstrSetup (vi)); 
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: itscope_ResetWithDefaults
 * Purpose:  This function resets the instrument and applies default settings
 *           from the IVI Configuration Store based on the logical name
 *           from which the session was created.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ResetWithDefaults (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));
   _checkErr( itscope_reset(vi));
   _checkErr( Ivi_ApplyDefaultSetup(vi));

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/**************************************************************************** 
 *  Function: itscope_Disable
 *  Purpose:  This function places the instrument in a quiescent state as 
 *            quickly as possible.
 ****************************************************************************/
ViStatus _VI_FUNC itscope_Disable (ViSession vi)
{
    return VI_SUCCESS;
}

/*****************************************************************************
 * Function: itscope_self_test                                                       
 * Purpose:  This function executes the instrument self-test and returns the 
 *           result.                                                         
 *****************************************************************************/
ViStatus _VI_FUNC itscope_self_test (ViSession vi, ViInt16 *testResult, ViChar testMessage[])
{
    ViStatus    error = VI_SUCCESS;
    ViInt16     result = 0;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (testResult == VI_NULL)
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Test Result");
    if (testMessage == VI_NULL)
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Test Message");

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        //ViSession   io = Ivi_IOSession(vi); /* call only when locked */
    
        if (testResult && testMessage)
            {
                /* The *TST? command tests the GPIB interface, and always returns 0 */ 
            *testMessage = '\0';
            //_viCheckErr( viQueryf (io, "*TST?", "%hd", &result));
			_viCheckErr(_viQueryf(vi, "*TST?", "%hd", &result));

            if (result == 0)
                sprintf (testMessage, "Passed Self Test");
            else
                sprintf (testMessage, "Failed Self Test.");
            }
        }
    else
        {
            sprintf (testMessage, "Passed Self Test.");
        }
    
   _checkErr (itscope_CheckStatus (vi));

Error:
    *testResult = result;
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: itscope_error_query                                                     
 * Purpose:  This function queries the instrument error queue and returns   
 *           the result.                                                     
 *****************************************************************************/
ViStatus _VI_FUNC itscope_error_query (ViSession vi, ViInt32 *errCode, ViChar errMessage[])
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (errCode == VI_NULL)
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error Code");
    if (errMessage == VI_NULL)
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

    if (!Ivi_Simulating(vi))                /* call only when locked */
        {
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */
    
        if (errCode && errMessage)
            {
            ViInt32 tmpErr = 0;
            
            /*check to see if there is an error waiting*/
            //_viCheckErr( viQueryf(io, ":EVM?", "%ld, \"%[^\"]", errCode, errMessage)); 
			_viCheckErr(_viQueryf(vi, ":EVM?", "%ld, \"%[^\"]", errCode, errMessage)); //Returns event code, message from the event queue
            if (*errCode == 1)   /*if an error waiting, then must do a *ESR? to get to the queue*/
                {
                    /*allows to get the next error message from queue*/
                    //_viCheckErr( viQueryf(io, "*ESR?", "%ld", &tmpErr));  
					_viCheckErr(_viQueryf(vi, "*ESR?", "%ld", &tmpErr)); // event status register
                    //_viCheckErr( viQueryf(io, ":EVM?", "%ld, \"%[^\"]", errCode, errMessage));
					_viCheckErr(_viQueryf(vi, ":EVM?", "%ld, \"%[^\"]", errCode, errMessage));
                }
            }
        }
    else
        {
                /* Simulate Error Query */
            if (errCode)
                *errCode = 0;
            if (errMessage)
                sprintf (errMessage, "%s", "No Error");
        }
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: itscope_error_message                                                  
 * Purpose:  This function translates the error codes returned by this       
 *           instrument driver into user-readable strings.  
 *
 *           Note:  The caller can pass VI_NULL for the vi parameter.  This 
 *           is useful if one of the init functions fail.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_error_message (ViSession vi, ViStatus errorCode, 
                                          ViChar errorMessage[256])
{
    ViStatus    error = VI_SUCCESS;
    
    static      IviStringValueTable errorTable = 
        {
            {ITSCOPE_ERROR_NOT_VALID_FOR_MODEL,  "The operation or attribute is not valid for this model."}, 
            {ITSCOPE_ERROR_OPTION_NOT_INSTALLED, "The option required is not installed."}, 
            IVISCOPE_ERROR_CODES_AND_MSGS, 
            {ITSCOPE_ERROR_NO_WFMS_AVAILABLE,    "No free waveform structure to create new waveform." }, 
            {VI_NULL,                               VI_NULL}
        };
        
    if (vi)
        Ivi_LockSession(vi, VI_NULL);

        /* all VISA and IVI error codes are handled as well as codes in the table */
    if (errorMessage == VI_NULL)
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

   _checkErr( Ivi_GetSpecificDriverStatusDesc(vi, errorCode, errorMessage, errorTable));

Error:
    if (vi)
        Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: itscope_revision_query                                                  
 * Purpose:  This function returns the driver and instrument revisions.      
 *****************************************************************************/
ViStatus _VI_FUNC itscope_revision_query (ViSession vi, ViChar driverRev[], 
                                           ViChar instrRev[])
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (driverRev == VI_NULL)
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Driver Revision");
    if (instrRev == VI_NULL)
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Instrument Revision");

   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_SPECIFIC_DRIVER_REVISION, 
                                        0, 256, driverRev));
   _checkErr( Ivi_GetAttributeViString (vi, "", ITSCOPE_ATTR_INSTRUMENT_FIRMWARE_REVISION, 
                                        0, 256, instrRev));
   _checkErr( itscope_CheckStatus (vi));

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: itscope_InvalidateAllAttributes
 * Purpose:  This function invalidates the cached value of all attributes.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_InvalidateAllAttributes (ViSession vi)
{
    return Ivi_InvalidateAllAttributes(vi);
}

/**************************************************************************** 
 *  Function: itscope_GetChannelName
 *  Purpose:  This function returns the highest-level channel name that
 *            corresponds to the specific driver channel string that is in
 *            the channel table at an index you specify.
 ****************************************************************************/
ViStatus _VI_FUNC itscope_GetChannelName (ViSession vi, ViInt32 index,
                                           ViInt32 bufferSize,
                                           ViChar name[])
{
   ViStatus         error = VI_SUCCESS;
   ViConstString    channelName;
   ViInt32          actualSize = 0;

   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (bufferSize != 0)
    {
        if (name == VI_NULL)
        {
            _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Channel Name");
        }
    }
    
    /* Get channel name */
    _viCheckParm( Ivi_GetNthChannelString (vi, index, &channelName), 2, "Index");
    
    actualSize = (ViInt32)strlen(channelName) + 1;
    
    /* Prepare return values */
    if (bufferSize == 0)
    {
        error = actualSize;
    }
    else 
    {
        if (bufferSize > 0)
        {
            if (actualSize > bufferSize)
            {
                error = actualSize;
                actualSize = bufferSize;
            }
        }
        memcpy(name, channelName, actualSize-1);
        name[actualSize-1] = '\0';
    }

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureChannel
 *  Purpose:  This function configures the most common attributes of 
 *            the oscilloscope vertical subsystem.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureChannel (ViSession vi, 
                                              ViConstString channel, 
                                              ViReal64 range, 
                                              ViReal64 offset, 
                                              ViInt32 coupling, 
                                              ViReal64 probeAttenuation, 
                                              ViBoolean enabled)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViBoolean (vi, channel, ITSCOPE_ATTR_CHANNEL_ENABLED, 0, 
                                            enabled), 7, "Enabled");  
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_PROBE_ATTENUATION, 0, 
                                           probeAttenuation), 6, "Probe Attenuation");                                           
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, channel, ITSCOPE_ATTR_VERTICAL_COUPLING, 0, 
                                          coupling), 5, "Coupling");
        /* Set this after Probe Attenuation */
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 0, 
                                           range*probeAttenuation), 3, "Range");
        /* Set this after Probe Attenuation and Vertical Range */
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_OFFSET, 0, 
                                           offset), 4, "Offset");
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 *  Function: itscope_ConfigureChanCharacteristics
 *  Purpose:  This function configures the less common attributes of 
 *            the oscilloscope channel subsystem.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureChanCharacteristics (ViSession vi, 
                                                         ViConstString channel, 
                                                         ViReal64 inputImpedance, 
                                                         ViReal64 maxInputFrequency)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_INPUT_IMPEDANCE, 
                                           0, inputImpedance), 3, "Input Impedance");

    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_MAX_INPUT_FREQUENCY, 
                                           0, maxInputFrequency), 4, "Maximum Input Frequency");
    
   _checkErr (itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_AutoProbeSenseValue
 *  Purpose:  This function returns the actual probe attenuation when you
 *            configure the oscilloscope to sense the probe attenuation.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_AutoProbeSenseValue (ViSession vi, 
                                                ViConstString channel, 
                                                ViReal64 *autoProbeSenseValue)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (autoProbeSenseValue == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, 
                     "Null address for Auto Probe Sense Value");
    }
    
   _checkErr( Ivi_GetAttributeViReal64 (vi, channel, 
                                        ITSCOPE_ATTR_PROBE_SENSE_VALUE, 
                                        0, autoProbeSenseValue));
    
   _checkErr (itscope_CheckStatus (vi));
                                        
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 *  Function: itscope_ConfigureAcquisitionRecord
 *  Purpose:  This function configures the most common attributes of
 *            the oscilloscope acquisition subsystem.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureAcquisitionRecord (ViSession vi, 
                                                ViReal64 timePerRecord, 
                                                ViInt32 minNumPts, 
                                                ViReal64 acquisitionStartTime)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS, 
                                          0, minNumPts), 3, "Minimum Record Length");

        /* Set this after Horz Min Num Pts */
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                           0, timePerRecord), 2, "Time Per Record");
    
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_ACQUISITION_START_TIME, 
                                           0, acquisitionStartTime), 4, "Acquisition Start Time");

   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ActualRecordLength
 *  Purpose:  This function returns the actual waveform record length the 
 *            oscilloscope acquires.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ActualRecordLength (ViSession vi, 
                                               ViInt32 *actualRecordLength)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (actualRecordLength == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, 
                     "Null address for Actual Record Length");
    }
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                       0, actualRecordLength));
    
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureTrigger
 *  Purpose:  This function configures common triggering attributes such
 *            as trigger type and holdoff.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTrigger (ViSession vi, 
                                ViInt32 triggerType, 
                                ViReal64 holdoff)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 
                                          0, triggerType), 2, "Trigger Type");

        /* Invalidated by Trigger Type */
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_HOLDOFF, 
                                           0, holdoff), 3, "Holdoff");

   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureTriggerCoupling
 *  Purpose:  This function configures trigger coupling.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTriggerCoupling (ViSession vi,
                                    ViInt32 coupling)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, 
                                ITSCOPE_ATTR_TRIGGER_COUPLING, 
                                0, coupling), 
                                2, "Trigger Coupling");
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureTriggerLevel
 *  Purpose:  This function configures trigger level of a specified channel
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTriggerLevel (ViSession vi,
                                                    ViConstString channel,
                                                    ViReal64 level)
{
    ViStatus    error = VI_SUCCESS; 
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));     

    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel,
                                            ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH,
                                            0, level), 3, "Level");
   _checkErr( itscope_CheckStatus(vi) );                                            

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureTriggerThreshold
 *  Purpose:  This function configures trigger threshold of a specified channel
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTriggerThreshold(ViSession vi,
                                                        ViConstString channel,
                                                        ViReal64 upperThreshold,
                                                        ViReal64 lowerThreshold)                                                    
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL) );
    
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH,
                                            0, upperThreshold), 3, "Upper Threshold");
                                            
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH,
                                            0, lowerThreshold), 4, "Lower Threshold");
                                            
   _checkErr( itscope_CheckStatus(vi) );
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureEdgeTriggerSource
 *  Purpose:  This function configures the edge triggering attributes.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureEdgeTriggerSource (ViSession vi, 
                                                      ViConstString source,
                                                      ViReal64 level,   
                                                      ViInt32 slope)
{
    ViStatus  error = VI_SUCCESS;
    ViReal64  triggerLevel = level;
    ViInt32   modelNo;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

   _checkErr(Ivi_GetAttributeViInt32(vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &modelNo));
    
   
    /* 
        If the trigger source is AC Line, set the trigger level to 
        the zero crossing. /* This is to keep the behavior of this 
        function compliant with ivi.
    */
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                0, source), 2, "Trigger Source");

    if (strcmp (source, ITSCOPE_VAL_AC_LINE) == 0)
        triggerLevel = 0.0;
        
        /* Invalidated by Trigger Source */
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_LEVEL, 0, 
                                           triggerLevel), 3, "Level");
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SLOPE, 0, 
                                          slope), 4, "Slope");
    
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureDelayTriggerSource
 *  Purpose:  This function configures the scope for delay triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureDelayTriggerSource ( ViSession vi,
                                                        ViConstString source,
                                                        ViInt32 coupling,
                                                        ViReal64 level,
                                                        ViInt32 slope)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_DELAY_TRIGGER_SOURCE,
                                            0, source), 2, "B Trigger Source" );

    if (strcmp (source, ITSCOPE_VAL_AC_LINE) == 0)
        level = 0.0;
        
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DELAY_TRIGGER_COUPLING,
                                            0, coupling), 3, "B Trigger Coupling");
                                            
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DELAY_TRIGGER_LEVEL,
                                            0, level), 4, "B Trigger Level");
                                            
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DELAY_TRIGGER_SLOPE,
                                            0, slope), 5, "B Trigger Slope");
                                            
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;                                           
}

/*****************************************************************************
 *  Function: itscope_ConfigureDelayTriggerMode
 *  Purpose:  This function configures the delay trigger mode for delay 
 *            triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureDelayTriggerMode (ViSession vi,
                                                     ViInt32 mode)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DELAY_TRIGGER_MODE,
                                            0, mode), 2, "B Trigger Mode");
    
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;    
}

/******************************************************************************
 *  Function: itscope_ConfigureDelayTriggerEvents
 *  Purpose:  This function configures the trigger events for delay triggering.
 ******************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureDelayTriggerEvents (ViSession vi,
                                                       ViInt32 events)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DELAY_TRIGGER_EVENTS,
                                            0, events), 2, "B Trigger Events");

   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureDelayTriggerTime
 *  Purpose:  This function configures the trigger time for delay triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureDelayTriggerTime (ViSession vi,
                                                     ViReal64 time)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViReal64( vi, VI_NULL, ITSCOPE_ATTR_DELAY_TRIGGER_TIME,
                                            0, time), 2, "B Trigger Time");

   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureGlitchTriggerSource
 *  Purpose:  This function configures the scope for glitch triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureGlitchTriggerSource (ViSession vi,
                                                        ViConstString source,
                                                        ViReal64 level,  
                                                        ViReal64 glitchWidth, 
                                                        ViInt32 glitchPolarity,
                                                        ViInt32 glitchCondition)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

        /* ATTR_TRIGGER_SOURCE invalidates ATTR_TRIGGER_LEVEL and must be set first */
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                   0, source), 2, "Trigger Source");

    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_TRIGGER_LEVEL, 
                                           0, level), 
                                           3, "Trigger Level");
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_GLITCH_WIDTH, 
                                           0, glitchWidth), 
                                           4, "Glitch Width");
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, 
                                            ITSCOPE_ATTR_GLITCH_POLARITY, 
                                            0, glitchPolarity), 
                                            5, "Glitch Trigger Polarity");
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_GLITCH_CONDITION,
                                            0, glitchCondition), 
                                            6, "Glitch Condition");

   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureTVTriggerSource
 *  Purpose:  This function configures the scope for TV triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTVTriggerSource (ViSession vi,
                                    ViConstString source,
                                    ViInt32 TVSignalFormat, 
                                    ViInt32 TVEvent, 
                                    ViInt32 TVPolarity)
{
    ViStatus   error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                   0, source), 2, "Trigger Source");
    
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT, 
                                          0, TVSignalFormat), 3, "TV Signal Format");
        /* Set this after Signal Type */
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_EVENT, 
                                          0, TVEvent), 4, "TV Trigger Event");
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_POLARITY, 
                                          0, TVPolarity), 5, "TV Trigger Polarity");

   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureTVTriggerLineNumber
 *  Purpose:  This function configures the line number for TV triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTVTriggerLineNumber (ViSession vi, 
                                                        ViInt32 lineNumber)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, 
                                ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER, 
                                0, lineNumber), 
                                2, "TV Trigger Line Number");
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
                      
}

/*****************************************************************************
 *  Function: itscope_ConfigureTVTriggerFieldHoldoff
 *  Purpose:  This function configures the video trigger holdoff for TV triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTVTriggerFieldHoldoff (ViSession vi,
                                                          ViReal64 holdoff)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_FIELD_HOLDOFF,
                                            0, holdoff), 2, "Trigger Holdoff");
            
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;   
}

/*****************************************************************************
 *  Function: itscope_ConfigureTVTriggerHDTV
 *  Purpose:  This function configures the HDTV format for TV triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTVTriggerHDTV (ViSession vi,
                                                  ViInt32 format)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT,
                                            0, format), 2, "HDTV Format");
    
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;       
}

/*****************************************************************************
 *  Function: itscope_ConfigureTVTriggerHDTV
 *  Purpose:  This function configures custom format and scan rate for TV triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTVTriggerCustom (ViSession vi,
                                                    ViInt32 format,
                                                    ViInt32 scanRate,
                                                    ViReal64 linePeriod,
                                                    ViReal64 syncInterval)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_FORMAT,
                                            0, format), 2, "Custom Format");
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE,
                                            0, scanRate), 3, "Custom Scan Rate");
    
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_LINE_PERIOD,
                                            0, linePeriod), 4, "Custom Line Period");
    
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SYNC_INTERVAL,
                                            0, syncInterval), 5, "Custom Sync Interval");
    
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;   
}

/*****************************************************************************
 *  Function: itscope_ConfigureRuntTriggerSource
 *  Purpose:  This function configures the scope for runt triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureRuntTriggerSource (ViSession vi,
                                    ViConstString source,
                                    ViReal64 runtLowThreshold, 
                                    ViReal64 runtHighThreshold, 
                                    ViInt32 runtPolarity)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, 
                                            ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                            0, source), 2, "Trigger Source");

    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_RUNT_LOW_THRESHOLD, 
                                           0, runtLowThreshold), 3, "Runt Low Threshold");

    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_RUNT_HIGH_THRESHOLD, 
                                           0, runtHighThreshold), 4, "Runt High Threshold");
    
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_RUNT_POLARITY, 
                                          0, runtPolarity), 5, "Runt Trigger Polarity");

   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureRuntTriggerCondition
 *  Purpose:  This function configures the condition for runt triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureRuntTriggerCondition (ViSession vi,
                                                            ViReal64 runtWidth,
                                                            ViInt32 runtCondition)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_RUNT_WIDTH,
                                            0, runtWidth), 2, "Runt Width" );
                                            
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_RUNT_WHEN,
                                            0, runtCondition), 3, "Runt Condition" );

   _checkErr (itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;   
}

/*****************************************************************************
 *  Function: ConfigureAcLineTriggerSlope
 *  Purpose:  This function configures the oscilloscope for AC Line triggering
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureAcLineTriggerSlope ( ViSession vi, 
                                        ViInt32 acLineSlope)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_AC_LINE_TRIGGER_SLOPE,
                                            0, acLineSlope), 
                                            2, "AC Line Slope");
   _checkErr(itscope_CheckStatus (vi));
                                            
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureTransitionTriggerSource
 *  Purpose:  This function configures the scope for transition triggering.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTransitTriggerSource (ViSession vi,
                                                    ViConstString source,
                                                    ViInt32 polarity, 
                                                    ViInt32 when, 
                                                    ViReal64 deltaTime, 
                                                    ViReal64 lowThreshold, 
                                                    ViReal64 highThreshold)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                            0, source), 2, "Trigger Source");
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TRANSITION_LOW_THRESHOLD, 
                                            0, lowThreshold), 6, "Transition Low Threshold");
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TRANSITION_HIGH_THRESHOLD, 
                                            0, highThreshold), 7, "Transition High Threshold"); 
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TRANSITION_DELTA_TIME, 
                                            0, deltaTime), 5, "Transition Delta Time");                                             
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRANSITION_WHEN, 
                                            0, when), 4, "Transition Trigger When");                                            
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRANSITION_POLARITY, 
                                            0, polarity), 3, "Transition Trigger Polarity");
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureLogicTriggerThreshold
 *  Purpose:  This function configures logic trigger threshold for the 
 *            specified channel.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureLogicTriggerThreshold (ViSession vi,
                                                            ViConstString channel,
                                                            ViReal64 threshold)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL)); 
    
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_LOGIC_THRES_BY_CH,
                                            0, threshold), 3, "Logic Threshold");
                                            
   _checkErr (itscope_CheckStatus (vi));                                            
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;    
}

/*****************************************************************************
 *  Function: itscope_ConfigurePatternTrigger
 *  Purpose:  This function configures the scope for logic pattern trigger.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigurePatternTrigger (ViSession vi,
                                                            ViInt32 function,
                                                            ViReal64 deltaTime,
                                                            ViInt32 condition)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_PATTERN_FUNCTION,
                                            0, function), 2, "Loggic Pattern Function");
                                            
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_PATTERN_DELTATIME,
                                            0, deltaTime), 3, "Logic Pattern Delta Time");
                                            
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_PATTERN_WHEN,
                                            0, condition), 4, "Logic Pattern Condition");
                                            
   _checkErr (itscope_CheckStatus (vi));                                            
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error; 
}

/*****************************************************************************
 *  Function: itscope_ConfigurePatternTriggerInput
 *  Purpose:  This function configures the trigger input for logic pattern 
 *            trigger.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigurePatternTriggerInput (ViSession vi,
                                                                ViConstString channel,
                                                                ViInt32 logicInput)
{
    ViStatus    error = VI_SUCCESS; 
    
   _checkErr( Ivi_LockSession (vi, VI_NULL)); 
    
    _viCheckParm (Ivi_SetAttributeViInt32(vi, channel, ITSCOPE_ATTR_PATTERN_INPUT,
                                            0, logicInput), 2, "Logic Pattern Trigger Input");
                                            
   _checkErr (itscope_CheckStatus (vi));                                            
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;    
}

/*****************************************************************************
 *  Function: itscope_ConfigureStateTrigger
 *  Purpose:  This function configures the scope for logic state trigger.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureStateTrigger (ViSession vi,
                                                        ViInt32 function)
{
    ViStatus    error = VI_SUCCESS; 
    
   _checkErr( Ivi_LockSession (vi, VI_NULL)); 
    
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_STATE_FUNCTION,
                                            0, function), 2, "Loggic State Function");
    
   _checkErr (itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureStateTriggerDataInput
 *  Purpose:  This function configures the data source input for logic
 *            state trigger.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureStateTriggerDataInput (ViSession vi,
                                                                ViConstString channel,
                                                                ViInt32 logicInput)
{
    ViStatus    error = VI_SUCCESS; 
    
   _checkErr( Ivi_LockSession (vi, VI_NULL)); 
    
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, channel, ITSCOPE_ATTR_STATE_INPUT,
                                            0, logicInput), 3, "Logic State Data Input");
    
   _checkErr (itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;    
}

/*****************************************************************************
 *  Function: itscope_ConfigureStateTriggerClockInput
 *  Purpose:  This function configures the clock source for logic state
 *            trigger.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureStateTriggerClockInput (ViSession vi,
                                                                    ViInt32 source,
                                                                    ViInt32 edge)
{
    ViStatus    error = VI_SUCCESS; 
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_STATE_CLOCK_SOURCE,
                                            0, source), 2, "Logic State Clock Source");
                                            
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_STATE_CLOCK_EDGE,
                                            0, edge), 3, "Logic State Clock Edge");                                         
                                            
   _checkErr (itscope_CheckStatus (vi));                                                
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;     
}

/*****************************************************************************
 *  Function: itscope_ConfigureSetholdTriggerSource
 *  Purpose:  This fucntion configures the scope for logic sethold trigger.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureSetholdTriggerSource( ViSession vi,
                                                            ViInt32 dataSource,
                                                            ViInt32 clockSource,
                                                            ViInt32 clockEdge,
                                                            ViReal64 setTime,
                                                            ViReal64 holdTime)
{
    ViStatus    error = VI_SUCCESS;
    
    ViInt32     originalClockSource;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));   
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE,
                                            0, &originalClockSource));
    
    if (originalClockSource == dataSource)
    {
        _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE,
                                            0, clockSource), 3, "Logic Sethold Clock Source");
        
        _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_DATA_SOURCE,
                                            0, dataSource), 2, "Logic Sethold Data Source");
    }
    else
    {
        _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_DATA_SOURCE,
                                            0, dataSource), 2, "Logic Sethold Data Source");

        _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE,
                                            0, clockSource), 3, "Logic Sethold Clock Source");
    }
                                        
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_CLOCK_EDGE,
                                            0, clockEdge), 4, "Logic Sethold Clock Edge");
                                            
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_SETTIME,
                                            0, setTime), 5, "Logic Sethold Set Time");
                                            
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_HOLDTIME,
                                            0, holdTime), 6, "Logic Sethold Hold Time");
                                            
   _checkErr (itscope_CheckStatus (vi));                                                

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error; 
}






/*****************************************************************************
 *  Function: itscope_ConfigureAcquisitionType
 *  Purpose:  This function sets the waveform acquisition type.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_ConfigureAcquisitionType (ViSession vi, 
                                                     ViInt32 acquisitionType)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_ACQUISITION_TYPE, 
                                          0, acquisitionType), 2, "Acquisition Type");
	ViInt32 acquisitionType2;
	_viCheckParm(Ivi_GetAttributeViInt32(vi, VI_NULL, ITSCOPE_ATTR_ACQUISITION_TYPE,
		0, &acquisitionType2), 2, "Acquisition Type");

   _checkErr (itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 *  Function: itscope_ConfigureNumAverages
 *  Purpose:  This function sets the number of averages for the averaging
 *            acquisition type.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_ConfigureNumAverages (ViSession vi, 
                                                  ViInt32 numberOfAverages)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_NUM_AVERAGES, 
                                          0, numberOfAverages), 2, "Number Of Averages");
    
   _checkErr (itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureNumEnvelopes
 *  Purpose:  This function sets the number of envelopes for the envelope
 *            acquisition type.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_ConfigureNumEnvelopes (ViSession vi, 
                                                   ViInt32 numberOfEnvelopes)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_NUM_ENVELOPES, 
                                          0, numberOfEnvelopes), 2, "Number Of Envelopes");
    
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: itscope_ConfigureMidRef                                     
 * Purpose:  Configure an immediate delay measurement.                                                    
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureMidRef (ViSession vi, ViReal64 mid)
{
    ViStatus  error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
   _checkErr( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_MID2_REF, 0, 
                                        mid));
   _checkErr( itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
       
}


/*****************************************************************************
 *  Function: itscope_ConfigureRefLevels
 *  Purpose:  This function configures the reference levels for
 *            waveform measurements.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_ConfigureRefLevels (ViSession vi, 
                                                ViReal64 low, 
                                                ViReal64 mid, 
                                                ViReal64 high)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_HIGH_REF, 
                                           0, high), 4, "High");

    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_LOW_REF, 
                                           0, low), 2, "Low");
    
    _viCheckParm( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_MID_REF, 
                                           0, mid), 3, "Mid");
                                           
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureMeasurement
 *  Purpose:  This function configures the method the oscilloscope uses to
 *            calculate the measurement reference levels and whether to use
 *            gating for measurements.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_ConfigureMeasurement (ViSession vi, 
                                                  ViInt32 referenceLevelCalcMethod, 
                                                  ViInt32 gating)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_REF_CALC_METH, 
                 0, referenceLevelCalcMethod), 2, "Reference Level Calc Method");

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_GATING, 
                                            0, gating), 3, "Gating");
    
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
    
}

/*****************************************************************************
 *  Function: itscope_AutoSetup                                     
 *  Purpose:  This function performs and autosetup on the instrument.        
 *****************************************************************************/
ViStatus _VI_FUNC itscope_AutoSetup (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

   _checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, ITSCOPE_ATTR_INITIATE_CONTINUOUS, 
                                           0, VI_TRUE)); 

    if (!Ivi_Simulating (vi)) /* call only when locked */
    {
        //ViSession   io = Ivi_IOSession (vi); /* call only when locked */

       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        //_viCheckErr( viPrintf (io, ":AUTOS EXEC;"));
		_viCheckErr(_viPrintf(vi, ":AUTOS EXEC;"));
    }
   _checkErr( itscope_WaitForOPC (vi, 5000)); 
   _checkErr( Ivi_InvalidateAllAttributes (vi));  
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureWfmBufferSize
 *  Purpose:  This function initiates the waveform buffer size.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_ConfigureWfmBufferSize(ViSession vi, 
                                                ViInt32 bufferSize)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));  
    
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_FETCH_BUFFER_SIZE,
                                            0, bufferSize));
  
Error:
    Ivi_UnlockSession (vi, VI_NULL); 
    return error;    
}

/*****************************************************************************
 *  Function: itscope_ReadWaveform
 *  Purpose:  This function initiates a new waveform acquisition and
 *            returns a waveform from a specific channel.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ReadWaveform (ViSession vi, 
                                         ViConstString channel, 
                                         ViInt32 waveformSize, 
                                         ViInt32 maxTime, 
                                         ViReal64 waveform[], 
                                         ViInt32 *actualPoints, 
                                         ViReal64 *initialX, 
                                         ViReal64 *xIncrement)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channelEnabled;
    ViInt32     acquisitionType;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (waveform == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 5, "Null address for Waveform");
    }       
    if (actualPoints == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 6, "Null address for Actual Points");
    }       
    if (initialX == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 7, "Null address for Initial X");
    }       
    if (xIncrement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 8, "Null address for X Increment");
    }       

   _checkErr( Ivi_GetAttributeViBoolean (vi, channel, ITSCOPE_ATTR_CHANNEL_ENABLED, 0, 
                                        &channelEnabled));
    if (!channelEnabled)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read waveform from disabled channel.");
    }

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_ACQUISITION_TYPE, 0, 
                                        &acquisitionType));

    if ((acquisitionType == ITSCOPE_VAL_PEAK_DETECT)
            || (acquisitionType == ITSCOPE_VAL_ENVELOPE))
    {
        _viCheckErrElab(IVI_ERROR_INVALID_CONFIGURATION, 
                        "Use ReadMinMaxWaveform for acquisition types "
                        "Peak Detect or Envelope.");
    }
        

        /* Read Waveform */
   _checkErr( itscope_InitiateAcquisition (vi)); 
    
   _checkErr( itscope_WaitForOPC (vi, maxTime)); 
    
   _checkErr( itscope_FetchWaveform (vi, channel, waveformSize, waveform, 
                                      actualPoints, initialX, xIncrement));
    
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ReadMinMaxWaveform                            
 *  Purpose:  This function initiates new waveform acquisition and           
 *            returns min and max waveforms from a specific channel.         
 *            Call this function only when ITSCOPE_ATTR_ACQUISITION_TYPE       
 *            is ITSCOPE_VAL_PEAK_DETECT or ITSCOPE_VAL_ENVELOPE.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ReadMinMaxWaveform (ViSession vi, 
                                               ViConstString channel, 
                                               ViInt32 waveformSize, 
                                               ViInt32 maxTime, 
                                               ViReal64 minWaveform[], 
                                               ViReal64 maxWaveform[], 
                                               ViInt32 *actualPoints, 
                                               ViReal64 *initialX, 
                                               ViReal64 *xIncrement)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channelEnabled;
    ViInt32     acquisitionType;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (minWaveform == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 5, "Null address for minWaveform");
    }
    if (maxWaveform == VI_NULL)
    {        
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 6, "Null address for maxWaveform");
    }       
    if (actualPoints == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 7, "Null address for Actual Points");
    }       
    if (initialX == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 8, "Null address for Initial X");
    }       
    if (xIncrement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 9, "Null address for X Increment");
    }       
    
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel, ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                         0, &channelEnabled));
    if (!channelEnabled)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read waveform from disabled channel.");
    }
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_ACQUISITION_TYPE, 
                                       0, &acquisitionType));

    if ( (acquisitionType != ITSCOPE_VAL_PEAK_DETECT)
            && (acquisitionType != ITSCOPE_VAL_ENVELOPE) )
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Use ReadMinMaxWaveform only for acquisition types "
                        "Peak Detect or Envelope.");
    }
    /* Read Min and Max Waveforms */
   _checkErr( itscope_InitiateAcquisition(vi));
    
   _checkErr( itscope_WaitForOPC(vi, maxTime));
    
   _checkErr( itscope_FetchMinMaxWaveform (vi, channel, waveformSize, 
                                            minWaveform, maxWaveform, 
                                            actualPoints, initialX, 
                                            xIncrement));
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ReadWaveformMeasurement                       
 *  Purpose:  This function initiates a new waveform acquisition and         
 *            returns a specified waveform measurement from a specific       
 *            channel.                                                       
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ReadWaveformMeasurement (ViSession vi, 
                                                    ViConstString channel, 
                                                    ViInt32 measFunction, 
                                                    ViInt32 maxTime, 
                                                    ViReal64 *measurement)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channelEnabled;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    IviScope_InterchangeCheck (vi, "itscope_ReadWaveformMeasurement");
    
        /* Not checking the following parameters: channel, maxTime */
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel, ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                         0, &channelEnabled));
    if (!channelEnabled)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read a waveform measurement from disabled channel.");
    }
    
    if (measurement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 5, "Null address for Measurement");
    }
    
        /* Read Waveform Measurement */
   _checkErr( itscope_InitiateAcquisition (vi));
    
   _checkErr( itscope_WaitForOPC (vi, maxTime));

   _checkErr( itscope_FetchWaveformMeasurement (vi, channel, measFunction, 
                                                 measurement));

   _checkErr( itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ReadPhaseMeasurement                       
 *  Purpose:  This function initiates a new waveform acquisition and         
 *            returns a phase measurement.                        
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ReadPhaseMeasurement (ViSession vi,
                                                ViConstString measureSource1,
                                                ViConstString measureSource2,
                                                ViInt32 maxTime,
                                                ViReal64 *measurement)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channel1Enabled;
    ViBoolean   channel2Enabled;
    ViConstString channel1 = VI_NULL, channel2 = VI_NULL;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm (Ivi_CoerceChannelName (vi, measureSource1, &channel1),
                                         2, "Measurement Source1");
    
    _viCheckParm (Ivi_CoerceChannelName (vi, measureSource2, &channel2),
                                         3, "Measurement Source2");
    
        /* Not checking the following parameters: maxTime */
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel1, ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                         0, &channel1Enabled));
    
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel2, ITSCOPE_ATTR_CHANNEL_ENABLED,
                                         0, &channel2Enabled));
    
    if (!channel1Enabled || !channel2Enabled)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read a waveform measurement from disabled channel.");
    }
    
    if (measurement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 5, "Null address for Measurement");
    }
    
        /* Read Waveform Measurement */
   _checkErr( itscope_InitiateAcquisition (vi));
    
   _checkErr( itscope_WaitForOPC (vi, maxTime));

   _checkErr( itscope_FetchPhaseMeasurement (vi, channel1, channel2, 
                                                 measurement));
   _checkErr( itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ReadDelayMeasurement                       
 *  Purpose:  This function initiates a new waveform acquisition and         
 *            returns a delay measurement.                        
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ReadDelayMeasurement (ViSession vi,
                                                ViConstString measureSource1,
                                                ViConstString measureSource2,
                                                ViInt32 delayEdge1,
                                                ViInt32 delayEdge2,
                                                ViInt32 delayDirection,
                                                ViInt32 maxTime,
                                                ViReal64 *measurement)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channel1Enabled;
    ViBoolean   channel2Enabled;
    ViConstString channel1 = VI_NULL, channel2 = VI_NULL;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    _viCheckParm (Ivi_CoerceChannelName (vi, measureSource1, &channel1),
                                         2, "Measurement Source1");
    
    _viCheckParm (Ivi_CoerceChannelName (vi, measureSource2, &channel2),
                                         3, "Measurement Source2");
    
        /* Not checking the following parameters: maxTime */
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel1, ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                         0, &channel1Enabled));
    
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel2, ITSCOPE_ATTR_CHANNEL_ENABLED,
                                         0, &channel2Enabled));
    
    if (!channel1Enabled || !channel2Enabled)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read a waveform measurement from disabled channel.");
    }
    
    if (measurement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 8, "Null address for Measurement");
    }
    
        /* Read Waveform Measurement */
   _checkErr( itscope_InitiateAcquisition (vi));
    
   _checkErr( itscope_WaitForOPC (vi, maxTime));

   _checkErr( itscope_FetchDelayMeasurement (vi, channel1, channel2, delayEdge1,
                                                 delayEdge2, delayDirection, measurement));
   _checkErr( itscope_CheckStatus (vi));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_InitiateAcquisition
 *  Purpose:  This function initiates waveform acquisition in single 
 *            acquisition mode.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_InitiateAcquisition (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     model; 
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    IviScope_InterchangeCheck (vi, "itscope_InitiateAcquisition");
    /*
       If the minimum number of points is greater than the 
       record length, then another attribute must have changed the
       record length of the oscilloscope and invalidated the 
       ITSCOPE_ATTR_HORZ_RECORD_LENGTH attribute.

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                       0, &recordLen));
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ITSCOPE_ATTR_HORZ_MIN_NUM_PTS, 
                                       0, &minNumPts));
    if (minNumPts > recordLen)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Current record length is less than the minimum number of points.");
    }
    */
    
        /* Initiate the acquisition */
    if (!Ivi_Simulating (vi))                /* call only when locked */
    {
        //ViSession   io = Ivi_IOSession (vi); /* call only when locked */
        /* Do not find additional needs for math channel setup, remove it from source 
           code to enhance speed.
        */
        /*
       _checkErr( Ivi_GetAttributeViBoolean (vi, "MATH", ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                             0, &mathEnabled));
                                             
        if (mathEnabled)
        {
            Delay (1.0); */ /* delay to ensure MATH setup. Allow for 1 sec  */
        /*}
        */

       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        
        /* Set Continuous Acquisition to FALSE */
        /*  Remove it to support continuous acquisition mode */
        /*
       _checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, ITSCOPE_ATTR_INITIATE_CONTINUOUS,
                                            0, VI_FALSE) );
        */
        
            /* Set Acquisition State to RUN */
        //_viCheckErr( viPrintf (io, ":ACQ:STATE RUN"));
		_viCheckErr(_viPrintf(vi, ":ACQ:STATE RUN"));
        
        /* Invalidate Preamble attributes */
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
        if(model == ITSCOPE_VAL_PE520K1)
        {
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YZERO) );
            
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YOFF) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YOFF) );           

           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YMULT) );       
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YMULT) );  
               
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YZERO) ); 
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YOFF) );  
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YMULT) );      
        }
        else
        {
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH3", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH4", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YZERO) );      
            
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YOFF) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "CH3", ITSCOPE_ATTR_DATA_YOFF) );        
           _checkErr( Ivi_InvalidateAttribute (vi, "CH4", ITSCOPE_ATTR_DATA_YOFF) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YOFF) ); 
            
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YMULT) );       
           _checkErr( Ivi_InvalidateAttribute (vi, "CH3", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH4", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YMULT) );  
               
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YZERO) ); 
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YOFF) );  
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YOFF) );
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YMULT) );      
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YMULT) );      
        }
        
       _checkErr( Ivi_InvalidateAttribute (vi, VI_NULL, ITSCOPE_ATTR_DATA_XINCR) );
       _checkErr( Ivi_InvalidateAttribute (vi, VI_NULL, ITSCOPE_ATTR_DATA_XZERO) );
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_InitiateContinuousAcquisition
 *  Purpose:  This function initiates waveform acquisition in continuous
 *            acquisition mode. Set the waveform size element to the desired 
 *            amount.
 *  Note:     All channels 
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_InitiateContinuousAcquisition (ViSession vi, 
                                                            ViInt32 waveformSize)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     horzRecordLength, stopIndex;  
    ViInt32     acquisitionType, dataWidth=1;
    ViInt32     wfmFormat=1;
    ViInt32     model;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    IviScope_InterchangeCheck (vi, "itscope_InitiateAcquisition");
    /*
       If the minimum number of points is greater than the 
       record length, then another attribute must have changed the
       record length of the oscilloscope and invalidated the 
       ITSCOPE_ATTR_HORZ_RECORD_LENGTH attribute.

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                       0, &recordLen));
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ITSCOPE_ATTR_HORZ_MIN_NUM_PTS, 
                                       0, &minNumPts));
    if (minNumPts > recordLen)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Current record length is less than the minimum number of points.");
    }
    */ 
    
        /* Initiate the acquisition */
    if (!Ivi_Simulating (vi))                /* call only when locked */
    {
        ViBoolean   mathEnabled;
        //ViSession   io = Ivi_IOSession (vi); /* call only when locked */
        
       _checkErr( Ivi_GetAttributeViBoolean (vi, "MATH", ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                             0, &mathEnabled));
                                             
        /* delay to ensure MATH setup. Allow for 1 sec */
        /* Do not find neccessary use for math channel setup, remove it from source code
        if (mathEnabled)
        {
            Delay (1.0);  
        }
        */
        
       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
         
            /* Set Continuous Acquisition to TRUE */
       _checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, ITSCOPE_ATTR_INITIATE_CONTINUOUS,
                                                0, VI_TRUE) );
            
            /* Set Acquisition State to RUN */
       //_viCheckErr( viPrintf (io, ":ACQ:STATE RUN"));
		_viCheckErr(_viPrintf(vi, ":ACQ:STATE RUN"));
        
       _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_WAVEFORM_SIZE,
                                            0, waveformSize));
        
       _checkErr( Ivi_GetAttributeViInt32  (vi, VI_NULL, 
                                            ITSCOPE_ATTR_ACQUISITION_TYPE, 
                                            0, &acquisitionType));              
        switch (acquisitionType) 
        {
            case ITSCOPE_VAL_NORMAL:
            {
                dataWidth = 2;
                wfmFormat = 1;
                break;
            }            
            case ITSCOPE_VAL_AVERAGE:
            case ITSCOPE_VAL_HI_RES:
            {
                dataWidth = 2;
                wfmFormat = 1;
                break;
            }
            case ITSCOPE_VAL_PEAK_DETECT:
            case ITSCOPE_VAL_ENVELOPE:
            {
                dataWidth = 1;
                wfmFormat = 2;
                break;
            }
            default:
            {
                _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                                "Use InitiateContinuousAcquisition for acquisition types "
                                "Peak Detect or Envelope.");
            }
        }
       
        /* Get record length, not support Ref1 ~ Ref4 */
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                           0, &horzRecordLength));
        if(wfmFormat == 2)
        {
            horzRecordLength *= 2; 
            waveformSize     *= 2;  
        }
        
        if (horzRecordLength > waveformSize)
        {
            stopIndex = waveformSize;
        }        
        else
        {
            stopIndex = horzRecordLength;
        }       
        
       _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_WIDTH, 0, dataWidth) );
       _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_START_POS, 0, 1) );
       _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_STOP_POS, 0, stopIndex) );    

       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
        if(model == ITSCOPE_VAL_PE520K1)
        {
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YZERO) );
            
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YOFF) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YOFF) );           

           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YMULT) );       
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YMULT) );  
            
            /* Intend not to support Reference channel in 
               InitiateContinuousAcquisition && FetchContinuousAcquisition 
               for simplification.
               
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YZERO) ); 
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YOFF) );  
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YOFF) );       
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YMULT) );      
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YMULT) );      
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YMULT) );          
            */
        }
        else if(model == ITSCOPE_VAL_PE510K1)
        {
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH3", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH4", ITSCOPE_ATTR_DATA_YZERO) );
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YZERO) );      
            
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YOFF) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "CH3", ITSCOPE_ATTR_DATA_YOFF) );        
           _checkErr( Ivi_InvalidateAttribute (vi, "CH4", ITSCOPE_ATTR_DATA_YOFF) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YOFF) ); 
            
           _checkErr( Ivi_InvalidateAttribute (vi, "CH1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH2", ITSCOPE_ATTR_DATA_YMULT) );       
           _checkErr( Ivi_InvalidateAttribute (vi, "CH3", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "CH4", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "MATH", ITSCOPE_ATTR_DATA_YMULT) );  
            
            /* Intend not to support Reference channel in 
               InitiateContinuousAcquisition && FetchContinuousAcquisition 
               for simplification.
               
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YZERO) ); 
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YZERO) ); 
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YOFF) );  
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YOFF) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YOFF) );
            
           _checkErr( Ivi_InvalidateAttribute (vi, "REF1", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF2", ITSCOPE_ATTR_DATA_YMULT) );      
           _checkErr( Ivi_InvalidateAttribute (vi, "REF3", ITSCOPE_ATTR_DATA_YMULT) );
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YMULT) );      
           _checkErr( Ivi_InvalidateAttribute (vi, "REF4", ITSCOPE_ATTR_DATA_YMULT) );          
            */
        }
        
       _checkErr( Ivi_InvalidateAttribute (vi, VI_NULL, ITSCOPE_ATTR_DATA_XINCR) );
       _checkErr( Ivi_InvalidateAttribute (vi, VI_NULL, ITSCOPE_ATTR_DATA_XZERO) );
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_StopContinuousAcquisition
 *  Purpose:  This function stops waveform acquisition in continuous
 *            acquisition mode.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_StopContinuousAcquisition (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    IviScope_InterchangeCheck (vi, "itscope_InitiateAcquisition");

        /* Initiate the acquisition */
    if (!Ivi_Simulating (vi))                /* call only when locked */
    {
        //ViSession   io = Ivi_IOSession (vi); /* call only when locked */ 

       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE)); 
        
            /* Set Acquisition State to STOP */
        //_viCheckErr( viPrintf (io, ":ACQ:STATE STOP"));
		_viCheckErr(_viPrintf(vi, ":ACQ:STATE STOP"));
    }
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ForceTrigger
 *  Purpose:  This function forces a trigger with a software command.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ForceTrigger (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))                   /* call only when locked */
    {
        ViSession   io = Ivi_IOSession (vi);    /* call only when locked */

       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        //_viCheckErr( viPrintf (io, "%s", "*TRG"));
		_viCheckErr(_viPrintf(vi, "%s", "*TRG"));
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_AcquisitionStatus
 *  Purpose:  This function determines if an acquisition is in
 *            progress or has completed. If it cannot determine the acquisition
 *            status, it returns ITSCOPE_VAL_ACQ_STATUS_UNKNOWN.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_AcquisitionStatus (ViSession vi, ViInt32 *statusRef)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     status = ITSCOPE_VAL_ACQ_COMPLETE;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (statusRef == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Status");
    }
    
    if (!Ivi_Simulating (vi))                   /* call only when locked */
    {
        //ViSession   io = Ivi_IOSession (vi);    /* call only when locked */
        ViInt32     acqStatus;
    
        //_viCheckErr( viQueryf (io, ":ACQ:STATE?", "%ld", &acqStatus));
		_viCheckErr(_viQueryf(vi, ":ACQ:STATE?", "%ld", &acqStatus));
        if (acqStatus != 0)
        {
            status = ITSCOPE_VAL_ACQ_IN_PROGRESS;
        }
    }
        
Error:
    *statusRef = status;
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_FetchWaveform
 *  Purpose:  This function fetches a waveform from a specified
 *            channel from previously initiated acquisition.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_FetchWaveform (ViSession vi, 
                                           ViConstString channel, 
                                           ViInt32 waveformSize, 
                                           ViReal64 waveform[], 
                                           ViInt32 *actualPointsRef, 
                                           ViReal64 *initialXRef, 
                                           ViReal64 *xIncrementRef)
{
    ViStatus      error = VI_SUCCESS;
    ViInt32       actualPoints;
    ViReal64      xIncrement, initialX;
    ViConstString dataSource;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_CoerceChannelName (vi, channel, &dataSource), 2, "Invalid Channel");
    if (waveform == VI_NULL)
    {    
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Waveform");
    }  
    if (actualPointsRef == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 5, "Null address for Actual Points");
    }
    if (initialXRef == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 6, "Null address for Initial X");
    }
    if (xIncrementRef == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 7, "Null address for X Increment");
    }
    
        /* Fetch Waveform */
    if (!Ivi_Simulating (vi))                   /* call only when locked */
    {
       // ViSession   io = Ivi_IOSession (vi);    /* call only when locked */
        ViReal64    yZero, yOffset, yMultiplier;
        
       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

       _checkErr( itscope_FetchRawWaveform (vi, 0, dataSource,
                                             &yZero, &yOffset, &yMultiplier, 
                                             &xIncrement, &initialX, 
                                             waveformSize, waveform,
                                             &actualPoints));
                                             
        itscope_ScaleRawWaveform (actualPoints, waveform, yZero, yOffset, yMultiplier);
    }
    else
    {
        ViInt32     x;
        ViReal64    yRange, simOffset = 0.0;
        ViInt32     triggerSlope, vCoup;
        ViReal64    k, level, theta, offset;

       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                           0, &actualPoints));
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, 
                                            ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &yRange)); 
       _checkErr( Ivi_GetAttributeViInt32 (vi, channel, 
                                           ITSCOPE_ATTR_VERTICAL_COUPLING, 
                                           0, &vCoup));
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, 
                                            ITSCOPE_ATTR_VERTICAL_OFFSET, 
                                            0, &offset)); 
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_TRIGGER_SLOPE, 
                                           0, &triggerSlope));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, 
                                            ITSCOPE_ATTR_TRIGGER_LEVEL, 
                                            0, &level));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, 
                                            ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                            0, &xIncrement));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, 
                                            ITSCOPE_ATTR_ACQUISITION_START_TIME, 
                                            0, &initialX));
        theta = asin (2*level/yRange);
        if (triggerSlope == ITSCOPE_VAL_POSITIVE)
        {
            k = 1.0;
        }           
        else 
        {
            k = -1.0;
        }           
        
        if( actualPoints>waveformSize ) 
        {
            actualPoints = waveformSize;  /* Checking number of points to write */
        }           
        xIncrement /= actualPoints;

        if (vCoup == ITSCOPE_VAL_DC)
            simOffset = 0.5;
        if (vCoup == ITSCOPE_VAL_GND)
            simOffset = 0.0;     /*i changed this variable from K to simOffset */
            
        for (x = 0; x < actualPoints; x++)
        {
            ViReal64 y = simOffset + k * 2.5 * sin (xIncrement * 12560 * x + k * theta) + /* ~2 periods of 1kHz sinewave */
                            (!(x%20)) * (16384 - rand())/150000.0;
            waveform[x] = (offset + yRange/2) > y ? ((offset - yRange/2) < y ? y : (offset - yRange/2)) : (offset + yRange/2);
        }            
    }
    *actualPointsRef = actualPoints;
    *initialXRef = initialX;
    *xIncrementRef = xIncrement;
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_FetchMinMaxWaveform                           
 *  Purpose:  This function fetches a min and max waveform from a waveform 
 *            acquisition that the user or driver previously initiated on a 
 *            specific channel.                     
 *            Call this function only when ITSCOPE_ATTR_ACQUISITION_TYPE
 *            is ITSCOPE_VAL_PEAK_DETECT or ITSCOPE_VAL_ENVELOPE.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_FetchMinMaxWaveform (ViSession vi, 
                                                ViConstString channel, 
                                                ViInt32 waveformSize, 
                                                ViReal64 minWaveform[], 
                                                ViReal64 maxWaveform[], 
                                                ViInt32 *actualPointsRef, 
                                                ViReal64 *initialXRef, 
                                                ViReal64 *xIncrementRef)
{
    ViStatus        error = VI_SUCCESS;
    ViInt32         acquisitionType, actualPoints;
    ViReal64        initialX, xIncrement;
    ViConstString   dataSource;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_CoerceChannelName(vi, channel, &dataSource), 2, "Channel");
    
    if (minWaveform == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for minWaveform");
    }
    
    if (maxWaveform == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 5, "Null address for maxWaveform");
    }
    
    if (actualPointsRef == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 6, "Null address for Actual Points");
    }
    
    if (initialXRef == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 7 , "Null address for Initial X");
    }
    
    if (xIncrementRef == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 8 , "Null address for X Increment");
    }
    
        /* Fetch Min and Max Waveforms */
   _checkErr( Ivi_GetAttributeViInt32  (vi, VI_NULL, ITSCOPE_ATTR_ACQUISITION_TYPE, 0, 
                                        &acquisitionType));
    
    if ((acquisitionType != ITSCOPE_VAL_PEAK_DETECT) 
            && (acquisitionType != ITSCOPE_VAL_ENVELOPE))
    {           
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Use FetchMinMaxWaveform only for acquisition types "
                        "Peak Detect or Envelope.");
    }
    
    if (!Ivi_Simulating (vi))               /* call only when locked */
    {
        ViSession   io = Ivi_IOSession (vi);    /* call only when locked */
        ViReal64    yZero, yOffset, yMultiplier;
        
       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

       _checkErr( itscope_FetchRawMinMaxWaveform (vi, io, dataSource,
                                                   &yZero, &yOffset, &yMultiplier,
                                                   &xIncrement, &initialX, 
                                                   waveformSize, minWaveform, 
                                                   maxWaveform, &actualPoints));
        
        
        itscope_ScaleRawWaveform (actualPoints, minWaveform, yZero, yOffset, yMultiplier);
        itscope_ScaleRawWaveform (actualPoints, maxWaveform, yZero, yOffset, yMultiplier);
    }
    else
    {
        ViInt32     x;
        ViReal64    yRange, k, theta, level;
        ViInt32     triggerSlope;
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS, 
                                           0, &actualPoints));
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &yRange)); 
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SLOPE, 
                                           0, &triggerSlope));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_LEVEL, 
                                            0, &level));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                            0, &xIncrement));
        theta = asin (2*level/yRange);
        if (triggerSlope == ITSCOPE_VAL_POSITIVE)
            k = 1.0;
        else 
            k = -1.0;

        initialX = 0.0;
        actualPoints /= 4;
        if( actualPoints>waveformSize ) 
            actualPoints = waveformSize;  /* Checking number of points to write */
        xIncrement *= 4;
        for (x = 0; x < actualPoints; x++) 
        {
                /* add glitch to the every 20th element in the waveform array */
            maxWaveform[x] = k * yRange * .49 * sin (xIncrement * x + k * theta)
                            + (!(x%20)) * rand()/150000.0;
            minWaveform[x] = k * yRange * .51 * sin (xIncrement * x + k * theta)
                            - (!(x%20)) * rand()/160000.0;
        }
    }
    *actualPointsRef = actualPoints;
    *xIncrementRef = xIncrement;
    *initialXRef = initialX;
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_FetchWaveformMeasurement                      
 *  Purpose:  This function fetches a specified waveform measurement         
 *            from a specific channel from a previously initiated            
 *            waveform acquisition.                                          
 *****************************************************************************/
ViStatus _VI_FUNC itscope_FetchWaveformMeasurement (ViSession vi, 
                                                     ViConstString channel, 
                                                     ViInt32 measFunction, 
                                                     ViReal64 *measurement)
                                                     
{ 
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channelEnabled;
    ViBoolean   nonHistSource;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    nonHistSource = (ViBoolean) strcmp(channel,ITSCOPE_VAL_HISTOGRAM);
    if(nonHistSource)
    {
       _checkErr( Ivi_GetAttributeViBoolean (vi, channel, ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                         0, &channelEnabled));
        if (!channelEnabled)
        {
            _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                            "Cannot read a waveform measurement from disabled channel.");
        }
    }
    
    if (measurement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Measurement");
    }
    
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_MEAS_SOURCE, 
                                           0, channel), 2, "Measure Source");

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_FUNCTION, 
                                          0, measFunction), 3, "Measurement Function");

    if(nonHistSource)
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_MEAS_VALUE, 
                                        0, measurement)); 
    }
    else
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, ITSCOPE_VAL_CH_1, ITSCOPE_ATTR_MEAS_VALUE, 
                                        0, measurement));   
    }
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_FetchPhaseMeasurement                      
 *  Purpose:  This function fetches the phase measurement.         
 *****************************************************************************/
ViStatus _VI_FUNC itscope_FetchPhaseMeasurement (ViSession vi,
                                                    ViConstString measureSource1,
                                                    ViConstString measureSource2,
                                                    ViReal64 *measurement)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channel1Enabled;
    ViBoolean   channel2Enabled;
    ViConstString channel1 = VI_NULL, channel2 = VI_NULL;
    ViBoolean   nonHistSource;
    nonHistSource = (ViBoolean) strcmp(measureSource1,ITSCOPE_VAL_HISTOGRAM);
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if(nonHistSource)
    {
       _checkErr (Ivi_CoerceChannelName (vi, measureSource1, &channel1));
       _checkErr( Ivi_GetAttributeViBoolean (vi, channel1, ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                         0, &channel1Enabled));
        if (!channel1Enabled)
        {
            _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read a waveform measurement from disabled channel.");
        }
    }
    else
    {
        channel1 = measureSource1;
    }
   _checkErr (Ivi_CoerceChannelName (vi, measureSource2, &channel2));
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel2, ITSCOPE_ATTR_CHANNEL_ENABLED,
                                         0, &channel2Enabled));
    if (!channel2Enabled)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read a waveform measurement from disabled channel.");
    }
    
    if (measurement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Measurement");
    }
    
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_MEAS_SOURCE, 
                                           0, channel1), 2, "Measure Source");
    
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_MEAS_SOURCE2,
                                           0, channel2), 3, "Measure Source2");

   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_FUNCTION, 
                                           0, ITSCOPE_VAL_PHASE));

    if(nonHistSource)
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel1, ITSCOPE_ATTR_MEAS_VALUE, 
                                        0, measurement));
    }
    else
    {
         _checkErr( Ivi_GetAttributeViReal64 (vi, ITSCOPE_VAL_CH_1, ITSCOPE_ATTR_MEAS_VALUE, 
                                        0, measurement));
    }
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_FetchDelayMeasurement                      
 *  Purpose:  This function fetches the delay measurement.         
 *****************************************************************************/
ViStatus _VI_FUNC itscope_FetchDelayMeasurement (ViSession vi,
                                                    ViConstString measureSource1,
                                                    ViConstString measureSource2,
                                                    ViInt32 delayEdge1,
                                                    ViInt32 delayEdge2,
                                                    ViInt32 delayDirection,
                                                    ViReal64 *measurement)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   channel1Enabled;
    ViBoolean   channel2Enabled;
    ViConstString channel1 = VI_NULL, channel2 = VI_NULL;
    ViBoolean   nonHistSource = VI_TRUE;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    nonHistSource = (ViBoolean) strcmp(measureSource1,ITSCOPE_VAL_HISTOGRAM);
    if(nonHistSource)
    {
       _checkErr (Ivi_CoerceChannelName (vi, measureSource1, &channel1));  
       _checkErr( Ivi_GetAttributeViBoolean (vi, channel1, ITSCOPE_ATTR_CHANNEL_ENABLED, 
                                         0, &channel1Enabled));
        if (!channel1Enabled)
        {
            _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read a waveform measurement from disabled channel.");
        }
    }
   _checkErr (Ivi_CoerceChannelName (vi, measureSource2, &channel2)); 
   _checkErr( Ivi_GetAttributeViBoolean (vi, channel2, ITSCOPE_ATTR_CHANNEL_ENABLED,
                                         0, &channel2Enabled));
    
    if (!channel2Enabled)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Cannot read a waveform measurement from disabled channel.");   
    }
    
    if (measurement == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 7, "Null address for Measurement");
    }
    
    if(nonHistSource)
    {
        _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_MEAS_SOURCE, 
                                           0, channel1), 2, "Measurement Source1");
    }
    else
    {
        _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_MEAS_SOURCE, 
                                           0, measureSource1), 2, "Measurement Source1");   
    }
    
    _viCheckParm( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_MEAS_SOURCE2,
                                           0, channel2), 3, "Measurement Source2");
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_DEL_EDGE,
                                           0, delayEdge1), 4, "Measure Source1 Delay Edge");
    
    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_DEL_EDGE2,
                                           0, delayEdge2), 5, "Measure Source2 Delay Edge");

    _viCheckParm( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_DEL_DIRECTION,
                                           0, delayDirection), 6, "Measure Delay Direction");

   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_FUNCTION, 
                                           0, ITSCOPE_VAL_DELAY));

    if(nonHistSource)
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel1, ITSCOPE_ATTR_MEAS_VALUE, 
                                           0, measurement)); 
    }
    else
    {
        /* channelName only for simulation model.
          Hist model do not support                 */
       _checkErr( Ivi_GetAttributeViReal64 (vi, ITSCOPE_VAL_CH_1, ITSCOPE_ATTR_MEAS_VALUE, 
                                           0, measurement)); 
    }
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_Abort
 *  Purpose:  This function aborts a previously initiated acquisition.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_Abort (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))                   /* call only when locked */
    {
        //ViSession   io = Ivi_IOSession (vi);    /* call only when locked */

       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

            /* Set Acquisition State to STOP */
        //_viCheckErr( viPrintf (io, "%s", ":ACQ:STATE STOP"));
		_viCheckErr(_viPrintf(vi, "%s", ":ACQ:STATE STOP"));
    }
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureMathChannel                                
 *  Purpose:  This function configures a math channel to for single or dual
 *            waveform math. 
 *            NOTE: FFTs, use itscope_ConfigureMathChannelFFT.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureMathChannel (ViSession vi,
                                                ViInt32 mathSource1,
                                                ViInt32 mathFunction,
                                                ViInt32 mathSource2)
{
    ViStatus error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
   _checkErr (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_TYPE,
                                            0, ITSCOPE_VAL_MATH_TYPE_DUAL));
    
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_SOURCE_1ST,
                                           0, mathSource1), 2, "Math Source 1");
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_SOURCE_2ND,
                                           0, mathSource2), 4, "Math Source 2");
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_FUNCTION, 
                                          0, mathFunction), 3, "Math Function");
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}
             
/*****************************************************************************
 *  Function: itscope_ConfigureMathChannelFFT                               
 *  Purpose:  This function configures a math channel for FFTs. A math channel
 *            must be configured before it can be enabled.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureMathChannelFFT (ViSession vi, 
                                                    ViInt32 mathSource1,
                                                    ViInt32 fftWindow, 
                                                    ViInt32 fftVertScale)
{
    ViStatus error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

   _checkErr (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_TYPE,
                                            0, ITSCOPE_VAL_MATH_TYPE_FFT));

    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_SOURCE_1ST,
                                           0, mathSource1), 2, "Math Source 1");
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_FFT_WINDOW, 
                                          0, fftWindow), 3, "FFT Window");
    _viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_FFT_VERT_SCALE, 
                                          0, fftVertScale), 4, "FFT Vert Scale");
   _checkErr (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_FUNCTION, 0,
                                       ITSCOPE_VAL_MATH_FFT));
    
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_ConfigureMathChannelAdvanced                               
 *  Purpose:  Configures a math channel using a mathematical expression.  
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureMathChannelAdvanced (ViSession vi,
                                                        ViReal64 var1,
                                                        ViReal64 var2,
                                                        ViConstString funcDefinition)
{
    ViStatus error = VI_SUCCESS;

   _checkErr( Ivi_LockSession (vi, VI_NULL));
    
   _checkErr (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_TYPE,
                                            0, ITSCOPE_VAL_MATH_TYPE_ADVANCED));
    
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_ADV_VAR1, 
                                            0, var1), 2, "Math VAR1");
                                            
    _viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_ADV_VAR2,
                                            0, var2), 3, "Math VAR2");
                                            
    _viCheckParm (Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_ADV_DEFINE,
                                            0, funcDefinition), 4, 
                                            "Math Advanced Function Definition");
    
   _checkErr (itscope_CheckStatus (vi));
                                            
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_EnableMathChannel
 *  Purpose:  This enables/disables the MATH channel.
 *****************************************************************************/
ViStatus _VI_FUNC  itscope_EnableMathChannel (ViSession vi, 
                                                ViBoolean enableMathChannel)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL)); 
    _viCheckParm( Ivi_SetAttributeViBoolean (vi, "MATH", 
                                            ITSCOPE_ATTR_CHANNEL_ENABLED, 0, 
                                            enableMathChannel), 2, "Reference Channel");      
   _checkErr (itscope_CheckStatus (vi)); 

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}                                               


/*****************************************************************************
 *  Function: itscope_EnableReferenceChannel
 *  Purpose:  This enables/disables a reference channel.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_EnableReferenceChannel (ViSession vi, 
                                                    ViConstString refChannel, 
                                                    ViBoolean enableRefChannel)
{
    ViStatus    error = VI_SUCCESS;
    ViString    coercedRefChannel;
    
   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_CoerceChannelName (vi, refChannel, (ViConstString *)&coercedRefChannel),
                                        2, "Ref Channel");
    if (strncmp (coercedRefChannel, "REF", 3) != 0)
        _viCheckParm( IVI_ERROR_INVALID_VALUE, 2, "Reference Channel");
    _viCheckParm( Ivi_SetAttributeViBoolean (vi, coercedRefChannel, 
                                            ITSCOPE_ATTR_CHANNEL_ENABLED, 0, 
                                            enableRefChannel), 3, "Enable Reference Channel");  
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: itscope_StoreWfmToRefChannel
 *  Purpose:  This function stores a waveform to a reference channel.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_StoreWfmToRefChannel (ViSession vi, 
                                                 ViConstString sourceChannel, 
                                                 ViConstString refChannel)
{
    ViStatus        error = VI_SUCCESS;
    ViConstString   sourceChanStr, refChanStr;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_CoerceChannelName (vi, sourceChannel, &sourceChanStr), 
                2, "Source Channel");
    _viCheckParm( Ivi_CoerceChannelName (vi, refChannel, &refChanStr), 
                3, "Reference Channel");
    if (strncmp (refChanStr, "REF", 3) != 0)
        _viCheckParm( IVI_ERROR_INVALID_VALUE, 3, "Reference Channel");
        
    /* Save channnel to instrument's reference channel */
    if (!Ivi_Simulating(vi))                /* call only when locked */
    {
        //ViSession   io = Ivi_IOSession (vi); /* call only when locked */

        //_viCheckErr( viPrintf (io, "SAV:WAVE %s,%s", sourceChanStr, refChanStr));
		_viCheckErr(_viPrintf(vi, ":SAV:WAVE %s,%s", sourceChanStr, refChanStr));
    }
   _checkErr (itscope_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}



/*===========================================================================*
 *= IviScope Action Event Group Functions =============================*
 *===========================================================================*/         
            
            
             
/*****************************************************************************
 *  Function: itscope_IsInvalidWfmElement
 *  Purpose:  This function takes one of the Waveform Array's element value
 *            that you obtain from the ReadWaveform or FetchWaveform function
 *            and determines if the value is a valid measurement value or a
 *            value indicating that the oscilloscope could not sample a
 *            voltage.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_IsInvalidWfmElement (ViSession vi,
                                                ViReal64 elementValue,
                                                ViBoolean *isInvalid)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     type;
    
   _checkErr( Ivi_LockSession(vi, VI_NULL));
    
    if (isInvalid == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Is Invalid.");
    }
    
   _viCheckErr( Ivi_GetViReal64Type (elementValue, &type));
    
    if (type == IVI_VAL_TYPE_NAN)
    {
        *isInvalid = VI_TRUE;
    }       
    else
    {
        *isInvalid = VI_FALSE;
    }       

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: itscope_SampleRate                                    
 * Purpose:  Returns the sample rate of the instrument. 
 *****************************************************************************/
ViStatus _VI_FUNC itscope_SampleRate (ViSession vi, ViReal64 *sampleRate)
{
    return ( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_SAMPLE_RATE,
                                       IVI_VAL_DIRECT_USER_CALL, sampleRate));
}

/*****************************************************************************
 * Function: itscope_SampleMode
 * Purpose:  Returns the sample mode of the instrument. 
 *****************************************************************************/
ViStatus _VI_FUNC itscope_SampleMode (ViSession vi, ViInt32 *mode)
{
    return ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SAMPLE_MODE,
                                      IVI_VAL_DIRECT_USER_CALL, mode));
}

/*****************************************************************************
 * Function: itscope_ConfigureInitiateContinuous                                     
 * Purpose:  This function configures the oscilloscope to perform a continuous 
 *           acquisition. 
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureInitiateContinuous (ViSession vi,
                                                        ViBoolean continuousAcquisition)
{
    return Ivi_SetAttributeViBoolean (vi, VI_NULL, ITSCOPE_ATTR_INITIATE_CONTINUOUS,
                                      IVI_VAL_DIRECT_USER_CALL, continuousAcquisition); 
}

/*****************************************************************************
 * Function: itscope_ConfigureTriggerModifier                                     
 * Purpose:  This function configures the oscilloscope trigger modifier. 
 *****************************************************************************/
ViStatus _VI_FUNC itscope_ConfigureTriggerModifier (ViSession vi,
                                                    ViInt32 triggerModifier)
{
    return (Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_MODIFIER,
                                     IVI_VAL_DIRECT_USER_CALL, triggerModifier)); 
}

/*****************************************************************************
 * Function: itscope_GetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to get 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession attributes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC itscope_GetAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViInt32 *value)
{                                                                                                           
    return Ivi_GetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           
ViStatus _VI_FUNC itscope_GetAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViReal64 *value)
{                                                                                                           
    return Ivi_GetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC itscope_GetAttributeViString (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViInt32 bufSize, 
                                                 ViChar value[]) 
{   
    return Ivi_GetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     bufSize, value);
}   
ViStatus _VI_FUNC itscope_GetAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViBoolean *value)
{                                                                                                           
    return Ivi_GetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC itscope_GetAttributeViSession (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViSession *value)
{                                                                                                           
    return Ivi_GetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: itscope_SetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to set 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession datatypes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC itscope_SetAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViInt32 value)
{                                                                                                           
    return Ivi_SetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           
ViStatus _VI_FUNC itscope_SetAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViReal64 value)
{                                                                                                           
    return Ivi_SetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC itscope_SetAttributeViString (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViConstString value) 
{   
    return Ivi_SetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}   
ViStatus _VI_FUNC itscope_SetAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViBoolean value)
{                                                                                                           
    return Ivi_SetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC itscope_SetAttributeViSession (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViSession value)
{                                                                                                           
    return Ivi_SetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: itscope_CheckAttribute<type> Functions                                  
 * Purpose:  These functions enable the instrument driver user to check  
 *           attribute values directly.  These functions check the value you
 *           specify even if you set the ITSCOPE_ATTR_RANGE_CHECK 
 *           attribute to VI_FALSE.  There are typesafe versions for ViInt32, 
 *           ViReal64, ViString, ViBoolean, and ViSession datatypes.                         
 *****************************************************************************/
ViStatus _VI_FUNC itscope_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViInt32 value)
{                                                                                                           
    return Ivi_CheckAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value);
}
ViStatus _VI_FUNC itscope_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                   ViAttr attributeId, ViReal64 value)
{                                                                                                           
    return Ivi_CheckAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value);
}                                                                                                           
ViStatus _VI_FUNC itscope_CheckAttributeViString (ViSession vi, ViConstString channelName, 
                                                   ViAttr attributeId, ViConstString value)  
{   
    return Ivi_CheckAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value);
}   
ViStatus _VI_FUNC itscope_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                    ViAttr attributeId, ViBoolean value)
{                                                                                                           
    return Ivi_CheckAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value);
}                                                                                                           
ViStatus _VI_FUNC itscope_CheckAttributeViSession (ViSession vi, ViConstString channelName, 
                                                    ViAttr attributeId, ViSession value)
{                                                                                                           
    return Ivi_CheckAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value);
}                                                                                                           

/**************************************************************************** 
 *  Function: itscope_GetNextCoercionRecord                            
 *  Purpose:  Get the next coercion record from the queue.           
 ****************************************************************************/
ViStatus _VI_FUNC itscope_GetNextCoercionRecord (ViSession vi, ViInt32 bufferSize, ViChar record[])
{
    return Ivi_GetNextCoercionString (vi, bufferSize, record);
}

/*****************************************************************************
 * Function: itscope_LockSession and itscope_UnlockSession Functions                        
 * Purpose:  These functions enable the instrument driver user to lock the 
 *           session around a sequence of driver calls during which other
 *           execution threads must not disturb the instrument state.
 *                                                                          
 *           NOTE:  The callerHasLock parameter must be a local variable 
 *           initialized to VI_FALSE and passed by reference, or you can pass 
 *           VI_NULL.                     
 *****************************************************************************/
ViStatus _VI_FUNC itscope_LockSession (ViSession vi, ViBoolean *callerHasLock)  
{                                              
    return Ivi_LockSession(vi, callerHasLock);      
}                                              
ViStatus _VI_FUNC itscope_UnlockSession (ViSession vi, ViBoolean *callerHasLock) 
{                                              
    return Ivi_UnlockSession(vi, callerHasLock);    
}   

/**************************************************************************** 
 *  Function: itscope_GetNextInterchangeWarning,
 *            itscope_ResetInterchangeCheck, and
 *            itscope_ClearInterchangeWarnings
 *  Purpose:  These functions allow the user to retrieve interchangeability
 *            warnings, reset the driver's interchangeability checking 
 *            state, and clear all previously logged interchangeability warnings.
 ****************************************************************************/
ViStatus _VI_FUNC itscope_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[])
{
    return Ivi_GetNextInterchangeCheckString (vi, bufferSize, warnString);
}

ViStatus _VI_FUNC itscope_ResetInterchangeCheck (ViSession vi)
{
    return Ivi_ResetInterchangeCheck (vi);
}

ViStatus _VI_FUNC itscope_ClearInterchangeWarnings (ViSession vi)
{
    return Ivi_ClearInterchangeWarnings (vi);
}

/*****************************************************************************
 * Function: itscope_GetError and itscope_ClearError Functions                       
 * Purpose:  These functions enable the instrument driver user to  
 *           get or clear the error information the driver associates with the
 *           IVI session.                                                        
 *****************************************************************************/
ViStatus _VI_FUNC itscope_GetError (ViSession vi, 
                                     ViStatus *errorCode, 
                                     ViInt32 bufferSize,
                                     ViChar description[])  
{
    ViStatus error = VI_SUCCESS;
    ViStatus primary = VI_SUCCESS,
             secondary = VI_SUCCESS;
    ViChar   elaboration[256] = "";
    ViChar   errorMessage[1024] = "";
    ViChar  *appendPoint = errorMessage;
    ViInt32  actualSize = 0;
    ViBoolean locked = VI_FALSE;

    /* Lock Session */
    if (vi != 0)
        {
       _checkErr( Ivi_LockSession(vi, &locked));
        }

    /* Test for nulls and acquire error data */
    if (bufferSize != 0)
        {
        if (errorCode == VI_NULL)
            {
            _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error");
            }
        if (description == VI_NULL)
            {
            _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Description");
            }
       _checkErr( Ivi_GetErrorInfo (vi, &primary, &secondary, elaboration));
        }

    else
        {
       _checkErr( Ivi_GetAttributeViString(vi, VI_NULL, IVI_ATTR_ERROR_ELABORATION, 0, 256, elaboration));
       _checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_SECONDARY_ERROR, 0, &secondary));
       _checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_PRIMARY_ERROR, 0, &primary));
        }
        
    /* Format data */
    if (primary != VI_SUCCESS)
        {
        ViChar msg[256] = "";
       _checkErr( itscope_error_message (vi, primary, msg));
        appendPoint += sprintf(appendPoint, "Primary Error: (Hex 0x%08X) %s", primary, msg);
        }
    
    if (secondary != VI_SUCCESS)
        {
        ViChar msg[256] = "";
       _checkErr( itscope_error_message (vi, secondary, msg));
        appendPoint += sprintf(appendPoint, "\nSecondary Error: (Hex 0x%08X) %s", secondary, msg);
        }
    
    if (elaboration[0])
        {
        sprintf(appendPoint, "\nElaboration: %s", elaboration);
        }
    
    actualSize = (ViInt32)strlen(errorMessage) + 1;
    
    /* Prepare return values */
    if (bufferSize == 0)
        {
        error = actualSize;
        }
    else 
        {
        if (bufferSize > 0)
            {
            if (actualSize > bufferSize)
                {
                error = actualSize;
                actualSize = bufferSize;
                }
            }
        memcpy(description, errorMessage, actualSize-1);
        description[actualSize-1] = '\0';
        }
    
    if (errorCode) 
        {
        *errorCode = primary;
        }
    
Error:
    /* Unlock Session */
    Ivi_UnlockSession (vi, &locked);
    return error;
}   

ViStatus _VI_FUNC itscope_ClearError (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}

/*****************************************************************************
 * Function: viWrite and viRead Functions
 * Purpose:  These functions enable the instrument driver user to
 *           write and read commands directly to and from the instrument.
 * 
 *           Notes: 1) These functions bypass the IVI attribute state caching.  
 *                  viWrite invalidates the cached values for all
 *                  attributes.
 *                  2) These functions are supported with the IVI Compliance
 *                  Package (ICP) 4.6.1 and later. If you are using an
 *                  earlier version of ICP, you can still use the itscope_WriteInstrData
 *                  and itscope_ReadInstrData functions under the Obsolete folder.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_viWrite (ViSession vi, ViByte buffer[], ViInt64 count, ViInt64 *returnCount)
{
    return Ivi_viWrite (vi, buffer, count, returnCount);
}
ViStatus _VI_FUNC itscope_viRead (ViSession vi, ViInt64 bufferSize, ViByte buffer[], ViInt64 *returnCount)
{
    return Ivi_viRead (vi, bufferSize, buffer, returnCount);
}

/*****************************************************************************
 * Function: WriteInstrData and ReadInstrData Functions                      
 * Purpose:  These functions enable the instrument driver user to  
 *           write and read commands directly to and from the instrument.            
 *                                                                           
 *           Notes: 1) These functions bypass the IVI attribute state caching.
 *                  WriteInstrData invalidates the cached values for all
 *                  attributes.
 *                  2) These functions are obsolete since IVI Compliance
 *                  Package (ICP) 4.6.1. You can use itscope_viWrite
 *                  and itscope_viRead instead.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_WriteInstrData (ViSession vi, ViConstString writeBuffer)   
{   
    return Ivi_WriteInstrData (vi, writeBuffer);    
}   
ViStatus _VI_FUNC itscope_ReadInstrData (ViSession vi, ViInt32 numBytes, 
                                          ViChar rdBuf[], ViInt32 *bytesRead)  
{   
    return Ivi_ReadInstrData (vi, numBytes, rdBuf, bytesRead);   
} 


/****************************************************************************
 *--------- Obsolete Inherent functions ------------------------------------*
 ****************************************************************************/
/*****************************************************************************
 * Function: itscope_GetErrorInfo and itscope_ClearErrorInfo Functions                       
 * Purpose:  These functions enable the instrument driver user to  
 *           get or clear the error information the driver associates with the
 *           IVI session.                                                        
 *****************************************************************************/
ViStatus _VI_FUNC itscope_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                         ViStatus *secondaryError, ViChar errorElaboration[256])  
{                                                                                                           
    return Ivi_GetErrorInfo(vi, primaryError, secondaryError, errorElaboration);                                
}                                                                                                           

ViStatus _VI_FUNC itscope_ClearErrorInfo (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}

/*****************************************************************************
 *-------------------- Utility Functions (Not Exported) ---------------------*
 *****************************************************************************/

/*****************************************************************************
 *  Function: itscope_BuildChannelList
 *  Purpose:  This function build channel list for special instrument model.
 *****************************************************************************/
static ViStatus itscope_BuildChannelList (ViInt32 model, ViChar channelList[])
{
    ViStatus error = VI_SUCCESS;
    int pos = 0;
    
    if( model == ITSCOPE_VAL_PE510K1)
    {
        pos += sprintf(channelList+pos, "%s,", ANALOG_CHANNEL_LIST_4);
        pos += sprintf(channelList+pos, "%s", VIRTUAL_CHANNEL_LIST_4);
    }  
	else if(model == ITSCOPE_VAL_PE520K1)
    {
        pos += sprintf(channelList+pos, "%s,", ANALOG_CHANNEL_LIST_2);
        pos += sprintf(channelList+pos, "%s", VIRTUAL_CHANNEL_LIST_2);
    }
    else
    {
       _checkErr(IVI_ERROR_INVALID_PARAMETER);  
    }
 
Error:
    return error;   
}

/*****************************************************************************
 *  Function: itscope_GetProbeAttenuation
 *  Purpose:  This function always returns the actual probe attenuation.
 *****************************************************************************/
static ViStatus itscope_GetProbeAttenuation (ViSession vi, 
                                              ViConstString channelName, 
                                              ViReal64 *value)
{
    ViStatus   error = VI_SUCCESS;
    ViReal64   probeAttenuation;
    ViInt32    notProbeSensing;

   _checkErr( Ivi_GetAttributeViReal64 (vi, channelName, 
                                        ITSCOPE_ATTR_PROBE_ATTENUATION,
                                        0, &probeAttenuation));
   _viCheckErr( Ivi_CompareWithPrecision (0, probeAttenuation, 
                                          ITSCOPE_VAL_PROBE_SENSE_ON, 
                                          &notProbeSensing));
    if (notProbeSensing == 0)   /* probe auto sense is turned on */
   _checkErr( Ivi_GetAttributeViReal64 (vi, channelName, 
                                        ITSCOPE_ATTR_PROBE_SENSE_VALUE,
                                        0, &probeAttenuation));
    *value = probeAttenuation;
     
Error:
    return error;
}

/*****************************************************************************
 *  Function: itscope_SetCachingOnProbeDependentAttributes
 *  Purpose:  This function alters the state of the IVI_VAL_NEVER_CACHE
 *            flag of the probe dependent attributes based on the state
 *            parameter.
 *****************************************************************************/
static ViStatus itscope_SetCachingOnProbeDependentAttributes (ViSession vi, 
                                                               ViBoolean cache)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr (itscope_ChangeOneAttributeFlag (vi, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                               IVI_VAL_NEVER_CACHE, (ViBoolean)!cache));
   _checkErr (itscope_ChangeOneAttributeFlag (vi, ITSCOPE_ATTR_VERTICAL_OFFSET, 
                                               IVI_VAL_NEVER_CACHE, (ViBoolean)!cache));

Error:
    return error;
}

/*****************************************************************************
 * Function: itscope_changeRangeWithPrecision                                  
 * Purpose:  This function verifies that the value is not less than the min
 *           and not greater than the max.  The function uses the comparison precision
 *           for the attribute identified by attributeId for the comparison.
 *           If the value is less than min or greater than max, the function
 *           return if the value returns IVI_ERROR_INVALID_VALUE.
 *****************************************************************************/
static ViStatus itscope_CheckRangeWithPrecision (ViSession vi, ViAttr attributeId, 
                                                  ViReal64 value, ViReal64 min, 
                                                  ViReal64 max)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  comparePrecision;
    ViInt32  compareResult;
    
   _checkErr( Ivi_GetAttrComparePrecision (vi, attributeId, &comparePrecision));

   _viCheckErr( Ivi_CompareWithPrecision (comparePrecision, value, min, &compareResult));
    if (compareResult == -1)
       _viCheckErr( IVI_ERROR_INVALID_VALUE);

   _viCheckErr( Ivi_CompareWithPrecision (comparePrecision, value, max, &compareResult));
    if (compareResult == 1)
       _viCheckErr( IVI_ERROR_INVALID_VALUE);

Error:
    return error;
}

/*****************************************************************************
 * Function: itscope_CoerceToResolution                                     
 * Purpose:  Coerce a given value to the next highest step using a given step 
 *           value.                                                                                                    
 *****************************************************************************/
static ViStatus itscope_CoerceToResolution (ViSession vi, ViReal64 value, 
                                             ViReal64 resolution, 
                                             ViInt32 comparePrecision, 
                                             ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 absValue;
    ViReal64 newValue = value;
    ViReal64 resMultiplier;
    ViInt32  compareResult;
    
    if (resolution <= 0.0)
       _viCheckErr( IVI_ERROR_INVALID_VALUE);    

    absValue = fabs (value);

    modf (absValue/resolution, &resMultiplier);
    Ivi_CompareWithPrecision (comparePrecision, resMultiplier, absValue/resolution, &compareResult);
    if (compareResult)
        resMultiplier = ceil (absValue/resolution);

    newValue = resolution * resMultiplier;

    if (value < 0.0)
        newValue *= -1.0;

Error:
    *coercedValue = newValue;
    return error;
}

/*****************************************************************************
 *  Function: itscope_FetchContinuousWaveform
 *  Purpose:  This function fetches a waveform from a specified channel and 
 *            optinally return ActualPoints(Number of points actually acquired
 *            in waveform), InitialX(The time in relation to the Trigger Event
 *            of the first point in the waveform in seconds), XIncrement(The 
 *            time between points in the acquired waveform in seconds). The 
 *            function is used only when the instrument runs in continuous 
 *            mode and is in running state.
 *
 *  Note:     There is no guarantee that the waveform fetched by this function
 *            is the latest one acquired by instrument. While using in loops 
 *            to repeatedly acquiring waveform, it is possible that several
 *            fetched waveform is from the same acquisition.
 *            
 *            Call this function only when ITSCOPE_ATTR_ACQUISITION_TYPE is
 *            ITSCOPE_VAL_NORMAL, ITSCOPE_VAL_HI_RES or ITSCOPE_VAL_AVERAGE.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_FetchContinuousWaveform (ViSession vi, 
                                                    ViConstString channel, 
                                                    ViReal64 waveform[],
                                                    ViInt32 *actualPointsRef,
                                                    ViReal64 *initialXRef, 
                                                    ViReal64 *xIncrementRef)
{
    ViStatus      error = VI_SUCCESS;
    ViInt32       actualPoints;
    ViReal64      xIncrement, initialX;
    ViConstString dataSource;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_CoerceChannelName (vi, channel, &dataSource), 2, "Invalid Channel");

    if (waveform == VI_NULL)
    {    
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Waveform");
    }  
    
        /* Fetch Waveform */
    if (!Ivi_Simulating (vi))                   /* call only when locked */
    {
        ViSession   io = Ivi_IOSession (vi);    /* call only when locked */
        ViReal64    yZero, yOffset, yMultiplier;
        
       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

       _checkErr( itscope_FetchContinuousRawWaveform (vi, io, dataSource, &yZero, &yOffset, &yMultiplier, 
                                                        xIncrementRef, initialXRef, waveform, 
                                                        &actualPoints));
        
        itscope_ScaleRawWaveform (actualPoints, waveform, yZero, yOffset, yMultiplier);
        if(actualPointsRef)
        {
            *actualPointsRef = actualPoints;
        }
    }
    else
    {
        ViInt32     x;
        ViReal64    yRange, simOffset = 0.0;
        ViInt32     triggerSlope, vCoup;
        ViReal64    k, level, theta, offset;
        ViChar      channel[BUFFER_SIZE] = {0};
        ViInt32     waveformSize;

       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                           0, &actualPoints));
       _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, 
                                            ITSCOPE_ATTR_INPUT_DATA_SOURCE, 
                                            0, BUFFER_SIZE, channel) );
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, 
                                            ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &yRange)); 
       _checkErr( Ivi_GetAttributeViInt32 (vi, channel, 
                                           ITSCOPE_ATTR_VERTICAL_COUPLING, 
                                           0, &vCoup));
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, 
                                            ITSCOPE_ATTR_VERTICAL_OFFSET, 
                                            0, &offset)); 
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_TRIGGER_SLOPE, 
                                           0, &triggerSlope));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, 
                                            ITSCOPE_ATTR_TRIGGER_LEVEL, 
                                            0, &level));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, 
                                            ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                            0, &xIncrement));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, 
                                            ITSCOPE_ATTR_ACQUISITION_START_TIME, 
                                            0, &initialX));
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL,
                                            ITSCOPE_ATTR_WAVEFORM_SIZE, 
                                            0, &waveformSize));
                                            
        theta = asin (2*level/yRange);
        if (triggerSlope == ITSCOPE_VAL_POSITIVE)
        {
            k = 1.0;
        }           
        else 
        {
            k = -1.0;
        }           
        if( actualPoints>waveformSize ) 
            actualPoints = waveformSize;  /* Checking number of points to write */
        xIncrement /= actualPoints;

        if (vCoup == ITSCOPE_VAL_DC)
            simOffset = 0.5;
        if (vCoup == ITSCOPE_VAL_GND)
            simOffset = 0.0;     /* i changed this variable from K to simOffset */
            
        for (x = 0; x < actualPoints; x++)
        {
            ViReal64 y = simOffset + k * 2.5 * sin (xIncrement * 12560 * x + k * theta) + /* ~2 periods of 1kHz sinewave */
                            (!(x%20)) * (16384 - rand())/150000.0;
            waveform[x] = (offset + yRange/2) > y ? ((offset - yRange/2) < y ? y : (offset - yRange/2)) : (offset + yRange/2);
        }  
        
        if(actualPointsRef)
        {
            *actualPointsRef = actualPoints;        
        }
        if(initialXRef)
        {
            *initialXRef = initialX;        
        }
        if(xIncrementRef)
        {
            *xIncrementRef = xIncrement;        
        }
    }
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;   
}

/*****************************************************************************
 * Function: itscope_FetchContinuousRawWaveform 
 * Purpose:  This function fetches and returns an unscaled waveform in 
 *           condition that the instrument runs in continuous mode. The 
 *           waveform can be fetched from an input channel(CH1-CH4), a 
 *           reference channel (REF1-REF4), or a math channel MATH. To transfer 
 *           less data than the horizontal record length,  This 
 *           may be desired if Extended Acqusition mode is enabled, but you do 
 *           not want to transfer that much data.
 * NOTE:     Call this only when NOT Simulating.   
 *****************************************************************************/
static ViStatus itscope_FetchContinuousRawWaveform (ViSession vi, ViSession io, 
                                                       ViConstString dataSource,
                                                       ViReal64 *yZero,
                                                       ViReal64 *yOffset, 
                                                       ViReal64 *yMultiplier,
                                                       ViReal64 *xIncrement, 
                                                       ViReal64 *initialX, 
                                                       ViReal64 waveform[],
                                                       ViInt32 *actualPoints)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   dataWidth = 2;
    ViInt32   bytesRead, numDigits, numDataBytes;
    ViInt32   numReadings, totalBytesRead;
    ViInt32   bufferSize;
    ViChar*   rdBuffer = VI_NULL;
    ViReal64  *data = waveform;
    ViBoolean doneWithRead;
    ViInt32   waveformSize;
    ViInt32   lastReadBytes=0; 
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_FETCH_BUFFER_SIZE, 0, &bufferSize) );
   _checkErr( Ivi_Alloc(vi, bufferSize, (ViAddr*)&rdBuffer) );
    
   _checkErr( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_INPUT_DATA_SOURCE, 0, dataSource)); 
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_WIDTH, 0, &dataWidth) ); 

    /* Replace ":WFMPRE:YZE?;:WFMPRE:YMULT?;:WFMPRE:XINCR?;:WFMPRE:XZERO?;" */
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YZERO, 0, yZero) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YOFF, 0, yOffset) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YMULT, 0, yMultiplier) );
    if(xIncrement)
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XINCR, 0, xIncrement) );     
    }
    if(initialX)
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XZERO, 0, initialX) );           
    }
        
        /* Waveform Query */
    //_viCheckErr( viPrintf (io, "%s", ":CURV?"));
	_viCheckErr(_viPrintf(vi, "%s", ":CURV?"));
    
         /* Read how many digits make up the entry for number of bytes */
    //_viCheckErr( viRead (io, rdBuffer, 2, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, 2, &bytesRead));
    rdBuffer[bytesRead] = 0;
    if (Scan (rdBuffer, "%s>#%i", &numDigits) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }
    
        /* Read how many bytes make up the data */
    //_viCheckErr( viRead (io, rdBuffer, numDigits, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, numDigits, &bytesRead));
    rdBuffer[bytesRead] = 0;
    if (Scan (rdBuffer, "%s>%i", &numDataBytes) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }
    
        /* Calculate the Number of Readings */
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_WAVEFORM_SIZE, 0, &waveformSize) );
    numReadings = numDataBytes/dataWidth;
    if (numReadings > waveformSize)
    {
        numReadings = waveformSize;
        numDataBytes = numReadings * dataWidth;
    }
    *actualPoints = numReadings;
    
        /* Read the Data into the Array */
    totalBytesRead = 0;
    doneWithRead = VI_FALSE;
    while (!doneWithRead)
    {
        //_viCheckErr( viRead (io, rdBuffer, bufferSize, &bytesRead));
		_viCheckErr(_viRead(vi, rdBuffer, bufferSize, &bytesRead));

        if (totalBytesRead + bytesRead >= numDataBytes)
        {
            lastReadBytes = bytesRead;
            bytesRead = numDataBytes - totalBytesRead;
            doneWithRead = VI_TRUE;
        }
        
        totalBytesRead += bytesRead;
        if (dataWidth == 1)
        {
            if (Scan (rdBuffer, "%*i[zb1]>%*f",bytesRead, bytesRead, data)!= 1)
            {                       
               _viCheckErr( VI_ERROR_INV_RESPONSE);
            }
        }
        else
        {
            bytesRead /= 2;
        
            if ((Scan (rdBuffer, "%*i[zb2o10]>%*f[b8]", 
                           bytesRead, bytesRead, data)) != 1)
            {
               _viCheckErr( VI_ERROR_INV_RESPONSE);
            }                
        }
        data += bytesRead;
    }
    
    /* Read the extra data and empty the waveform buffer in the oscilloscope */
    if(rdBuffer[lastReadBytes-1] != '\n')
    {       
        //viScanf (io, "%*[^\n]");
		_viScanf(vi, "%*[^\n]");
    }

Error:
    Ivi_Free(vi, rdBuffer);
    return error;   
}
 
/*****************************************************************************
 * Function: itscope_FetchRawWaveform                                       
 * Purpose:  This function fetches and returns an unscaled waveform from the  
 *           instrument. The waveform can be fetched from an input channel   
 *           (CH1-CH4), a reference channel (REF1-REF4), or a math channel   
 *           MATH. To transfer less data than the horizontal record 
 *           length, set the arraySize element in the waveformData parameter 
 *           to the desired amount. This may be desired if Extended          
 *           Acqusition mode is enabled, but you do not want to transfer     
 *           that much data.NOTE: Call this only when NOT Simulating.        
 *****************************************************************************/
static ViStatus itscope_FetchRawWaveform (ViSession vi, ViSession io, 
                                           ViConstString dataSource,
                                           ViReal64 *yZero,
                                           ViReal64 *yOffset, 
                                           ViReal64 *yMultiplier,
                                           ViReal64 *xIncrement, 
                                           ViReal64 *initialX, 
                                           ViInt32 waveformSize, 
                                           ViReal64 waveform[],
                                           ViInt32 *actualPoints)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   acquisitionType, dataWidth = 1;
    ViInt32   bytesRead, numDigits, numDataBytes;
    ViInt32   numReadings, totalBytesRead;
    ViInt32   horzRecordLength, stopIndex;
    ViInt32   bufferSize;
    ViChar*   rdBuffer = VI_NULL;
    ViReal64  *data = waveform;
    ViBoolean doneWithRead;
    ViInt32   lastReadBytes=0;
	ViReal64  v_range;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_FETCH_BUFFER_SIZE, 0, &bufferSize) );
   _checkErr( Ivi_Alloc(vi, bufferSize, (ViAddr*)&rdBuffer) );
    
   _checkErr( Ivi_SetAttributeViString (vi, VI_NULL, 
                                        ITSCOPE_ATTR_INPUT_DATA_SOURCE, 
                                        0, dataSource));

   _checkErr( Ivi_GetAttributeViInt32  (vi, VI_NULL, 
                                        ITSCOPE_ATTR_ACQUISITION_TYPE, 
                                        0, &acquisitionType));

   _checkErr(Ivi_GetAttributeViReal64(vi, dataSource,
										ITSCOPE_ATTR_VERTICAL_RANGE,
										0, &v_range));

    switch (acquisitionType) 
    {
        case ITSCOPE_VAL_NORMAL:
        {
            dataWidth = 2;
            break;
        }            
        case ITSCOPE_VAL_AVERAGE:
        case ITSCOPE_VAL_HI_RES:
        {
            dataWidth = 2;
            break;
        }
        default:
        {
            _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                                "Use FetchMinMaxWaveform for acquisition types "
                                "Peak Detect or Envelope.");
        }        
    }

    if( strncmp (dataSource, ITSCOPE_VAL_CH_1, (strlen(dataSource)+1)) == 0 
        || strncmp (dataSource, ITSCOPE_VAL_CH_2, (strlen(dataSource)+1)) == 0 
            || strncmp (dataSource, ITSCOPE_VAL_CH_3, (strlen(dataSource)+1)) == 0
                || strncmp (dataSource, ITSCOPE_VAL_CH_4, (strlen(dataSource)+1)) == 0 
                    || strncmp (dataSource, ITSCOPE_VAL_MATH, (strlen(dataSource)+1)) == 0 )
    {
        /* If CH1 ~ CH4 is specified, get record length from the ITSCOPE_ATTR_HORZ_RECORD_LENGTH attribute */
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                                                    0, &horzRecordLength));
    }
    else
    {
        /* If REF1 ~ REF4 is specified, get record length from the "WFMPRE:NR_PT?" command */
        //_viCheckErr( viQueryf(io, "WFMPRE:NR_PT?", "%ld", &horzRecordLength) );
		_viCheckErr(_viQueryf(vi, ":WFMPRE:NR_PT?", "%ld", &horzRecordLength));
    }

    if (horzRecordLength > waveformSize)
    {
        stopIndex = waveformSize;
    }        
    else
    {
        stopIndex = horzRecordLength;
    }   
    
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_WIDTH, 0, dataWidth));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_START_POS, 0, 1));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_STOP_POS, 0, stopIndex));
    
    /* Replace with cache attributes
   _viCheckErr( viQueryf (io, ":WFMPRE:YZE?;:WFMPRE:YOFF?;:WFMPRE:YMULT?;:WFMPRE:XINCR?;:WFMPRE:XZERO?;",
                          "%le;%le;%le;%le", yZero, yOffset, yMultiplier, xIncrement, initialX));
    */
        /* Replace ":WFMPRE:YZE?;:WFMPRE:YOFF?;:WFMPRE:YMULT?;:WFMPRE:XINCR?;:WFMPRE:XZERO?;" */
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YZERO, 0, yZero) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YOFF, 0, yOffset) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YMULT, 0, yMultiplier) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XINCR, 0, xIncrement) );     
   _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XZERO, 0, initialX) );           
                          
        /* Waveform Query */
    //_viCheckErr( viPrintf (io, "%s", ":CURV?"));
	_viCheckErr(_viPrintf(vi, "%s", ":CURV?"));

        /* Read how many digits make up the entry for number of bytes */
    //_viCheckErr( viRead (io, rdBuffer, 2, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, 2, &bytesRead));
    rdBuffer[bytesRead] = 0 ;
    if (Scan (rdBuffer, "%s>#%i", &numDigits) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }
    
        /* Read how many bytes make up the data */
    //_viCheckErr( viRead (io, rdBuffer, numDigits, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, numDigits, &bytesRead));
    rdBuffer[bytesRead] = 0;
    if (Scan (rdBuffer, "%s>%i", &numDataBytes) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }
    
        /* Calculate the Number of Readings */
    numReadings = numDataBytes/dataWidth;
    if (numReadings > waveformSize)
    {
        numReadings = waveformSize;
        numDataBytes = numReadings * dataWidth;
    }
    *actualPoints = numReadings;  
    
        /* Read the Data into the Array */
    totalBytesRead = 0;
    doneWithRead = VI_FALSE;
    while (!doneWithRead)
    {
        //_viCheckErr( viRead (io, rdBuffer, bufferSize, &bytesRead));
		_viCheckErr(_viRead(vi, rdBuffer, bufferSize, &bytesRead));

        if (totalBytesRead + bytesRead >= numDataBytes)
        {
            lastReadBytes = bytesRead;
            bytesRead = numDataBytes - totalBytesRead;
            doneWithRead = VI_TRUE;
        }
        
        totalBytesRead += bytesRead;
		ViInt32 samples = bytesRead / dataWidth;
		if (dataWidth == 1)
        {
            if ((Scan (rdBuffer, "%*i[zb1]>%*f", 
                       samples, samples, data)) != 1)
            {                       
               _viCheckErr( VI_ERROR_INV_RESPONSE);
            }
        }
        else  if (dataWidth == 2)
        {
			short *rbuf = (short*)rdBuffer;
			if ((Scan(rdBuffer, "%*i[zb2]>%*f",
                       samples, samples, data)) != 1)
            {
               _viCheckErr( VI_ERROR_INV_RESPONSE);
            }                
			for (int ii = 0; ii < samples; ii++) {	// samples to volts
				data[ii] = ((v_range/2)*data[ii]) / 32768.0;
			}
		}
        data += samples;
    }

    /* Read the extra data and empty the waveform buffer in the oscilloscope */
    if(rdBuffer[lastReadBytes-1] != '\n')
    {       
        //viScanf (io, "%*[^\n]");
		_viScanf(vi, "%*[^\n]");
    }
    
Error:
    Ivi_Free(vi, rdBuffer);
    return error;
}

/*****************************************************************************
 *  Function: itscope_FetchContinuousMinMaxWaveform                           
 *  Purpose:  This function fetches a min and max waveform from a waveform 
 *            acquisition that the user or driver previously initiated on a 
 *            specific channel.                     
 *  Note:     Call this function only when ITSCOPE_ATTR_ACQUISITION_TYPE
 *            is ITSCOPE_VAL_PEAK_DETECT or ITSCOPE_VAL_ENVELOPE.
 *****************************************************************************/
ViStatus _VI_FUNC itscope_FetchContinuousMinMaxWaveform (ViSession vi,
                                                    ViConstString channel, 
                                                    ViReal64 minWaveform[], 
                                                    ViReal64 maxWaveform[], 
                                                    ViInt32 *actualPointsRef, 
                                                    ViReal64 *initialXRef, 
                                                    ViReal64 *xIncrementRef)
{
    ViStatus        error = VI_SUCCESS;
    ViInt32         actualPoints;
    ViReal64        xIncrement, initialX;
    ViConstString   dataSource;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

    _viCheckParm( Ivi_CoerceChannelName (vi, channel, &dataSource), 2, "Invalid Channel");

    if (minWaveform == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for minWaveform");
    }
    
    if (maxWaveform == VI_NULL)
    {
        _viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for maxWaveform");
    }
    
        /* Fetch Min and Max Waveforms */
    if (!Ivi_Simulating (vi))               /* call only when locked */
    {
        ViSession   io = Ivi_IOSession (vi);    /* call only when locked */
        ViReal64    yZero, yOffset, yMultiplier;
        
       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
                                                   
       _checkErr( itscope_FetchContinuousRawMinMaxWaveform (vi, io, dataSource, &yZero, &yOffset, &yMultiplier,
                                                                xIncrementRef, initialXRef, 
                                                                minWaveform, maxWaveform,
                                                                &actualPoints));
        
        itscope_ScaleRawWaveform (actualPoints, minWaveform, yZero, yOffset, yMultiplier);
        itscope_ScaleRawWaveform (actualPoints, maxWaveform, yZero, yOffset, yMultiplier);
        if(actualPointsRef)   
        {
            *actualPointsRef = actualPoints;     
        }
    }
    else
    {
        ViInt32     x;
        ViReal64    yRange, k, theta, level;
        ViInt32     triggerSlope;
        ViChar      channel[BUFFER_SIZE] = {0};
        ViInt32     waveformSize;
        
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS, 
                                           0, &actualPoints));
       _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, 
                                            ITSCOPE_ATTR_INPUT_DATA_SOURCE, 
                                            0, BUFFER_SIZE, channel) );
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &yRange)); 
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SLOPE, 
                                           0, &triggerSlope));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_LEVEL, 
                                            0, &level));
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                            0, &xIncrement));
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_WAVEFORM_SIZE,
                                            0, &waveformSize));
                                            
        theta = asin (2*level/yRange);
        if (triggerSlope == ITSCOPE_VAL_POSITIVE)
            k = 1.0;
        else 
            k = -1.0;

        initialX = 0.0;
        actualPoints /= 4;
        if( actualPoints>waveformSize ) 
            actualPoints = waveformSize;  /* Checking number of points to write */
        xIncrement *= 4;
        for (x = 0; x < actualPoints; x++) 
        {
                /* add glitch to the every 20th element in the waveform array */
            maxWaveform[x] = k * yRange * .49 * sin (xIncrement * x + k * theta)
                            + (!(x%20)) * rand()/150000.0;
            minWaveform[x] = k * yRange * .51 * sin (xIncrement * x + k * theta)
                            - (!(x%20)) * rand()/160000.0;
        }
        
        if(actualPointsRef)
        {
            *actualPointsRef = actualPoints;         
        }
        if(xIncrementRef)
        {
            *xIncrementRef = xIncrement;         
        }
        if(initialXRef)
        {
            *initialXRef = initialX;        
        }
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: itscope_FetchContinuousRawMinMaxWaveform 
 * Purpose:  
 *****************************************************************************/
static ViStatus itscope_FetchContinuousRawMinMaxWaveform (ViSession vi, ViSession io,
                                                             ViConstString dataSource,
                                                             ViReal64 *yZero,
                                                             ViReal64 *yOffset, 
                                                             ViReal64 *yMultiplier,
                                                             ViReal64 *xIncrement, 
                                                             ViReal64 *initialX, 
                                                             ViReal64 minWaveform[],
                                                             ViReal64 maxWaveform[],
                                                             ViInt32 *actualPoints)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   dataWidth = 1;
    ViInt32   bytesRead, numDigits, numDataBytes;
    ViInt32   totalBytesRead;
    ViInt32   bufferSize;
    ViChar*   rdBuffer = VI_NULL;
    ViBoolean doneWithRead;
    ViInt32   waveformSize;
    ViReal64  *minData, *maxData; 
    ViInt32   counter = 0;   
    ViInt32   lastReadBytes=0;
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_FETCH_BUFFER_SIZE, 0, &bufferSize) );
   _checkErr( Ivi_Alloc(vi, bufferSize, (ViAddr*)&rdBuffer) );

   _checkErr( Ivi_SetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_INPUT_DATA_SOURCE, 0, dataSource));   
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_WIDTH, 0, &dataWidth) ); 

    /* Replace ":WFMPRE:YZE?;:WFMPRE:YMULT?;:WFMPRE:XINCR?;:WFMPRE:XZERO?;" */
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YZERO, 0, yZero) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YOFF, 0, yOffset) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YMULT, 0, yMultiplier) );
    if(xIncrement)
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XINCR, 0, xIncrement) );     
    }
    if(initialX)
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XZERO, 0, initialX) );           
    }
        
        /* Waveform Query */
    //_viCheckErr( viWrite (io, ":CURV?", 6, VI_NULL));
	_viCheckErr(_viPrintf(vi, "%s", ":CURV?"));

        /* Read how many digits make up the entry for number of bytes */
    //_viCheckErr( viRead (io, rdBuffer, 2, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, 2, &bytesRead));
    rdBuffer[bytesRead]=0;
    if (Scan (rdBuffer, "%s>#%i", &numDigits) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }       
        
        /* Read how many bytes make up the data */
    //_viCheckErr( viRead (io, rdBuffer, numDigits, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, numDigits, &bytesRead));
    rdBuffer[bytesRead]=0;
    if (Scan (rdBuffer, "%s>%i", &numDataBytes) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }       
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_WAVEFORM_SIZE, 0, &waveformSize) );
    waveformSize *= 2;
    if (numDataBytes > waveformSize)
    {
        numDataBytes = waveformSize;
    }
    *actualPoints = numDataBytes / 2; /*return the fact that we are giving half */
                                          /*as many points as databytes read        */
    
        /* Read Data into Arrays */
    totalBytesRead = 0;
    doneWithRead = VI_FALSE;
    minData = minWaveform;
    maxData = maxWaveform;
    while (!doneWithRead)
    {
        ViInt32 i;

        //_viCheckErr( viRead (io, rdBuffer, bufferSize, &bytesRead));
		_viCheckErr(_viRead(vi, rdBuffer, bufferSize, &bytesRead));
        
        if (totalBytesRead + bytesRead >= numDataBytes)
        {
            lastReadBytes = bytesRead;
            bytesRead = numDataBytes - totalBytesRead;
            doneWithRead = VI_TRUE;
        }
        
        totalBytesRead += bytesRead;

        for (i=0; i < bytesRead; i += 2)
        {
            minData[counter + (i/2)] = (ViReal64) rdBuffer[i];
            maxData[counter + (i/2)] = (ViReal64) rdBuffer[i+1];
        }
        counter += i/2;
    }

    /* Read the extra data and empty the waveform buffer in the oscilloscope */
    if(rdBuffer[lastReadBytes-1] != '\n')
    {       
        //viScanf (io, "%*[^\n]");
		_viScanf(vi, "%*[^\n]");
    }   
    
Error:
    Ivi_Free(vi, rdBuffer);
    return error;   
}    
    
/*****************************************************************************
 * Function: itscope_FetchRawMinMaxWaveform                                 
 * Purpose:  This function fetches and returns an unscaled waveform from the 
 *           instrument. The waveform can be fetched from an input channel   
 *           (CH1-CH4), a reference channel (REF1-REF4), or a math channel   
 *           MATH. To transfer less data than the horizontal record 
 *           length, set the arraySize element in the waveformData parameter 
 *           to the desired amount. This may be desired if Extended          
 *           Acqusition mode is enabled, but you do not want to transfer     
 *           that much data.                                                 
 *           NOTE: Call thisonly when ITSCOPE_ATTR_ACQUISITION_TYPE is      
 *           ITSCOPE_VAL_PEAK_DETECT or ITSCOPE_VAL_ENVELOPE.              
 *           NOTE: Call this only when NOT Simulating.                       
 *****************************************************************************/
static ViStatus itscope_FetchRawMinMaxWaveform (ViSession vi, ViSession io,
                                                 ViConstString dataSource,
                                                 ViReal64 *yZero,
                                                 ViReal64 *yOffset, 
                                                 ViReal64 *yMultiplier,
                                                 ViReal64 *xIncrement, 
                                                 ViReal64 *initialX, 
                                                 ViInt32 waveformSize,
                                                 ViReal64 minWaveform[],
                                                 ViReal64 maxWaveform[],
                                                 ViInt32 *actualPoints)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     horzRecordLength, stopIndex;
    ViInt32     numDigits, numDataBytes, totalBytesRead, bytesRead;
    ViInt32     counter = 0;
    ViInt32     bufferSize;
    ViChar*     rdBuffer = VI_NULL;
    ViReal64    *minData, *maxData;
    ViBoolean   doneWithRead;
    ViInt32     lastReadBytes=0;

    /* Assign Buffer */
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_FETCH_BUFFER_SIZE, 0, &bufferSize) );
   _checkErr( Ivi_Alloc(vi, bufferSize, (ViAddr*)&rdBuffer) );    
    
   _checkErr( Ivi_SetAttributeViString (vi, VI_NULL, 
                                        ITSCOPE_ATTR_INPUT_DATA_SOURCE, 
                                        0, dataSource));
                                        
    if( strncmp (dataSource, ITSCOPE_VAL_CH_1, (strlen(dataSource)+1)) == 0 
        || strncmp (dataSource, ITSCOPE_VAL_CH_2, (strlen(dataSource)+1)) == 0 
            || strncmp (dataSource, ITSCOPE_VAL_CH_3, (strlen(dataSource)+1)) == 0
                || strncmp (dataSource, ITSCOPE_VAL_CH_4, (strlen(dataSource)+1)) == 0 
                    || strncmp (dataSource, ITSCOPE_VAL_MATH, (strlen(dataSource)+1)) == 0 )
    {
        /* If CH1 ~ CH4 is specified, get record length from the ITSCOPE_ATTR_HORZ_RECORD_LENGTH attribute */  
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                           0, &horzRecordLength));
    }
    else
    {
        /* If REF1 ~ REF4 is specified, get record length from the "WFMPRE:NR_PT?" command */
        //_viCheckErr( viQueryf(io, "WFMPRE:NR_PT?", "%ld", &horzRecordLength) );
		_viCheckErr(_viQueryf(vi, ":WFMPRE:NR_PT?", "%ld", &horzRecordLength));
    }                                        

    horzRecordLength *= 2; /*this is necessary as the record length is half of the instrument value*/
    waveformSize     *= 2;
    if (horzRecordLength > waveformSize)
    {
        stopIndex = waveformSize;
    }       
    else
    {
        stopIndex = horzRecordLength;
    }       

    /* There is stopIndex points in both min and max waveforms. */
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_WIDTH, 0, 2));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_START_POS, 0, 1));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_STOP_POS, 0, stopIndex));    
 
    /* Replace with cache attributes
   _viCheckErr( viQueryf (io, ":WFMPRE:YZE?;:WFMPRE:YOFF?;:WFMPRE:YMULT?;:WFMPRE:XINCR?;:WFMPRE:XZERO?;",
                          "%le;%le;%le;%le", yZero, yOffset, yMultiplier, xIncrement, initialX));
    */
    
    /* Replace ":WFMPRE:YZE?;:WFMPRE:YOFF?;:WFMPRE:YMULT?;:WFMPRE:XINCR?;:WFMPRE:XZERO?;" */
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YZERO, 0, yZero) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YOFF, 0, yOffset) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, dataSource, ITSCOPE_ATTR_DATA_YMULT, 0, yMultiplier) );
   _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XINCR, 0, xIncrement) );     
   _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_DATA_XZERO, 0, initialX) );                          
                          
        /* Waveform Query */
    //_viCheckErr( viWrite (io, ":CURV?", 6, VI_NULL));
	_viCheckErr(_viPrintf(vi, "%s", ":CURV?"));

        /* Read how many digits make up the entry for number of bytes */
    //_viCheckErr( viRead (io, rdBuffer, 2, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, 2, &bytesRead));

    rdBuffer[bytesRead]=0;
    if (Scan (rdBuffer, "%s>#%i", &numDigits) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }       
        
        /* Read how many bytes make up the data */
    //_viCheckErr( viRead (io, rdBuffer, numDigits, &bytesRead));
	_viCheckErr(_viRead(vi, rdBuffer, numDigits, &bytesRead));
    rdBuffer[bytesRead]=0;
    if (Scan (rdBuffer, "%s>%i", &numDataBytes) != 1) 
    {
       _viCheckErr( VI_ERROR_INV_RESPONSE);
    }       
    
    if (numDataBytes > waveformSize)
    {
        numDataBytes = waveformSize;
    }
    *actualPoints = numDataBytes / 2; /*return the fact that we are giving half */
                                      /*as many points as databytes read        */
        
        /* Read Data into Arrays */
    totalBytesRead = 0;
    doneWithRead = VI_FALSE;
    minData = minWaveform;
    maxData = maxWaveform;
    while (!doneWithRead)
    {
        ViInt32 i;

        //_viCheckErr( viRead (io, rdBuffer, bufferSize, &bytesRead));
		_viCheckErr(_viRead(vi, rdBuffer, bufferSize, &bytesRead));

        if (totalBytesRead + bytesRead >= numDataBytes)
        {
            lastReadBytes = bytesRead;
            bytesRead = numDataBytes - totalBytesRead;
            doneWithRead = VI_TRUE;
        }
        
        totalBytesRead += bytesRead;

        for (i=0; i < bytesRead; i += 2)
        {
            minData[counter + (i/2)] = (ViReal64) rdBuffer[i];
            maxData[counter + (i/2)] = (ViReal64) rdBuffer[i+1];
        }
        counter += i/2;
    }

    /* Read the extra data and empty the waveform buffer in the oscilloscope */
    if(rdBuffer[lastReadBytes-1] != '\n')
    {       
        //viScanf (io, "%*[^\n]");
		_viScanf(vi, "%*[^\n]");
    }
    
Error:
    Ivi_Free(vi, rdBuffer);
    return error;
}

/*****************************************************************************
 * Function: itscope_ScaleRawWaveform                                       
 * Purpose:  This function scales a waveform retrieved from the oscilloscope 
 *           with one of the functions itscope_FetchRawWaveform or          
 *           itscope_FetchRawMinMaxWaveform.                                   
 *           NOTE: Call this only when NOT simulating.                          
 *****************************************************************************/
static void itscope_ScaleRawWaveform (ViInt32 waveformSize, ViReal64 waveform[],
                                       ViReal64 yZero, ViReal64 yOffset, ViReal64 yMultiplier)
                                
{
    ViInt32   i;
    ViReal64  *rawData = waveform;

    for (i=0; i< waveformSize; i++) 
    /*  waveform[i] = rawData[i] * yMultiplier + yOffset;        */
        waveform[i] = (rawData[i]-yOffset)*yMultiplier+yZero;      
}

/*****************************************************************************
 * Function: itscopeResolutionOne_CoerceCallback                                       
 * Purpose:  This coerce callback is used by attributes whose resoluton is 1.0 
 *****************************************************************************/
static ViStatus _VI_FUNC itscopeResolutionOne_CoerceCallback (ViSession vi, 
                                                               ViConstString channelName, 
                                                               ViAttr attributeId, 
                                                               ViReal64 value, 
                                                               ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;

   _checkErr (itscope_CoerceToResolution (vi, value, 1.0, 14, &newValue));

Error:
    *coercedValue = newValue;
    return error;
}

/*****************************************************************************
 * Function: itscopeStdViInt32_WriteCallback                                       
 * Purpose:  This write callback is used by attributes with a range table that
 *           contains the base GPIB string in the customInfo parameter. This 
 *           function supports mulit-channel attributes whose GPIB command
 *           follows the pattern: :FUNCTION:CH<X> ARG 
  *****************************************************************************/
static ViStatus _VI_FUNC itscopeStdViInt32_WriteCallback (ViSession vi, 
                                                           ViSession io, 
                                                           ViConstString channelName, 
                                                           ViAttr attributeId, 
                                                           ViInt32 value)
{
    ViStatus         error = VI_SUCCESS;
    ViString         cmdStr;
    IviRangeTablePtr rangeTable;
    ViConstString    commandSeparator;
    ViConstString    channelStr;
    
    if (channelName == VI_NULL || *channelName == '\0')
    {
        commandSeparator = "";
        channelStr = "";
    }
    else
    {
        commandSeparator = ":";
        channelStr = channelName;
    }

   _checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));

    if (rangeTable->type == IVI_VAL_DISCRETE)
    {
       _viCheckErr( Ivi_GetViInt32EntryFromValue (value, rangeTable, 
              VI_NULL, VI_NULL, VI_NULL, VI_NULL, &cmdStr, VI_NULL));
             
       _viCheckErr( _viPrintf (vi, "%s%s%s %s;", rangeTable->customInfo, 
                              commandSeparator, channelStr, cmdStr));
    }
    else 
       _viCheckErr( _viPrintf (vi, "%s%s%s %ld;", rangeTable->customInfo, 
                              commandSeparator, channelStr, value));

Error:
    return error;
}

/*****************************************************************************
 * Function: itscopeStdViInt32_ReadCallback                                       
 * Purpose:  This write callback is used by attributes with a range table that
 *           contains the base command string in the userData parameter. This 
 *           function supports mulit-channel attributes whose command
 *           follows the pattern: :FUNCTION:CH<X>? 
 *****************************************************************************/
static ViStatus _VI_FUNC itscopeStdViInt32_ReadCallback (ViSession vi, 
                                                          ViSession io, 
                                                          ViConstString channelName, 
                                                          ViAttr attributeId, 
                                                          ViInt32 *value)
{
    ViStatus         error = VI_SUCCESS;
    ViChar           buf[BUFFER_SIZE]="";
    IviRangeTablePtr rangeTable;

	_checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));

    if (channelName == VI_NULL || *channelName == '\0')
    {
       _viCheckErr( _viPrintf (vi, "%s?", rangeTable->customInfo));

    }
    else 
    {
       _viCheckErr( _viPrintf (vi, "%s:%s?", rangeTable->customInfo, channelName));
    }
  
   _viCheckErr( _viRead (vi, buf, BUFFER_SIZE, VI_NULL));  
    
    if (rangeTable->type == IVI_VAL_DISCRETE)
    {
		if (Ivi_GetViInt32EntryFromString (buf, rangeTable, value, VI_NULL, VI_NULL, VI_NULL, VI_NULL) < VI_SUCCESS)
	    {
	       _viCheckErr( VI_ERROR_INV_RESPONSE);
	    }
    }
	
    else 
    {
        if (Scan (buf, "%s>%i", value) != 1)
        {
           _viCheckErr( VI_ERROR_INV_RESPONSE);
        }
    }
    
Error:
    return error;
}

/*****************************************************************************
 * Function: itscopeStdViReal64_ReadCallback                                       
 * Purpose:  This write callback is used by attributes with a range table    
 *           that contains the base GPIB string in the userData parameter.   
 *           This function supports multi-channel attributes whose GPIB      
 *           command follows the pattern: :FUNCTION:CH<X> ARG                
 *****************************************************************************/
static ViStatus _VI_FUNC itscopeStdViReal64_ReadCallback (ViSession vi, 
                                                           ViSession io, 
                                                           ViConstString channelName, 
                                                           ViAttr attributeId, 
                                                           ViReal64 *value)
{
    ViStatus            error = VI_SUCCESS;
    ViChar              rdBuffer[BUFFER_SIZE];
    IviRangeTablePtr    rangeTable;

	_checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));

    if (channelName == VI_NULL || *channelName == '\0')
    {
        //_viCheckErr( viPrintf (io, "%s?", rangeTable->customInfo));
		_viCheckErr(_viPrintf( vi, "%s?", rangeTable->customInfo));
    }
    else 
    {
        //_viCheckErr( viPrintf (io, "%s:%s?", rangeTable->customInfo, channelName));
		_viCheckErr(_viPrintf(vi, "%s:%s?", rangeTable->customInfo, channelName));
    }
    
    //_viCheckErr( viRead (io, rdBuffer, BUFFER_SIZE, VI_NULL));  
	_viCheckErr(_viRead(vi, rdBuffer, BUFFER_SIZE, VI_NULL));

    if (Scan (rdBuffer, "%s>%f[e]", value) != 1)
       _viCheckErr( VI_ERROR_INV_RESPONSE);

Error:
    return error;
}

/*****************************************************************************
 * Function: itscopeStdViReal64_WriteCallback                                       
 * Purpose:  This write callback is used by attributes with a range table    
 *           that contains the base GPIB string in the userData parameter.   
 *           This function supports multi-channel attributes whose GPIB      
 *           command follows the pattern: :FUNCTION:CH<X> ARG?               
 *****************************************************************************/
static ViStatus _VI_FUNC itscopeStdViReal64_WriteCallback (ViSession vi, 
                                                            ViSession io, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViReal64 value)
{
    ViStatus         error = VI_SUCCESS;
    ViChar           wrBuffer[BUFFER_SIZE];
    IviRangeTablePtr rangeTable;
    ViInt32          numBytes;

   _checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));

    Fmt (wrBuffer, "%s<%s", rangeTable->customInfo);
    numBytes = NumFmtdBytes();

    if (channelName != VI_NULL && *channelName != '\0')
    {
        Fmt (wrBuffer, "%s[a]<:%s", channelName);
        numBytes += NumFmtdBytes();
    }
    Fmt (wrBuffer, "%s[a]< %f[e]", value);
    numBytes += NumFmtdBytes();
    
    //_viCheckErr(viBufWrite (io, wrBuffer, numBytes, VI_NULL));
	_viCheckErr(_viBufWrite(vi, wrBuffer, numBytes, VI_NULL));
    
Error:
    return error;
}

/****************************************************************************** 
 * Function: itscopeStdViStringSource_CheckCallback
 * Purpose:  This callback function checks that a given value is included in the
 *           range table associated with a string attribute.
 ******************************************************************************/
static ViStatus _VI_FUNC itscopeStdViStringSource_CheckCallback (ViSession vi, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value)
{
    ViStatus         error = VI_SUCCESS;
    ViConstString    coercedValue;
    IviRangeTablePtr rangeTable;

	_checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));

    error = Ivi_CoerceChannelName (vi, value, &coercedValue);
    if (error < VI_SUCCESS) 
    {
        coercedValue = value;
    }
    
   _viCheckErr( Ivi_GetViInt32EntryFromString (coercedValue, rangeTable, 
                                               VI_NULL, VI_NULL, VI_NULL, 
                                               VI_NULL, VI_NULL));

Error:
    return error;
}
                                           
/****************************************************************************** 
 * Function: itscopeStdViStringSource_WriteCallback
 * Purpose:  This write callback is used by string attributes that contain the 
 *           base GPIB command in the userData element of the associated range 
 *           table.
 ******************************************************************************/    
static ViStatus _VI_FUNC itscopeStdViStringSource_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value)
{
    ViStatus         error = VI_SUCCESS;
    ViString         sourceCmdString;
    IviRangeTablePtr rangeTable;

    error = itscope_CoerceStringSourceToCmd (vi, value, &sourceCmdString);
    if(error < VI_SUCCESS && attributeId ==ITSCOPE_ATTR_MEAS_SOURCE)
    {
        if(strcmp(value,ITSCOPE_VAL_HISTOGRAM) == 0)
        {
            error = VI_SUCCESS;
        }
    }
   _checkErr( error);
   _checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));
    //_viCheckErr( viPrintf (io, "%s %s", rangeTable->customInfo, sourceCmdString));
	_viCheckErr(_viPrintf(vi, "%s %s", rangeTable->customInfo, sourceCmdString));
    
Error:
    return error;
}
    
/****************************************************************************** 
 * Function: itscopeStdViStringSource_ReadCallback
 * Purpose:  This function coerces a given source name to a string that can be  
 *           used by the driver.
 ******************************************************************************/
static ViStatus _VI_FUNC itscopeStdViStringSource_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 const ViConstString cacheValue)
{
    ViStatus         error = VI_SUCCESS;
    ViChar           rdBuffer[BUFFER_SIZE];
    size_t           bufSize = sizeof(rdBuffer);
    ViConstString    userChannelName;
    IviRangeTablePtr rangeTable;

	_checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));

    //_viCheckErr( viQueryf (io, "%s?", "%#s", rangeTable->customInfo, &bufSize, rdBuffer));
	_viCheckErr(_viQueryf(vi, "%s?", "%#s", rangeTable->customInfo, &bufSize, rdBuffer));

    error = itscope_GetStr1PtrFromStr2 (triggerSourceStringTable, &userChannelName, 
                                         rdBuffer);
    
    if (error < VI_SUCCESS)
        error = Ivi_GetUserChannelName(vi, rdBuffer, &userChannelName);
    
    if (error < VI_SUCCESS)
        error = VI_ERROR_INV_RESPONSE;
                
   _viCheckErr(error);

   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, userChannelName));

Error:
    return error;
}

/****************************************************************************** 
 * Function: itscope_CoerceStringSourceToCmd
 * Purpose:  This function coerces a given source name to a string that can be  
 *           used by the driver.
 ******************************************************************************/ 
static ViStatus itscope_CoerceStringSourceToCmd (ViSession vi, ViConstString source, 
                                                  ViConstString *sourceCmdString)
{
    ViStatus error = VI_SUCCESS;

    error = itscope_GetStr2PtrFromStr1 (triggerSourceStringTable, 
                                         sourceCmdString, source);
    if (error < VI_SUCCESS)
    {
        error =  itscope_GetStr2PtrFromStr1(measurementSourceStringTable, 
                                         sourceCmdString, source);
        if(error < VI_SUCCESS)
        {
           _checkErr( Ivi_CoerceChannelName (vi, source, sourceCmdString));
        }
    }
Error:
    return error;
}

/*****************************************************************************
 *  Function: itscope_ChangeOneAttributeFlag
 *  Purpose:  This function alters a single attribute flag.
 *            Pass the flag in the flagBit parameter.
 *            Pass the new state of the flag in the flagState parameter, the
 *            value VI_TRUE sets a flag bit, the value of VI_FALSE resets it.
 *****************************************************************************/
static ViStatus itscope_ChangeOneAttributeFlag (ViSession vi, ViAttr attributeId, 
                                                 ViInt32 flagBit, ViBoolean flagState)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     flags;
    
   _checkErr( Ivi_GetAttributeFlags (vi, attributeId, &flags));
    
    /* 
        Changed this so that VI_FALSE resets the flag bit and 
        VI_TRUE sets it. Using inverse logic seemed misleading 
    */
    
    flags = flagState ? (flags|flagBit) : (flags&~flagBit);
   _checkErr( Ivi_SetAttributeFlags (vi, attributeId, flags));
    
Error:
    return error;
}

/*****************************************************************************
 *  Function: itscope_GetStr1PtrFromStr2
 *  Purpose:  This function searches the string/string table the driver
 *            defines with the itscopeStringTable type. 
 *            Use this function when you know the string in the second table 
 *            column. Function returns the pointer to the corresponding string
 *            in the first column.
 *****************************************************************************/
static ViStatus itscope_GetStr1PtrFromStr2 (itscopeStringTable strTable, 
                                             ViConstString *str1, ViConstString str2)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     i = 0;
    ViBoolean   found = VI_FALSE;
    
    *str1 = VI_NULL;
    while (!found && strTable[i].col1)
    {
        if (strcmp (strTable[i].col2, str2) == 0)
        {
            *str1 = strTable[i].col1;
            found = VI_TRUE;
        }
        i++;
    }
    if (!found)
       _checkErr( IVI_ERROR_INVALID_VALUE);
Error:  
    return error;
}

/*****************************************************************************
 *  Function: itscope_GetStr2PtrFromStr1
 *  Purpose:  This function searches the string/string table the driver
 *            defines with the itscopeStringTable type. 
 *            Use this function when you know the string in the first table 
 *            column. Function returns the pointer to the corresponding string
 *            in the second column.
 *****************************************************************************/
static ViStatus itscope_GetStr2PtrFromStr1 (itscopeStringTable strTable, 
                                             ViConstString *str2, ViConstString str1)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  i=0;
    ViBoolean found = VI_FALSE;
    
    *str2 = VI_NULL;
    while (!found && strTable[i].col2)
    {
        if (strcmp (strTable[i].col1, str1) == 0)
        {
            *str2 = strTable[i].col2;
            found = VI_TRUE;
        }
        i++;
    }
    if (!found)
       _checkErr( IVI_ERROR_INVALID_VALUE);
        
Error:
    return error;
}

/*****************************************************************************
 *  Function: itscope_GetStr2BufFromStr1
 *  Purpose:  This function searches the string/string table the driver
 *            defines with the itscopeStringTable type. 
 *            Use this function when you know the string in the first table 
 *            column. Function fills the buffer you pass as the str2 parameter
 *            with the contents of the respective second column.
 *****************************************************************************/
static ViStatus itscope_GetStr2BufFromStr1 (itscopeStringTable strTable, 
                                             ViConstString str1, ViChar str2[])
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     i = 0;
    ViBoolean   found = VI_FALSE;
    
    if (str2)
        str2[0] = 0;
    
    while (!found && strTable[i].col1)
    {
        if (strcmp (strTable[i].col1, str1) == 0)
        {
            if (str2)
                strcpy (str2, strTable[i].col2);
            found = VI_TRUE;
        }
        i++;
    }
    
    if (!found)
       _checkErr( IVI_ERROR_INVALID_VALUE);
Error:
    return error;
}

/*****************************************************************************
* Function: itscope_CheckStatus
* Purpose:  This function checks the status of the instrument to detect
*           whether the instrument has encountered an error.  This function
*           is called at the end of most exported driver functions.  If the
*           instrument reports an error, this function returns
*           IVI_ERROR_INSTR_SPECIFIC.  The user can set the
*           IVI_ATTR_QUERY_INSTR_STATUS attribute to VI_FALSE to disable this
*           check and increase execution speed.
*
*           Note:  Call this function only when the session is locked.
*****************************************************************************/
static ViStatus itscope_CheckStatus(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle;
	_checkErr(error = get_brd_handle(vi));
	handle = error;

	if (Ivi_QueryInstrStatus(vi) && Ivi_NeedToCheckStatus(vi) && !Ivi_Simulating(vi))
	{
		_checkErr(itscope_CheckStatusCallback(vi, Ivi_IOSession(vi)));
		_checkErr(Ivi_SetNeedToCheckStatus(vi, VI_FALSE));
	}

Error:
	return error;
}

/*****************************************************************************
 * Function: itscope_WaitForOPC                                                  
 * Purpose:  This function waits for the instrument to complete the      
 *           execution of all pending operations.  This function is a        
 *           wrapper for the WaitForOPCCallback.  It can be called from 
 *           other instrument driver functions. 
 *
 *           The maxTime parameter specifies the maximum time to wait for
 *           operation complete in milliseconds.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus itscope_WaitForOPC (ViSession vi, ViInt32 maxTime) 
{
    ViStatus    error = VI_SUCCESS;
    //ViSession   io = Ivi_IOSession(vi);

    if (!Ivi_Simulating(vi))
        {
            ViBoolean opcDone = VI_FALSE;
            //_viCheckErr( viQueryf (io, "*OPC?", "%hd", &opcDone));
			_viCheckErr(_viQueryf(vi, "*OPC?", "%hd", &opcDone));
        }

Error:
    return error;
}

/*****************************************************************************
 * Function: itscope_NonInvasiveInstrSetup
 * Purpose:  This function sends a invasive setup to the instrument. The
 *           itscope_IviInit function calls this function when the user
 *           passes VI_FALSE for the reset parameter and specify TRUE to 
 *           NonInvasiveInit in option string. This function is used to configure
 *           the instrument with neccessary settings without modifying 
 *           its state.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus itscope_NonInvasiveInstrSetup (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;  
    
        /* Invalidate all attributes */
   _checkErr( Ivi_InvalidateAllAttributes (vi));
    if (!Ivi_Simulating(vi))    
    {
        ViInt32   model;
        //ViSession   io = Ivi_IOSession(vi); /* call only when locked */    
       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model));

		// Set default parameters
		_checkErr(pe_open_default(vi, model));
		//_checkErr(adc_open_default(vi, model));
	}
  
    /*
        Initiaze waveform data parameter
    */
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_WIDTH, 0, 2));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_START_POS, 0, 1));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_STOP_POS, 0, 8192));
    
Error:
    return error;
}

/*****************************************************************************
 * Function: itscope_DefaultInstrSetup                                               
 * Purpose:  This function sends a default setup to the instrument.  The    
 *           itscope_reset function calls this function.  The 
 *           itscope_IviInit function calls this function when the
 *           user passes VI_FALSE for the reset parameter.  This function is 
 *           useful for configuring settings that other instrument driver 
 *           functions require.    
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus itscope_DefaultInstrSetup (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
   
        /* Invalidate all attributes */
   _checkErr( Ivi_InvalidateAllAttributes (vi));

    if (!Ivi_Simulating(vi))
    {
        ViInt32   model;

       _checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model));

		// Set default parameters
		_checkErr( pe_open_default(vi, model));
		//_checkErr( adc_open_default(vi, model));
	}
  
    /*
        Initiaze waveform data parameter
    */
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_WIDTH, 0, 2));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_START_POS, 0, 1));
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_DATA_STOP_POS, 0, 8192));
    
   _checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, ITSCOPE_ATTR_INITIATE_CONTINUOUS, 
                                           0, VI_FALSE));     

   _checkErr( Ivi_SetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_ACQUISITION_START_TIME,
                                        0, 0.0));
                                        
Error:
    return error;
}

/*****************************************************************************
 *------------------------ Global Session Callbacks -------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: itscope_CheckStatusCallback                                               
 * Purpose:  This function queries the instrument to determine if it has 
 *           encountered an error.  If the instrument has encountered an 
 *           error, this function returns the IVI_ERROR_INSTRUMENT_SPECIFIC 
 *           error code.  This function is called by the 
 *           itscope_CheckStatus utility function.  The 
 *           Ivi_SetAttribute and Ivi_GetAttribute functions invoke this 
 *           function when the optionFlags parameter includes the
 *           IVI_VAL_DIRECT_USER_CALL flag.
 *           
 *           The user can disable calls to this function by setting the 
 *           IVI_ATTR_QUERY_INSTR_STATUS attribute to VI_FALSE.  The driver can 
 *           disable the check status callback for a particular attribute by 
 *           setting the IVI_VAL_DONT_CHECK_STATUS flag.
 *****************************************************************************/
static ViStatus _VI_FUNC itscope_CheckStatusCallback (ViSession vi, ViSession io)
{
    ViStatus    error = VI_SUCCESS;
    ViInt16     esr = 0;
   
	BRD_Handle handle;
	_checkErr(error = get_brd_handle(vi));
	handle = error;

    /* Query instrument status */
    //_viCheckErr( viQueryf (io, "*ESR?", "%hd", &esr));
	_viCheckErr(_viQueryf(vi, "*ESR?", "%hd", &esr));
    
    /* Convert status information into a message */
    
Error:
    return error;
}

/*****************************************************************************
 * Function: itscope_WaitForOPCCallback                                               
 * Purpose:  This function waits until the instrument has finished processing 
 *           all pending operations.  This function is called by the 
 *           itscope_WaitForOPC utility function.  The IVI engine invokes
 *           this function in the following two cases:
 *           - Before invoking the read callback for attributes for which the 
 *             IVI_VAL_WAIT_FOR_OPC_BEFORE_READS flag is set.
 *           - After invoking the write callback for attributes for which the 
 *             IVI_VAL_WAIT_FOR_OPC_AFTER_WRITES flag is set.
 *****************************************************************************/
static ViStatus _VI_FUNC itscope_WaitForOPCCallback (ViSession vi, ViSession io)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     opcTimeout;
    ViUInt16    statusByte;
    ViUInt16    _interface = VI_INTF_USB;
    
    
   _viCheckErr( viGetAttribute (io, VI_ATTR_INTF_TYPE, &_interface));
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_OPC_TIMEOUT, 0, 
                                       &opcTimeout));
    if (_interface == VI_INTF_TCPIP)
    {
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_OPC_TIMEOUT, 0, 
                                            &opcTimeout));
       _viCheckErr( viEnableEvent (io, VI_EVENT_SERVICE_REQ, VI_QUEUE, VI_NULL));
       _viCheckErr( viDiscardEvents (io, VI_EVENT_SERVICE_REQ, VI_QUEUE) );
    
        //_viCheckErr( viPrintf (io, "*OPC"));
		_viCheckErr(_viPrintf(vi, "*OPC"));

            /* wait for SRQ */
       _viCheckErr( viWaitOnEvent (io, VI_EVENT_SERVICE_REQ, opcTimeout, VI_NULL, VI_NULL));
       _viCheckErr( viDisableEvent (io, VI_EVENT_SERVICE_REQ, VI_QUEUE));
        
            /* clean up after SRQ */
       _viCheckErr( viBufWrite (io, "*CLS", 4, VI_NULL));
       _viCheckErr( viReadSTB (io, &statusByte));

    }
    else 
    {
        ViBoolean   opcDone = VI_FALSE;
		
        //_viCheckErr( viQueryf (io, "*OPC?", "%hd", &opcDone));
		_viCheckErr(_viQueryf(vi, "*OPC?", "%hd", &opcDone));
    }

Error:
    if (_interface == VI_INTF_USB)
    {
        viBufWrite (io, "*CLS", 4, VI_NULL);  
        viDisableEvent (io, VI_EVENT_SERVICE_REQ, VI_QUEUE);           
        if (error == VI_ERROR_TMO)
        {
            Ivi_SetErrorInfo(vi, VI_TRUE, ITSCOPE_ERROR_MAX_TIME_EXCEEDED, 0, VI_NULL);
        }            
    }
    return error;
}

/*****************************************************************************
 *----------------- Attribute Range Tables and Callbacks --------------------*
 *****************************************************************************/

    /*- ITSCOPE_ATTR_ID_QUERY_RESPONSE -*/
static ViStatus _VI_FUNC itscopeAttrIdQueryResponse_ReadCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rdBuffer[BUFFER_SIZE];

    //_viCheckErr( viQueryf (io, "*IDN?", "%256[^\n]", rdBuffer));
	_viCheckErr(_viQueryf(vi, "*IDN?", "%256[^\n]", rdBuffer));
   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_DRIVER_REVISION -*/
static ViStatus _VI_FUNC itscopeAttrDriverRevision_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      driverRevision[256];
    
    
    sprintf (driverRevision, 
             "Driver: PEK1 %d.%d, Compiler: %s %3.2f, "
             "Components: IVIEngine %.2f, VISA-Spec %.2f", 
             ITSCOPE_MAJOR_VERSION, ITSCOPE_MINOR_VERSION,
             IVI_COMPILER_NAME, IVI_COMPILER_VER_NUM, IVI_ENGINE_MAJOR_VERSION+ 
             IVI_ENGINE_MINOR_VERSION/1000.0, Ivi_ConvertVISAVer(VI_SPEC_VERSION));

   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, driverRevision));
Error:
    return error;
}

    /*- ITSCOPE_ATTR_CHANNEL_ENABLED -*/ 
static ViStatus _VI_FUNC itscopeAttrChannelEnabled_WriteCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViBoolean value)
{
    ViStatus         error = VI_SUCCESS;
        
    //_viCheckErr( viPrintf (io, ":SEL:%s %s;", channelName, value ? "ON" : "OFF"));
	_viCheckErr(_viPrintf(vi, ":SEL:%s %s;", channelName, value ? "ON" : "OFF"));
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrChannelEnabled_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViBoolean *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     state;
    
    //_viCheckErr( viQueryf (io, ":SEL:%s?", "%ld", channelName, &state));
	_viCheckErr(_viQueryf(vi, ":SEL:%s?", "%ld", channelName, &state));
    *value = (state == 0) ? VI_FALSE : VI_TRUE;

Error:
    return error;
}

    /*- ITSCOPE_ATTR_PROBE_ATTENUATION -*/

static IviRangeTableEntry attrProbeAttenuationRangeTableEntries[] =
{
    {ITSCOPE_VAL_PROBE_SENSE_ON,    0.0,    0.0,    "AUTO",     0}, 
    {0.001,                         0.0,    0.0,    "0.001",    0},
    {0.002,                         0.0,    0.0,    "0.002",    0},
    {0.005,                         0.0,    0.0,    "0.005",    0},
    {0.01,                          0.0,    0.0,    "0.01",     0},
    {0.02,                          0.0,    0.0,    "0.02",     0},
    {0.05,                          0.0,    0.0,    "0.05",     0},
    {0.1,                           0.0,    0.0,    "0.1",      0},
    {0.2,                           0.0,    0.0,    "0.2",      0},
    {0.5,                           0.0,    0.0,    "0.5",      0},
    {1.0,                           0.0,    0.0,    "1.0",      0},
    {2.0,                           0.0,    0.0,    "2.0",      0},
    {5.0,                           0.0,    0.0,    "5.0",      0}, 
    {10.0,                          0.0,    0.0,    "10.0",     0},
    {20.0,                          0.0,    0.0,    "20.0",     0},
    {50.0,                          0.0,    0.0,    "50.0",     0},
    {100.0,                         0.0,    0.0,    "100.0",    0},
    {200.0,                         0.0,    0.0,    "200.0",    0},
    {500.0,                         0.0,    0.0,    "500.0",    0},
    {1000.0,                        0.0,    0.0,    "1000.0",   0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrProbeAttenuationRangeTable =
{
    IVI_VAL_DISCRETE,                    
    VI_FALSE, 
    VI_FALSE, 
    VI_NULL,
    attrProbeAttenuationRangeTableEntries,
};

/* 
 * The probe attenuation range table before changing probe gain representation.   
 * 
static IviRangeTableEntry attrProbeAttenuationRangeTableEntries[] =
{
    {ITSCOPE_VAL_PROBE_SENSE_ON,    0.0,    0.0,    "AUTO",     0}, 
    {1.0,                           0.0,    0.0,    "1.0",      0},
    {10.0,                          0.0,    0.0,    "10.0",     0},
    {100.0,                         0.0,    0.0,    "100.0",    0},
    {1000.0,                        0.0,    0.0,    "1000.0",   0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
*/

static ViStatus _VI_FUNC itscopeAttrProbeAttenuation_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   probeSense = VI_FALSE;
    ViReal64    probeGain;
    
   _checkErr( Ivi_GetAttributeViBoolean (vi, channelName, ITSCOPE_ATTR_PROBE_SENSE, 
                                         0, &probeSense));
    if (probeSense)
    {
        *value = ITSCOPE_VAL_PROBE_SENSE_ON;
    }
    else
    {
       _checkErr( Ivi_GetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_PROBE_GAIN, 0, &probeGain));
        
        *value = 1.0/probeGain;
    }
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrProbeAttenuation_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     notProbeSensing = 0;
    
   
   _viCheckErr( Ivi_CompareWithPrecision (0, value, ITSCOPE_VAL_PROBE_SENSE_ON, 
                                          &notProbeSensing));
    if (notProbeSensing)
    {
       _checkErr( itscope_SetCachingOnProbeDependentAttributes (vi, VI_TRUE));
 
       _checkErr( Ivi_SetAttributeViBoolean (vi, channelName, ITSCOPE_ATTR_PROBE_SENSE, 
                                             0, VI_FALSE));
        
       _checkErr( Ivi_SetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_PROBE_GAIN, 0, (1.0 / value)) );
    }
    else
    {
       _checkErr( itscope_SetCachingOnProbeDependentAttributes (vi, VI_FALSE));
       _checkErr( Ivi_SetAttributeViBoolean (vi, channelName, ITSCOPE_ATTR_PROBE_SENSE, 
                                             0, VI_TRUE));
    }

Error:
    return error;
}

    /*- ITSCOPE_ATTR_PROBE_SENSE_VALUE -*/
static ViStatus _VI_FUNC itscopeAttrProbeSenseValue_ReadCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    probeGain; 
    
   _checkErr( Ivi_GetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_PROBE_GAIN, 0, &probeGain));
    
    *value = 1.0/probeGain; 

Error:
    return error;
}

    /*- ITSCOPE_ATTR_PROBE_GAIN -*/
static ViStatus _VI_FUNC itscopeAttrProbeGain_ReadCallback (ViSession vi, 
                                                            ViSession io, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf (io, ":%s:PRO:GAIN?", "%le", channelName, value));
	_viCheckErr(_viQueryf(vi, ":%s:PRO:GAIN?", "%le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrProbeGain_WriteCallback (ViSession vi, 
                                                             ViSession io, 
                                                             ViConstString channelName, 
                                                             ViAttr attributeId, 
                                                             ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viPrintf (io, ":%s:PRO:GAIN %le", channelName, value));
	_viCheckErr(_viPrintf(vi, ":%s:PRO:GAIN %le", channelName, value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_VERTICAL_RANGE -*/
static IviRangeTableEntry __attrVerticalRangeRangeTableEntries[] =
{
    {0,         1.5E-2,     1.0E-2,     "",     0},
    {1.5E-2,    3.5E-2,     2.0E-2,     "",     0},
    {3.5E-2,    7.5E-2,     5.0E-2,     "",     0},
    {7.5E-2,    15.0E-2,    10.0E-2,    "",     0},
    {15.0E-2,   35.0E-2,    20.0E-2,    "",     0},
    {35.0E-2,   75.0E-2,    50.0E-2,    "",     0},
    {75.0E-2,   1.5,        1.0,        "",     0},
    {1.5,       3.5,        2.0,        "",     0},
    {3.5,       7.5,        5.0,        "",     0},
    {7.5,       15,         10,         "",     0},
    {15,        35,         20,         "",     0},
    {35,        75,         50,         "",     0},
    {75,        100,        100,        "",     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTableEntry attrVerticalRangeRangeTableEntries[] =
{
	//{ 0,         100,     1.35,     "",     0 },
	{ 0,         100,     2.7,     "",     0 },
	{ IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrVerticalRangeRangeTable =
{
    IVI_VAL_COERCED,
    VI_FALSE,
    VI_TRUE,
    VI_NULL,
    attrVerticalRangeRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrVerticalRange_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 probeAttenuation, scaleValue = (value / 10.0);

   _checkErr( itscope_GetProbeAttenuation (vi, channelName, &probeAttenuation));
    
    /* Adjust for Probe Attenuation */
   _checkErr( Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, scaleValue/probeAttenuation));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrVerticalRange_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    probeAttenuation, probeDivideValue = value; 
    
   _checkErr( itscope_GetProbeAttenuation (vi, channelName, &probeAttenuation)); 
    
    /* Adjust for Probe Attenuation */
   _checkErr( Ivi_DefaultCoerceCallbackViReal64 (vi, channelName, attributeId, value/probeAttenuation, &probeDivideValue) );
    
Error:
    *coercedValue = probeDivideValue*probeAttenuation;
    return error;
}

static ViStatus _VI_FUNC itscopeAttrVerticalRange_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    scaleValue;
    ViString    channelString;
    /*the SCAle command is used to set the division size and the oscilloscope has 10 divisions 
    therefore to set the range, we divide the input by 10. */
	scaleValue = value/10.0;
    Ivi_CoerceChannelName(vi,channelName,&channelString);
    if(strncmp(channelString,"CH",2)==0)
    {
        //_viCheckErr( viPrintf (io, ":%s:SCA %le;", channelString, scaleValue));
		_viCheckErr(_viPrintf(vi, ":%s:SCA %le;", channelString, scaleValue));
    }
    else
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                        "Invalid channel name.");   
    }
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrVerticalRange_ReadCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 verticalScale;

    //_viCheckErr( viQueryf (io, ":%s:SCA?", "%le", channelName, &verticalScale));
	_viCheckErr(_viQueryf(vi, ":%s:SCA?", "%le", channelName, &verticalScale));
    *value = verticalScale * 10.0;
    
Error:
    return error;
}


    /*- ITSCOPE_ATTR_VERTICAL_OFFSET -*/
static ViStatus _VI_FUNC itscopeAttrVerticalOffset_CheckCallback (ViSession vi, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 offsetLimit = 1.0, vertRange, vertScale, probeAttenuation;

   _checkErr( itscope_GetProbeAttenuation (vi, channelName, &probeAttenuation));
    
   _checkErr( Ivi_GetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                        0, &vertRange)); 

        /* Adjust values for total attenuation */
    value /= probeAttenuation;
    vertScale = (vertRange / 10.0) / probeAttenuation;
	    
        /* Set limit based on the value of the vertScale */
    if  ( 1e-3 <= vertScale && vertScale <= 9.95e-3)
    {
        offsetLimit = 0.1; 
    }
    else if (9.95e-3 < vertScale && vertScale <= 99.5e-3)
    {
        offsetLimit = 1.0; 
    }
    else if (99.5e-3 < vertScale && vertScale <= 995.0e-3) 
    {
        offsetLimit = 10.0;
    }
    else if (995.0e-3 < vertScale && vertScale <= 10.0)
    {
        offsetLimit = 100.0;
    }
    
   _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                -offsetLimit, offsetLimit));
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrVerticalOffset_WriteCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViReal64 value)

{
    ViStatus error = VI_SUCCESS;

    //_viCheckErr( viPrintf (io, ":%s:OFFS %le;", channelName, value));
	_viCheckErr(_viPrintf(vi, ":%s:OFFS %le;", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrVerticalOffset_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    
    //_viCheckErr( viQueryf (io, ":%s:OFFS?", "%le", channelName, value));
	_viCheckErr(_viQueryf(vi, ":%s:OFFS?", "%le", channelName, value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_VERTICAL_COUPLING -*/ 
static IviRangeTableEntry attrVerticalCouplingRangeTableEntries[] =
{
    {ITSCOPE_VAL_AC,    0,  0,  "AC",   0}, 
    {ITSCOPE_VAL_DC,    0,  0,  "DC",   0}, 
    {ITSCOPE_VAL_GND,   0,  0,  "GND",  0},
	{ITSCOPE_VAL_DCREJ,   0,  0,  "DCREJ",  0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable  attrVerticalCouplingRangeTable =       
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    VI_NULL, 
    attrVerticalCouplingRangeTableEntries, 
};

static ViStatus _VI_FUNC itscopeAttrVerticalCoupling_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViInt32 value)

{
    ViStatus  error = VI_SUCCESS;
    ViString  couplingStr;
    
    
   _viCheckErr( Ivi_GetViInt32EntryFromValue (value, &attrVerticalCouplingRangeTable, 
                        VI_NULL, VI_NULL, VI_NULL, VI_NULL, &couplingStr, VI_NULL));
    
   //_viCheckErr( viPrintf (io, ":%s:COUP %s;", channelName, couplingStr));
	_viCheckErr(_viPrintf(vi, ":%s:COUP %s;", channelName, couplingStr));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrVerticalCoupling_ReadCallback (ViSession vi, 
                                                                    ViSession io, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViInt32 *value)
 
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rdBuffer[BUFFER_SIZE];

    //_viCheckErr( viQueryf (io, ":%s:COUP?", "%s", channelName, rdBuffer));
	_viCheckErr(_viQueryf(vi, ":%s:COUP?", "%s", channelName, rdBuffer));
   _viCheckErr( Ivi_GetViInt32EntryFromString (rdBuffer, &attrVerticalCouplingRangeTable, 
                                               value, VI_NULL, VI_NULL, 
                                               VI_NULL, VI_NULL));
Error:
    return error;
    
}

    /*- ITSCOPE_ATTR_MAX_INPUT_FREQUENCY -*/
static IviRangeTableEntry attrMaxInputFrequency_1000_RangeTableEntries[] = 
{
    {0.0,       20.0e6,     20.0e6,     "TWENTY",   0},
    {20.0e6,    250.0e6,    250.0e6,    "TWOFIFTY", 0},
    {250.0e6,   IVI_VIREAL64_MAX,   1000.0e6,   "FULL",     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}    
};

static IviRangeTable attrMaxInputFrequency_1000_RangeTable =
{
    IVI_VAL_COERCED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrMaxInputFrequency_1000_RangeTableEntries,
};
    
static IviRangeTableEntry attrMaxInputFrequency_500_RangeTableEntries[] =
{
    {0.0,       20.0e6,     20.0e6,     "TWENTY",   0},
    {20.0e6,    250.0e6,    250.0e6,    "TWOFIFTY", 0},
    {250.0e6,   IVI_VIREAL64_MAX,   500.0e6,    "FULL",     0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMaxInputFrequency_500_RangeTable =
{
    IVI_VAL_COERCED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrMaxInputFrequency_500_RangeTableEntries,
};
    

static ViStatus _VI_FUNC itscopeAttrMaxInputFrequency_RangeTableCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     model;
    IviRangeTablePtr    tblPtr = VI_NULL;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model));
    
    switch (model) 
    {
        case ITSCOPE_VAL_PE510K1:
        {
            tblPtr = &attrMaxInputFrequency_500_RangeTable;
            break;
        }
        case ITSCOPE_VAL_PE520K1:
        {
            tblPtr = &attrMaxInputFrequency_1000_RangeTable;
            break;
        }
    }
    
Error:
    *rangeTablePtr = tblPtr;
    return error;   
}

static ViStatus _VI_FUNC itscopeAttrMaxInputFrequency_ReadCallback (ViSession vi, 
                                                             ViSession io, 
                                                             ViConstString channelName, 
                                                             ViAttr attributeId, 
                                                             ViReal64 *value)
{
    ViStatus            error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViReal64            readValue = 0.0;

   _checkErr(Ivi_GetAttrRangeTable (vi, channelName, ITSCOPE_ATTR_MAX_INPUT_FREQUENCY, &tblPtr));
    
    //_viCheckErr( viQueryf (io, ":%s:BAN?", "%le", channelName, &readValue));
	_viCheckErr(_viQueryf(vi, ":%s:BAN?", "%le", channelName, &readValue));

   _viCheckErr( Ivi_GetViReal64EntryFromValue(readValue, tblPtr, VI_NULL, VI_NULL, value, VI_NULL, VI_NULL, VI_NULL) );                                
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrMaxInputFrequency_WriteCallback (ViSession vi, 
                                                              ViSession io, 
                                                              ViConstString channelName, 
                                                              ViAttr attributeId, 
                                                              ViReal64 value)
{
    ViStatus        error = VI_SUCCESS;
    ViString        commandStr = "";
    IviRangeTablePtr    tblPtr = VI_NULL;

   _checkErr(Ivi_GetAttrRangeTable (vi, channelName, ITSCOPE_ATTR_MAX_INPUT_FREQUENCY, &tblPtr));

   _viCheckErr( Ivi_GetViReal64EntryFromValue (value, tblPtr, VI_NULL, VI_NULL,
                                               VI_NULL, VI_NULL, &commandStr,
                                               VI_NULL));

    //_viCheckErr( viPrintf (io, ":%s:BAN %s;", channelName, commandStr));
	_viCheckErr(_viPrintf(vi, ":%s:BAN %s;", channelName, commandStr));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_INPUT_IMPEDANCE -*/
static IviRangeTableEntry attrPEK1ChannelImpedanceRangeTableEntries[] =
{
    {50.0,      0, 0, "FIFTY", 0},
    {1.0e+06,   0, 0, "MEG", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
   
static IviRangeTable attrPEK1ChannelImpedanceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrPEK1ChannelImpedanceRangeTableEntries,
};


static ViStatus _VI_FUNC itscopeAttrChannelImpedance_ReadCallback (ViSession vi, 
                                                                    ViSession io, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    ViChar   rdBuffer[BUFFER_SIZE];
    IviRangeTablePtr rangeTable;

   _checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));
     
    //_viCheckErr( viQueryf (io, ":%s:IMP?", "%s", channelName, rdBuffer));
	_viCheckErr(_viQueryf(vi, ":%s:IMP?", "%s", channelName, rdBuffer));
   _viCheckErr( Ivi_GetViReal64EntryFromString (rdBuffer, rangeTable, 
                                                value, VI_NULL, VI_NULL, VI_NULL, VI_NULL));
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrChannelImpedance_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value)
{
    ViStatus  error = VI_SUCCESS;
    ViString  channelImpedanceStr;
    IviRangeTablePtr rangeTable; 
    
   _checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));

   _viCheckErr( Ivi_GetViReal64EntryFromValue (value, rangeTable, 
                        VI_NULL, VI_NULL, VI_NULL, VI_NULL, &channelImpedanceStr, VI_NULL));
    //_viCheckErr( viPrintf (io, ":%s:IMP %s;", channelName, channelImpedanceStr));
	_viCheckErr(_viPrintf(vi, ":%s:IMP %s;", channelName, channelImpedanceStr));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrInputImpedance_RangeTableCallback (ViSession vi,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    {
        tblPtr = &attrPEK1ChannelImpedanceRangeTable;
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}


    /*- ITSCOPE_ATTR_HORZ_TIME_PER_RECORD -*/
static IviRangeTableEntry attrPE520K1HorzTimePerRecordRangeTableEntries[] =
{
    {IVI_VIREAL64_MAX_NEG,  1.5E-8,             1.0E-8,     "",     0},
    {1.5E-8,                3.0E-8,             2.0E-8,     "",     0},
    {3.0E-8,                7.0E-8,             4.0E-8,     "",     0},
    {7.0E-8,                15.0E-8,            10.0E-8,    "",     0},
    {15.0E-8,               30.0E-8,            20.0E-8,    "",     0},
    {30.0E-8,               70.0E-8,            40.0E-8,    "",     0},
    {70.0E-8,               150.0E-8,           100.0E-8,   "",     0},
    {150.0E-8,              300.0E-8,           200.0E-8,   "",     0},
    {300.0E-8,              700.0E-8,           400.0E-8,   "",     0},
    {700.0E-8,              1.5E-5,             1.0E-5,     "",     0},
    {1.5E-5,                3.0E-5,             2.0E-5,     "",     0},
    {3.0E-5,                6.0E-5,             4.0E-5,     "",     0},
    {6.0E-5,                15.0E-5,            10.0E-5,    "",     0},
    {15.0E-5,               30.0E-5,            20.0E-5,    "",     0},
    {30.0E-5,               60.0E-5,            40.0E-5,    "",     0},
    {60.0E-5,               150.0E-5,           100.0E-5,   "",     0},
    {150.0E-5,              300.0E-5,           200.0E-5,   "",     0},
    {300.0E-5,              700.0E-5,           400.0E-5,   "",     0},
    {700.0E-5,              1.5E-2,             1.0E-2,     "",     0},
    {1.5E-2,                3.0E-2,             2.0E-2,     "",     0},
    {3.0E-2,                7.0E-2,             4.0E-2,     "",     0},
    {7.0E-2,                15.0E-2,            10.0E-2,    "",     0},
    {15.0E-2,               30.0E-2,            20.0E-2,    "",     0},
    {30.0E-2,               70.0E-2,            40.0E-2,    "",     0},
    {70.0E-2,               150.0E-2,           100.0E-2,   "",     0},
    {150.0E-2,              300.0E-2,           200.0E-2,   "",     0},
    {300.0E-2,              700.0E-2,           400.0E-2,   "",     0},
    {700.0E-2,              15,                 10,         "",     0},
    {15,                    30,                 20,         "",     0},
    {30,                    70,                 40,         "",     0},
    {70,                    150,                100,        "",     0},
    {150,                   300,                200,        "",     0},
    {300,                   700,                400,        "",     0},
    {700,                   1500,               1000,       "",     0},
    {1500,                  3000,               2000,       "",     0},
    {3000,                  7000,               4000,       "",     0},
    {7000,                  IVI_VIREAL64_MAX,   10000,      "",     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
static IviRangeTable attrPE520K1HorzTimePerRecordRangeTable =
{
    IVI_VAL_COERCED,
    VI_FALSE,
    VI_TRUE,
    VI_NULL,
    attrPE520K1HorzTimePerRecordRangeTableEntries,
};


/*
500MHz - 2ns
400MHz - 2.5ns
*/

static IviRangeTableEntry attrPE510K1HorzTimePerRecordRangeTableEntries[] =
{
	/*
    {IVI_VIREAL64_MAX_NEG,  700E-11,            400E-11,    "",     0}, // 0.004ns * 1000 point = 4ns (4.0E-9) - 500MHz sample rate - interpolation 500 -2 point
    {700E-11,               1.5E-8,             1.0E-8,     "",     0}, // 0.01ns * 1000 point = 10ns (1.0E-8) - 500MHz sample rate - interpolation 200 - 
    {1.5E-8,                3.0E-8,             2.0E-8,     "",     0}, // 0.02ns * 1000 point = 20ns (2.0E-8) - 500MHz sample rate - interpolation 100
    {2.0E-8,                7.0E-8,             4.0E-8,     "",     0}, // 0.04ns * 1000 point = 40ns (4.0E-8) - 500MHz sample rate - interpolation 50 - 20 point
	*/
    {IVI_VIREAL64_MAX_NEG,  15.0E-8,            10.0E-8,    "",     0}, // 0.1ns * 1000 point = 100ns (20.0E-8) - 500MHz sample rate - interpolation 20 - 50 point
	{15.0E-8,               30.0E-8,            20.0E-8,    "",     0}, // 0.2ns * 1000 point = 200ns (20.0E-8) - 500MHz sample rate - interpolation 10 - 100 point
    {30.0E-8,               70.0E-8,            40.0E-8,    "",     0}, // 0.4ns * 1000 point = 400ns (40.0E-8) - 500MHz sample rate - interpolation 5 - 200 point
    {70.0E-8,               150.0E-8,           100.0E-8,   "",     0}, // 1ns * 1000 point = 1 mks (1.0E-6 or 100.0E-8) - 500MHz sample rate - interpolation 2 - 500 point
    
	{150.0E-8,              300.0E-8,           200.0E-8,   "",     0}, // 2ns * 1000 point = 2 mks (2.0E-6 or 200.0E-8) - 500MHz sample rate - 1024 point

	{300.0E-8,              700.0E-8,           400.0E-8,   "",     0},	// 4ns * 1000 point = 4 mks (4.0E-6 or 400.0E-8) - 500MHz sample rate - decimation 2	- 2048 point
	
	{0.9E-5,                1.1E-5,             1.0E-5,     "",     0}, // 10ns * 1000 point = 10 mks (10.0E-6 or 1000.0E-8) - 500MHz sample rate - decimation 5 - 8192 point
	{1.1E-5,                3.0E-5,             2.0E-5,     "",     0}, // 20ns * 1000 point = 20 mks (2.0E-5) - 500MHz sample rate - decimation 10 - 16K point
	{3.0E-5,                7.0E-5,             4.0E-5,     "",     0}, // 40ns * 1000 point = 40 mks (4.0E-5) - 500MHz sample rate - decimation 20 - 32K point

	{7.0E-5,                15.0E-5,            10.0E-5,    "",     0}, // 100ns* 1000 point = 100 mks (10.0E-5) - 500MHz sample rate - decimation 50 - 64K point
	{15.0E-5,               30.0E-5,            20.0E-5,    "",     0}, // 200ns* 1000 point = 200 mks (20.0E-5) - 500MHz sample rate - decimation 100 - 128K point
    {30.0E-5,               70.0E-5,            40.0E-5,    "",     0}, // 400ns* 1000 point = 400 mks (40.0E-5) - 500MHz sample rate - decimation 200 - 256K point
    
	{0.7E-3,                1.5E-3,             1.0E-3,     "",     0}, // 1mks * 1000 point = 1ms (1.0E-3) - 50MHz sample rate - decimation 50 - 64K point
	{1.5E-3,                3.0E-3,             2.0E-3,     "",     0}, // 2mks * 1000 point = 2ms (2.0E-3) - 50MHz sample rate - decimation 100 - 128K point
	{3.0E-3,                7.0E-3,             4.0E-3,     "",     0}, // 4mks * 1000 point = 4ms (4.0E-3) - 50MHz sample rate - decimation 200 - 256K point
	{0.7E-2,                IVI_VIREAL64_MAX,   1.0E-2,     "",     0}, // 10mks * 1000 point = 10ms (1.0E-2) - 50MHz sample rate - decimation 500 - 512K point
	
	{1.5E-2,                3.0E-2,             2.0E-2,     "",     0}, // 20mks * 1000 point = 20ms (2.0E-2) - 50MHz sample rate - decimation 1000 - 1M point
	{3.0E-2,                7.0E-2,             4.0E-2,     "",     0}, // 40mks * 1000 point = 40ms (4.0E-2) - 50MHz sample rate - decimation 2000 - 2M point
	
    {7.0E-2,                15.0E-2,            10.0E-2,    "",     0}, // 100mks * 1000 point = 100ms (10.0E-2) - 50MHz sample rate - decimation 5000  - 8M point
    {15.0E-2,               30.0E-2,            20.0E-2,    "",     0}, // 200mks * 1000 point = 200ms (20.0E-2) - 50MHz sample rate - decimation 10000 - 16M point
    {30.0E-2,               70.0E-2,            40.0E-2,    "",     0}, // 400mks * 1000 point = 400ms (40.0E-2) - 50MHz sample rate - decimation 20000 - 32M point
	
    {70.0E-2,               150.0E-2,           100.0E-2,   "",     0}, // 1000mks * 1000 point = 1s (100.0E-2) - 50MHz sample rate - decimation 50000 - 64M point
    {150.0E-2,              300.0E-2,           200.0E-2,   "",     0}, // 2000mks * 1000 point = 2s (200.0E-2) - 50MHz sample rate - decimation 100000 - 128M point
    {300.0E-2,              700.0E-2,           400.0E-2,   "",     0}, // 4000mks * 1000 point = 4s (400.0E-2) - 50MHz sample rate - decimation 200000 - 256M point
    /*
	{700.0E-2,              15,                 10,         "",     0}, // 10ms * 1000 point = 10s (10.0) - 500MHz sample rate - decimation 5000000
	{15,                    30,                 20,         "",     0}, // 20ms * 1000 point = 20s (20.0) - 500MHz sample rate - decimation 20000000
    {30,                    70,                 40,         "",     0}, // 40ms * 1000 point = 40s (40.0) - 500MHz sample rate - decimation 40000000

    {70,                    150,                100,        "",     0}, // 100ms * 1000 point = 100s (100.0) - 500MHz sample rate - decimation 50000000
	{150,                   300,                200,        "",     0},	// 200ms * 1000 point = 200s (200.0) - 500MHz sample rate - decimation 200000000
	{300,                   700,                400,        "",     0},	// 400ms * 1000 point = 400s (400.0) - 500MHz sample rate - decimation 400000000

    {700,                   1500,               1000,       "",     0}, // 1s * 1000 point = 1000s (1000.0) - 500MHz sample rate - decimation 500000000                                          
	{1500,                  3000,               2000,       "",     0},	// 2s * 1000 point = 2000s (2000.0) - 500MHz sample rate - decimation 2000000000
	{3000,                  7000,               4000,       "",     0},	// 4s * 1000 point = 4000s (4000.0) - 500MHz sample rate - decimation 4000000000

    {7000,                  IVI_VIREAL64_MAX,   1.0E4,      "",     0},	// 10s * 1000 point = 10000s (10000.0) - 500MHz sample rate - decimation 5000000000
	*/
    {IVI_RANGE_TABLE_LAST_ENTRY}                        
};
static IviRangeTable attrPE510K1HorzTimePerRecordRangeTable =
{
    IVI_VAL_COERCED,
    VI_FALSE,
    VI_TRUE,
    VI_NULL,
    attrPE510K1HorzTimePerRecordRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrHorzTimePerRecord_ReadCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    /* ":HOR:SEC" is the same with ":HOR:SCA" */ 
   //_viCheckErr( viQueryf (io, ":HOR:SEC?", "%le", value));
	_viCheckErr(_viQueryf(vi, ":HOR:SEC?", "%le", value));
    *value *= 10.0;

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrHorzTimePerRecord_WriteCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    /* ":HOR:SEC" is the same with ":HOR:SCA" */
    //_viCheckErr( viPrintf (io, ":HOR:SEC %le;", value/10.0));
	_viCheckErr(_viPrintf(vi, ":HOR:SEC %le;", value / 10.0));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrHorzTimePerRecord_RangeTableCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    if( instrModelNo == ITSCOPE_VAL_PE510K1)
    {
        tblPtr = &attrPE510K1HorzTimePerRecordRangeTable;
    }
    else if( instrModelNo == ITSCOPE_VAL_PE520K1)
    {
        tblPtr = &attrPE520K1HorzTimePerRecordRangeTable;
    }
    
Error:
    *rangeTablePtr = tblPtr;
    return error;
}

    /*- ITSCOPE_ATTR_HORZ_MIN_NUM_PTS -*/
static IviRangeTableEntry attrPEK1HorzMinNumPtsRangeTableEntries[] =
{
    {0, 0x20000000, 0, "", 0},			// 0 .. 512MBytes
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPEK1HorzMinNumPtsRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrPEK1HorzMinNumPtsRangeTableEntries,
};


static ViStatus _VI_FUNC itscopeAttrHorzMinNumPts_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value)
{
    return Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TOTAL_POINTS,
                                    0, value);
}

static ViStatus _VI_FUNC itscopeAttrHorzMinNumPts_RangeTableCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    {
        tblPtr = &attrPEK1HorzMinNumPtsRangeTable;
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

    /*- ITSCOPE_ATTR_TOTAL_POINTS -*/
static IviRangeTableEntry attrPEK1TotalPointsRangeTableEntries[] =
{
    {0,         6144,       4096,      "", 0},
    {6145,      12000,      8192,      "", 0},
    {12001,     24000,      16384,     "", 0},   
    {24001,     48000,      32768,     "", 0},
    {48001,     0x18000,    0x10000,   "", 0},  // 64K
    {0x18001,   0x30000,    0x20000,   "", 0},  // 128K
    {0x30001,   0x60000,    0x40000,   "", 0},  // 256K
	{0x60001,   0xC0000,    0x80000,   "", 0 }, // 512K
	{0xC0001,   0x180000,   0x100000,  "", 0 }, // 1M
	{0x180001,  0x300000,   0x200000,  "", 0 }, // 2M
	{0x300001,  0x600000,   0x400000,  "", 0 }, // 4M
	{0x600001,  0xC00000,   0x800000,  "", 0 }, // 8M
	{0xC00001,  0x1800000,  0x1000000, "", 0 }, // 16M
	{0x1800001, 0x3000000,  0x2000000, "", 0 }, // 32M
	{0x3000001, 0x6000000,  0x4000000, "", 0 }, // 64M
	{0x6000001, 0xC000000,  0x8000000, "", 0 }, // 128M
	{0xC000001, 0x18000000,  0x10000000, "", 0 }, // 256M
	{0x18000001,0x20000000,  0x20000000, "", 0 }, // 512M
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPEK1TotalPointsRangeTable =
{
    IVI_VAL_COERCED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrPEK1TotalPointsRangeTableEntries,
};


static ViStatus _VI_FUNC itscopeAttrTotalPoints_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viPrintf (io, ":HOR:RECORDL %ld;", value));
	_viCheckErr(_viPrintf(vi, ":HOR:RECORDL %ld;", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTotalPoints_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viQueryf(io, ":HOR:RECORDL?", "%ld", value));
	_viCheckErr(_viQueryf(vi, ":HOR:RECORDL?", "%ld", value));
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTotalPoints_RangeTableCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    {
        tblPtr = &attrPEK1TotalPointsRangeTable;
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}
    
    /*- ITSCOPE_ATTR_HORZ_RECORD_LENGTH -*/
static ViStatus _VI_FUNC itscopeAttrHorzRecordLength_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     temp = 0, acqType;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ITSCOPE_ATTR_TOTAL_POINTS, 
                                       0, &temp));
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ITSCOPE_ATTR_ACQUISITION_TYPE, 
                                       0, &acqType));
    
    if (acqType == ITSCOPE_VAL_PEAK_DETECT || acqType == ITSCOPE_VAL_ENVELOPE)  
    {
        temp /= 2;
    }
        
    *value = temp;

Error:
    return error;
}

    /*- ITSCOPE_ATTR_HORZ_SAMPLE_RATE -*/
static ViStatus _VI_FUNC itscopeAttrHorzSampleRate_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;

    //_viCheckErr( viQueryf(io, ":HOR:MAI:SAMPLER?", "%lf", value)); 
	_viCheckErr(_viQueryf(vi, ":HOR:MAI:SAMPLER?", "%lf", value));

Error:
    return error;
}


    /*- ITSCOPE_ATTR_MAIN_TRIGGER_TYPE -*/
static IviRangeTableEntry attrMainTriggerTypeRangeTableEntries[] =
{
    {ITSCOPE_VAL_EDGE_TRIGGER,      0, 0, "EDGE",   0},
    {ITSCOPE_VAL_LOGIC_TRIGGER,     0, 0, "LOGIC",  0},
    {ITSCOPE_VAL_PULSE_TRIGGER,     0, 0, "PULSE",  0},
    {ITSCOPE_VAL_SERIAL_TRIGGER,    0, 0, "BUS",    0},
    {ITSCOPE_VAL_TV_TRIGGER,        0, 0, "VIDEO",  0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMainTriggerTypeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:A:TYP",
    attrMainTriggerTypeRangeTableEntries,
};

    /*- ITSCOPE_ATTR_EDGE_TRIGGER_CLASS -*/
static IviRangeTableEntry attrEdgeTriggerClassRangeTableEntries[] =
{
    {ITSCOPE_VAL_EDGE_TRIGGER,  0,  0,  "CH1",  0},
    {ITSCOPE_VAL_AC_LINE_TRIGGER,   0,  0,  "LINE", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrEdgeTriggerClassRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrEdgeTriggerClassRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrEdgeTriggerClass_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    strACLine;  
    
   _viCheckErr( Ivi_GetViInt32EntryFromValue ( ITSCOPE_VAL_AC_LINE_TRIGGER, &attrEdgeTriggerClassRangeTable,
                                              VI_NULL, VI_NULL, VI_NULL, VI_NULL, &strACLine, VI_NULL ));
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:EDGE:SOU?", "%s", buf) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:EDGE:SOU?", "%s", buf));
    
    if( strncmp( buf, strACLine, strlen(strACLine)) == 0 )
    {
        *value = ITSCOPE_VAL_AC_LINE_TRIGGER;   
    }
    else
    {
        *value = ITSCOPE_VAL_EDGE_TRIGGER;
    }
        
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrEdgeTriggerClass_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     currentValue;
    ViChar      buf[BUFFER_SIZE];
    ViString    strACLine;
    
   _viCheckErr( Ivi_GetViInt32EntryFromValue ( ITSCOPE_VAL_AC_LINE_TRIGGER, &attrEdgeTriggerClassRangeTable,
                                              VI_NULL, VI_NULL, VI_NULL, VI_NULL, &strACLine, VI_NULL ));
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:EDGE:SOU?", "%s", buf) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:EDGE:SOU?", "%s", buf));

    if( strncmp( buf, strACLine, strlen(strACLine) ) == 0 )
    {
        currentValue = ITSCOPE_VAL_AC_LINE_TRIGGER; 
    }
    else
    {
        currentValue = ITSCOPE_VAL_EDGE_TRIGGER;
    }
    
    if(value != currentValue)
    {
        if(value == ITSCOPE_VAL_EDGE_TRIGGER)
        {
           _viCheckErr( Ivi_GetViInt32EntryFromValue ( ITSCOPE_VAL_EDGE_TRIGGER, &attrEdgeTriggerClassRangeTable,
                                                      VI_NULL, VI_NULL, VI_NULL, VI_NULL, &strACLine, VI_NULL ));           
            //_viCheckErr( viPrintf (io, ":TRIG:A:EDGE:SOU %s;", strACLine) );
			_viCheckErr(_viPrintf(vi, ":TRIG:A:EDGE:SOU %s;", strACLine));
        }
        else
        {
           _viCheckErr( Ivi_GetViInt32EntryFromValue ( ITSCOPE_VAL_AC_LINE_TRIGGER, &attrEdgeTriggerClassRangeTable,
                                                      VI_NULL, VI_NULL, VI_NULL, VI_NULL, &strACLine, VI_NULL ));   
            //_viCheckErr( viPrintf (io, ":TRIG:A:EDGE:SOU %s;", strACLine) );                                                   
			_viCheckErr(_viPrintf(vi, ":TRIG:A:EDGE:SOU %s;", strACLine));
        }
        
        /* Invalidate attribute ITSCOPE_ATTR_TRIGGER_SOURCE */
       _checkErr ( Ivi_InvalidateAttribute (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE) );
    }

Error:
    return error;
}

    /*- ITSCOPE_ATTR_LOGIC_TRIGGER_CLASS -*/
static IviRangeTableEntry attrLogicTriggerClassRangeTableEntries[] =
{
    {ITSCOPE_VAL_PATTERN_TRIGGER,   0,  0,  "LOGIC",    0}, 
    {ITSCOPE_VAL_STATE_TRIGGER,     0,  0,  "LOGIC",    0},
    {ITSCOPE_VAL_SETHOLD_TRIGGER,   0,  0,  "SETHOLD",  0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrLogicTriggerClassRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    "",
    attrLogicTriggerClassRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrLogicTriggerClass_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViInt32     triggerType, clockSource;
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:LOGI:CLA?", "%s", buf) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:LOGI:CLA?", "%s", buf));
    
   _viCheckErr( Ivi_GetViInt32EntryFromString (buf, &attrLogicTriggerClassRangeTable, &triggerType, 
                                              VI_NULL, VI_NULL, VI_NULL, VI_NULL ));
    
    if(triggerType == ITSCOPE_VAL_SETHOLD_TRIGGER)                                              
    {
        *value = ITSCOPE_VAL_SETHOLD_TRIGGER;   
    }
    else
    {
       _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_STATE_CLOCK_SOURCE, 0, &clockSource) );
        if(clockSource == ITSCOPE_VAL_SOUR_NONE)
        {
            *value = ITSCOPE_VAL_PATTERN_TRIGGER;
        }
        else
        {
            *value = ITSCOPE_VAL_STATE_TRIGGER;
        }
    }

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrLogicTriggerClass_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViString    strTriggerType;
    
    switch(value)
    {
        case ITSCOPE_VAL_PATTERN_TRIGGER:
        {
           _viCheckErr( Ivi_GetViInt32EntryFromValue ( ITSCOPE_VAL_PATTERN_TRIGGER, &attrLogicTriggerClassRangeTable, 
                                                      VI_NULL, VI_NULL, VI_NULL, VI_NULL, &strTriggerType, VI_NULL) );
            
            //_viCheckErr( viPrintf (io, ":TRIG:A:LOGI:CLA %s;", strTriggerType) ); 
			_viCheckErr(_viPrintf(vi, ":TRIG:A:LOGI:CLA %s;", strTriggerType));

           _checkErr ( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_STATE_CLOCK_SOURCE, 0, ITSCOPE_VAL_SOUR_NONE));
            break;
        }
        case ITSCOPE_VAL_STATE_TRIGGER:
        {
           _viCheckErr( Ivi_GetViInt32EntryFromValue ( ITSCOPE_VAL_STATE_TRIGGER, &attrLogicTriggerClassRangeTable, 
                                                      VI_NULL, VI_NULL, VI_NULL, VI_NULL, &strTriggerType, VI_NULL) );
            
            //_viCheckErr( viPrintf (io, ":TRIG:A:LOGI:CLA %s;", strTriggerType) ); 
			_viCheckErr(_viPrintf(vi, ":TRIG:A:LOGI:CLA %s;", strTriggerType));

           _checkErr ( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_STATE_CLOCK_SOURCE, 0, ITSCOPE_VAL_SOUR_CH1));
            break;
        }
        case ITSCOPE_VAL_SETHOLD_TRIGGER:
        {
           _viCheckErr( Ivi_GetViInt32EntryFromValue ( ITSCOPE_VAL_SETHOLD_TRIGGER, &attrLogicTriggerClassRangeTable, 
                                                      VI_NULL, VI_NULL, VI_NULL, VI_NULL, &strTriggerType, VI_NULL) );
            
            //_viCheckErr( viPrintf (io, ":TRIG:A:LOGI:CLA %s;", strTriggerType) ); 
			_viCheckErr(_viPrintf(vi, ":TRIG:A:LOGI:CLA %s;", strTriggerType));

            break;
        }
    }

Error:
    return error;
}

    /*- ITSCOPE_ATTR_PULSE_TRIGGER_CLASS -*/
static IviRangeTableEntry attrPulseTriggerClassRangeTableEntries[] =
{
    {ITSCOPE_VAL_GLITCH_TRIGGER,        0, 0, "WIDTH",      0},
    {ITSCOPE_VAL_RUNT_TRIGGER,          0, 0, "RUNT",       0},
    {ITSCOPE_VAL_TRANSITION_TRIGGER,    0, 0, "TRANSITION", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPulseTriggerClassRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:A:PUL:CLA",
    attrPulseTriggerClassRangeTableEntries,
};



    /*- ITSCOPE_ATTR_TRIGGER_TYPE -*/
static IviRangeTableEntry attrPEK1TriggerTypeRangeTableEntries[] =
    {
        {ITSCOPE_VAL_EDGE_TRIGGER, 0, 0, "EDGE", 0},
        {ITSCOPE_VAL_RUNT_TRIGGER, 0, 0, "RUNT", 0},
        {ITSCOPE_VAL_GLITCH_TRIGGER, 0, 0, "GLITCH", 0},
        {ITSCOPE_VAL_TRANSITION_TRIGGER, 0, 0, "TRANSITION", 0},
        {ITSCOPE_VAL_STATE_TRIGGER, 0, 0, "STATE", 0},
        {ITSCOPE_VAL_PATTERN_TRIGGER, 0, 0, "PATTERN", 0},
        {ITSCOPE_VAL_SETHOLD_TRIGGER, 0, 0, "SETHOLD", 0},
        {ITSCOPE_VAL_TV_TRIGGER, 0, 0, "VIDEO", 0},
        {ITSCOPE_VAL_AC_LINE_TRIGGER, 0, 0, "AC", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
   
static IviRangeTable attrPEK1TriggerTypeRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        attrPEK1TriggerTypeRangeTableEntries,
    };


static ViStatus _VI_FUNC itscopeAttrTriggerType_ReadCallback (ViSession vi, 
                                                               ViSession io, 
                                                               ViConstString channelName, 
                                                               ViAttr attributeId, 
                                                               ViInt32 *value)
{
    ViInt32    triggerType;
    ViStatus   error = VI_SUCCESS;
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MAIN_TRIGGER_TYPE, 
                                       0, &triggerType));

    switch (triggerType)
    {
        case ITSCOPE_VAL_EDGE_TRIGGER:
        {
           _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_EDGE_TRIGGER_CLASS,
                                                0, &triggerType));
            break;                                              
        
        }
        case ITSCOPE_VAL_LOGIC_TRIGGER:
        {
           _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_TRIGGER_CLASS, 
                                               0, &triggerType));
            break;
        }
        case ITSCOPE_VAL_PULSE_TRIGGER:
        {    
           _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_PULSE_TRIGGER_CLASS, 
                                               0, &triggerType));
            break;
        }
        case ITSCOPE_VAL_SERIAL_TRIGGER:
        {
           _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SERIAL_TRIGGER_CLASS,
                                               0, &triggerType));
            break;        
        }
    }
        
Error:
    *value = triggerType;
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerType_WriteCallback (ViSession vi, 
                                                                ViSession io, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViInt32 value)

{
    ViStatus          error = VI_SUCCESS;

    switch (value)
    {
            /* Edge Triggers */
        case ITSCOPE_VAL_EDGE_TRIGGER:
        case ITSCOPE_VAL_AC_LINE_TRIGGER: 
        {
           _checkErr ( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MAIN_TRIGGER_TYPE,
                                                0, ITSCOPE_VAL_EDGE_TRIGGER));
            
           _checkErr ( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_EDGE_TRIGGER_CLASS,
                                                0, value));
            break;
        }
            /* Pulse Triggers */
        case ITSCOPE_VAL_RUNT_TRIGGER:   
        case ITSCOPE_VAL_GLITCH_TRIGGER:
        case ITSCOPE_VAL_TRANSITION_TRIGGER:
        {
           _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MAIN_TRIGGER_TYPE, 
                                               0, ITSCOPE_VAL_PULSE_TRIGGER));
                           
           _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_PULSE_TRIGGER_CLASS, 
                                               0, value));
            break;
        }
            /* Logic Triggers */
        case ITSCOPE_VAL_STATE_TRIGGER:    
        case ITSCOPE_VAL_PATTERN_TRIGGER: 
        case ITSCOPE_VAL_SETHOLD_TRIGGER:
        {
           _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MAIN_TRIGGER_TYPE, 
                                               0, ITSCOPE_VAL_LOGIC_TRIGGER));
            
           _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_TRIGGER_CLASS, 
                                               0, value));
            break;
        }
            /* Video Triggers */
        default: 
        {    
           _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MAIN_TRIGGER_TYPE, 
                                               0, value));
            break;
        }
    }
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerType_CheckCallback (ViSession vi, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_DefaultCheckCallbackViInt32 (vi, "", ITSCOPE_ATTR_TRIGGER_TYPE, value));
    
    /* Advanced Option check is needed! */
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerType_RangeTableCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    if (IS_PEK1 (instrModelNo))
    {
        tblPtr = &attrPEK1TriggerTypeRangeTable; 
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

    /*- ITSCOPE_ATTR_TRIGGER_SOURCE -*/
static ViStatus _VI_FUNC itscopeAttrTriggerSource_CheckCallback (ViSession vi, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     triggerType;

   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 
                                      0, &triggerType));
    
    if ((triggerType == ITSCOPE_VAL_PATTERN_TRIGGER)
            || (triggerType == ITSCOPE_VAL_STATE_TRIGGER)
                || (triggerType == ITSCOPE_VAL_SETHOLD_TRIGGER))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }

    error = itscope_GetStr2BufFromStr1 (triggerSourceStringTable, value, VI_NULL);
    /* If SOUR1 ~ SOUR4 is specified */
    if (error < VI_SUCCESS)
    {
        ViString coercedChanStr;
        
        error = Ivi_CoerceChannelName (vi, value, &coercedChanStr);
        if (error >= VI_SUCCESS)   /* value is a valid channel name  */
        {
        
            if ((strcmp( coercedChanStr, "") == 0 ) || (strcmp( coercedChanStr, "MATH") == 0)
                || (strcmp( coercedChanStr, "REF1" ) == 0) || (strcmp( coercedChanStr, "REF2" ) == 0)
                  || (strcmp( coercedChanStr, "REF3" ) == 0) || (strcmp( coercedChanStr, "REF4" ) == 0)
                    ||(strncmp(coercedChanStr,"RF_",3) == 0))
            {
               _viCheckErr( IVI_ERROR_INVALID_VALUE);
            }
        }
        else
        {
            error = IVI_ERROR_INVALID_VALUE;
        }
    }
   _viCheckErr( error);    
     
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerSource_ReadCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 const ViConstString cacheValue)
{
    ViStatus       error = VI_SUCCESS;
    ViChar         rdBuffer[BUFFER_SIZE];
    size_t         bufSize = sizeof(rdBuffer);
    ViInt32        triggerType;
    ViString       commandStr = "";
    ViConstString  sourceName;
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 
                                      0, &triggerType));

    switch (triggerType)
    {
        case ITSCOPE_VAL_RUNT_TRIGGER:
        {
            commandStr = ":TRIG:A:PUL:RUNT:SOU?";
            break;
        }
        case ITSCOPE_VAL_GLITCH_TRIGGER:
        {
            commandStr = ":TRIG:A:PUL:WID:SOU?";
            break;
        }
        case ITSCOPE_VAL_TRANSITION_TRIGGER:
        {
            commandStr = ":TRIG:A:PUL:TRAN:SOU?";
            break;
        }
        case ITSCOPE_VAL_TV_TRIGGER:
        {
            commandStr = ":TRIG:A:VID:SOU?";
            break;
        }
        case ITSCOPE_VAL_EDGE_TRIGGER:
        case ITSCOPE_VAL_AC_LINE_TRIGGER:   
        default:
        {
            commandStr = ":TRIG:A:EDGE:SOU?"; 
            break;
        }
    }
    //_viCheckErr( viPrintf (io, "%s", commandStr));
	//_viCheckErr(_viPrintf(vi, "%s", commandStr));
	
	//_viCheckErr( viScanf (io, "%#s", &bufSize, rdBuffer));
	//_viCheckErr(_viScanf(vi, "%s", rdBuffer));

	_viCheckErr(_viQueryf(vi, commandStr, "%s", rdBuffer));

    error = itscope_GetStr1PtrFromStr2 (triggerSourceStringTable, &sourceName, rdBuffer);
    if (error < VI_SUCCESS)
    {
            /* Assume the buf contains an instrument channel string. */
       _checkErr( Ivi_GetUserChannelName (vi, rdBuffer, &sourceName));
    }

   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, sourceName));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerSource_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViConstString value)

{
    ViStatus        error = VI_SUCCESS;
    ViInt32         triggerType;
    ViChar          sourceNameBuf[BUFFER_SIZE];
    ViString        cmdStringHeader = "";
    
    if (value[0] != '\0')
    {
       _checkErr(Ivi_GetAttributeViInt32(vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0, &triggerType));

        switch (triggerType)
        {
            case ITSCOPE_VAL_RUNT_TRIGGER:
            {
                if(strncmp(value,"D",1) == 0)
                {
                   _viCheckErr(IVI_ERROR_INVALID_VALUE);
                }
                cmdStringHeader = ":TRIG:A:PUL:RUNT:SOU";
                break;
            }
            case ITSCOPE_VAL_GLITCH_TRIGGER:
            {
                cmdStringHeader = ":TRIG:A:PUL:WID:SOU";
                break;
            }
            case ITSCOPE_VAL_TRANSITION_TRIGGER:
            {
                cmdStringHeader = ":TRIG:A:PUL:TRAN:SOU";
                break;
            }
            case ITSCOPE_VAL_TV_TRIGGER:
            {
                cmdStringHeader = ":TRIG:A:VID:SOU";
                break;
            }
            case ITSCOPE_VAL_EDGE_TRIGGER:
            case ITSCOPE_VAL_AC_LINE_TRIGGER:
            default:            
            {
                cmdStringHeader = ":TRIG:A:EDGE:SOU";
                break;
            }
        }

        error = itscope_GetStr2BufFromStr1 (triggerSourceStringTable, value, sourceNameBuf);
        if (error >= VI_SUCCESS)
        {
            if ((triggerType == ITSCOPE_VAL_AC_LINE_TRIGGER)
                    || (triggerType == ITSCOPE_VAL_RUNT_TRIGGER)
                        || (triggerType == ITSCOPE_VAL_TRANSITION_TRIGGER))
                    
            {
                error = IVI_ERROR_INVALID_CONFIGURATION;
               _viCheckErr(error);
            }
        }
        else 
        {
            ViConstString chanStr;

            error = Ivi_CoerceChannelName (vi, value, &chanStr);
            if (error >= VI_SUCCESS)
            {
                sprintf (sourceNameBuf, "%s", chanStr);
            }
        }
       _viCheckErr( error);

        //_viCheckErr( viPrintf (io, "%s %s", cmdStringHeader, sourceNameBuf));
		_viCheckErr(_viPrintf(vi, "%s %s", cmdStringHeader, sourceNameBuf));
    }

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TRIGGER_LEVEL -*/
static ViStatus _VI_FUNC itscopeAttrTriggerLevel_ReadCallback (ViSession vi, 
                                                                ViSession io, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:LEV?", "%le", value));
	_viCheckErr(_viQueryf(vi, ":TRIG:A:LEV?", "%le", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerLevel_WriteCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viPrintf (io, ":TRIG:A:LEV %le", value));
	_viCheckErr(_viPrintf(vi, ":TRIG:A:LEV %le", value));
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerLevel_CheckCallback (ViSession vi, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      tSource[BUFFER_SIZE];
    ViInt32     triggerType;
    ViReal64    ext = 0.0;
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));
    
    if (!(triggerType == ITSCOPE_VAL_EDGE_TRIGGER 
            || triggerType == ITSCOPE_VAL_AC_LINE_TRIGGER
                || triggerType == ITSCOPE_VAL_GLITCH_TRIGGER)) 
    {
         _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, "Cannot set trigger level when "
                         "trigger type is not Edge or Glitch Trigger.");
    }

   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 0, BUFFER_SIZE, tSource));
    
    if (!strcmp (tSource, ITSCOPE_VAL_EXTERNAL))
        ext = 1.0;
    
    if (ext != 0.0)
    {   
        /*- Trigger source is External -*/
       _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, -8.0 * ext, 8.0 * ext));
    }
    else if (strcmp(tSource, ITSCOPE_VAL_AC_LINE) == 0)
    {   
        /*- Trigger source is AC Line -*/
        if(value != 0.0)
        {
            _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, "Cannot set trigger level when "
                                                             "trigger source is AC line.");
        }                                                             
    } 
    else if (strstr (tSource, "CH"))
    {   
        /*- Trigger source is input channel -*/
        ViReal64      rangeLimit, limit;
        ViString      channel;
        
       _checkErr( Ivi_CoerceChannelName (vi, tSource, &channel));
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit));
                                            
        limit = rangeLimit*0.8;
        /******************************************************************** 
         * In old firmware version, trigger level limitation of 40 exists.
         * But it is removed in new firmware version.
         * So remove the limitation of 40 from the check callback function.
        
        if(limit > 40)
        {
            limit = 40;
        }
        
        *********************************************************************/  
        
       _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));
    }

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH -*/
static ViStatus _VI_FUNC itscopeAttrTriggerLevelByCh_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf (io, ":TRIG:A:LEV:%s?", "%le", channelName, value));
	_viCheckErr(_viQueryf(vi, ":TRIG:A:LEV:%s?", "%le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerLevelByCh_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viPrintf (io, ":TRIG:A:LEV:%s %le", channelName, value) );
	_viCheckErr(_viPrintf(vi, ":TRIG:A:LEV:%s %le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerLevelByCh_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      tSource[BUFFER_SIZE];
    ViInt32     triggerType;
    ViReal64    ext = 0.0;
    
   _checkErr( Ivi_CoerceChannelName (vi, channelName, VI_NULL) );
    
    if( !strstr(channelName, "CH") && !strstr(channelName, "D") )
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);         
    }
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));
    
    if (!(triggerType == ITSCOPE_VAL_EDGE_TRIGGER || 
          triggerType == ITSCOPE_VAL_GLITCH_TRIGGER)) 
    {
         _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, "Cannot set trigger level when "
                         "trigger type is not Edge or Glitch Trigger.");
    }

   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
              0, BUFFER_SIZE, tSource));
    
    if (!strcmp (tSource, ITSCOPE_VAL_EXTERNAL))
        ext = 1.0;
    
    if (ext != 0.0)
    {   /*- Trigger source is External -*/
       _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, -8.0 * ext, 8.0 * ext));
    }
    else if (strcmp(tSource, ITSCOPE_VAL_AC_LINE) == 0)
    {   /*- Trigger source is AC Line -*/
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, "Cannot set trigger level when "
                                                             "trigger source is AC line.");
    } 
    else if (strstr (channelName, "CH"))
    {   /*- Trigger source is input channel -*/
        ViReal64      rangeLimit, limit;
        
       _checkErr( Ivi_GetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit));

        limit = rangeLimit*0.8;                                            
        /******************************************************************** 
         * In old firmware version, trigger level limitation of 40 exists.
         * But it is removed in new firmware version.
         * So remove the limitation of 40 from the check callback function.

        if(limit > 40)
        {
            limit = 40;
        }

        *********************************************************************/  
        
       _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));
    }

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH -*/
static ViStatus _VI_FUNC itscopeAttrTriggerUpperthresByCh_ReadCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf (io, ":TRIGger:A:UPPerthreshold:%s?", "%le", channelName, value) );
	_viCheckErr(_viQueryf(vi, ":TRIGger:A:UPPerthreshold:%s?", "%le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerUpperthresByCh_WriteCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viPrintf (io, ":TRIGger:A:UPPerthreshold:%s %le", channelName, value) );
	_viCheckErr(_viPrintf(vi, ":TRIGger:A:UPPerthreshold:%s %le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerThresholdByCh_CheckCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    rangeLimit, limit;
    ViInt32     triggerType;
    
    if( !((strcmp (channelName, "CH1") == 0)
            || (strcmp (channelName, "CH2") == 0)
                || (strcmp (channelName, "CH3") == 0)
                    || (strcmp (channelName, "CH4") == 0)
                        || (strcmp(channelName, "RF") ==0)) )
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    } 
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));
    if (!((triggerType == ITSCOPE_VAL_RUNT_TRIGGER)
            || (triggerType == ITSCOPE_VAL_TRANSITION_TRIGGER)))
    {               
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit) );

    limit = rangeLimit*0.8;
    /******************************************************************** 
     * In old firmware version, trigger level limitation of 40 exists.
     * But it is removed in new firmware version.
     * So remove the limitation of 40 from the check callback function.
    
        if(limit > 40)
        {
            limit = 40;
        }

    *********************************************************************/
    
   _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, -limit, limit)); 
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH -*/
static ViStatus _VI_FUNC itscopeAttrTriggerLowerthresByCh_ReadCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viQueryf (io, ":TRIGger:A:LOWerthreshold:%s?", "%le", channelName, value) );
	_viCheckErr(_viQueryf(vi, ":TRIGger:A:LOWerthreshold:%s?", "%le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerLowerthresByCh_WriteCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viPrintf (io, ":TRIGger:A:LOWerthreshold:%s %le", channelName, value) );
	_viCheckErr(_viPrintf(vi, ":TRIGger:A:LOWerthreshold:%s %le", channelName, value));

Error:
    return error;
}


    /*- ITSCOPE_ATTR_TRIGGER_HOLDOFF -*/
static IviRangeTableEntry attrTriggerHoldoffRangeTableEntries[] =
{
    {0.0,       20.0e-9,    0,  "",     0},
    {20.0e-9,   8.0,        0,  "",     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
   
static IviRangeTable attrTriggerHoldoffRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    "",
    attrTriggerHoldoffRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrTriggerHoldoff_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:HOLD:TIM?", "%lf", value) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:HOLD:TIM?", "%lf", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerHoldoff_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viPrintf (io, ":TRIG:A:HOLD:TIM %le", value) );
	_viCheckErr(_viPrintf(vi, ":TRIG:A:HOLD:TIM %le", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTriggerHoldoff_CoerceCallback (ViSession vi, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViReal64 value, 
                                                                    ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;
    ViInt32  comparePrecision;
    
   _checkErr( Ivi_GetAttrComparePrecision (vi, attributeId, &comparePrecision));

    if ((value >= 0) && (value <= 20.0e-9))
    {
        newValue = 20.0e-9;
    }
    else
    {
       _checkErr( itscope_CoerceToResolution (vi, value, 2.0e-9, comparePrecision, &newValue));
    }
   
Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_TRIGGER_MODIFIER -*/
static IviRangeTableEntry attrTriggerModifierRangeTableEntries[] =
{
    {ITSCOPE_VAL_NO_TRIGGER_MOD,    0,  0,  "NORMAL",  0}, 
    {ITSCOPE_VAL_AUTO,              0,  0,  "AUTO",    0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   

static IviRangeTable  attrTriggerModifierRangeTable =       
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    ":TRIG:A:MOD",
    attrTriggerModifierRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_TRIGGER_COUPLING -*/
static IviRangeTableEntry attrPEK1TriggerCouplingRangeTableEntries[] =
{ 
    {ITSCOPE_VAL_DC,            0,  0,  "DC",       0},
    {ITSCOPE_VAL_LF_REJECT,     0,  0,  "LFREJ",    0}, 
    {ITSCOPE_VAL_HF_REJECT,     0,  0,  "HFREJ",    0}, 
    {ITSCOPE_VAL_NOISE_REJECT,  0,  0,  "NOISEREJ", 0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   

static IviRangeTable  attrPEK1TriggerCouplingRangeTable =       
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    ":TRIG:A:EDGE:COUP", 
    attrPEK1TriggerCouplingRangeTableEntries, 
};

static ViStatus _VI_FUNC itscopeAttrTriggerCoupling_RangeTableCallback (ViSession vi,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    {
        tblPtr = &attrPEK1TriggerCouplingRangeTable;
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

    /*- ITSCOPE_ATTR_TRIGGER_SLOPE -*/
static IviRangeTableEntry attrTriggerSlopeRangeTableEntries[] =
{ 
    {ITSCOPE_VAL_POSITIVE,     0,   0,  "RISE", 0}, 
    {ITSCOPE_VAL_NEGATIVE,     0,   0,  "FALL", 0}, 
    {ITSCOPE_VAL_EITHER,       0,   0,  "EITHER", 0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   

static IviRangeTable  attrTriggerSlopeRangeTable =
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    ":TRIG:A:EDGE:SLO", 
    attrTriggerSlopeRangeTableEntries, 
};


    /*- ITSCOPE_ATTR_TV_TRIGGER_POLARITY -*/
static IviRangeTableEntry attrTvTriggerPolarityRangeTableEntries[] =
{
    {ITSCOPE_VAL_TV_POSITIVE,   0, 0, "POSITIVE", 0},
    {ITSCOPE_VAL_TV_NEGATIVE,   0, 0, "NEGATIVE", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrTvTriggerPolarityRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:VID:POL",
    attrTvTriggerPolarityRangeTableEntries,
};

    /*- ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT -*/
static IviRangeTableEntry attrPEK1TvTriggerSignalFormatRangeTableEntries[] =
{
    {ITSCOPE_VAL_NTSC,          0, 0, "NTSC",           0},
    {ITSCOPE_VAL_PAL,           0, 0, "PAL",            0},
    {ITSCOPE_VAL_SECAM,         0, 0, "SECAM",          0},
    {ITSCOPE_VAL_BILEVELCUSTOM, 0, 0, "BILEVELCUSTOM",  0},
    {ITSCOPE_VAL_TRILEVELCUSTOM,0, 0, "TRILEVELCUSTOM", 0},
    {ITSCOPE_VAL_HD480P60,      0, 0, "HD480P60",       0},  
    {ITSCOPE_VAL_HD576P50,      0, 0, "HD576P50",       0},  
    {ITSCOPE_VAL_HD720P30,      0, 0, "HD720P30",       0},  
    {ITSCOPE_VAL_HD720P50,      0, 0, "HD720P50",       0},
    {ITSCOPE_VAL_HD720P60,      0, 0, "HD720P60",       0},
    {ITSCOPE_VAL_HD875I60,      0, 0, "HD875I60",       0},
    {ITSCOPE_VAL_HD1080P24,     0, 0, "HD1080P24",      0},
    {ITSCOPE_VAL_HD1080SF24,    0, 0, "HD1080SF24",     0},
    {ITSCOPE_VAL_HD1080I50,     0, 0, "HD1080I50",      0},
    {ITSCOPE_VAL_HD1080I60,     0, 0, "HD1080I60",      0},
    {ITSCOPE_VAL_HD1080P25,     0, 0, "HD1080P25",      0},
    {ITSCOPE_VAL_HD1080P30,     0, 0, "HD1080P30",      0},
    {ITSCOPE_VAL_HD1080P50,     0, 0, "HD1080P50",      0},
    {ITSCOPE_VAL_HD1080P60,     0, 0, "HD1080P60",      0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};         /*HDTV and CUSTOM type is not suported any more in new firmware in 4k model*/

static IviRangeTable attrPEK1TvTriggerSignalFormatRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrPEK1TvTriggerSignalFormatRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrTvTriggerSignalFormat_ReadCallback (ViSession vi, 
                                                                       ViSession io, 
                                                                       ViConstString channelName, 
                                                                       ViAttr attributeId, 
                                                                       ViInt32 *value)
{
    ViChar      buf[BUFFER_SIZE];
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr rangeTable;
    
   _checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));
    
   //_viCheckErr( viQueryf(io, ":TRIG:A:VID:STAN?", "%s", buf));
   _viCheckErr(_viQueryf(vi, ":TRIG:A:VID:STAN?", "%s", buf));

    if (Ivi_GetViInt32EntryFromString (buf, rangeTable, value, 
                                       VI_NULL, VI_NULL, VI_NULL, VI_NULL) < VI_SUCCESS)                                   
       _viCheckErr( VI_ERROR_INV_RESPONSE);
        
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerSignalFormat_WriteCallback (ViSession vi, 
                                                                        ViSession io, 
                                                                        ViConstString channelName, 
                                                                        ViAttr attributeId, 
                                                                        ViInt32 value)
{
    ViStatus    error    = VI_SUCCESS;
    ViString    tvTriggerSignalTypeStr;
    ViAttr      videoSignalType = value;
    IviRangeTablePtr rangeTable;
    
   _checkErr( Ivi_GetAttrRangeTable (vi, channelName, attributeId, &rangeTable));
    
        /* Set the base type */
   _checkErr( Ivi_GetViInt32EntryFromValue (videoSignalType, rangeTable, 
                                            VI_NULL, VI_NULL, VI_NULL, VI_NULL, 
                                            &tvTriggerSignalTypeStr, VI_NULL));

   //_viCheckErr( viPrintf (io, ":TRIG:A:VID:STAN %s;", tvTriggerSignalTypeStr));
   _viCheckErr(_viPrintf(vi, ":TRIG:A:VID:STAN %s;", tvTriggerSignalTypeStr));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerSignalFormat_RangeTableCallback (ViSession vi,
                                                                              ViConstString channelName,
                                                                              ViAttr attributeId,
                                                                              IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    {
        tblPtr = &attrPEK1TvTriggerSignalFormatRangeTable;
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_EVENT -*/
static IviRangeTableEntry attrTvTriggerEventRangeTableEntries[] =
{
    {ITSCOPE_VAL_TV_EVENT_FIELD1,       0,  0,  "ODD",          0},  /* ODD  */
    {ITSCOPE_VAL_TV_EVENT_FIELD2,       0,  0,  "EVEN",         0},  /* EVEN */
    {ITSCOPE_VAL_TV_EVENT_ANY_FIELD,    0,  0,  "ALLFIELDS",    0},
    {ITSCOPE_VAL_TV_EVENT_ANY_LINE,     0,  0,  "ALLLINES",     0},
    {ITSCOPE_VAL_TV_EVENT_LINE_NUMBER,  0,  0,  "NUMERIC",      0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrTvTriggerEventRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    "TRIGger:A:VIDeo:FIELD",
    attrTvTriggerEventRangeTableEntries,
};
    
static ViStatus _VI_FUNC itscopeAttrTvTriggerEvent_WriteCallback (ViSession vi, 
                                                                   ViSession io, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    
   _checkErr( itscopeStdViInt32_WriteCallback (vi, io, "", attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerEvent_ReadCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViInt32 *value)
{
    ViStatus  error = VI_SUCCESS;
    
   _checkErr( itscopeStdViInt32_ReadCallback (vi, io, "", attributeId, value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER -*/
static ViStatus _VI_FUNC itscopeAttrTvTriggerLineNumber_ReadCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    
   //_viCheckErr( viQueryf(io, ":TRIG:A:VID:LINE?", "%ld", value));
	_viCheckErr(_viQueryf(vi, ":TRIG:A:VID:LINE?", "%ld", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerLineNumber_WriteCallback (ViSession vi, 
                                                                  ViSession io, 
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId, 
                                                                  ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

   //_viCheckErr( viPrintf (io, ":TRIG:A:VID:LINE %ld;", value));
	_viCheckErr(_viPrintf(vi, ":TRIG:A:VID:LINE %ld;", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerLineNumber_CheckCallback  (ViSession vi, 
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId, 
                                                                   ViInt32 value)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   tvTriggerType, typeRange = 1;
        
        /* Get Trigger Type */
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT, 0, 
                                        &tvTriggerType));
    switch (tvTriggerType)
    {
        case ITSCOPE_VAL_PAL:
        case ITSCOPE_VAL_SECAM:
        case ITSCOPE_VAL_HDTV:
        case ITSCOPE_VAL_CUSTOM:
        case ITSCOPE_VAL_BILEVELCUSTOM: 
        case ITSCOPE_VAL_TRILEVELCUSTOM:
        case ITSCOPE_VAL_HD576P50:
        {
            typeRange = 625;
            break;
        }           
        case ITSCOPE_VAL_NTSC:
        case ITSCOPE_VAL_HD480P60: 
        {
            typeRange = 525;
            break;
        }
        case ITSCOPE_VAL_HD720P30:
        case ITSCOPE_VAL_HD720P50:
        case ITSCOPE_VAL_HD720P60: 
        {
            typeRange = 750;
            break;
        }
        case ITSCOPE_VAL_HD875I60:
        {
            typeRange = 875;
            break;
        }
        case ITSCOPE_VAL_HD1080P24:
        case ITSCOPE_VAL_HD1080SF24:
        case ITSCOPE_VAL_HD1080I50:
        case ITSCOPE_VAL_HD1080I60:
        case ITSCOPE_VAL_HD1080P25: 
        case ITSCOPE_VAL_HD1080P30:
        case ITSCOPE_VAL_HD1080P50:
        case ITSCOPE_VAL_HD1080P60:
        {
            typeRange = 1125;
            break;
        }
    }
    
    error = (value >= 1 && value <= typeRange) ? VI_SUCCESS : IVI_ERROR_INVALID_VALUE;

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT -*/
static IviRangeTableEntry attrPEK1TvTriggerHdtvFormatRangeTableEntries[] =
    {
        {ITSCOPE_VAL_HDTV_1080P24, 0, 0, "HD1080P24", 0},
        {ITSCOPE_VAL_HDTV_720P60, 0, 0, "HD720P60", 0},
        {ITSCOPE_VAL_HDTV_480P60, 0, 0, "HD480P60", 0},
        {ITSCOPE_VAL_HDTV_1080I50, 0, 0, "HD1080I50", 0},
        {ITSCOPE_VAL_HDTV_1080P25, 0, 0, "HD1080P25", 0},
        {ITSCOPE_VAL_HDTV_1080I60, 0, 0, "HD1080I60", 0},
        {ITSCOPE_VAL_HDTV_1080PSF24, 0, 0, "HD1080SF24", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrPEK1TvTriggerHdtvFormatRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        ":TRIG:A:VID:HD:FORM",
        attrPEK1TvTriggerHdtvFormatRangeTableEntries,
    };


static ViStatus _VI_FUNC itscopeAttrTvTriggerHdtvFormat_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
       
   _checkErr( itscopeStdViInt32_ReadCallback (vi, io, "", attributeId, value));   
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerHdtvFormat_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( itscopeStdViInt32_WriteCallback (vi, io, "", attributeId, value));    
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerHdtvFormat_RangeTableCallback (ViSession vi,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    {
        tblPtr = &attrPEK1TvTriggerHdtvFormatRangeTable;
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_FIELD_HOLDOFF -*/
static IviRangeTableEntry attrTvTriggerFieldHoldoffRangeTableEntries[] =
    {
        {0, 8.5, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrTvTriggerFieldHoldoffRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrTvTriggerFieldHoldoffRangeTableEntries,
    };

static ViStatus _VI_FUNC itscopeAttrTvTriggerFieldHoldoff_ReadCallback (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
   _viCheckErr( viQueryf (io, ":TRIG:A:VID:HOLD:FIELD?", "%Lf", value) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerFieldHoldoff_WriteCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    
   _viCheckErr( viPrintf (io, ":TRIG:A:VID:HOLD:FIELD %Lf", value) );

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_FORMAT -*/
static IviRangeTableEntry attrTvTriggerCustomFormatRangeTableEntries[] =
    {
        {ITSCOPE_VAL_TV_CUSTOM_INTERLACED,  0, 0, "INTERLA", 0},
        {ITSCOPE_VAL_TV_CUSTOM_PROGRESSIVE, 0, 0, "PROG", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrTvTriggerCustomFormatRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_TRUE,
        VI_TRUE,
        ":TRIG:A:VID:CUST:FORM",
        attrTvTriggerCustomFormatRangeTableEntries,
    };

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomFormat_ReadCallback (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( itscopeStdViInt32_ReadCallback (vi, io, "", attributeId, value));     
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomFormat_WriteCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( itscopeStdViInt32_WriteCallback (vi, io, "", attributeId, value));        
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE -*/
static IviRangeTableEntry attrTvTriggerCustomScanRateRangeTableEntries[] =
    {
        {ITSCOPE_VAL_TV_RATE15K, 0, 0, "RATE15K", 0},
        {ITSCOPE_VAL_TV_RATE20K, 0, 0, "RATE20K", 0},
        {ITSCOPE_VAL_TV_RATE25K, 0, 0, "RATE25K", 0},
        {ITSCOPE_VAL_TV_RATE35K, 0, 0, "RATE35K", 0},
        {ITSCOPE_VAL_TV_RATE50K, 0, 0, "RATE50K", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrTvTriggerCustomScanRateRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_TRUE,
        VI_TRUE,
        ":TRIG:A:VID:CUST:SCAN",
        attrTvTriggerCustomScanRateRangeTableEntries,
    };

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomScanRate_ReadCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( itscopeStdViInt32_ReadCallback (vi, io, "", attributeId, value));     
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomScanRate_WriteCallback (ViSession vi,
                                                                           ViSession io,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( itscopeStdViInt32_WriteCallback (vi, io, "", attributeId, value));    
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_LINE_PERIOD -*/
static IviRangeTableEntry attrTvTriggerCustomLinePeriodRangeTableEntries[] =
    {
        {0, 500E-6, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrTvTriggerCustomLinePeriodRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrTvTriggerCustomLinePeriodRangeTableEntries,
    };

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomLinePeriod_ReadCallback (ViSession vi,
                                                                            ViSession io,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
   _viCheckErr( viQueryf (io, ":TRIG:A:VID:CUST:LINEP?", "%Lf", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomLinePeriod_WriteCallback (ViSession vi,
                                                                             ViSession io,
                                                                             ViConstString channelName,
                                                                             ViAttr attributeId,
                                                                             ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    
   _viCheckErr( viPrintf (io, ":TRIG:A:VID:CUST:LINEP %Lf", value) );          

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SYNC_INTERVAL -*/ 
static IviRangeTableEntry attrTvTriggerCustomSyncIntervalRangeTableEntries[] =
    {
        {0, 200E-6, 0, "", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrTvTriggerCustomSyncIntervalRangeTable =
    {
        IVI_VAL_RANGED,
        VI_TRUE,
        VI_TRUE,
        VI_NULL,
        attrTvTriggerCustomSyncIntervalRangeTableEntries,
    };

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomSyncInterval_ReadCallback (ViSession vi,
                                                                              ViSession io,
                                                                              ViConstString channelName,
                                                                              ViAttr attributeId,
                                                                              ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
   _viCheckErr( viQueryf (io, ":TRIG:A:VID:CUST:SYNCI?", "%Lf", value));    

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTvTriggerCustomSyncInterval_WriteCallback (ViSession vi,
                                                                               ViSession io,
                                                                               ViConstString channelName,
                                                                               ViAttr attributeId,
                                                                               ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    
   _viCheckErr( viPrintf (io, ":TRIG:A:VID:CUST:SYNCI %Lf", value) );

Error:
    return error;
}

    /*- ITSCOPE_ATTR_RUNT_HIGH_THRESHOLD -*/
static ViStatus _VI_FUNC itscopeAttrRuntHighThreshold_WriteCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
   _checkErr ( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH,
                                            0, value ) );
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrRuntHighThreshold_ReadCallback (ViSession vi, 
                                                                        ViSession io, 
                                                                        ViConstString channelName, 
                                                                        ViAttr attributeId, 
                                                                        ViReal64 *value)
{
    ViStatus  error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
    
    if( !((strcmp(channel, "CH1") == 0)
            || (strcmp(channel, "CH2") == 0)
                || (strcmp(channel, "CH3") == 0)
                    || (strcmp(channel, "CH4") == 0)
                        ||(strcmp(channel, "RF") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH,
                                            0, value ) );   

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrRuntHighThreshold_CheckCallback (ViSession vi, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    rangeLimit, limit;
    ViInt32     triggerType;
    ViChar      triggerSource[BUFFER_SIZE]; 
    ViString    channel;    
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, triggerSource));
   _checkErr( Ivi_CoerceChannelName(vi, triggerSource, &channel) ); 
    
    if( !((strcmp (channel, "CH1") == 0)
            || (strcmp (channel, "CH2") == 0)
                || (strcmp (channel, "CH3") == 0)
                    || (strcmp (channel, "CH4") == 0)
                        ||(strcmp (channel, "RF") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    } 
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));
    if (!(triggerType == ITSCOPE_VAL_RUNT_TRIGGER))
    {               
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }   
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit) );
                                            
    limit = rangeLimit*0.8;
    /******************************************************************** 
     * In old firmware version, trigger level limitation of 40 exists.
     * But it is removed in new firmware version.
     * So remove the limitation of 40 from the check callback function.
    
        if(limit > 40)
        {
            limit = 40;
        }

    *********************************************************************/
    
   _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));

Error:
    return error;                                                    
}


    /*- ITSCOPE_ATTR_RUNT_LOW_THRESHOLD -*/
static ViStatus _VI_FUNC itscopeAttrRuntLowThreshold_WriteCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
  
   _checkErr ( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH,
                                            0, value ) );
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrRuntLowThreshold_ReadCallback (ViSession vi, 
                                                                    ViSession io, 
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId, 
                                                                    ViReal64 *value)
{
    ViStatus  error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
    
    if( !((strcmp(channel, "CH1") == 0)
            || (strcmp(channel, "CH2") == 0)
                || (strcmp(channel, "CH3") == 0)
                    || (strcmp(channel, "CH4") == 0)
                        ||(strcmp(channel,"RF") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH,
                                            0, value ) );   

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrRuntLowThreshold_CheckCallback (ViSession vi, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    rangeLimit, limit;
    ViInt32     triggerType;
    ViChar      triggerSource[BUFFER_SIZE]; 
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, triggerSource));
   _checkErr( Ivi_CoerceChannelName(vi, triggerSource, &channel) );
                                                
    if( !((strcmp (channel, "CH1") == 0)
            || (strcmp (channel, "CH2") == 0)
                || (strcmp (channel, "CH3") == 0)
                    || (strcmp (channel, "CH4") == 0)
                        ||(strcmp (channel, "RF") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    } 
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));
    if (!(triggerType == ITSCOPE_VAL_RUNT_TRIGGER))
    {               
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }   
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit) );
    limit = rangeLimit*0.8;
    
    /******************************************************************** 
     * In old firmware version, trigger level limitation of 40 exists.
     * But it is removed in new firmware version.
     * So remove the limitation of 40 from the check callback function.
    
        if(limit > 40)
        {
            limit = 40;
        }

    *********************************************************************/
    
   _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));

Error:
    return error; 
}
    

    /*- ITSCOPE_ATTR_RUNT_POLARITY -*/
static IviRangeTableEntry attrRuntPolarityRangeTableEntries[] =
{
    {ITSCOPE_VAL_RUNT_POSITIVE, 0, 0, "POSITIVE", 0},
    {ITSCOPE_VAL_RUNT_NEGATIVE, 0, 0, "NEGATIVE", 0},
    {ITSCOPE_VAL_RUNT_EITHER,   0, 0, "EITHER",   0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
        
static IviRangeTable attrRuntPolarityRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:A:PUL:RUNT:POL",
    attrRuntPolarityRangeTableEntries,
};

    /*- ITSCOPE_ATTR_RUNT_WHEN -*/
static IviRangeTableEntry attrRuntWhenRangeTableEntries[] =
{
    {ITSCOPE_VAL_RUNT_WHEN_OCCURS,      0,  0, "OCCURS",    0},
    {ITSCOPE_VAL_RUNT_WHEN_WIDER_THAN,  0,  0, "MORETHAN",  0},
    {ITSCOPE_VAL_RUNT_WHEN_LESS_THAN,   0,  0, "LESSTHAN",  0},
    {ITSCOPE_VAL_RUNT_WHEN_EQUAL,       0,  0, "EQUAL",     0},
    {ITSCOPE_VAL_RUNT_WHEN_NOTEQUAL,    0,  0, "UNEQUAL",   0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
        
static IviRangeTable attrRuntWhenRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:A:PUL:RUNT:WHE",
    attrRuntWhenRangeTableEntries,
};

    /*- ITSCOPE_ATTR_RUNT_WIDTH -*/
static IviRangeTableEntry attrRuntWidthRangeTableEntries[] =
{
    /*************************************
     * Minimum Runt Width in old firmware
     * version is 2.5e-9. Bus is 4.0e-9
     * in new firmware. Remove the old 
     * value from the range table.
     
    {2.5e-9,    8.0,    0,  "",     0}, 
     *************************************/
     
    {4.0e-9,    8.0,    0,  "",     0},                                                                                                                                                                                                    
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrRuntWidthRangeTable =
{
    IVI_VAL_RANGED, 
    VI_TRUE, 
    VI_TRUE, 
    ":TRIG:A:PUL:RUNT:WID", 
    attrRuntWidthRangeTableEntries, 
};

static ViStatus _VI_FUNC itscopeAttrRuntWidth_CoerceCallback (ViSession vi, ViConstString channelName, 
                                                               ViAttr attributeId, 
                                                               ViReal64 value, 
                                                               ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;
    
    /**************************************************************************
     * The resolution in old firmware version is 0.5ns, but 0.8ns in new 
     * firmware. So remove the old resolution from the driver source code.
     
       _checkErr( itscope_CoerceToResolution (vi, value, 0.5e-9, 12, &newValue));
    
    ***************************************************************************/
    
   _checkErr( itscope_CoerceToResolution (vi, value, 0.8e-9, 12, &newValue) );
    
Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_GLITCH_POLARITY -*/
static IviRangeTableEntry attrGlitchPolarityRangeTableEntries[] =
{
    {ITSCOPE_VAL_GLITCH_POSITIVE,  0,   0,  "POSITIVE", 0}, 
    {ITSCOPE_VAL_GLITCH_NEGATIVE,  0,   0,  "NEGATIVE", 0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
static IviRangeTable  attrGlitchPolarityRangeTable =
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    ":TRIG:A:PUL:WID:POL", 
    attrGlitchPolarityRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_GLITCH_CONDITION -*/
static IviRangeTableEntry attrGlitchConditionRangeTableEntries[] =
{
    {ITSCOPE_VAL_GLITCH_GREATER_THAN,      0, 0, "MORETHAN",   0},
    {ITSCOPE_VAL_GLITCH_LESS_THAN,         0, 0, "LESSTHAN",   0},
    {ITSCOPE_VAL_GLITCH_WITHIN_5_PERCENT,  0, 0, "EQUAL",      0},
    {ITSCOPE_VAL_GLITCH_OUTSIDE_5_PERCENT, 0, 0, "UNEQUAL",     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrGlitchConditionRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:A:PUL:WID:WHE",
    attrGlitchConditionRangeTableEntries,
};

    /*- ITSCOPE_ATTR_GLITCH_WIDTH -*/
static IviRangeTableEntry attrGlitchWidthRangeTableEntries[] =
{
    /*************************************
     * Minimum Runt Width in old firmware
     * version is 2.5e-9. Bus is 4.0e-9
     * in new firmware. Remove the old 
     * value from the range table.
     
    {2.5e-9,    8.0,    0,      "",     0}, 
     *************************************/
    
    {4.0e-09,   8.0,    0,      "",     0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
static IviRangeTable  attrGlitchWidthRangeTable =
{
    IVI_VAL_RANGED, 
    VI_TRUE, 
    VI_TRUE, 
    ":TRIG:A:PUL:WID:WID", 
    attrGlitchWidthRangeTableEntries, 
};

static ViStatus _VI_FUNC itscopeAttrGlitchWidth_CoerceCallback (ViSession vi,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 value,
                                                           ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;
    
    /**************************************************************************
     * The resolution in old firmware version is 0.5ns, but 0.8ns in new 
     * firmware. So remove the old resolution from the driver source code.
     
       _checkErr( itscope_CoerceToResolution (vi, value, 0.5e-9, 12, &newValue));
    
    ***************************************************************************/
    
   _checkErr( itscope_CoerceToResolution (vi, value, 0.8e-9, 12, &newValue));
    
Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_AC_LINE_TRIGGER_SLOPE -*/
static IviRangeTableEntry attrAcLineTriggerSlopeRangeTableEntries[] =
{
    {ITSCOPE_VAL_AC_LINE_POSITIVE, 0, 0, "", 0},
    {ITSCOPE_VAL_AC_LINE_NEGATIVE, 0, 0, "", 0},
    {ITSCOPE_VAL_AC_LINE_EITHER,   0, 0, "", 0},     
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrAcLineTriggerSlopeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrAcLineTriggerSlopeRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrAcLineTriggerSlope_ReadCallback (ViSession vi, 
                                                                     ViSession io,
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     val;
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SLOPE, 0, &val));
    if (val == ITSCOPE_VAL_POSITIVE)
    {
        val = ITSCOPE_VAL_AC_LINE_POSITIVE;
    }
    else if (val == ITSCOPE_VAL_NEGATIVE)
    {
        val = ITSCOPE_VAL_AC_LINE_NEGATIVE;
    }
    else if (val == ITSCOPE_VAL_EITHER)
    {
        val = ITSCOPE_VAL_AC_LINE_EITHER;   
    }
    
    *value = val;

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrAcLineTriggerSlope_WriteCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     newVal = ITSCOPE_VAL_NEGATIVE;
    ViInt32     modelNo;
    
   _viCheckErr(Ivi_GetAttributeViInt32(vi,VI_NULL,ITSCOPE_ATTR_MODEL,0,&modelNo));
    
    if (value == ITSCOPE_VAL_AC_LINE_POSITIVE)
    {
        newVal = ITSCOPE_VAL_POSITIVE;
    }
    else if (value == ITSCOPE_VAL_AC_LINE_EITHER)    /*Only available for MOD3K*/
    {
        {
           _viCheckErr(IVI_ERROR_INVALID_CONFIGURATION);
        }
    }        
    
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SLOPE, 0, newVal));
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_ACQUISITION_TYPE -*/
static IviRangeTableEntry attrAcquisitionTypeRangeTableEntries[] =
{
    {ITSCOPE_VAL_NORMAL,        0, 0, "SAMPLE",     0},
    {ITSCOPE_VAL_HI_RES,        0, 0, "HIRES",      0},
    {ITSCOPE_VAL_PEAK_DETECT,   0, 0, "PEAKDETECT", 0},
    {ITSCOPE_VAL_ENVELOPE,      0, 0, "ENVELOPE",   0},
    {ITSCOPE_VAL_AVERAGE,       0, 0, "AVERAGE",    0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
    
static IviRangeTable attrAcquisitionTypeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":ACQ:MOD",
    attrAcquisitionTypeRangeTableEntries,
};

    /*- ITSCOPE_ATTR_ACQUISITION_START_TIME -*/
static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_WriteCallback (ViSession vi, 
                                                              ViSession io, 
                                                              ViConstString channelName, 
                                                              ViAttr attributeId, 
                                                              ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    horzTime, sendTime;
    
   _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD,
									   0, &horzTime));
   // pretrigger setup 

   sendTime = value;
    //_viCheckErr( viPrintf (io, ":HOR:DEL:TIM %le", sendTime));
	_viCheckErr(_viPrintf(vi, ":HOR:DEL:TIM %le", sendTime));

Error:
    return error;
}
 
static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_ReadCallback (ViSession vi, 
                                                             ViSession io, 
                                                             ViConstString channelName, 
                                                             ViAttr attributeId, 
                                                             ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    delay, horzTime;
    
   _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD,
                                        0, &horzTime));
    //_viCheckErr( viQueryf (io, ":HOR:DEL:TIM?", "%le", &delay));
	_viCheckErr(_viQueryf(vi, ":HOR:DEL:TIM?", "%le", &delay));
    *value = delay;

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_CheckCallback (ViSession vi, 
                                                                         ViConstString channelName, 
                                                                         ViAttr attributeId, 
                                                                         ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 horzTime;
    ViInt32 recordLength;
    ViReal64 bottomValue = 200.0e-9;
    
   _checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD,
                                        0, &horzTime));
    
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 0, &recordLength) );
    if(recordLength >= 10000000)
    {
        bottomValue = 10.0e-3;                   /* 10ms */
    }
    else if(recordLength >= 1000000)
    {
        bottomValue = 1000.0e-6;                 /* 1000us */ 
    }
    else if(recordLength >= 100000)
    {
        bottomValue = 100.0e-6;                  /* 100us */ 
    }
    else if(recordLength >= 10000)
    {
        bottomValue = 10.0e-6;                   /* 10us */ 
    }
    
    /* Range: -horzTime ~ 50.0 when the horizontal time is greater than bottomValue */
    if(horzTime > bottomValue)
    {
        if ((value < -horzTime) || (value > 50.0))
        {
           _viCheckErr( IVI_ERROR_INVALID_VALUE);             
        }       
    }
    /* Range: -2us ~ 50.0 when the horizontal time is equal to or lesser than bottomValue */
    else
    {
        if ((value < -bottomValue) || (value > 50.0))
        {
           _viCheckErr( IVI_ERROR_INVALID_VALUE);       
        }
    }
        
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrAcquisitionStartTime_CoerceCallback (ViSession vi, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value, 
                                                                      ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;
    ViInt32  comparePrecision;
    
   _checkErr( Ivi_GetAttrComparePrecision (vi, attributeId, &comparePrecision));

    /**************************************************************************************
     * The resolution in old firmware version is 4ns, but 400ns in new 
     * firmware. So remove the old resolution from the driver source code.
     
   _checkErr( itscope_CoerceToResolution (vi, value, 4.0e-9, comparePrecision, &newValue));
    ***************************************************************************************/
    
   _checkErr( itscope_CoerceToResolution (vi, value, 400.0e-9, comparePrecision, &newValue));

Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_INITIATE_CONTINUOUS -*/
static ViStatus _VI_FUNC itscopeAttrInitiateContinuous_ReadCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViBoolean *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rdBuf[256];

    //_viCheckErr( viQueryf (io, ":ACQ:STOPA?", "%s", rdBuf));
	_viCheckErr(_viQueryf(vi, ":ACQ:STOPA?", "%s", rdBuf));
	*value = (rdBuf[0] == 'R');

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrInitiateContinuous_WriteCallback (ViSession vi, 
                                                                       ViSession io, 
                                                                       ViConstString channelName, 
                                                                       ViAttr attributeId, 
                                                                       ViBoolean value)

{
    ViStatus    error = VI_SUCCESS;
	
    if(value == VI_TRUE)
    {
        //_viCheckErr( viPrintf (io, ":ACQ:STOPA %s", "RUNST") );  
		_viCheckErr(_viPrintf(vi, ":ACQ:STOPA %s", "RUNST"));
    }
    else
    {
        //_viCheckErr( viPrintf (io, ":ACQ:STOPA %s", "SEQ") );    
		_viCheckErr(_viPrintf(vi, ":ACQ:STOPA %s", "SEQ"));
    }
	
Error:
    return error;
}

    /*- ITSCOPE_ATTR_NUM_AVERAGES -*/
static IviRangeTableEntry attrNumAveragesRangeTableEntries[] =
    {
        {0, 2, 2, "2", 0},
        {2, 4, 4, "4", 0},
        {4, 8, 8, "8", 0},
        {8, 16, 16, "16", 0},
        {16, 32, 32, "32", 0},
        {32, 64, 64, "64", 0},
        {64, 128, 128, "128", 0},
        {128, 256, 256, "256", 0},
        {256, 512, 512, "512", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };
   
static IviRangeTable attrNumAveragesRangeTable =
    {
        IVI_VAL_COERCED,
        VI_FALSE,
        VI_TRUE,
        ":ACQ:NUMAV",
        attrNumAveragesRangeTableEntries,
    };

    /*- ITSCOPE_ATTR_NUM_ENVELOPES -*/
static IviRangeTableEntry attrNumEnvelopesRangeTableEntries[] =
{
    {ITSCOPE_VAL_INFINITE, ITSCOPE_VAL_INFINITE, 0, "0", 0},
    {1, 2000, 0, "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
   
static IviRangeTable attrNumEnvelopesRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    "",  
    attrNumEnvelopesRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrNumEnvelopes_ReadCallback (ViSession vi, 
                                                                ViSession io,
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViInt32 *value)
{
    ViStatus error = VI_SUCCESS; 
    ViChar buf[BUFFER_SIZE];

    if (!Ivi_Simulating (vi))
    {
        //_viCheckErr( viQueryf (io, ":ACQ:NUME?", "%s", buf) );
		_viCheckErr(_viQueryf(vi, ":ACQ:NUME?", "%s", buf));
        if( strstr(buf, "INFINITE") )
        {
            *value = ITSCOPE_VAL_INFINITE;
        }
        else
        {
            sscanf(buf,"%d",value);  
        }
    }
Error:
    return error;
}
                                                                
static ViStatus _VI_FUNC itscopeAttrNumEnvelopes_WriteCallback (ViSession vi, 
                                                                 ViSession io, 
                                                                 ViConstString channelName, 
                                                                 ViAttr attributeId, 
                                                                 ViInt32 value)

{
    ViStatus error = VI_SUCCESS;
    
    if (!Ivi_Simulating (vi))
    {
        if (value == ITSCOPE_VAL_INFINITE)
        {
            //_viCheckErr( viPrintf (io, ":ACQ:NUME INFI;"));
			_viCheckErr(_viPrintf(vi, ":ACQ:NUME INFI;"));
        }
        else
        {
            ///_viCheckErr( viPrintf (io, ":ACQ:NUME %ld;", value));  
			_viCheckErr(_viPrintf(vi, ":ACQ:NUME %ld;", value));
        }
    }
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_DELAY_TRIGGER_MODE  -*/
static IviRangeTableEntry attrDelayTriggerModeRangeTableEntries[] =
{
    {ITSCOPE_VAL_TRIG_AFTER_TIME,   0, 0, "TIME",   0},
    {ITSCOPE_VAL_TRIG_AFTER_EVENTS, 0, 0, "EVENTS", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
   
static IviRangeTable attrDelayTriggerModeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:B:BY",
    attrDelayTriggerModeRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrDelayTriggerMode_CheckCallback (ViSession vi, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  triggerType;   

    
   _checkErr( Ivi_DefaultCheckCallbackViInt32 (vi, "", ITSCOPE_ATTR_DELAY_TRIGGER_MODE, 
                                               value));
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE,  
                                      0, &triggerType));

    if ((triggerType != ITSCOPE_VAL_EDGE_TRIGGER) && 
                (triggerType != ITSCOPE_VAL_TV_TRIGGER))
    {
            _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, 
                            "The trigger type must be set to ITSCOPE_VAL_EDGE_TRIGGER "
                            "or ITSCOPE_VAL_TV_TRIGGER for the Delay Trigger Mode you "
                            "selected.");
    }

Error:
    return error;
}

    /*- ITSCOPE_ATTR_DELAY_TRIGGER_EVENTS -*/
static IviRangeTableEntry attrDelayTriggerEventsRangeTableEntries[] =
{ 
    {1,     9999999,    0,      "",     0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable  attrDelayTriggerEventsRangeTable =       
{
    IVI_VAL_RANGED, 
    VI_FALSE, 
    VI_FALSE, 
    ":TRIG:B:EVENTS:COUN", 
    attrDelayTriggerEventsRangeTableEntries, 
};
    
    /*- ITSCOPE_ATTR_DELAY_TRIGGER_COUPLING -*/
static IviRangeTableEntry attrPEK1DelayTriggerCouplingRangeTableEntries[] =
{
    {ITSCOPE_VAL_DC,            0,      0,          "DC",       0},
    {ITSCOPE_VAL_HF_REJECT,     0,      0,          "HFREJ",    0},
    {ITSCOPE_VAL_LF_REJECT,     0,      0,          "LFREJ",    0},
    {ITSCOPE_VAL_NOISE_REJECT,  0,      0,          "NOISEREJ", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
   
static IviRangeTable attrPEK1DelayTriggerCouplingRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:B:EDGE:COUP",
    attrPEK1DelayTriggerCouplingRangeTableEntries,
};


static ViStatus _VI_FUNC itscopeAttrDelayTriggerCoupling_RangeTableCallback (ViSession vi,
                                                                             ViConstString channelName,
                                                                             ViAttr attributeId,
                                                                             IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32 instrModelNo;
    
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &instrModelNo) );
    
    {
        tblPtr = &attrPEK1DelayTriggerCouplingRangeTable;
    }

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

    /*- ITSCOPE_ATTR_DELAY_TRIGGER_SLOPE -*/
static IviRangeTableEntry attrDelayTriggerSlopeRangeTableEntries[] =
{ 
    {ITSCOPE_VAL_POSITIVE,  0,  0,  "RISE", 0}, 
    {ITSCOPE_VAL_NEGATIVE,  0,  0,  "FALL", 0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   
static IviRangeTable  attrDelayTriggerSlopeRangeTable =       
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    ":TRIG:B:EDGE:SLO", 
    attrDelayTriggerSlopeRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_DELAY_TRIGGER_LEVEL -*/
static ViStatus _VI_FUNC itscopeAttrDelayTriggerLevel_ReadCallback (ViSession vi, 
                                                                     ViSession io, 
                                                                     ViConstString channelName, 
                                                                     ViAttr attributeId, 
                                                                     ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viQueryf(io, ":TRIG:B:LEV?", "%le", value));
	_viCheckErr(_viQueryf(vi, ":TRIG:B:LEV?", "%le", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrDelayTriggerLevel_WriteCallback (ViSession vi, 
                                                                      ViSession io, 
                                                                      ViConstString channelName, 
                                                                      ViAttr attributeId, 
                                                                      ViReal64 value)

{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viPrintf (io, ":TRIG:B:LEV %le;", value));
	_viCheckErr(_viPrintf(vi, ":TRIG:B:LEV %le;", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrDelayTriggerLevel_CheckCallback  (ViSession vi, 
                                                                       ViConstString channelName, 
                                                                       ViAttr attributeId, 
                                                                       ViReal64 value)
{
    ViChar      tSource[BUFFER_SIZE];
    ViStatus    error = VI_SUCCESS;
    ViInt32     triggerType;
    ViReal64    ext = 0.0;
    
   _checkErr(Ivi_GetAttributeViInt32(vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0, &triggerType));
    
    if (triggerType != ITSCOPE_VAL_EDGE_TRIGGER)
    {
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, "Delay Trigger Level can"
                        " only be set when the Trigger Type is Edge.");
    }
    
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                            0, BUFFER_SIZE, tSource));
    
    if (!strcmp (tSource, ITSCOPE_VAL_EXTERNAL))
    {
        ext = 1.0;
    }
    
    if (ext != 0.0)
    {   
        /*- Trigger source is External -*/
       _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, -8.0 * ext, 8.0 * ext));
    }
    else if (strcmp(tSource, ITSCOPE_VAL_AC_LINE) == 0)
    {   
        /*- Trigger source is AC Line -*/
        _viCheckErrElab( IVI_ERROR_INVALID_CONFIGURATION, "Cannot set trigger level when "
                                                             "trigger source is AC line.");
    } 
    else
    {   
        /*- Trigger source is input channel -*/
        ViReal64      rangeLimit, limit;
        ViString      channel;
        
       _checkErr( Ivi_CoerceChannelName (vi, tSource, &channel));
       _checkErr( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit));
        limit = rangeLimit*0.8;    
                                            
    /******************************************************************** 
     * In old firmware version, trigger level limitation of 40 exists.
     * But it is removed in new firmware version.
     * So remove the limitation of 40 from the check callback function.
    
        if(limit > 40)
        {
            limit = 40;
        }

    *********************************************************************/
    
       _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));
    }                                                    

Error:
    return error;
}

    /*- ITSCOPE_ATTR_DELAY_TRIGGER_TIME -*/
static IviRangeTableEntry attrDelayTriggerTimeRangeTableEntries[] =
{
    /*************************************
     * Minimum Runt Width in old firmware
     * version is 2.5e-9. Bus is 4.0e-9
     * in new firmware. Remove the old 
     * value from the range table.
     
    {2.5e-9,    8,  0,  "", 0}, 
     *************************************/
    
    {4.0e-9,    8,  0,  "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrDelayTriggerTimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:B:TIM",
    attrDelayTriggerTimeRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrDelayTriggerTime_CoerceCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 value,
                                                                     ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;

    /**************************************************************************
     * The resolution in old firmware version is 0.5ns, but 0.8ns in new 
     * firmware. So remove the old resolution from the driver source code.
     
       _checkErr( itscope_CoerceToResolution (vi, value, 0.5e-9, 12, &newValue));
    
    ***************************************************************************/
    
   _checkErr( itscope_CoerceToResolution (vi, value, 0.8e-9, 12, &newValue));

Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_DELAY_TRIGGER_SOURCE -*/  
static ViStatus _VI_FUNC itscopeAttrDelayTriggerSource_CheckCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViConstString value)
{
    ViStatus    error = VI_SUCCESS;
    ViString    coercedChanStr;

    error = itscope_GetStr2BufFromStr1 (triggerSourceStringTable, value, VI_NULL);
    if(error < VI_SUCCESS)
    {
       _checkErr( Ivi_CoerceChannelName (vi, value, &coercedChanStr) );
        if( (strcmp(coercedChanStr, "RF") != 0) && (strncmp( coercedChanStr, "CH",2) != 0))
        {
            error = IVI_ERROR_INVALID_VALUE;
           _viCheckErr(error);
        }
    }
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrDelayTriggerSource_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar         rdBuffer[BUFFER_SIZE];  
    size_t        bufSize = sizeof(rdBuffer); 
    ViConstString  sourceName;
    
    //_viCheckErr( viQueryf (io, ":TRIG:B:EDGE:SOU?", "%#s", &bufSize, rdBuffer) );
	_viCheckErr(_viQueryf(vi, ":TRIG:B:EDGE:SOU?", "%#s", &bufSize, rdBuffer));

    error = itscope_GetStr1PtrFromStr2 (triggerSourceStringTable, &sourceName, rdBuffer);
    if (error < VI_SUCCESS)
    {
            /* Assume the buf contains an instrument channel string. */
       _checkErr( Ivi_GetUserChannelName (vi, rdBuffer, &sourceName));
    }

   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, sourceName));    

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrDelayTriggerSource_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViConstString value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      sourceNameBuf[BUFFER_SIZE] = {0}; 
    
    error = itscope_GetStr2BufFromStr1 (triggerSourceStringTable, value, sourceNameBuf); 
    if(error <  VI_SUCCESS)
    {
        ViConstString chanStr; 
        
       _checkErr( Ivi_CoerceChannelName (vi, value, &chanStr));
        sprintf (sourceNameBuf, "%s", chanStr);
    }

    //_viCheckErr( viPrintf (io, ":TRIG:B:EDGE:SOU %s", sourceNameBuf));   
	_viCheckErr(_viPrintf(vi, ":TRIG:B:EDGE:SOU %s", sourceNameBuf));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_TRANSITION_WHEN -*/
static IviRangeTableEntry attrTransitionWhenRangeTableEntries[] =
{
    {ITSCOPE_VAL_TRANSITION_FASTER_THAN,    0,  0,  "FASTER",   0},
    {ITSCOPE_VAL_TRANSITION_SLOWER_THAN,    0,  0,  "SLOWER",   0},
    {ITSCOPE_VAL_TRANSITION_EQUAL,          0,  0,  "EQUAL",    0},
    {ITSCOPE_VAL_TRANSITION_UNEQUAL,        0,  0,  "UNEQUAL",  0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
static IviRangeTable attrTransitionWhenRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:A:PUL:TRAN:WHE",
    attrTransitionWhenRangeTableEntries,
};

    /*- ITSCOPE_ATTR_TRANSITION_POLARITY -*/
static IviRangeTableEntry attrTransitionPolarityRangeTableEntries[] =
{
    {ITSCOPE_VAL_TRANSITION_POSITIVE,   0, 0, "POSITIVE",   0},
    {ITSCOPE_VAL_TRANSITION_NEGATIVE,   0, 0, "NEGATIVE",   0},
    {ITSCOPE_VAL_TRANSITION_EITHER,     0, 0, "EITHER",     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
static IviRangeTable attrTransitionPolarityRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    ":TRIG:A:TRAN:POL",
    attrTransitionPolarityRangeTableEntries,
};
    
    /*- ITSCOPE_ATTR_TRANSITION_DELTA_TIME -*/
static IviRangeTableEntry attrTransitionDeltaTimeRangeTableEntries[] =
{
    /*************************************
     * Minimum Runt Width in old firmware
     * version is 2.5e-9. Bus is 4.0e-9
     * in new firmware. Remove the old 
     * value from the range table.
     
    {2.5e-9,    8,  0,  "", 0}, 
     *************************************/
    
    {4.0e-9,    8,  0,  "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
static IviRangeTable attrTransitionDeltaTimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:TRAN:DELT",
    attrTransitionDeltaTimeRangeTableEntries,
};
 
static ViStatus _VI_FUNC itscopeAttrTransitionDeltaTime_CoerceCallback (ViSession vi,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 value,
                                                                         ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;

    /**************************************************************************
     * The resolution in old firmware version is 0.5ns, but 0.8ns in new 
     * firmware. So remove the old resolution from the driver source code.
     
       _checkErr( itscope_CoerceToResolution (vi, value, 0.25e-9, 12, &newValue));
    
    ***************************************************************************/

   _checkErr( itscope_CoerceToResolution (vi, value, 0.8e-9, 12, &newValue));

Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_TRANSITION_HIGH_THRESHOLD -*/
static ViStatus _VI_FUNC itscopeAttrTransitionHighThreshold_ReadCallback (ViSession vi,
                                                                           ViSession io,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 *value)
{
    ViStatus  error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
    
    if( !((strcmp(channel, "CH1") == 0)
            || (strcmp(channel, "CH2") == 0)
                || (strcmp(channel, "CH3") == 0)
                    || (strcmp(channel, "CH4") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH,
                                            0, value ) );   

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTransitionHighThreshold_WriteCallback (ViSession vi,
                                                                            ViSession io,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
    
    if( !((strcmp(channel, "CH1") == 0)
            || (strcmp(channel, "CH2") == 0)
                || (strcmp(channel, "CH3") == 0)
                    || (strcmp(channel, "CH4") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
  
   _checkErr ( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH,
                                            0, value ) );
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTransitionHighThreshold_CheckCallback (ViSession vi,
                                                                            ViConstString channelName,
                                                                            ViAttr attributeId,
                                                                            ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    rangeLimit, limit;
    ViInt32     triggerType;
    ViChar      triggerSource[BUFFER_SIZE]; 
    ViString    channel;        
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, triggerSource));
   _checkErr( Ivi_CoerceChannelName (vi, triggerSource, &channel) );
                                        
    if( !((strcmp (channel, "CH1") == 0)
            || (strcmp (channel, "CH2") == 0)
                || (strcmp (channel, "CH3") == 0)
                    || (strcmp (channel, "CH4") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    } 
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));
    if (!(triggerType == ITSCOPE_VAL_TRANSITION_TRIGGER))
    {               
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }   
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit) );
    limit = rangeLimit*0.8;                                     
    
    /******************************************************************** 
     * In old firmware version, trigger level limitation of 40 exists.
     * But it is removed in new firmware version.
     * So remove the limitation of 40 from the check callback function.
    
        if(limit > 40)
        {
            limit = 40;
        }

    *********************************************************************/
    
   _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));

Error:
    return error;                                 
}
                                    

    /*- ITSCOPE_ATTR_TRANSITION_LOW_THRESHOLD -*/
static ViStatus _VI_FUNC itscopeAttrTransitionLowThreshold_ReadCallback (ViSession vi,
                                                                          ViSession io,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          ViReal64 *value)
{
    ViStatus  error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
    
    if( !((strcmp(channel, "CH1") == 0)
            || (strcmp(channel, "CH2") == 0)
                || (strcmp(channel, "CH3") == 0)
                    || (strcmp(channel, "CH4") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH,
                                            0, value ) );   

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTransitionLowThreshold_WriteCallback (ViSession vi,
                                                                           ViSession io,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE];
    ViString    channel;
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL,  ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, buf));
   _checkErr( Ivi_CoerceChannelName (vi, buf, &channel));
    
    if( !((strcmp(channel, "CH1") == 0)
            || (strcmp(channel, "CH2") == 0)
                || (strcmp(channel, "CH3") == 0)
                    || (strcmp(channel, "CH4") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
  
   _checkErr ( Ivi_SetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH,
                                            0, value ) );
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrTransitionLowThreshold_CheckCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    rangeLimit, limit;
    ViInt32     triggerType;
    ViChar      triggerSource[BUFFER_SIZE]; 
    ViString    channel;    
        
   _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                        0, BUFFER_SIZE, triggerSource));
   _checkErr( Ivi_CoerceChannelName (vi, triggerSource, &channel));  
    
    if( !((strcmp (channel, "CH1") == 0)
            || (strcmp (channel, "CH2") == 0)
                || (strcmp (channel, "CH3") == 0)
                    || (strcmp (channel, "CH4") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    } 
    
   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));
    if (!(triggerType == ITSCOPE_VAL_TRANSITION_TRIGGER))
    {               
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }   
    
   _checkErr ( Ivi_GetAttributeViReal64 (vi, channel, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit) );
                                            
    limit = rangeLimit*0.8; 
    /******************************************************************** 
     * In old firmware version, trigger level limitation of 40 exists.
     * But it is removed in new firmware version.
     * So remove the limitation of 40 from the check callback function.
    
        if(limit > 40)
        {
            limit = 40;
        }

    *********************************************************************/
    
   _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));

Error:
    return error;
}
                                    

    /*- Checks validity of logical inputs -*/
    
    /*- ITSCOPE_ATTR_LOGIC_FUNCTION -*/
static IviRangeTableEntry attrLogicFunctionRangeTableEntries[] =
{
    {ITSCOPE_VAL_LOGIC_AND,     0, 0, "AND",    0},
    {ITSCOPE_VAL_LOGIC_NAND,    0, 0, "NAND",   0},
    {ITSCOPE_VAL_LOGIC_NOR,     0, 0, "NOR",    0},
    {ITSCOPE_VAL_LOGIC_OR,      0, 0, "OR",     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrLogicFunctionRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:LOGI:FUNC",
    attrLogicFunctionRangeTableEntries,
};

    /*- ITSCOPE_ATTR_LOGIC_WHEN -*/
static IviRangeTableEntry attrLogicWhenRangeTableEntries[] =
{
    {ITSCOPE_VAL_LOGIC_TRUE,        0, 0, "TRUE",       0},
    {ITSCOPE_VAL_LOGIC_FALSE,       0, 0, "FALSE",      0},
    {ITSCOPE_VAL_LOGIC_LESSTHAN,    0, 0, "LESSTHAN",   0},
    {ITSCOPE_VAL_LOGIC_MORETHAN,    0, 0, "MORETHAN",   0},
    {ITSCOPE_VAL_LOGIC_EQUAL,       0, 0, "EQUAL",      0},
    {ITSCOPE_VAL_LOGIC_UNEQUAL,     0, 0, "UNEQUAL",    0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrLogicWhenRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:LOGI:PAT:WHE",
    attrLogicWhenRangeTableEntries,
};

    /*- ITSCOPE_ATTR_LOGIC_INPUT_BY_CH -*/
static IviRangeTableEntry attrLogicInputByChanRangeTableEntries[] =
{
    {ITSCOPE_VAL_LOGIC_HIGH,    0, 0, "HIGH",   0},
    {ITSCOPE_VAL_LOGIC_LOW,     0, 0, "LOW",    0},
    {ITSCOPE_VAL_LOGIC_X,       0, 0, "X",      0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrLogicInputByChanRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrLogicInputByChanRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrLogicInputByCh_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      input[BUFFER_SIZE];
    
    memset(input, 0, sizeof(ViChar)*BUFFER_SIZE);
    
    //_viCheckErr( viQueryf(io, ":TRIG:A:LOGI:INPut:%s?", "%s", channelName, input) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:LOGI:INPut:%s?", "%s", channelName, input));

   _viCheckErr( Ivi_GetViInt32EntryFromString( input, &attrLogicInputByChanRangeTable,
                                                value, VI_NULL, VI_NULL, VI_NULL, VI_NULL) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrLogicInputByCh_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViString    input;

   _viCheckErr( Ivi_GetViInt32EntryFromValue( value, &attrLogicInputByChanRangeTable,
                                            VI_NULL, VI_NULL, VI_NULL,
                                            VI_NULL, &input, VI_NULL    ) );

    //_viCheckErr( viPrintf(io, ":TRIG:A:LOGI:INPut:%s %s;", channelName, input) );
	_viCheckErr(_viPrintf(vi, ":TRIG:A:LOGI:INPut:%s %s;", channelName, input));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_LOGIC_THRES_BY_CH -*/
static ViStatus _VI_FUNC itscopeAttrLogicThresByCh_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf (io, ":TRIG:A:LOGI:THR:%s?", "%le", channelName, value) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:LOGI:THR:%s?", "%le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrLogicThresByCh_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viPrintf (io, ":TRIG:A:LOGI:THR:%s %le", channelName, value) );
	_viCheckErr(_viPrintf(vi, ":TRIG:A:LOGI:THR:%s %le", channelName, value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrLogicThresByCh_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    rangeLimit, limit;
    ViInt32     triggerType;
    
    if( !((strcmp (channelName, "CH1") == 0)
            || (strcmp (channelName, "CH2") == 0)
                || (strcmp (channelName, "CH3") == 0)
                    || (strcmp (channelName, "CH4") == 0)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }

   _checkErr(Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_TRIGGER_TYPE, 0,
                                      &triggerType));

    if(!((triggerType == ITSCOPE_VAL_PATTERN_TRIGGER)
            || (triggerType == ITSCOPE_VAL_STATE_TRIGGER)
                || (triggerType == ITSCOPE_VAL_SETHOLD_TRIGGER)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);         
    }

   _checkErr ( Ivi_GetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &rangeLimit) );
    limit = rangeLimit*0.8;
    
    /******************************************************************** 
     * In old firmware version, trigger level limitation of 40 exists.
     * But it is removed in new firmware version.
     * So remove the limitation of 40 from the check callback function.
    
        if(limit > 40)
        {
            limit = 40;
        }

    *********************************************************************/
    
   _checkErr( itscope_CheckRangeWithPrecision (vi, attributeId, value, 
                                                    -limit, limit));    
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_PATTERN_FUNCTION -*/
static ViStatus _VI_FUNC itscopeAttrPatternFunction_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_FUNCTION,
                                            0, value) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrPatternFunction_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr ( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_FUNCTION,
                                            0, value) );

Error:
    return error;
}    

    /*- ITSCOPE_ATTR_PATTERN_WHEN -*/
static ViStatus _VI_FUNC itscopeAttrPatternWhen_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_WHEN,
                                        0, value) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrPatternWhen_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

	_checkErr ( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_WHEN,
                                            0, value) );

Error:
    return error;
}    

    /*- ITSCOPE_ATTR_PATTERN_INPUT -*/
static ViStatus _VI_FUNC itscopeAttrPatternInput_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr ( Ivi_GetAttributeViInt32 (vi, channelName, ITSCOPE_ATTR_LOGIC_INPUT_BY_CH,
                                            0, value) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrPatternInput_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr ( Ivi_SetAttributeViInt32 (vi, channelName, ITSCOPE_ATTR_LOGIC_INPUT_BY_CH,
                                            0, value) );

Error:
    return error;
}

    /*- ITSCOPE_ATTR_PATTERN_DELTATIME -*/
static IviRangeTableEntry attrPatternDeltatimeRangeTableEntries[] =
{
    /*************************************
     * Minimum Runt Width in old firmware
     * version is 2.5e-9. Bus is 4.0e-9
     * in new firmware. Remove the old 
     * value from the range table.
     
    {2.5e-9,    8,  0,  "", 0}, 
     *************************************/
    
    {4.0e-9,    8.0,    0, "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPatternDeltatimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:LOGI:PAT:WHE:LESSL",
    attrPatternDeltatimeRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrPatternDeltatime_CoerceCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value,
                                                                      ViReal64 *coercedValue)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 newValue = value;

    /**************************************************************************
     * The resolution in old firmware version is 0.5ns, but 0.8ns in new 
     * firmware. So remove the old resolution from the driver source code.
     
       _checkErr( itscope_CoerceToResolution (vi, value, 0.5e-9, 12, &newValue));
    
    ***************************************************************************/

   _checkErr( itscope_CoerceToResolution (vi, value, 0.8e-9, 12, &newValue));

Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_STATE_FUNCTION -*/
static ViStatus _VI_FUNC itscopeAttrStateFunction_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_FUNCTION,
                                        0, value) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrStateFunction_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_FUNCTION,
                                        0, value) );

Error:
    return error;
}    

    /*- ITSCOPE_ATTR_STATE_WHEN -*/
static ViStatus _VI_FUNC itscopeAttrStateWhen_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    
   _checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_WHEN,
                                        0, value) );
                                        
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrStateWhen_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_LOGIC_WHEN, 
                                        0, value) );
                                        
Error:
    return error;
}

    /*- ITSCOPE_ATTR_STATE_INPUT -*/
static ViStatus _VI_FUNC itscopeAttrStateInput_ReadCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_GetAttributeViInt32 (vi, channelName, ITSCOPE_ATTR_LOGIC_INPUT_BY_CH,
                                        0, value) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrStateInput_WriteCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

   _checkErr( Ivi_SetAttributeViInt32 (vi, channelName, ITSCOPE_ATTR_LOGIC_INPUT_BY_CH,
                                        0, value) );

Error:
    return error;
}

    /*- ITSCOPE_ATTR_STATE_CLOCK_EDGE -*/
static IviRangeTableEntry attrStateClockEdgeRangeTableEntries[] =
{
    {ITSCOPE_VAL_CLOCK_RISE, 0, 0, "RISE", 0},
    {ITSCOPE_VAL_CLOCK_FALL, 0, 0, "FALL", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrStateClockEdgeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:LOGI:INP:CLOC:EDGE",
    attrStateClockEdgeRangeTableEntries,
};

    /*- ITSCOPE_ATTR_STATE_CLOCK_SOURCE -*/
static IviRangeTableEntry attrPEK1StateClockSourceRangeTableEntries[] =
{
    {ITSCOPE_VAL_SOUR_CH1,  0, 0, "CH1",    0},
    {ITSCOPE_VAL_SOUR_CH2,  0, 0, "CH2",    0},
    {ITSCOPE_VAL_SOUR_CH3,  0, 0, "CH3",    0},
    {ITSCOPE_VAL_SOUR_CH4,  0, 0, "CH4",    0},
    {ITSCOPE_VAL_SOUR_NONE, 0, 0, "NONE",   0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPEK1StateClockSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrPEK1StateClockSourceRangeTableEntries,
};
    

static ViStatus _VI_FUNC itscopeAttrStateClockSource_RangeTableCallback (ViSession vi,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32     model;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
    if(IS_PEK1(model))
        tblPtr = &attrPEK1StateClockSourceRangeTable;

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

static ViStatus _VI_FUNC itscopeAttrStateClockSource_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      source[BUFFER_SIZE];
    IviRangeTablePtr    tblPtr = VI_NULL;  
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:LOGI:INP:CLOC:SOU?", "%s", source) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:LOGI:INP:CLOC:SOU?", "%s", source));

   _checkErr ( Ivi_GetAttrRangeTable(vi, channelName, attributeId, &tblPtr) );
   _viCheckErr( Ivi_GetViInt32EntryFromString(source, tblPtr,  
                                                value, VI_NULL, VI_NULL, VI_NULL, VI_NULL) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrStateClockSource_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViString    source;     
    IviRangeTablePtr    tblPtr = VI_NULL; 
    
   _checkErr ( Ivi_GetAttrRangeTable(vi, channelName, attributeId, &tblPtr) );  
   _viCheckErr( Ivi_GetViInt32EntryFromValue(value, tblPtr,
                                            VI_NULL, VI_NULL, VI_NULL, VI_NULL, &source, VI_NULL) );
    
    //_viCheckErr( viPrintf (io, ":TRIG:A:LOGI:INP:CLOC:SOU %s", source) );
	_viCheckErr(_viPrintf(vi, ":TRIG:A:LOGI:INP:CLOC:SOU %s", source));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_SETHOLD_CLOCK_EDGE -*/
static IviRangeTableEntry attrSetholdClockEdgeRangeTableEntries[] =
{
    {ITSCOPE_VAL_CLOCK_RISE, 0, 0, "RISE", 0},
    {ITSCOPE_VAL_CLOCK_FALL, 0, 0, "FALL", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrSetholdClockEdgeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:LOGI:SETH:CLOC:EDGE",
    attrSetholdClockEdgeRangeTableEntries,
};

    /*- ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE -*/
static IviRangeTableEntry attrPEK1SetholdClockSourceRangeTableEntries[] =
{
    {ITSCOPE_VAL_SOUR_CH1, 0, 0, "CH1", 0},
    {ITSCOPE_VAL_SOUR_CH2, 0, 0, "CH2", 0},
    {ITSCOPE_VAL_SOUR_CH3, 0, 0, "CH3", 0},
    {ITSCOPE_VAL_SOUR_CH4, 0, 0, "CH4", 0},
	{ITSCOPE_VAL_SOUR_CH5, 0, 0, "CH5", 0},
	{ITSCOPE_VAL_SOUR_CH6, 0, 0, "CH6", 0},
	{ITSCOPE_VAL_SOUR_CH7, 0, 0, "CH7", 0},
	{ITSCOPE_VAL_SOUR_CH8, 0, 0, "CH8", 0},
	{ITSCOPE_VAL_SOUR_EXT, 0, 0, "AUX", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPEK1SetholdClockSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:SETH:CLOC:SOU",
    attrPEK1SetholdClockSourceRangeTableEntries,
};


static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_RangeTableCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32     model;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
    if(IS_PEK1(model))
        tblPtr = &attrPEK1SetholdClockSourceRangeTable;

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_CheckCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     dataSource;
    
   _checkErr (Ivi_DefaultCheckCallbackViInt32 (vi, channelName, attributeId, value));
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_DATA_SOURCE,
                                        0, &dataSource));
    if (dataSource == value)
    {
        _viCheckErrElab (IVI_ERROR_INVALID_CONFIGURATION,
                        "Invalid Channel Name."
                        "Setup and Hold Trigger.");
    }

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      source[BUFFER_SIZE]; 
    IviRangeTablePtr    tblPtr = VI_NULL; 
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:SETH:CLOC:SOU?", "%s", source) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:SETH:CLOC:SOU?", "%s", source));

   _checkErr ( Ivi_GetAttrRangeTable(vi, channelName, attributeId, &tblPtr) );
   _viCheckErr( Ivi_GetViInt32EntryFromString(source, tblPtr,  
                                                value, VI_NULL, VI_NULL, VI_NULL, VI_NULL) );

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrSetholdClockSource_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViString    source;
    IviRangeTablePtr    tblPtr = VI_NULL;  
    
   _checkErr ( Ivi_GetAttrRangeTable(vi, channelName, attributeId, &tblPtr) );  
   _viCheckErr( Ivi_GetViInt32EntryFromValue(value, tblPtr,
                                            VI_NULL, VI_NULL, VI_NULL, VI_NULL, &source, VI_NULL) );
    
    //_viCheckErr( viPrintf (io, ":TRIG:A:SETH:CLOC:SOU %s", source) );                                               
	_viCheckErr(_viPrintf(vi, ":TRIG:A:SETH:CLOC:SOU %s", source));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_SETHOLD_DATA_SOURCE -*/
static IviRangeTableEntry attrDPO4KSetholdDataSourceRangeTableEntries[] =
{
    {ITSCOPE_VAL_SOUR_CH1, 0, 0, "CH1", 0},
    {ITSCOPE_VAL_SOUR_CH2, 0, 0, "CH2", 0},
    {ITSCOPE_VAL_SOUR_CH3, 0, 0, "CH3", 0},
    {ITSCOPE_VAL_SOUR_CH4, 0, 0, "CH4", 0},
    {ITSCOPE_VAL_SOUR_EXT, 0, 0, "AUX", 0},    
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrDPO4KSetholdDataSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:SETH:DAT:SOU",
    attrDPO4KSetholdDataSourceRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_RangeTableCallback (ViSession vi,
                                                                          ViConstString channelName,
                                                                          ViAttr attributeId,
                                                                          IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32     model;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
    if(IS_PEK1(model))
        tblPtr = &attrDPO4KSetholdDataSourceRangeTable;

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     clockSource;
    
   _checkErr (Ivi_DefaultCheckCallbackViInt32 (vi, channelName, attributeId, value));
   _checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE,
                                        0, &clockSource));
    if (clockSource == value)
    {
        _viCheckErrElab (IVI_ERROR_INVALID_CONFIGURATION,
                        "Invalid Channel Name."
                        "Setup and Hold Trigger.");
    }

Error:
    return error;
}
    
static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[BUFFER_SIZE]; 
    IviRangeTablePtr    tblPtr = VI_NULL;    
    
    //_viCheckErr( viQueryf (io, ":TRIG:A:SETH:DAT:SOU?", "%s", buf) );
	_viCheckErr(_viQueryf(vi, ":TRIG:A:SETH:DAT:SOU?", "%s", buf));

   _checkErr ( Ivi_GetAttrRangeTable(vi, channelName, attributeId, &tblPtr) );  
   _viCheckErr( Ivi_GetViInt32EntryFromString ( buf, tblPtr, value,
                                                VI_NULL, VI_NULL, VI_NULL, VI_NULL ) ); 
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrSetholdDataSource_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViString    strClockSource;  
    IviRangeTablePtr    tblPtr = VI_NULL;
    
   _checkErr ( Ivi_GetAttrRangeTable(vi, channelName, attributeId, &tblPtr) );  
   _viCheckErr( Ivi_GetViInt32EntryFromValue (value, tblPtr, VI_NULL,
                                            VI_NULL, VI_NULL, VI_NULL, &strClockSource, VI_NULL ) );    

    //_viCheckErr( viPrintf (io, ":TRIG:A:SETH:DAT:SOU %s", strClockSource) );
	_viCheckErr(_viPrintf(vi, ":TRIG:A:SETH:DAT:SOU %s", strClockSource));

Error:
    return error;
}    

    /*- ITSCOPE_ATTR_SETHOLD_HOLDTIME -*/
static IviRangeTableEntry attrPEK1SetholdHoldtimeRangeTableEntries[] =
{
    /*************************************
     * Minimum Runt Width in old firmware
     * version is 2.5e-9. Bus is 4.0e-9
     * in new firmware. Remove the old 
     * value from the range table.
     
    {2.5e-9,    8,  0,  "", 0}, 
     *************************************/
    
    {4.0e-9,    8,  0,  "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPEK1SetholdHoldtimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:LOGI:SETH:HOLDT",
    attrPEK1SetholdHoldtimeRangeTableEntries,
};


static ViStatus _VI_FUNC itscopeAttrSetholdHoldtime_RangeTableCallback (ViSession vi,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32     model;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
    if(IS_PEK1(model))
        tblPtr = &attrPEK1SetholdHoldtimeRangeTable;

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

static ViStatus _VI_FUNC itscopeAttrSetholdHoldtime_CoerceCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value,
                                                                    ViReal64 *coercedValue)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    newValue = value;
    ViInt32     model;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
    
    {
       _checkErr( itscope_CoerceToResolution (vi, value, 0.8e-9, 12, &newValue));  
    }
    
Error:
    *coercedValue = newValue;
    return error;
}

    /*- ITSCOPE_ATTR_SETHOLD_SETTIME -*/
static IviRangeTableEntry attrPEK1SetholdSettimeRangeTableEntries[] =
{
    /*************************************
     * Minimum Runt Width in old firmware
     * version is 2.5e-9. Bus is 4.0e-9
     * in new firmware. Remove the old 
     * value from the range table.
     
    {2.5e-9,    8,  0,  "", 0}, 
     *************************************/
     
    {0.0e-9,    8,  0,  "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrPEK1SetholdSettimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    ":TRIG:A:LOGI:SETH:SETT",
    attrPEK1SetholdSettimeRangeTableEntries,
};

static ViStatus _VI_FUNC itscopeAttrSetholdSettime_RangeTableCallback (ViSession vi,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       IviRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    IviRangeTablePtr    tblPtr = VI_NULL;
    ViInt32     model;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
    if(IS_PEK1(model))
        tblPtr = &attrPEK1SetholdSettimeRangeTable;

Error:
    *rangeTablePtr = tblPtr;
    return error;
}
    
static ViStatus _VI_FUNC itscopeAttrSetholdSettime_CoerceCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value,
                                                                   ViReal64 *coercedValue)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    newValue = value;
    ViInt32     model;

   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );
    
    {
       _checkErr( itscope_CoerceToResolution (vi, value, 0.8e-9, 12, &newValue));  
    }

Error:
    *coercedValue = newValue;
    return error;
}






   
    /*- ITSCOPE_ATTR_INPUT_DATA_SOURCE -*/
static IviRangeTableEntry attrInputDataSourceRangeTableEntries[] =
{
    {0,   0,      0,        ITSCOPE_VAL_CH_1,     0}, 
    {0,   0,      0,        ITSCOPE_VAL_CH_2,     0}, 
    {0,   0,      0,        ITSCOPE_VAL_CH_3,     0}, 
    {0,   0,      0,        ITSCOPE_VAL_CH_4,     0}, 
	{0,   0,      0,        ITSCOPE_VAL_CH_5,     0},
	{0,   0,      0,        ITSCOPE_VAL_CH_6,     0},
	{0,   0,      0,        ITSCOPE_VAL_CH_7,     0},
	{0,   0,      0,        ITSCOPE_VAL_CH_8,     0},
	{0,   0,      0,        ITSCOPE_VAL_MATH,     0},
    {0,   0,      0,        ITSCOPE_VAL_REF_1,    0}, 
    {0,   0,      0,        ITSCOPE_VAL_REF_2,    0}, 
    {0,   0,      0,        ITSCOPE_VAL_REF_3,    0}, 
    {0,   0,      0,        ITSCOPE_VAL_REF_4,    0},
	{0,   0,      0,        ITSCOPE_VAL_REF_5,    0 },
	{0,   0,      0,        ITSCOPE_VAL_REF_6,    0 },
	{0,   0,      0,        ITSCOPE_VAL_REF_7,    0 },
	{0,   0,      0,        ITSCOPE_VAL_REF_8,    0 },
	{IVI_RANGE_TABLE_LAST_ENTRY}
};   

static IviRangeTable  attrInputDataSourceRangeTable =
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    ":DAT:SOU", 
    attrInputDataSourceRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_MEAS_REF_CALC_METH -*/
static IviRangeTableEntry attrMeasRefCalcMethRangeTableEntries[] =
{
    {ITSCOPE_VAL_MEAS_HISTOGRAM,    0, 0, "HISTOGRAM",  0},
    {ITSCOPE_VAL_MEAS_MIN_MAX,      0, 0, "MINMAX",     0},
    {ITSCOPE_VAL_MEAS_AUTO,         0, 0, "AUTO",       0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMeasRefCalcMethRangeTable =
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    ":MEASU:METH", 
    attrMeasRefCalcMethRangeTableEntries, 
};
    
    /*- ITSCOPE_ATTR_MEAS_SOURCE -*/  
static IviRangeTableEntry attrMeasSourceRangeTableEntries[] =
{
    {0,   0,      0,   ITSCOPE_VAL_CH_1,      0}, 
    {0,   0,      0,   ITSCOPE_VAL_CH_2,      0}, 
    {0,   0,      0,   ITSCOPE_VAL_CH_3,      0}, 
    {0,   0,      0,   ITSCOPE_VAL_CH_4,      0}, 
	{0,   0,      0,   ITSCOPE_VAL_CH_5,      0},
	{0,   0,      0,   ITSCOPE_VAL_CH_6,      0},
	{0,   0,      0,   ITSCOPE_VAL_CH_7,      0},
	{0,   0,      0,   ITSCOPE_VAL_CH_8,      0},
	{0,   0,      0,   ITSCOPE_VAL_REF_1,     0},
    {0,   0,      0,   ITSCOPE_VAL_REF_2,     0}, 
    {0,   0,      0,   ITSCOPE_VAL_REF_3,     0}, 
    {0,   0,      0,   ITSCOPE_VAL_REF_4,     0}, 
	{0,   0,      0,   ITSCOPE_VAL_REF_5,     0 },
	{0,   0,      0,   ITSCOPE_VAL_REF_6,     0 },
	{0,   0,      0,   ITSCOPE_VAL_REF_7,     0 },
	{0,   0,      0,   ITSCOPE_VAL_REF_8,     0 },
	{0,   0,      0,   ITSCOPE_VAL_MATH,      0},
    {0,   0,      0,   ITSCOPE_VAL_HISTOGRAM,     0},
    {IVI_RANGE_TABLE_LAST_ENTRY}  
};   

static IviRangeTable  attrMeasSourceRangeTable =       
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    "MEASU:IMM:SOURCE1", 
    attrMeasSourceRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_MEAS_SOURCE2 -*/  
static IviRangeTableEntry attrMeasSource2RangeTableEntries[] =
{
    {0,   0,      0,   ITSCOPE_VAL_CH_1,      0}, 
    {0,   0,      0,   ITSCOPE_VAL_CH_2,      0}, 
    {0,   0,      0,   ITSCOPE_VAL_CH_3,      0}, 
    {0,   0,      0,   ITSCOPE_VAL_CH_4,      0}, 
	{0,   0,      0,   ITSCOPE_VAL_CH_5,      0},
	{0,   0,      0,   ITSCOPE_VAL_CH_6,      0},
	{0,   0,      0,   ITSCOPE_VAL_CH_7,      0},
	{0,   0,      0,   ITSCOPE_VAL_CH_8,      0},
	{0,   0,      0,   ITSCOPE_VAL_REF_1,     0},
    {0,   0,      0,   ITSCOPE_VAL_REF_2,     0}, 
    {0,   0,      0,   ITSCOPE_VAL_REF_3,     0}, 
    {0,   0,      0,   ITSCOPE_VAL_REF_4,     0}, 
	{0,   0,      0,   ITSCOPE_VAL_REF_5,     0 },
	{0,   0,      0,   ITSCOPE_VAL_REF_6,     0 },
	{0,   0,      0,   ITSCOPE_VAL_REF_7,     0 },
	{0,   0,      0,   ITSCOPE_VAL_REF_8,     0 },
	{0,   0,      0,   ITSCOPE_VAL_MATH,      0},
    {IVI_RANGE_TABLE_LAST_ENTRY}  
};   

static IviRangeTable  attrMeasSource2RangeTable =       
{
    IVI_VAL_DISCRETE, 
    VI_FALSE, 
    VI_FALSE, 
    "MEASU:IMM:SOURCE2", 
    attrMeasSource2RangeTableEntries, 
};

    /*- ITSCOPE_ATTR_MEAS_DEL_DIRECTION -*/
static IviRangeTableEntry attrMeasDelDirectionRangeTableEntries[] =
{
    {ITSCOPE_VAL_DEL_DIR_BACKWARD, 0, 0, "BACKWARDS", 0},
    {ITSCOPE_VAL_DEL_DIR_FORWARD,  0, 0, "FORWARDS",  0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMeasDelDirectionRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    "MEASU:IMM:DEL:DIR",
    attrMeasDelDirectionRangeTableEntries,
};

    /*- ITSCOPE_ATTR_MEAS_DEL_EDGE -*/
static IviRangeTableEntry attrMeasDelEdgeRangeTableEntries[] =
{
    {ITSCOPE_VAL_DEL_EDGE_RISE, 0, 0, "RISE", 0},
    {ITSCOPE_VAL_DEL_EDGE_FALL, 0, 0, "FALL", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMeasDelEdgeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    "MEASU:IMM:DEL:EDGE1",
    attrMeasDelEdgeRangeTableEntries,
};

    /*- ITSCOPE_ATTR_MEAS_DEL_EDGE2 -*/
static IviRangeTableEntry attrMeasDelEdge2RangeTableEntries[] =
    {
        {ITSCOPE_VAL_DEL_EDGE_RISE, 0, 0, "RISE", 0},
        {ITSCOPE_VAL_DEL_EDGE_FALL, 0, 0, "FALL", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrMeasDelEdge2RangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        "MEASU:IMM:DEL:EDGE2",
        attrMeasDelEdge2RangeTableEntries,
    };

    /*- ITSCOPE_ATTR_MEAS_GATING -*/
static IviRangeTableEntry attrMeasGatingRangeTableEntries[] =
{
    {ITSCOPE_VAL_MEAS_GATING_OFF,       0, 0, "OFF",    0},
    {ITSCOPE_VAL_MEAS_GATING_SCREEN,    0, 0, "SCREEN", 0},
    {ITSCOPE_VAL_MEAS_GATING_CURSOR,    0, 0, "CURSOR", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMeasGatingRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":MEASU:GAT",
    attrMeasGatingRangeTableEntries,
};

    /*- ITSCOPE_ATTR_MEAS_HIGH_REF -*/
static IviRangeTableEntry attrMeasHighRefRangeTableEntries[] =
{
    {0.0,   100.0,  0,  "",     0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   
static IviRangeTable  attrMeasHighRefRangeTable =       
{
    IVI_VAL_RANGED, 
    VI_TRUE, 
    VI_TRUE, 
    ":MEASU:REFL:PERC:HIGH", 
    attrMeasHighRefRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_MEAS_LOW_REF -*/
static IviRangeTableEntry attrMeasLowRefRangeTableEntries[] =
{ 
    {0.0,   100.0,  0,  "",     0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   
static IviRangeTable  attrMeasLowRefRangeTable =       
{
    IVI_VAL_RANGED, 
    VI_TRUE, 
    VI_TRUE, 
    ":MEASU:REFL:PERC:LOW", 
    attrMeasLowRefRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_MEAS_MID_REF -*/
static IviRangeTableEntry attrMeasMidRefRangeTableEntries[] =
{ 
    {0.0,   100.0,  0,  "",     0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   
static IviRangeTable  attrMeasMidRefRangeTable =       
{
    IVI_VAL_RANGED, 
    VI_TRUE, 
    VI_TRUE, 
    ":MEASU:REFL:PERC:MID", 
    attrMeasMidRefRangeTableEntries, 
};
    
    /*- ITSCOPE_ATTR_MEAS_MID2_REF -*/
static IviRangeTableEntry attrMeasMid2RefRangeTableEntries[] =
{ 
    {0.0,   100.0,  0,  "",     0}, 
    {IVI_RANGE_TABLE_LAST_ENTRY}
};   
static IviRangeTable  attrMeasMid2RefRangeTable =       
{
    IVI_VAL_RANGED, 
    VI_TRUE, 
    VI_TRUE, 
    ":MEASU:REFL:PERC:MID2", 
    attrMeasMid2RefRangeTableEntries, 
};

    /*- ITSCOPE_ATTR_MEAS_VALUE -*/
static ViStatus _VI_FUNC itscopeAttrMeasureValue_ReadCallback (ViSession vi, 
                                                                ViSession io, 
                                                                ViConstString channelName, 
                                                                ViAttr attributeId, 
                                                                ViReal64 *value)
{
    ViStatus  error = VI_SUCCESS;
    ViInt32   timeout;
    if (!Ivi_Simulating(vi))
    {
       _viCheckErr(viGetAttribute(io,VI_ATTR_TMO_VALUE,&timeout));
		_viCheckErr(_viGetAttribute(vi, VI_ATTR_TMO_VALUE, &timeout));

		//_viCheckErr(viSetAttribute(io,VI_ATTR_TMO_VALUE,50000));
		_viCheckErr(_viSetAttribute(vi, VI_ATTR_TMO_VALUE, 50000));

		//_viCheckErr( viQueryf(io, ":MEASU:IMM:VAL?", "%le", value));
		_viCheckErr(_viQueryf(vi, ":MEASU:IMM:VAL?", "%le", value));

        //_viCheckErr(viSetAttribute(io,VI_ATTR_TMO_VALUE,timeout));
		_viCheckErr(_viSetAttribute(vi, VI_ATTR_TMO_VALUE, timeout));
    }
    else
    {
        ViInt32 function;
        ViReal64 range;
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MEAS_FUNCTION, 
                                           0, &function));
       _checkErr( Ivi_GetAttributeViReal64 (vi, channelName, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            0, &range));
        switch (function) 
        {
            case ITSCOPE_VAL_RISE_TIME:
            case ITSCOPE_VAL_FALL_TIME:
            {
                *value = 2e-6;
                break;
            }
            case ITSCOPE_VAL_FREQUENCY:
            {
                *value = rand ()/10.0 + 10000.0;
                break;
            }                
            case ITSCOPE_VAL_PERIOD:
            {
                *value = 1.0/(rand ()/10.0 + 10000.0);
                break;
            }                
            case ITSCOPE_VAL_VOLTAGE_RMS:
            {
                *value = range * 0.3535;
                break;
            }                
            case ITSCOPE_VAL_VOLTAGE_PEAK_TO_PEAK:
            {
                *value = range;
                break;
            }                
            case ITSCOPE_VAL_VOLTAGE_MAX:
            case ITSCOPE_VAL_VOLTAGE_HIGH:
            {
                *value = (1 - rand ()/120000.0) * range/2;
                break;
            }                
            case ITSCOPE_VAL_VOLTAGE_LOW:
            case ITSCOPE_VAL_VOLTAGE_MIN:
            {
                *value = (rand ()/120000.0 - 1) * range/2;
                break;
            }                
            case ITSCOPE_VAL_VOLTAGE_AVERAGE:
            {
                *value = 0;
                break;
            }                
            case ITSCOPE_VAL_WIDTH_NEG:
            case ITSCOPE_VAL_WIDTH_POS:
            {
                *value = 1e-4 * rand () / 30000.0;
                break;
            }                
            case ITSCOPE_VAL_DUTY_CYCLE_NEG:
            {
                *value = 30.0;
                break;
            }                
            case ITSCOPE_VAL_DUTY_CYCLE_POS:
            {
                *value = 70.0;
                break;
            }                
            case ITSCOPE_VAL_VOLTAGE_CYCLE_RMS:
            case ITSCOPE_VAL_VOLTAGE_CYCLE_AVERAGE:
            {
                *value = range * 0.353;
                break;
            }                
            case ITSCOPE_VAL_AMPLITUDE:
            {
                *value = range * 0.49;
                break;
            }                
            case ITSCOPE_VAL_POSITIVE_OVERSHOOT:
            case ITSCOPE_VAL_NEGATIVE_OVERSHOOT:
            {
                *value = 5.0;
                break;
            }
            case ITSCOPE_VAL_PHASE:
            {
                *value = rand() * 180;
                break;
            }
            default:
            {
                *value = rand() * 100;
                break;
            }
        }
    }
        
Error:
    return error;
}

    /*- ITSCOPE_ATTR_MATH_TYPE -*/
static IviRangeTableEntry attrMathTypeRangeTableEntries[] =
    {
        {ITSCOPE_VAL_MATH_TYPE_DUAL, 0, 0, "DUAL", 0},
        {ITSCOPE_VAL_MATH_TYPE_FFT, 0, 0, "FFT", 0},
        {ITSCOPE_VAL_MATH_TYPE_ADVANCED, 0, 0, "ADVANCED", 0},
        {ITSCOPE_VAL_MATH_TYPE_SPECTRUM, 0, 0, "SPECTRUM", 0},
        {IVI_RANGE_TABLE_LAST_ENTRY}
    };

static IviRangeTable attrMathTypeRangeTable =
    {
        IVI_VAL_DISCRETE,
        VI_TRUE,
        VI_TRUE,
        ":MATH:TYP",
        attrMathTypeRangeTableEntries,
    };

    /*- ITSCOPE_ATTR_MATH_SOURCE_1ST -*/
    /*- ITSCOPE_ATTR_MATH_SOURCE_2ND -*/
static IviRangeTableEntry attrMathSourceRangeTableEntries[] =
{
    {ITSCOPE_VAL_SOUR_CH1,      0, 0, "CH1",    0},
    {ITSCOPE_VAL_SOUR_CH2,      0, 0, "CH2",    0},
    {ITSCOPE_VAL_SOUR_CH3,      0, 0, "CH3",    0},
    {ITSCOPE_VAL_SOUR_CH4,      0, 0, "CH4",    0},
    {ITSCOPE_VAL_SOUR_REF1,     0, 0, "REF1",   0},
    {ITSCOPE_VAL_SOUR_REF2,     0, 0, "REF2",   0},
    {ITSCOPE_VAL_SOUR_REF3,     0, 0, "REF3",   0},
    {ITSCOPE_VAL_SOUR_REF4,     0, 0, "REF4",   0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMathSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrMathSourceRangeTableEntries,
};

    /*- ITSCOPE_ATTR_MATH_FUNCTION -*/
static IviRangeTableEntry attrMathFunctionRangeTableEntries[] =
{
    {ITSCOPE_VAL_MATH_ADD,          0, 0, "+",      0},
    {ITSCOPE_VAL_MATH_SUBTRACT,     0, 0, "-",      0},
    {ITSCOPE_VAL_MATH_MULTIPLY,     0, 0, "*",      0},
    {ITSCOPE_VAL_MATH_DIVIDE,       0, 0, "/",      0},
    {ITSCOPE_VAL_MATH_FFT,          0, 0, "FFT",    0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrMathFunctionRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrMathFunctionRangeTableEntries,
};
static ViStatus _VI_FUNC itscopeAttrMathFunction_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      funcString[BUFFER_SIZE] = {0};
    ViInt32     mathType;
    ViString    addString, subString, multiString, divString, fftString;
    
   _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_TYPE,
                                            0, &mathType) ); 
    
    if(mathType == ITSCOPE_VAL_MATH_TYPE_DUAL||mathType == ITSCOPE_VAL_MATH_TYPE_SPECTRUM)
    {
        //_viCheckErr( viQueryf (io, "MATH:DEFINE?", "%s", funcString) );             
		_viCheckErr(_viQueryf(vi, "MATH:DEFINE?", "%s", funcString));

       _viCheckErr( Ivi_GetViInt32EntryFromValue (ITSCOPE_VAL_MATH_ADD, &attrMathFunctionRangeTable,   
                                                    VI_NULL, VI_NULL, VI_NULL, VI_NULL, &addString, VI_NULL) );
       _viCheckErr( Ivi_GetViInt32EntryFromValue (ITSCOPE_VAL_MATH_SUBTRACT, &attrMathFunctionRangeTable,  
                                                    VI_NULL, VI_NULL, VI_NULL, VI_NULL, &subString, VI_NULL) ); 
       _viCheckErr( Ivi_GetViInt32EntryFromValue (ITSCOPE_VAL_MATH_MULTIPLY, &attrMathFunctionRangeTable,  
                                                    VI_NULL, VI_NULL, VI_NULL, VI_NULL, &multiString, VI_NULL) );
       _viCheckErr( Ivi_GetViInt32EntryFromValue (ITSCOPE_VAL_MATH_DIVIDE, &attrMathFunctionRangeTable,    
                                                    VI_NULL, VI_NULL, VI_NULL, VI_NULL, &divString, VI_NULL) ); 
                                                    
        if( strstr (funcString, addString) )                                                        
        {
            *value = ITSCOPE_VAL_MATH_ADD;
        }
        else if( strstr (funcString, subString) )
        {
            *value = ITSCOPE_VAL_MATH_SUBTRACT;
        }
        else if( strstr (funcString, multiString) )
        {
            *value = ITSCOPE_VAL_MATH_MULTIPLY;
        }
        else if( strstr (funcString, divString) )
        {
            *value = ITSCOPE_VAL_MATH_DIVIDE;       
        }
        else
        {
            error = IVI_ERROR_INVALID_VALUE;
           _viCheckErr(error);
        }
    }
    else if(mathType == ITSCOPE_VAL_MATH_TYPE_FFT)
    {
        //_viCheckErr( viQueryf (io, "MATH:DEFINE?", "%s", funcString) );     
		_viCheckErr(_viQueryf(vi, "MATH:DEFINE?", "%s", funcString));

       _viCheckErr( Ivi_GetViInt32EntryFromValue (ITSCOPE_VAL_MATH_FFT, &attrMathFunctionRangeTable,   
                                                    VI_NULL, VI_NULL, VI_NULL, VI_NULL, &fftString, VI_NULL) ); 
                                                    
        if( strstr (funcString, fftString) )
        {
            *value = ITSCOPE_VAL_MATH_FFT;
        }
        else
        {
            error = IVI_ERROR_INVALID_VALUE;
           _viCheckErr(error);
        }
    }
    else
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error); 
    }
        
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrMathFunction_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     mathType;
    ViInt32     source1, source2;
    ViString    source1Name, source2Name, operator;
    
   _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_TYPE,
                                            0, &mathType) );
                                            
    if(!((mathType == ITSCOPE_VAL_MATH_TYPE_DUAL)                                           
            || (mathType == ITSCOPE_VAL_MATH_TYPE_FFT)|| (mathType == ITSCOPE_VAL_MATH_TYPE_SPECTRUM)))
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
    
   _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_SOURCE_1ST,
                                            0, &source1) );
   _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_SOURCE_2ND,
                                            0, &source2) );
    
   _viCheckErr( Ivi_GetViInt32EntryFromValue (source1, &attrMathSourceRangeTable,
                                                VI_NULL, VI_NULL, VI_NULL, VI_NULL,
                                                    &source1Name, VI_NULL) );
   _viCheckErr( Ivi_GetViInt32EntryFromValue (source2, &attrMathSourceRangeTable,
                                                VI_NULL, VI_NULL, VI_NULL, VI_NULL,
                                                    &source2Name, VI_NULL) );
   _viCheckErr( Ivi_GetViInt32EntryFromValue   (value, &attrMathFunctionRangeTable,
                                                VI_NULL, VI_NULL, VI_NULL, VI_NULL,
                                                    &operator, VI_NULL) );
    

    /* SPECTRUM Operation*/
    /* FFT Operation */
    if(value == ITSCOPE_VAL_MATH_FFT)
    {
        //_viCheckErr( viPrintf (io, "MATH:DEFINE \"%s(%s)\"", operator, source1Name) );                                                                 
		_viCheckErr(_viPrintf(vi, "MATH:DEFINE \"%s(%s)\"", operator, source1Name));
    }
    /* Basic Math Operation */
    else
    {
        //_viCheckErr( viPrintf (io, "MATH:DEFINE \"%s%s%s\"", source1Name, operator, source2Name) );         
		_viCheckErr(_viPrintf(vi, "MATH:DEFINE \"%s%s%s\"", source1Name, operator, source2Name));
    }
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_FFT_VERT_SCALE -*/
static IviRangeTableEntry attrFFTVertScaleRangeTableEntries[] =
{
    {ITSCOPE_VAL_MATH_DBV_RMS,      0, 0, "DB",     0},
    {ITSCOPE_VAL_MATH_LINEAR_RMS,   0, 0, "LINEAR", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrFFTVertScaleRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    ":MATH:SPECT:MAG",
    attrFFTVertScaleRangeTableEntries,
};

    /*- ITSCOPE_ATTR_FFT_WINDOW -*/
static IviRangeTableEntry attrFFTWindowRangeTableEntries[] =
{
    {ITSCOPE_VAL_MATH_HANNING,      0, 0, "HANNING",        0},
    {ITSCOPE_VAL_MATH_RECTANGULAR,  0, 0, "RECTANGULAR",    0},
    {ITSCOPE_VAL_MATH_HAMMING,      0, 0, "HAMMING",        0},
    {ITSCOPE_VAL_MATH_BLACK_HARRIS, 0, 0, "BLACKMANHARRIS", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrFFTWindowRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    "MATH:SPECT:WIN",
    attrFFTWindowRangeTableEntries,
};

    /*- ITSCOPE_ATTR_ADV_VAR1 -*/
static IviRangeTableEntry attrAdvVar1RangeTableEntries[] =
{
    {-9.9E15, 9.9E15, 0, "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrAdvVar1RangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    "MATHVAR:VAR1",
    attrAdvVar1RangeTableEntries,
};

    /*- ITSCOPE_ATTR_ADV_VAR2 -*/   
static IviRangeTableEntry attrAdvVar2RangeTableEntries[] =
{
    {-9.9E15, 9.9E15, 0, "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrAdvVar2RangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    ":MATHVAR:VAR2",
    attrAdvVar2RangeTableEntries,
};
    
    /*- ITSCOPE_ATTR_ADV_DEFINE -*/
static ViStatus _VI_FUNC itscopeAttrAdvDefine_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      value[BUFFER_SIZE] = {0};
    ViInt32     mathType;  
    ViInt32     length;
    
   _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_TYPE,
                                            0, &mathType) );
    if(mathType != ITSCOPE_VAL_MATH_TYPE_ADVANCED)
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }
    
    //_viCheckErr( viQueryf (io, ":MATH:DEFINE?", "%s", value) );
	_viCheckErr(_viQueryf(vi, ":MATH:DEFINE?", "%s", value));

    length = (ViInt32)strlen (value);
    
    value[length-1] = '\0';
        
   _checkErr ( Ivi_SetValInStringCallback(vi, attributeId, value+1) );
    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrAdvDefine_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViConstString value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     mathType;  
    
   _checkErr ( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MATH_TYPE,
                                            0, &mathType) );
    if(mathType != ITSCOPE_VAL_MATH_TYPE_ADVANCED)
    {
        error = IVI_ERROR_INVALID_CONFIGURATION;
       _viCheckErr(error);
    }

    //_viCheckErr( viPrintf (io, ":MATH:DEFINE \"%s\"", value) );
	_viCheckErr(_viPrintf(vi, ":MATH:DEFINE \"%s\"", value));

Error:
    return error;
}







    /*- ITSCOPE_ATTR_INSTRUMENT_FIRMWARE_REVISION -*/
static ViStatus _VI_FUNC itscopeAttrInstrumentFirmwareRevision_ReadCallback (ViSession vi, 
                                                                    ViSession io,
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId,
                                                                    const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      idQ[BUFFER_SIZE], rdBuffer[BUFFER_SIZE];
    
   _checkErr( Ivi_GetAttributeViString (vi, "", ITSCOPE_ATTR_ID_QUERY_RESPONSE,
                                        0, BUFFER_SIZE, idQ));

    sscanf (idQ, "%*[^,],%*[^,],%*[^,],%256[^\n]", rdBuffer);

   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}
    
    /*- ITSCOPE_ATTR_INSTRUMENT_MANUFACTURER -*/
static ViStatus _VI_FUNC itscopeAttrInstrumentManufacturer_ReadCallback (ViSession vi, 
                                                                          ViSession io,
                                                                          ViConstString channelName, 
                                                                          ViAttr attributeId,
                                                                          const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      idQ[BUFFER_SIZE], rdBuffer[BUFFER_SIZE];
    
   _checkErr( Ivi_GetAttributeViString (vi, "", ITSCOPE_ATTR_ID_QUERY_RESPONSE,
                                        0, BUFFER_SIZE, idQ));

    sscanf (idQ, "%256[^,]", rdBuffer);

   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}
    
    /*- ITSCOPE_ATTR_INSTRUMENT_MODEL -*/
static ViStatus _VI_FUNC itscopeAttrInstrumentModel_ReadCallback (ViSession vi, 
                                                                   ViSession io,
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId,
                                                                   const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      idQ[BUFFER_SIZE], rdBuffer[BUFFER_SIZE] = {0};
    ViInt32     model;
    
    if (!Ivi_Simulating(vi))
    {
    
       _checkErr( Ivi_GetAttributeViString (vi, VI_NULL, ITSCOPE_ATTR_ID_QUERY_RESPONSE,
                                            0, BUFFER_SIZE, idQ));

        sscanf (idQ, "%*[^,],%256[^,]", rdBuffer);
    }
    else 
    {
       _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model));
      
        switch (model)
        {
            case ITSCOPE_VAL_PE510K1:
            {
                strcpy(rdBuffer, "PE510K1");
                break;
            }
            case ITSCOPE_VAL_PE520K1:
            {
                strcpy(rdBuffer, "PE520K1");
                break;
            }
        }
    }
    
   _checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}

    /*- ITSCOPE_ATTR_FETCH_BUFFER_SIZE -*/
static IviRangeTableEntry attrFetchBufferSizeRangeTableEntries[] =
{
    {MIN_FETCH_BUFFER_SIZE, MAX_FETCH_BUFFER_SIZE, 0, "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};

static IviRangeTable attrFetchBufferSizeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrFetchBufferSizeRangeTableEntries,
};
    
    /*- ITSCOPE_ATTR_DATA_WIDTH -*/
static ViStatus _VI_FUNC itscopeAttrDataWidth_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf(io, ":DAT:WID?", "%ld", value) );
	_viCheckErr(_viQueryf(vi, ":DAT:WID?", "%ld", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrDataWidth_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viPrintf(io, ":DAT:WID %ld", value) );
	_viCheckErr(_viPrintf(vi, ":DAT:WID %ld", value));

Error:
    return error;
}

    /*- ITSCOPE_`ATTR_DATA_START_POS -*/
static ViStatus _VI_FUNC itscopeAttrDataStartPos_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf(io, ":DAT:STAR?", "%ld", value) );
	_viCheckErr(_viQueryf(vi, ":DAT:STAR?", "%ld", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrDataStartPos_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viPrintf(io, ":DAT:STAR %ld", value) ); 
	_viCheckErr(_viPrintf(vi, ":DAT:STAR %ld", value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_DATA_STOP_POS -*/
static ViStatus _VI_FUNC itscopeAttrDataStopPos_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf(io, ":DAT:STOP?", "%ld", value) );
	_viCheckErr(_viQueryf(vi, ":DAT:STOP?", "%ld", value));

Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrDataStopPos_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viPrintf(io, ":DAT:STOP %ld", value) );
	_viCheckErr(_viPrintf(vi, ":DAT:STOP %ld", value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_DATA_YZERO -*/
static ViStatus _VI_FUNC itscopeAttrDataYZero_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf(io, ":WFMPRE:YZE?", "%le", value) );
	_viCheckErr(_viQueryf(vi, ":WFMPRE:YZERO?", "%le", value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_DATA_YOFF -*/
static ViStatus _VI_FUNC itscopeAttrDataYoff_ReadCallback (ViSession vi,
                                                           ViSession io,
                                                           ViConstString channelName,
                                                           ViAttr attributeId,
                                                           ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
    //_viCheckErr( viQueryf(io, ":WFMPRE:YOFF?", "%le", value) );
	_viCheckErr(_viQueryf(vi, ":WFMPRE:YOFF?", "%le", value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_DATA_YMULT -*/
static ViStatus _VI_FUNC itscopeAttrDataYmult_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf(io, ":WFMPRE:YMULT?", "%le", value) );  
	_viCheckErr(_viQueryf(vi, ":WFMPRE:YMULT?", "%le", value));

Error:
    return error;
}

    /*- ITSCOPE_ATTR_DATA_XINCR -*/
static ViStatus _VI_FUNC itscopeAttrDataXincr_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf (io, "WFMPRE:XINCR?", "%le", value) );
	_viCheckErr(_viQueryf(vi, ":WFMPRE:XINCR?", "%le", value));
	
Error:
    return error;
}

    /*- ITSCOPE_ATTR_DATA_XZERO -*/
static ViStatus _VI_FUNC itscopeAttrDataXzero_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    //_viCheckErr( viQueryf (io, "WFMPRE:XZERO?", "%le", value) );
	_viCheckErr(_viQueryf(vi, ":WFMPRE:XZERO?", "%le", value));

Error:
    return error;
}

static ViStatus itscope_buildRangeTable(ViSession vi)    
{
    ViStatus error = VI_SUCCESS;       
    ViInt32 tableSize = MAX_TABLE_SIZE; /* Max tablesize of the table, depend on how many items in it */
       
   _checkErr(Ivi_RangeTableNew (vi, tableSize + 1, IVI_VAL_DISCRETE, VI_FALSE, VI_FALSE, &attrMeasFunctionRangeTable));
       
    attrMeasFunctionRangeTable->customInfo = "MEASU:IMM:TYP";

   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 0,  ITSCOPE_VAL_RISE_TIME,           0.0, 0.0, "RISE",       0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 1,  ITSCOPE_VAL_FALL_TIME,           0.0, 0.0, "FALL",       0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 2,  ITSCOPE_VAL_FREQUENCY,           0.0, 0.0, "FREQUENCY",  0)); 
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 3,  ITSCOPE_VAL_PERIOD,              0.0, 0.0, "PERIOD",     0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 4,  ITSCOPE_VAL_VOLTAGE_RMS,             0.0, 0.0, "RMS",        0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 5,  ITSCOPE_VAL_VOLTAGE_PEAK_TO_PEAK,    0.0, 0.0, "PK2PK",      0));  
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 6,  ITSCOPE_VAL_VOLTAGE_MAX,             0.0, 0.0, "MAXIMUM",    0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 7,  ITSCOPE_VAL_VOLTAGE_MIN,             0.0, 0.0, "MINIMUM",    0));        
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 8,  ITSCOPE_VAL_VOLTAGE_HIGH,            0.0, 0.0, "HIGH",       0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 9,  ITSCOPE_VAL_VOLTAGE_LOW,             0.0, 0.0, "LOW",        0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 10, ITSCOPE_VAL_VOLTAGE_AVERAGE,         0.0, 0.0, "MEAN",       0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 11, ITSCOPE_VAL_WIDTH_NEG,           0.0, 0.0, "NWIDTH",     0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 12, ITSCOPE_VAL_WIDTH_POS,           0.0, 0.0, "PWIDTH",     0));        
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 13, ITSCOPE_VAL_DUTY_CYCLE_NEG,      0.0, 0.0, "NDUTY",      0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 14, ITSCOPE_VAL_DUTY_CYCLE_POS,      0.0, 0.0, "PDUTY",      0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 15, ITSCOPE_VAL_AMPLITUDE,           0.0, 0.0, "AMPLITUDE",  0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 16, ITSCOPE_VAL_VOLTAGE_CYCLE_RMS,   0.0, 0.0, "CRMS",       0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 17, ITSCOPE_VAL_VOLTAGE_CYCLE_AVERAGE, 0.0, 0.0, "CMEAN",        0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 18, ITSCOPE_VAL_POSITIVE_OVERSHOOT,  0.0, 0.0, "POVERSHOOT", 0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 19, ITSCOPE_VAL_NEGATIVE_OVERSHOOT,  0.0, 0.0, "NOVERSHOOT", 0)); 
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 20, ITSCOPE_VAL_TIMING_BURST_WIDTH,  0.0, 0.0, "BURST",      0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 21, ITSCOPE_VAL_PHASE,               0.0, 0.0, "PHASE",      0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 22, ITSCOPE_VAL_DELAY,               0.0, 0.0, "DELAY",      0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 23, ITSCOPE_VAL_AREA,                    0.0, 0.0, "AREA",       0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 24, ITSCOPE_VAL_CYCLE_AREA,          0.0, 0.0, "CAREA",      0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 25, ITSCOPE_VAL_CROSS_NEG,               0.0, 0.0, "NCROSS",     0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 26, ITSCOPE_VAL_CROSS_POS,               0.0, 0.0, "PCROSS",     0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 27, ITSCOPE_VAL_EDGE_POS,                0.0, 0.0, "PEDGECOUNT", 0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 28, ITSCOPE_VAL_EDGE_NEG,                0.0, 0.0, "NEDGECOUNT", 0));     
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 29, ITSCOPE_VAL_PULSE_POS,               0.0, 0.0, "PPULSECOUNT",0));  
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 30, ITSCOPE_VAL_PULSE_NEG,               0.0, 0.0, "NPULSECOUNT",0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 31, ITSCOPE_VAL_HISTOGRAM_HITS,          0.0, 0.0, "HIT",        0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 32, ITSCOPE_VAL_MEDIAN,                  0.0, 0.0, "MED",        0));    
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 33, ITSCOPE_VAL_SIGMA1,                  0.0, 0.0, "SIGMA1",     0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 34, ITSCOPE_VAL_SIGMA2,                  0.0, 0.0, "SIGMA2",     0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 35, ITSCOPE_VAL_SIGMA3,                  0.0, 0.0, "SIGMA3",     0));    
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 36, ITSCOPE_VAL_STDDEV,                  0.0, 0.0, "STD",        0));    
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 37, ITSCOPE_VAL_WAVEFORMS,               0.0, 0.0, "WAVEFORMS",  0));
   _viCheckErr(Ivi_SetRangeTableEntry (attrMeasFunctionRangeTable, 38, ITSCOPE_VAL_TOVERSHOOT,              0.0, 0.0, "TOV",        0));
    
Error:              
    return error;   
}

    /*- ITSCOPE_ATTR_LOWER_THRESHOLD -*/


static ViStatus _VI_FUNC itscopeAttrLowerThreshold_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     model;
    
   _checkErr (Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, value));
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );

    switch(model)
    {
        case ITSCOPE_VAL_PE510K1:
		case ITSCOPE_VAL_PE520K1:
			break;
        default:
           _viCheckErr(IVI_ERROR_INVALID_CONFIGURATION);     
    }

    
Error:
    return error;
}

/*- ITSCOPE_ATTR_UPPER_THRESHOLD -*/   

static ViStatus _VI_FUNC itscopeAttrUpperThreshold_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     model;
    
   _checkErr (Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, value));
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );

    switch(model)
    {
        case ITSCOPE_VAL_PE510K1:
		case ITSCOPE_VAL_PE520K1:
            break;
        default:
           _viCheckErr(IVI_ERROR_INVALID_CONFIGURATION);     
    }

    
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeBModelViInt32_CheckCallback (ViSession vi,
                                                      ViConstString channelName,
                                                      ViAttr attributeId,
                                                      ViInt32 value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     model;
    
   _checkErr(Ivi_DefaultCheckCallbackViInt32 (vi, channelName, attributeId, value));
   _checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ITSCOPE_ATTR_MODEL, 0, &model) );

    switch(model)
    {
		case ITSCOPE_VAL_PE510K1:
		case ITSCOPE_VAL_PE520K1:
            break;
        default:
           _viCheckErr(IVI_ERROR_INVALID_CONFIGURATION);     
    }

    
Error:
    return error;
}



        



/* ITSCOPE_ATTR_ACQUISITION_NUMBER */                 
static ViStatus _VI_FUNC itscopeAttrAcquisitionNumber_CoerceCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId, 
                                                                      ViInt32 value, ViInt32* coercedValue)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    newValue = value;   
    
    if((value>=1)&&(value<=100))
    {
       _checkErr( itscope_CoerceToResolution (vi, value, 1, 12, &newValue));
    }
    else if((value>=100)&&(value<=1000))
    {
       _checkErr( itscope_CoerceToResolution (vi, value, 10, 12, &newValue));
    }    
    else if((value>=1000)&&(value<=10000))
    {
       _checkErr( itscope_CoerceToResolution (vi, value, 100, 12, &newValue));
    }     
    else if((value>=10000)&&(value<=1000000))
    {
       _checkErr( itscope_CoerceToResolution (vi, value, 1000, 12, &newValue));
    } 
    
Error:
    *coercedValue = (ViInt32)newValue;
    return error;
}



static ViStatus _VI_FUNC itscopeAttrRfPhaseTraceDegreesNum_CoerceCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId, 
                                                                           ViInt32 value, ViInt32* coercedValue)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    absValue;
    ViReal64    newValue = value;
    ViReal64    resMultiplier;

    absValue = fabs (value);

    modf (absValue/180, &resMultiplier);
    
    if ((absValue/180-resMultiplier)>=0.5)
        resMultiplier = ceil (absValue/180);

    newValue = 180 * resMultiplier;

    if (value < 0.0)
        newValue *= -1.0;

    *coercedValue = (ViInt32)newValue;
    return error;
}

/*****************************************************************************
 *  Function: itscope_GetStringFromTable
 *  Purpose:  Read the string command from rangetable    
 *****************************************************************************/
static ViStatus itscope_GetStringFromTable(
        itscopeStringValueTable table,
    ViInt32               value,
    ViConstString*        string
)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   found = VI_FALSE;
    ViInt32     i;
    
    for( i = 0; table[i].string; i++)
    {
        if ( table[i].value == value )
        {
            *string = table[i].string;
            found = VI_TRUE;
            break;
        }
    }
    
    if ( !found )
        error = IVI_ERROR_INVALID_VALUE;

    return error;
}

/*****************************************************************************
 *  Function: itscope_GetRepCommandFromTable
 *  Purpose:  Read the repeated command from range table to concat with the 
 *            string command.
 *****************************************************************************/
static ViStatus itscope_GetRepCommandFromTable(
    itscopeStringValueTable table,
    ViInt32               value,
    ViConstString*        repCommand
)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   found = VI_FALSE;
    ViInt32     i;

    for( i = 0; table[i].repCommand; i++)
    {
        if ( table[i].value == value )
        {
            *repCommand = table[i].repCommand;
            found = VI_TRUE;
            break;
        }
    }

    if ( !found )
        error = IVI_ERROR_INVALID_VALUE;

    return error;
}


/*****************************************************************************
 *  Function: itscope_TestifyPEK1Model
 *  Purpose:  Testify if the current instrument is PEK1 Series.
 *****************************************************************************/
static ViStatus itscope_TestifyPEK1Model (ViSession vi)
{
    ViStatus error= VI_SUCCESS;
    ViInt32  modelNo;
   _viCheckErr(Ivi_GetAttributeViInt32(vi,VI_NULL,ITSCOPE_ATTR_MODEL,0,&modelNo));
    if(!IS_PEK1(modelNo))
    {
       _checkErr(IVI_ERROR_ATTRIBUTE_NOT_SUPPORTED);
    }
Error:
    return error;
}

static ViStatus _VI_FUNC itscopeAttrAdvVar1_RangeTableCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                IviRangeTablePtr *rangeTablePtr)
{
	ViStatus	error = VI_SUCCESS;
	IviRangeTablePtr	tblPtr = VI_NULL;
	ViInt32  modelNo;
   _viCheckErr(Ivi_GetAttributeViInt32(vi,VI_NULL,ITSCOPE_ATTR_MODEL,0,&modelNo));

	if(!IS_PEK1(modelNo)){
		tblPtr = &attrAdvVar1RangeTable;
	}
	else{
		tblPtr = &attrPEK1AdvVarRangeTable;
	}

Error:
	*rangeTablePtr = tblPtr;
	return error;
}

static ViStatus _VI_FUNC itscopeAttrAdvVar2_RangeTableCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                IviRangeTablePtr *rangeTablePtr)
{
	ViStatus	error = VI_SUCCESS;
	IviRangeTablePtr	tblPtr = VI_NULL;
	ViInt32  modelNo;
   _viCheckErr(Ivi_GetAttributeViInt32(vi,VI_NULL,ITSCOPE_ATTR_MODEL,0,&modelNo));

	if(!IS_PEK1(modelNo)){
		tblPtr = &attrAdvVar2RangeTable;
	}
	else{
		tblPtr = &attrPEK1AdvVarRangeTable;
	}


Error:
	*rangeTablePtr = tblPtr;
	return error;
}


/*****************************************************************************
 * Function: itscope_InitAttributes
 * Purpose:  This function adds attributes to the IVI session, initializes   
 *           instrument attributes, and sets attribute invalidation          
 *           dependencies.                                                   
 *****************************************************************************/
static ViStatus itscope_InitAttributes (ViSession vi, ViString idnString, ViInt32 instrModelNo)
{
    ViStatus    error = VI_SUCCESS;
    
        /*- Initialize instrument attributes --------------------------------*/            

   _checkErr( itscope_buildRangeTable(vi));
    
   _checkErr( Ivi_SetAttributeViInt32 (vi, "", ITSCOPE_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION,
                                       0, ITSCOPE_MAJOR_VERSION));
   _checkErr( Ivi_SetAttributeViInt32 (vi, "", ITSCOPE_ATTR_SPECIFIC_DRIVER_MINOR_VERSION,
                                       0, ITSCOPE_MINOR_VERSION));
   _checkErr( Ivi_SetAttrReadCallbackViString (vi, ITSCOPE_ATTR_SPECIFIC_DRIVER_REVISION,
                                               itscopeAttrDriverRevision_ReadCallback));
   _checkErr( Ivi_SetAttributeViInt32 (vi, "", ITSCOPE_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION,
                                       0, ITSCOPE_CLASS_SPEC_MAJOR_VERSION));
   _checkErr( Ivi_SetAttributeViInt32 (vi, "", ITSCOPE_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION,
                                       0, ITSCOPE_CLASS_SPEC_MINOR_VERSION));
   _checkErr( Ivi_SetAttributeViString (vi, "", ITSCOPE_ATTR_IO_SESSION_TYPE,
                                        0, ITSCOPE_IO_SESSION_TYPE));
   _checkErr( Ivi_SetAttributeViString (vi, "", ITSCOPE_ATTR_SUPPORTED_INSTRUMENT_MODELS,
                                        0, ITSCOPE_SUPPORTED_INSTRUMENT_MODELS));
   _checkErr( Ivi_SetAttrReadCallbackViString (vi, ITSCOPE_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                               itscopeAttrInstrumentFirmwareRevision_ReadCallback));
   _checkErr( Ivi_SetAttrReadCallbackViString (vi, ITSCOPE_ATTR_INSTRUMENT_MANUFACTURER,
                                               itscopeAttrInstrumentManufacturer_ReadCallback));
   _checkErr( Ivi_SetAttrReadCallbackViString (vi, ITSCOPE_ATTR_INSTRUMENT_MODEL,
                                               itscopeAttrInstrumentModel_ReadCallback));
   _checkErr( Ivi_SetAttributeViString (vi, "", ITSCOPE_ATTR_SPECIFIC_DRIVER_VENDOR,
                                        0, ITSCOPE_DRIVER_VENDOR));
   _checkErr( Ivi_SetAttributeViString (vi, "", ITSCOPE_ATTR_SPECIFIC_DRIVER_DESCRIPTION,
                                        0, ITSCOPE_DRIVER_DESCRIPTION));
   _checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_OPC_CALLBACK, 0, 
                                      itscope_WaitForOPCCallback));
   _checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_CHECK_STATUS_CALLBACK, 0, 
                                      itscope_CheckStatusCallback));
   _checkErr( Ivi_SetAttributeViBoolean (vi, VI_NULL, IVI_ATTR_SUPPORTS_WR_BUF_OPER_MODE, 
                                         0, VI_TRUE));
   _checkErr( Ivi_SetAttributeFlags (vi, ITSCOPE_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                     IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
   _checkErr( Ivi_SetAttributeFlags (vi, ITSCOPE_ATTR_INSTRUMENT_MANUFACTURER,
                                     IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
   _checkErr( Ivi_SetAttributeFlags (vi, ITSCOPE_ATTR_INSTRUMENT_MODEL,
                                     IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
                                 
   _checkErr( Ivi_SetAttributeViString (vi, "", ITSCOPE_ATTR_GROUP_CAPABILITIES, 0,
                                        "IviScopeBase,"
                                        "IviScopeTVTrigger,"
                                        "IviScopeRuntTrigger,"
                                        "IviScopeGlitchTrigger,"
                                        "IviScopeAcLineTrigger,"
                                        "IviScopeWaveformMeas,"
                                        "IviScopeMinMaxWaveform,"
                                        "IviScopeContinuousAcquisition,"
                                        "IviScopeAverageAcquisition,"
                                        "IviScopeSampleMode,"
                                        "IviScopeTriggerModifier,"
                                        "IviScopeAutoSetup"));

    /*- Acquisition Sub-system ----------------------------------------------------------*/
        /*- ITSCOPE_ATTR_ACQUISITION_TYPE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_ACQUISITION_TYPE,
                                       "ITSCOPE_ATTR_ACQUISITION_TYPE",
                                       ITSCOPE_VAL_NORMAL, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrAcquisitionTypeRangeTable));
                                        
        /*- ITSCOPE_ATTR_ACQUISITION_START_TIME -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_ACQUISITION_START_TIME,
                                        "ITSCOPE_ATTR_ACQUISITION_START_TIME", 0.0,	
                                        0,
                                        itscopeAttrAcquisitionStartTime_ReadCallback,
                                        itscopeAttrAcquisitionStartTime_WriteCallback,
                                        VI_NULL, 0));
   _checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_ACQUISITION_START_TIME, 
                                                 itscopeAttrAcquisitionStartTime_CoerceCallback));
   _checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_ACQUISITION_START_TIME, 
                                                 itscopeAttrAcquisitionStartTime_CheckCallback));
                                                 
        /*- ITSCOPE_ATTR_HORZ_MIN_NUM_PTS -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS,
                                       "ITSCOPE_ATTR_HORZ_MIN_NUM_PTS", 4096,
                                       IVI_VAL_USE_CALLBACKS_FOR_SIMULATION, VI_NULL,
                                       itscopeAttrHorzMinNumPts_WriteCallback,
                                       &attrPEK1HorzMinNumPtsRangeTable));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS,
                                             itscopeAttrHorzMinNumPts_RangeTableCallback));

        /*- ITSCOPE_ATTR_HORZ_RECORD_LENGTH -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_HORZ_RECORD_LENGTH,
                                       "ITSCOPE_ATTR_HORZ_RECORD_LENGTH", 8192,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       itscopeAttrHorzRecordLength_ReadCallback,
                                       VI_NULL, VI_NULL));
                                       
        /*- ITSCOPE_ATTR_HORZ_SAMPLE_RATE -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_HORZ_SAMPLE_RATE,
                                        "ITSCOPE_ATTR_HORZ_SAMPLE_RATE", 500.0e6,
                                        IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_NEVER_CACHE,
                                        itscopeAttrHorzSampleRate_ReadCallback,
                                        VI_NULL, VI_NULL, 0));

        /*- ITSCOPE_ATTR_HORZ_TIME_PER_RECORD -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD,
                                        "ITSCOPE_ATTR_HORZ_TIME_PER_RECORD", 16.384e-6,
                                        0, itscopeAttrHorzTimePerRecord_ReadCallback,
                                        itscopeAttrHorzTimePerRecord_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD,
                                             itscopeAttrHorzTimePerRecord_RangeTableCallback));        

        /*- ITSCOPE_ATTR_NUM_ENVELOPES -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_NUM_ENVELOPES,
                                       "ITSCOPE_ATTR_NUM_ENVELOPES",
                                       ITSCOPE_VAL_INFINITE, 0,
                                       itscopeAttrNumEnvelopes_ReadCallback,
                                       itscopeAttrNumEnvelopes_WriteCallback,
                                       &attrNumEnvelopesRangeTable));
    
        /*- ITSCOPE_ATTR_INITIATE_CONTINUOUS -*/
   _checkErr (Ivi_AddAttributeViBoolean (vi, ITSCOPE_ATTR_INITIATE_CONTINUOUS,
                                         "ITSCOPE_ATTR_INITIATE_CONTINUOUS",
                                         VI_FALSE, 0,
                                         itscopeAttrInitiateContinuous_ReadCallback,
                                         itscopeAttrInitiateContinuous_WriteCallback));
        
        /*- ITSCOPE_ATTR_NUM_AVERAGES -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_NUM_AVERAGES,
                                       "ITSCOPE_ATTR_NUM_AVERAGES", 16, 0,	// усреднение
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrNumAveragesRangeTable));

        /*- ITSCOPE_ATTR_SAMPLE_MODE -*/                                       
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_SAMPLE_MODE,
                                       "ITSCOPE_ATTR_SAMPLE_MODE",
                                       ITSCOPE_VAL_REAL_TIME,
                                       IVI_VAL_NOT_USER_WRITABLE, VI_NULL, VI_NULL,
                                       VI_NULL));
    
   
    /*- Channel Sub-system --------------------------------------------------------------*/
        /*- ITSCOPE_ATTR_CHANNEL_ENABLED -*/
   _checkErr (Ivi_AddAttributeViBoolean (vi, ITSCOPE_ATTR_CHANNEL_ENABLED,
                                         "ITSCOPE_ATTR_CHANNEL_ENABLED", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         itscopeAttrChannelEnabled_ReadCallback,
                                         itscopeAttrChannelEnabled_WriteCallback));

        /*- ITSCOPE_ATTR_PROBE_ATTENUATION -*/ 
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_PROBE_ATTENUATION,
                                        "ITSCOPE_ATTR_PROBE_ATTENUATION",
                                        ITSCOPE_VAL_PROBE_SENSE_ON,
                                        IVI_VAL_MULTI_CHANNEL,
                                        itscopeAttrProbeAttenuation_ReadCallback,
                                        itscopeAttrProbeAttenuation_WriteCallback,
                                        &attrProbeAttenuationRangeTable, 0));

        /*- ITSCOPE_ATTR_PROBE_SENSE_VALUE -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_PROBE_SENSE_VALUE,
                                        "ITSCOPE_ATTR_PROBE_SENSE_VALUE", 1.0,
                                        IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE,
                                        itscopeAttrProbeSenseValue_ReadCallback,
                                        VI_NULL, VI_NULL, 0));

        /*- ITSCOPE_ATTR_PROBE_SENSE -*/
   _checkErr (Ivi_AddAttributeViBoolean (vi, ITSCOPE_ATTR_PROBE_SENSE,
                                         "ITSCOPE_ATTR_PROBE_SENSE", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE 
                                         | IVI_VAL_HIDDEN,
                                         VI_NULL, VI_NULL));

        /*- ITSCOPE_ATTR_PROBE_GAIN -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_PROBE_GAIN,
                                        "ITSCOPE_ATTR_PROBE_GAIN", 1.0,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_HIDDEN,
                                        itscopeAttrProbeGain_ReadCallback,
                                        itscopeAttrProbeGain_WriteCallback, VI_NULL,
                                        0));

        /*- ITSCOPE_ATTR_VERTICAL_RANGE -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_VERTICAL_RANGE,
                                        "ITSCOPE_ATTR_VERTICAL_RANGE", 1,
                                        IVI_VAL_MULTI_CHANNEL,
                                        itscopeAttrVerticalRange_ReadCallback,
                                        itscopeAttrVerticalRange_WriteCallback,
                                        &attrVerticalRangeRangeTable, 0));
   _checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_VERTICAL_RANGE,
                                                 itscopeAttrVerticalRange_CoerceCallback));
   _checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_VERTICAL_RANGE,
                                                itscopeAttrVerticalRange_CheckCallback));
    
        /*- ITSCOPE_ATTR_VERTICAL_OFFSET -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_VERTICAL_OFFSET,
                                        "ITSCOPE_ATTR_VERTICAL_OFFSET", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        itscopeAttrVerticalOffset_ReadCallback,
                                        itscopeAttrVerticalOffset_WriteCallback,
                                        VI_NULL, 0));
   _checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_VERTICAL_OFFSET, 
                                                itscopeAttrVerticalOffset_CheckCallback));
                                  
        /*- ITSCOPE_ATTR_VERTICAL_COUPLING -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_VERTICAL_COUPLING,
                                       "ITSCOPE_ATTR_VERTICAL_COUPLING",
                                       ITSCOPE_VAL_DC, IVI_VAL_MULTI_CHANNEL,
                                       itscopeAttrVerticalCoupling_ReadCallback,
                                       itscopeAttrVerticalCoupling_WriteCallback,
                                       &attrVerticalCouplingRangeTable));

        /*- ITSCOPE_ATTR_MAX_INPUT_FREQUENCY -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_MAX_INPUT_FREQUENCY,
                                        "ITSCOPE_ATTR_MAX_INPUT_FREQUENCY", 250.0e6, //250MHz
                                        IVI_VAL_MULTI_CHANNEL,
                                        itscopeAttrMaxInputFrequency_ReadCallback,
                                        itscopeAttrMaxInputFrequency_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_MAX_INPUT_FREQUENCY,
                                             itscopeAttrMaxInputFrequency_RangeTableCallback));

        /*- ITSCOPE_ATTR_INPUT_IMPEDANCE -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_INPUT_IMPEDANCE,
                                        "ITSCOPE_ATTR_INPUT_IMPEDANCE", 50.0,	// 50Om
                                        IVI_VAL_MULTI_CHANNEL,
                                        itscopeAttrChannelImpedance_ReadCallback,
                                        itscopeAttrChannelImpedance_WriteCallback,
                                        &attrPEK1ChannelImpedanceRangeTable, 0));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_INPUT_IMPEDANCE,
                                             itscopeAttrInputImpedance_RangeTableCallback));
    
   
        /*- ITSCOPE_ATTR_TOTAL_POINTS -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TOTAL_POINTS,
                                       "ITSCOPE_ATTR_TOTAL_POINTS", 8192,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_HIDDEN,
                                       itscopeAttrTotalPoints_ReadCallback,
                                       itscopeAttrTotalPoints_WriteCallback,
                                       &attrPEK1TotalPointsRangeTable));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_TOTAL_POINTS,
                                             itscopeAttrTotalPoints_RangeTableCallback));

        /*- ITSCOPE_ATTR_TRIGGER_TYPE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TRIGGER_TYPE,
                                       "ITSCOPE_ATTR_TRIGGER_TYPE",
                                       ITSCOPE_VAL_EDGE_TRIGGER,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       itscopeAttrTriggerType_ReadCallback,
                                       itscopeAttrTriggerType_WriteCallback,
                                       &attrPEK1TriggerTypeRangeTable));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_TRIGGER_TYPE,
                                             itscopeAttrTriggerType_RangeTableCallback));
   _checkErr( Ivi_SetAttrCheckCallbackViInt32 (vi, ITSCOPE_ATTR_TRIGGER_TYPE, 
                                               itscopeAttrTriggerType_CheckCallback));

        /*- ITSCOPE_ATTR_TRIGGER_SOURCE -*/
   _checkErr (Ivi_AddAttributeViString (vi, ITSCOPE_ATTR_TRIGGER_SOURCE,
                                        "ITSCOPE_ATTR_TRIGGER_SOURCE",
                                        ITSCOPE_VAL_CH_1, 0,
                                        itscopeAttrTriggerSource_ReadCallback,
                                        itscopeAttrTriggerSource_WriteCallback));
   _checkErr( Ivi_SetAttrCheckCallbackViString (vi, ITSCOPE_ATTR_TRIGGER_SOURCE, 
                                                itscopeAttrTriggerSource_CheckCallback));

        /*- ITSCOPE_ATTR_TRIGGER_LEVEL -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRIGGER_LEVEL,
                                        "ITSCOPE_ATTR_TRIGGER_LEVEL", 0.0,
                                        IVI_VAL_NEVER_CACHE,
                                        itscopeAttrTriggerLevel_ReadCallback,
                                        itscopeAttrTriggerLevel_WriteCallback,
                                        VI_NULL, 0));
   _checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_TRIGGER_LEVEL, 
                                                itscopeAttrTriggerLevel_CheckCallback));
                                                
        /*- ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH -*/                                       
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH,
                                        "ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH", 0,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE,
                                        itscopeAttrTriggerLevelByCh_ReadCallback,
                                        itscopeAttrTriggerLevelByCh_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH,
                                                itscopeAttrTriggerLevelByCh_CheckCallback));
                                            
        /*- ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH -*/                                            
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH,
                                        "ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH", 0.8,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE,
                                        itscopeAttrTriggerUpperthresByCh_ReadCallback,
                                        itscopeAttrTriggerUpperthresByCh_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ITSCOPE_ATTR_TRIGGER_UPPERTHRES_BY_CH,
                                                itscopeAttrTriggerThresholdByCh_CheckCallback));
                                                
        /*- ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH -*/                                               
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH,
                                        "ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH", 0,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE,
                                        itscopeAttrTriggerLowerthresByCh_ReadCallback,
                                        itscopeAttrTriggerLowerthresByCh_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_TRIGGER_LOWERTHRES_BY_CH,
                                                itscopeAttrTriggerThresholdByCh_CheckCallback));
                                                
        /*- ITSCOPE_ATTR_TRIGGER_HOLDOFF -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRIGGER_HOLDOFF,
                                        "ITSCOPE_ATTR_TRIGGER_HOLDOFF", 20.0e-9, 0,
                                        itscopeAttrTriggerHoldoff_ReadCallback,
                                        itscopeAttrTriggerHoldoff_WriteCallback,
                                        &attrTriggerHoldoffRangeTable, 9));
   _checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_TRIGGER_HOLDOFF, 
                                                 itscopeAttrTriggerHoldoff_CoerceCallback));
                                        

        /*- ITSCOPE_ATTR_TRIGGER_MODIFIER -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TRIGGER_MODIFIER,
                                       "ITSCOPE_ATTR_TRIGGER_MODIFIER",
                                       ITSCOPE_VAL_NO_TRIGGER_MOD, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrTriggerModifierRangeTable));

        /*- ITSCOPE_ATTR_TRIGGER_COUPLING -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TRIGGER_COUPLING,
                                       "ITSCOPE_ATTR_TRIGGER_COUPLING",
                                       ITSCOPE_VAL_DC, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrPEK1TriggerCouplingRangeTable));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_TRIGGER_COUPLING,
                                             itscopeAttrTriggerCoupling_RangeTableCallback));
    
        /*- ITSCOPE_ATTR_TRIGGER_SLOPE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TRIGGER_SLOPE,
                                       "ITSCOPE_ATTR_TRIGGER_SLOPE",
                                       ITSCOPE_VAL_POSITIVE, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrTriggerSlopeRangeTable));
                                        
   
        /*- ITSCOPE_ATTR_AC_LINE_TRIGGER_SLOPE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_AC_LINE_TRIGGER_SLOPE,
                                       "ITSCOPE_ATTR_AC_LINE_TRIGGER_SLOPE",
                                       ITSCOPE_VAL_AC_LINE_POSITIVE,
                                       IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
                                       itscopeAttrAcLineTriggerSlope_ReadCallback,
                                       itscopeAttrAcLineTriggerSlope_WriteCallback,
                                       &attrAcLineTriggerSlopeRangeTable));

        /*- ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT,
                                       "ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT",
                                       ITSCOPE_VAL_NTSC, 0,
                                       itscopeAttrTvTriggerSignalFormat_ReadCallback,
                                       itscopeAttrTvTriggerSignalFormat_WriteCallback,
                                       &attrPEK1TvTriggerSignalFormatRangeTable));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi,
                                             ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT,
                                             itscopeAttrTvTriggerSignalFormat_RangeTableCallback));

        /*- ITSCOPE_ATTR_TV_TRIGGER_POLARITY -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_POLARITY,
                                       "ITSCOPE_ATTR_TV_TRIGGER_POLARITY",
                                       ITSCOPE_VAL_POSITIVE, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrTvTriggerPolarityRangeTable));

        /*- ITSCOPE_ATTR_TV_TRIGGER_EVENT -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_EVENT,
                                       "ITSCOPE_ATTR_TV_TRIGGER_EVENT",
                                       ITSCOPE_VAL_TV_EVENT_ANY_LINE, 0,
                                       itscopeAttrTvTriggerEvent_ReadCallback,
                                       itscopeAttrTvTriggerEvent_WriteCallback,
                                       &attrTvTriggerEventRangeTable));

        /*- ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER,
                                       "ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER", 1, 0,
                                       itscopeAttrTvTriggerLineNumber_ReadCallback,
                                       itscopeAttrTvTriggerLineNumber_WriteCallback,
                                       VI_NULL));
   _checkErr( Ivi_SetAttrCheckCallbackViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER, 
                                               itscopeAttrTvTriggerLineNumber_CheckCallback));
    
        /*- ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT,
                                       "ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT",
                                       ITSCOPE_VAL_HDTV_1080P24, 0,
                                       itscopeAttrTvTriggerHdtvFormat_ReadCallback,
                                       itscopeAttrTvTriggerHdtvFormat_WriteCallback,
                                       &attrPEK1TvTriggerHdtvFormatRangeTable));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_TV_TRIGGER_HDTV_FORMAT,
                                             itscopeAttrTvTriggerHdtvFormat_RangeTableCallback));
    
        /*- ITSCOPE_ATTR_TV_TRIGGER_FIELD_HOLDOFF -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TV_TRIGGER_FIELD_HOLDOFF,
                                        "ITSCOPE_ATTR_TV_TRIGGER_FIELD_HOLDOFF", 0,
                                        IVI_VAL_NEVER_CACHE,
                                        itscopeAttrTvTriggerFieldHoldoff_ReadCallback,
                                        itscopeAttrTvTriggerFieldHoldoff_WriteCallback,
                                        &attrTvTriggerFieldHoldoffRangeTable, 0));
    
        /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_FORMAT -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_FORMAT,
                                       "ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_FORMAT",
                                       ITSCOPE_VAL_TV_CUSTOM_INTERLACED, 0,
                                       itscopeAttrTvTriggerCustomFormat_ReadCallback,
                                       itscopeAttrTvTriggerCustomFormat_WriteCallback,
                                       &attrTvTriggerCustomFormatRangeTable));
    
        /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE,
                                       "ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SCAN_RATE",
                                       ITSCOPE_VAL_TV_RATE15K, 0,
                                       itscopeAttrTvTriggerCustomScanRate_ReadCallback,
                                       itscopeAttrTvTriggerCustomScanRate_WriteCallback,
                                       &attrTvTriggerCustomScanRateRangeTable));
    
        /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_LINE_PERIOD -*/ 
   _checkErr (Ivi_AddAttributeViReal64 (vi,
                                        ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_LINE_PERIOD,
                                        "ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_LINE_PERIOD",
                                        0, 0,
                                        itscopeAttrTvTriggerCustomLinePeriod_ReadCallback,
                                        itscopeAttrTvTriggerCustomLinePeriod_WriteCallback,
                                        &attrTvTriggerCustomLinePeriodRangeTable, 0));
    
        /*- ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SYNC_INTERVAL -*/ 
   _checkErr (Ivi_AddAttributeViReal64 (vi,
                                        ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SYNC_INTERVAL,
                                        "ITSCOPE_ATTR_TV_TRIGGER_CUSTOM_SYNC_INTERVAL",
                                        0, 0,
                                        itscopeAttrTvTriggerCustomSyncInterval_ReadCallback,
                                        itscopeAttrTvTriggerCustomSyncInterval_WriteCallback,
                                        &attrTvTriggerCustomSyncIntervalRangeTable,
                                        0));
    
        /*- ITSCOPE_ATTR_RUNT_HIGH_THRESHOLD -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_RUNT_HIGH_THRESHOLD,
                                        "ITSCOPE_ATTR_RUNT_HIGH_THRESHOLD", 0.8,
                                        IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                        itscopeAttrRuntHighThreshold_ReadCallback,
                                        itscopeAttrRuntHighThreshold_WriteCallback,
                                        VI_NULL, 0));
   _checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_RUNT_HIGH_THRESHOLD, 
                                                itscopeAttrRuntHighThreshold_CheckCallback));
    
        /*- ITSCOPE_ATTR_RUNT_LOW_THRESHOLD -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_RUNT_LOW_THRESHOLD,
                                        "ITSCOPE_ATTR_RUNT_LOW_THRESHOLD", 0,
                                        IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                        itscopeAttrRuntLowThreshold_ReadCallback,
                                        itscopeAttrRuntLowThreshold_WriteCallback,
                                        VI_NULL, 0));
   _checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_RUNT_LOW_THRESHOLD, 
                                                itscopeAttrRuntLowThreshold_CheckCallback));
    
        /*- ITSCOPE_ATTR_RUNT_POLARITY -*/
   _checkErr( Ivi_AddAttributeViInt32  (vi, ITSCOPE_ATTR_RUNT_POLARITY, 
                                        "ITSCOPE_ATTR_RUNT_POLARITY", 
                                        ITSCOPE_VAL_RUNT_POSITIVE, 0, 
                                        itscopeStdViInt32_ReadCallback, 
                                        itscopeStdViInt32_WriteCallback, 
                                        &attrRuntPolarityRangeTable));
    
        /*- ITSCOPE_ATTR_RUNT_WHEN -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_RUNT_WHEN,
                                       "ITSCOPE_ATTR_RUNT_WHEN",
                                       ITSCOPE_VAL_RUNT_WHEN_OCCURS, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrRuntWhenRangeTable));
    
        /*- ITSCOPE_ATTR_RUNT_WIDTH -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_RUNT_WIDTH,
                                        "ITSCOPE_ATTR_RUNT_WIDTH", 4.0e-9, 0,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrRuntWidthRangeTable, 0));
   _checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_RUNT_WIDTH, 
                                                 itscopeAttrRuntWidth_CoerceCallback));

        /*- ITSCOPE_ATTR_GLITCH_WIDTH -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_GLITCH_WIDTH,
                                        "ITSCOPE_ATTR_GLITCH_WIDTH", 4.0e-9, 0,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrGlitchWidthRangeTable, 0));
   _checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_GLITCH_WIDTH,
                                                 itscopeAttrGlitchWidth_CoerceCallback));

        /*- ITSCOPE_ATTR_GLITCH_POLARITY -*/
   _checkErr( Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_GLITCH_POLARITY, 
                                       "ITSCOPE_ATTR_GLITCH_POLARITY", 
                                       ITSCOPE_VAL_GLITCH_POSITIVE, 0, 
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback, 
                                       &attrGlitchPolarityRangeTable));
                                       
        /*- ITSCOPE_ATTR_GLITCH_CONDITION -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_GLITCH_CONDITION,
                                       "ITSCOPE_ATTR_GLITCH_CONDITION",
                                       ITSCOPE_VAL_GLITCH_LESS_THAN, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrGlitchConditionRangeTable));
    
        /*- ITSCOPE_ATTR_OPC_TIMEOUT -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_OPC_TIMEOUT,
                                       "ITSCOPE_ATTR_OPC_TIMEOUT", 50000,
                                       IVI_VAL_DONT_CHECK_STATUS | IVI_VAL_HIDDEN,
                                       VI_NULL, VI_NULL, VI_NULL));

       /*- ITSCOPE_ATTR_ID_QUERY_RESPONSE -*/
   _checkErr (Ivi_AddAttributeViString (vi, ITSCOPE_ATTR_ID_QUERY_RESPONSE,
                "ITSCOPE_ATTR_ID_QUERY_RESPONSE",
                idnString, IVI_VAL_NOT_USER_WRITABLE,
                itscopeAttrIdQueryResponse_ReadCallback, VI_NULL));

        /*- ITSCOPE_ATTR_DELAY_TRIGGER_MODE -*/
   _checkErr( Ivi_AddAttributeViInt32  (vi, ITSCOPE_ATTR_DELAY_TRIGGER_MODE, 
                                        "ITSCOPE_ATTR_DELAY_TRIGGER_MODE", 
                                        ITSCOPE_VAL_TRIG_AFTER_TIME, 0, 
                                        itscopeStdViInt32_ReadCallback, 
                                        itscopeStdViInt32_WriteCallback, 
                                        &attrDelayTriggerModeRangeTable));
   _checkErr( Ivi_SetAttrCheckCallbackViInt32 (vi, ITSCOPE_ATTR_DELAY_TRIGGER_MODE, 
                                               itscopeAttrDelayTriggerMode_CheckCallback));
                                        
        /*- ITSCOPE_ATTR_DELAY_TRIGGER_EVENTS -*/
   _checkErr( Ivi_AddAttributeViInt32  (vi, ITSCOPE_ATTR_DELAY_TRIGGER_EVENTS, 
                                        "ITSCOPE_ATTR_DELAY_TRIGGER_EVENTS", 
                                        2, 0, 
                                        itscopeStdViInt32_ReadCallback, 
                                        itscopeStdViInt32_WriteCallback, 
                                        &attrDelayTriggerEventsRangeTable));
    
        /*- ITSCOPE_ATTR_DELAY_TRIGGER_SOURCE -*/
   _checkErr (Ivi_AddAttributeViString (vi, ITSCOPE_ATTR_DELAY_TRIGGER_SOURCE,
                                        "ITSCOPE_ATTR_DELAY_TRIGGER_SOURCE",
                                        ITSCOPE_VAL_CH_1, 0,
                                        itscopeAttrDelayTriggerSource_ReadCallback,
                                        itscopeAttrDelayTriggerSource_WriteCallback));
   _checkErr (Ivi_SetAttrCheckCallbackViString (vi, ITSCOPE_ATTR_DELAY_TRIGGER_SOURCE,
                                                itscopeAttrDelayTriggerSource_CheckCallback));

        /*- ITSCOPE_ATTR_DELAY_TRIGGER_COUPLING -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_DELAY_TRIGGER_COUPLING,
                                       "ITSCOPE_ATTR_DELAY_TRIGGER_COUPLING",
                                       ITSCOPE_VAL_DC, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrPEK1DelayTriggerCouplingRangeTable));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_DELAY_TRIGGER_COUPLING,
                                             itscopeAttrDelayTriggerCoupling_RangeTableCallback));
    
        /*- ITSCOPE_ATTR_DELAY_TRIGGER_SLOPE -*/
   _checkErr( Ivi_AddAttributeViInt32  (vi, ITSCOPE_ATTR_DELAY_TRIGGER_SLOPE, 
                                        "ITSCOPE_ATTR_DELAY_TRIGGER_SLOPE", 
                                        ITSCOPE_VAL_POSITIVE, 0, 
                                        itscopeStdViInt32_ReadCallback, 
                                        itscopeStdViInt32_WriteCallback, 
                                        &attrDelayTriggerSlopeRangeTable));
    
        /*- ITSCOPE_ATTR_DELAY_TRIGGER_LEVEL -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_DELAY_TRIGGER_LEVEL,
                                        "ITSCOPE_ATTR_DELAY_TRIGGER_LEVEL", 0.0,
                                        IVI_VAL_NEVER_CACHE,
                                        itscopeAttrDelayTriggerLevel_ReadCallback,
                                        itscopeAttrDelayTriggerLevel_WriteCallback,
                                        VI_NULL, 0));
   _checkErr( Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_DELAY_TRIGGER_LEVEL, 
                                                itscopeAttrDelayTriggerLevel_CheckCallback));
    
        /*- ITSCOPE_ATTR_DELAY_TRIGGER_TIME -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_DELAY_TRIGGER_TIME,
                                        "ITSCOPE_ATTR_DELAY_TRIGGER_TIME", 4e-9, 0,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrDelayTriggerTimeRangeTable, 0));
   _checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_DELAY_TRIGGER_TIME,
                                                 itscopeAttrDelayTriggerTime_CoerceCallback));
        
        /*- ITSCOPE_ATTR_TRANSITION_WHEN -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TRANSITION_WHEN,
                                       "ITSCOPE_ATTR_TRANSITION_WHEN",
                                       ITSCOPE_VAL_TRANSITION_SLOWER_THAN, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrTransitionWhenRangeTable));

        /*- ITSCOPE_ATTR_TRANSITION_POLARITY -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_TRANSITION_POLARITY,
                                       "ITSCOPE_ATTR_TRANSITION_POLARITY",
                                       ITSCOPE_VAL_TRANSITION_POSITIVE, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrTransitionPolarityRangeTable));
    
        /*- ITSCOPE_ATTR_TRANSITION_DELTA_TIME -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRANSITION_DELTA_TIME,
                                        "ITSCOPE_ATTR_TRANSITION_DELTA_TIME", 4.0e-9,
                                        IVI_VAL_NEVER_CACHE,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrTransitionDeltaTimeRangeTable, 0));
   _checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_TRANSITION_DELTA_TIME,
                                                 itscopeAttrTransitionDeltaTime_CoerceCallback));

        /*- ITSCOPE_ATTR_TRANSITION_HIGH_THRESHOLD -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRANSITION_HIGH_THRESHOLD,
                                        "ITSCOPE_ATTR_TRANSITION_HIGH_THRESHOLD",
                                        0.8, IVI_VAL_NEVER_CACHE,
                                        itscopeAttrTransitionHighThreshold_ReadCallback,
                                        itscopeAttrTransitionHighThreshold_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_TRANSITION_HIGH_THRESHOLD,
                                                itscopeAttrTransitionHighThreshold_CheckCallback));
    
        /*- ITSCOPE_ATTR_TRANSITION_LOW_THRESHOLD -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_TRANSITION_LOW_THRESHOLD,
                                        "ITSCOPE_ATTR_TRANSITION_LOW_THRESHOLD", 0,
                                        IVI_VAL_NEVER_CACHE,
                                        itscopeAttrTransitionLowThreshold_ReadCallback,
                                        itscopeAttrTransitionLowThreshold_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_TRANSITION_LOW_THRESHOLD,
                                                itscopeAttrTransitionLowThreshold_CheckCallback));

        /*- ITSCOPE_ATTR_LOGIC_FUNCTION -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_LOGIC_FUNCTION,
                                       "ITSCOPE_ATTR_LOGIC_FUNCTION",
                                       ITSCOPE_VAL_LOGIC_AND, IVI_VAL_HIDDEN,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrLogicFunctionRangeTable));
                                       
        /*- ITSCOPE_ATTR_LOGIC_WHEN -*/                                        
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_LOGIC_WHEN,
                                       "ITSCOPE_ATTR_LOGIC_WHEN",
                                       ITSCOPE_VAL_LOGIC_TRUE, IVI_VAL_HIDDEN,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrLogicWhenRangeTable));
                                       
        /*- ITSCOPE_ATTR_LOGIC_INPUT_BY_CH -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_LOGIC_INPUT_BY_CH,
                                       "ITSCOPE_ATTR_LOGIC_INPUT_BY_CH",
                                       ITSCOPE_VAL_LOGIC_X,
                                       IVI_VAL_MULTI_CHANNEL | IVI_VAL_HIDDEN,
                                       itscopeAttrLogicInputByCh_ReadCallback,
                                       itscopeAttrLogicInputByCh_WriteCallback,
                                       &attrLogicInputByChanRangeTable));
 
        /*- ITSCOPE_ATTR_LOGIC_THRES_BY_CH -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_LOGIC_THRES_BY_CH,
                                        "ITSCOPE_ATTR_LOGIC_THRES_BY_CH", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        itscopeAttrLogicThresByCh_ReadCallback,
                                        itscopeAttrLogicThresByCh_WriteCallback,
                                        VI_NULL, 0));
   _checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ITSCOPE_ATTR_LOGIC_THRES_BY_CH,
                                                itscopeAttrLogicThresByCh_CheckCallback));
        
        /*- ITSCOPE_ATTR_PATTERN_FUNCTION -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_PATTERN_FUNCTION,
                                       "ITSCOPE_ATTR_PATTERN_FUNCTION",
                                       ITSCOPE_VAL_LOGIC_AND, IVI_VAL_NEVER_CACHE,
                                       itscopeAttrPatternFunction_ReadCallback,
                                       itscopeAttrPatternFunction_WriteCallback,
                                       VI_NULL));
    
        /*- ITSCOPE_ATTR_PATTERN_WHEN -*/
	_checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_PATTERN_WHEN,
	                                   "ITSCOPE_ATTR_PATTERN_WHEN",
	                                   ITSCOPE_VAL_LOGIC_TRUE, IVI_VAL_NEVER_CACHE,
	                                   itscopeAttrPatternWhen_ReadCallback,
	                                   itscopeAttrPatternWhen_WriteCallback,
	                                   &attrLogicWhenRangeTable));
                                       
        /*- ITSCOPE_ATTR_PATTERN_INPUT -*/    
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_PATTERN_INPUT,
                                       "ITSCOPE_ATTR_PATTERN_INPUT",
                                       ITSCOPE_VAL_LOGIC_X,
                                       IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE,
                                       itscopeAttrPatternInput_ReadCallback,
                                       itscopeAttrPatternInput_WriteCallback,
                                       VI_NULL));

        /*- ITSCOPE_ATTR_PATTERN_DELTATIME -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_PATTERN_DELTATIME,
                                        "ITSCOPE_ATTR_PATTERN_DELTATIME", 4.0e-9, 0,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrPatternDeltatimeRangeTable, 0));
   _checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_PATTERN_DELTATIME,
                                                 itscopeAttrPatternDeltatime_CoerceCallback));

        /*- ITSCOPE_ATTR_STATE_FUNCTION -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_STATE_FUNCTION,
                                       "ITSCOPE_ATTR_STATE_FUNCTION",
                                       ITSCOPE_VAL_LOGIC_AND, IVI_VAL_NEVER_CACHE,
                                       itscopeAttrStateFunction_ReadCallback,
                                       itscopeAttrStateFunction_WriteCallback,
                                       VI_NULL));
        
        /*- ITSCOPE_ATTR_STATE_WHEN -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_STATE_WHEN,
                                       "ITSCOPE_ATTR_STATE_WHEN",
                                       ITSCOPE_VAL_LOGIC_TRUE, IVI_VAL_NEVER_CACHE,
                                       itscopeAttrStateWhen_ReadCallback,
                                       itscopeAttrStateWhen_WriteCallback, VI_NULL));

        /*- ITSCOPE_ATTR_STATE_INPUT -*/                                       
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_STATE_INPUT,
                                       "ITSCOPE_ATTR_STATE_INPUT",
                                       ITSCOPE_VAL_LOGIC_X,
                                       IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE,
                                       itscopeAttrStateInput_ReadCallback,
                                       itscopeAttrStateInput_WriteCallback, VI_NULL));
                                       
        /*- ITSCOPE_ATTR_STATE_CLOCK_EDGE -*/                                      
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_STATE_CLOCK_EDGE,
                                       "ITSCOPE_ATTR_STATE_CLOCK_EDGE", 
                                       ITSCOPE_VAL_CLOCK_RISE, 0,
                                       itscopeStdViInt32_ReadCallback, 
                                       itscopeStdViInt32_WriteCallback, 
                                       &attrStateClockEdgeRangeTable));

        /*- ITSCOPE_ATTR_STATE_CLOCK_SOURCE -*/                                       
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_STATE_CLOCK_SOURCE,
                                       "ITSCOPE_ATTR_STATE_CLOCK_SOURCE",
                                       ITSCOPE_VAL_SOUR_NONE, 0,
                                       itscopeAttrStateClockSource_ReadCallback,
                                       itscopeAttrStateClockSource_WriteCallback,
                                       VI_NULL));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_STATE_CLOCK_SOURCE,
                                             itscopeAttrStateClockSource_RangeTableCallback));

        /*- ITSCOPE_ATTR_SETHOLD_CLOCK_EDGE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_SETHOLD_CLOCK_EDGE,
                                       "ITSCOPE_ATTR_SETHOLD_CLOCK_EDGE",
                                       ITSCOPE_VAL_CLOCK_RISE, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrSetholdClockEdgeRangeTable));

        /*- ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE -*/                                      
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE,
                                       "ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE",
                                       ITSCOPE_VAL_SOUR_CH1, 0,
                                       itscopeAttrSetholdClockSource_ReadCallback,
                                       itscopeAttrSetholdClockSource_WriteCallback,
                                       VI_NULL));
   _checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi, ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE,
                                               itscopeAttrSetholdClockSource_CheckCallback));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_SETHOLD_CLOCK_SOURCE,
                                             itscopeAttrSetholdClockSource_RangeTableCallback));
                                       
        /*- ITSCOPE_ATTR_SETHOLD_DATA_SOURCE -*/                                       
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_SETHOLD_DATA_SOURCE,
                                       "ITSCOPE_ATTR_SETHOLD_DATA_SOURCE",
                                       ITSCOPE_VAL_SOUR_CH2, 0,
                                       itscopeAttrSetholdDataSource_ReadCallback,
                                       itscopeAttrSetholdDataSource_WriteCallback,
                                       &attrDPO4KSetholdDataSourceRangeTable));
   _checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi, ITSCOPE_ATTR_SETHOLD_DATA_SOURCE,
                                               itscopeAttrSetholdDataSource_CheckCallback));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_SETHOLD_DATA_SOURCE,
                                             itscopeAttrSetholdDataSource_RangeTableCallback));
                                       
        /*- ITSCOPE_ATTR_SETHOLD_HOLDTIME -*/                                      
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_SETHOLD_HOLDTIME,
                                        "ITSCOPE_ATTR_SETHOLD_HOLDTIME", 4.0e-9, 0,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrPEK1SetholdHoldtimeRangeTable, 0));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_SETHOLD_HOLDTIME,
                                             itscopeAttrSetholdHoldtime_RangeTableCallback));
   _checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_SETHOLD_HOLDTIME,
                                                 itscopeAttrSetholdHoldtime_CoerceCallback));                                       
                                        
        /*- ITSCOPE_ATTR_SETHOLD_SETTIME -*/                                        
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_SETHOLD_SETTIME,
                                        "ITSCOPE_ATTR_SETHOLD_SETTIME", 4.0e-9, 0,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrPEK1SetholdSettimeRangeTable, 0));
   _checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_SETHOLD_SETTIME,
                                             itscopeAttrSetholdSettime_RangeTableCallback));
   _checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_SETHOLD_SETTIME,
                                                 itscopeAttrSetholdSettime_CoerceCallback));

        
        /*- ITSCOPE_ATTR_MEAS_SOURCE -*/
   _checkErr (Ivi_AddAttributeViString (vi, ITSCOPE_ATTR_MEAS_SOURCE,
                                        "ITSCOPE_ATTR_MEAS_SOURCE", ITSCOPE_VAL_CH_1,
                                        0, itscopeStdViStringSource_ReadCallback,
                                        itscopeStdViStringSource_WriteCallback));
   _checkErr( Ivi_SetStoredRangeTablePtr (vi, ITSCOPE_ATTR_MEAS_SOURCE, 
                                          &attrMeasSourceRangeTable));
   _checkErr( Ivi_SetAttrCheckCallbackViString (vi, ITSCOPE_ATTR_MEAS_SOURCE, 
                                                itscopeStdViStringSource_CheckCallback));
    
        /*- ITSCOPE_ATTR_MEAS_SOURCE2 -*/
   _checkErr (Ivi_AddAttributeViString (vi, ITSCOPE_ATTR_MEAS_SOURCE2,
                                        "ITSCOPE_ATTR_MEAS_SOURCE2",
                                        ITSCOPE_VAL_CH_1, 0,
                                        itscopeStdViStringSource_ReadCallback,
                                        itscopeStdViStringSource_WriteCallback));
   _checkErr( Ivi_SetStoredRangeTablePtr (vi, ITSCOPE_ATTR_MEAS_SOURCE2, 
                                          &attrMeasSource2RangeTable));
   _checkErr (Ivi_SetAttrCheckCallbackViString (vi, ITSCOPE_ATTR_MEAS_SOURCE2,
                                                itscopeStdViStringSource_CheckCallback));

        /*- ITSCOPE_ATTR_MEAS_FUNCTION -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MEAS_FUNCTION,
                                       "ITSCOPE_ATTR_MEAS_FUNCTION",
                                       ITSCOPE_VAL_PERIOD, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       attrMeasFunctionRangeTable));
    
        /*- ITSCOPE_ATTR_MEAS_DEL_DIRECTION -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MEAS_DEL_DIRECTION,
                                       "ITSCOPE_ATTR_MEAS_DEL_DIRECTION",
                                       ITSCOPE_VAL_DEL_DIR_BACKWARD, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrMeasDelDirectionRangeTable));
    
        /*- ITSCOPE_ATTR_MEAS_DEL_EDGE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MEAS_DEL_EDGE,
                                       "ITSCOPE_ATTR_MEAS_DEL_EDGE",
                                       ITSCOPE_VAL_DEL_EDGE_RISE, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrMeasDelEdgeRangeTable));
    
        /*- ITSCOPE_ATTR_MEAS_DEL_EDGE2 -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MEAS_DEL_EDGE2,
                                       "ITSCOPE_ATTR_MEAS_DEL_EDGE2",
                                       ITSCOPE_VAL_DEL_EDGE_RISE, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrMeasDelEdge2RangeTable));

        /*- ITSCOPE_ATTR_MEAS_REF_CALC_METH -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MEAS_REF_CALC_METH, 
                                       "ITSCOPE_ATTR_MEAS_REF_CALC_METH", 
                                       ITSCOPE_VAL_MEAS_AUTO, 0, 
                                       itscopeStdViInt32_ReadCallback, 
                                       itscopeStdViInt32_WriteCallback, 
                                       &attrMeasRefCalcMethRangeTable));
    
        /*- ITSCOPE_ATTR_MEAS_HIGH_REF -*/
   _checkErr( Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_MEAS_HIGH_REF, 
                                        "ITSCOPE_ATTR_MEAS_HIGH_REF", 
                                        90.0, 0, 
                                        itscopeStdViReal64_ReadCallback, 
                                        itscopeStdViReal64_WriteCallback, 
                                        &attrMeasHighRefRangeTable, 0));
   _checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_MEAS_HIGH_REF, 
                                                 itscopeResolutionOne_CoerceCallback));

        /*- ITSCOPE_ATTR_MEAS_LOW_REF -*/
   _checkErr( Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_MEAS_LOW_REF, 
                                        "ITSCOPE_ATTR_MEAS_LOW_REF", 
                                        10.0, 0, 
                                        itscopeStdViReal64_ReadCallback, 
                                        itscopeStdViReal64_WriteCallback, 
                                        &attrMeasLowRefRangeTable, 0));
   _checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_MEAS_LOW_REF, 
                                                 itscopeResolutionOne_CoerceCallback));

        /*- ITSCOPE_ATTR_MEAS_MID_REF -*/
   _checkErr( Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_MEAS_MID_REF, 
                                        "ITSCOPE_ATTR_MEAS_MID_REF", 
                                        50.0, 0, 
                                        itscopeStdViReal64_ReadCallback, 
                                        itscopeStdViReal64_WriteCallback, 
                                        &attrMeasMidRefRangeTable, 0));
   _checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_MEAS_MID_REF, 
                                                 itscopeResolutionOne_CoerceCallback));

        /*- ITSCOPE_ATTR_MEAS_MID2_REF -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_MEAS_MID2_REF,
                                        "ITSCOPE_ATTR_MEAS_MID2_REF", 50.0, 0,
                                        itscopeStdViReal64_ReadCallback,
                                        itscopeStdViReal64_WriteCallback,
                                        &attrMeasMid2RefRangeTable, 0));
   _checkErr( Ivi_SetAttrCoerceCallbackViReal64 (vi, ITSCOPE_ATTR_MEAS_MID2_REF, 
                                                 itscopeResolutionOne_CoerceCallback));
   
        /*- ITSCOPE_ATTR_MEAS_VALUE -*/
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_MEAS_VALUE,
                                        "ITSCOPE_ATTR_MEAS_VALUE", 0.0,
                                        IVI_VAL_NEVER_CACHE | IVI_VAL_HIDDEN | 
                                        IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | 
                                        IVI_VAL_MULTI_CHANNEL,
                                        itscopeAttrMeasureValue_ReadCallback,
                                        VI_NULL, VI_NULL, 0));

        /*- ITSCOPE_ATTR_MEAS_GATING -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MEAS_GATING,
                                       "ITSCOPE_ATTR_MEAS_GATING", 
                                       ITSCOPE_VAL_MEAS_GATING_SCREEN, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrMeasGatingRangeTable));

        /*- ITSCOPE_ATTR_INPUT_DATA_SOURCE -*/
   _checkErr (Ivi_AddAttributeViString (vi, ITSCOPE_ATTR_INPUT_DATA_SOURCE,
                                        "ITSCOPE_ATTR_INPUT_DATA_SOURCE",
                                        ITSCOPE_VAL_CH_1, IVI_VAL_HIDDEN,
                                        itscopeStdViStringSource_ReadCallback,
                                        itscopeStdViStringSource_WriteCallback));
   _checkErr( Ivi_SetStoredRangeTablePtr (vi, ITSCOPE_ATTR_INPUT_DATA_SOURCE, 
                                          &attrInputDataSourceRangeTable));
   _checkErr( Ivi_SetAttrCheckCallbackViString (vi, ITSCOPE_ATTR_INPUT_DATA_SOURCE, 
                                                itscopeStdViStringSource_CheckCallback));
    
        /*- ITSCOPE_ATTR_MAIN_TRIGGER_TYPE -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MAIN_TRIGGER_TYPE,
                                       "ITSCOPE_ATTR_MAIN_TRIGGER_TYPE",
                                       ITSCOPE_VAL_EDGE_TRIGGER,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_HIDDEN,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrMainTriggerTypeRangeTable));
                                       
        /*- ITSCOPE_ATTR_EDGE_TRIGGER_CLASS -*/                                       
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_EDGE_TRIGGER_CLASS,
                                       "ITSCOPE_ATTR_EDGE_TRIGGER_CLASS",
                                       ITSCOPE_VAL_EDGE_TRIGGER,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_HIDDEN,
                                       itscopeAttrEdgeTriggerClass_ReadCallback,
                                       itscopeAttrEdgeTriggerClass_WriteCallback,
                                       &attrEdgeTriggerClassRangeTable));

        /*- ITSCOPE_ATTR_PULSE_TRIGGER_CLASS -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_PULSE_TRIGGER_CLASS,
                                       "ITSCOPE_ATTR_PULSE_TRIGGER_CLASS",
                                       ITSCOPE_VAL_RUNT_TRIGGER,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_HIDDEN,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrPulseTriggerClassRangeTable));
                                       
        /*- ITSCOPE_ATTR_LOGIC_TRIGGER_CLASS -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_LOGIC_TRIGGER_CLASS,
                                       "ITSCOPE_ATTR_LOGIC_TRIGGER_CLASS",
                                       ITSCOPE_VAL_PATTERN_TRIGGER,
                                       IVI_VAL_NEVER_CACHE | IVI_VAL_HIDDEN,
                                       itscopeAttrLogicTriggerClass_ReadCallback,
                                       itscopeAttrLogicTriggerClass_WriteCallback,
                                       &attrLogicTriggerClassRangeTable));

        /*- ITSCOPE_ATTR_MATH_TYPE -*/  
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MATH_TYPE,
                                       "ITSCOPE_ATTR_MATH_TYPE",
                                       ITSCOPE_VAL_MATH_TYPE_DUAL, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrMathTypeRangeTable));
                                       
        /*- ITSCOPE_ATTR_MATH_FUNCTION -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MATH_FUNCTION,
                                       "ITSCOPE_ATTR_MATH_FUNCTION",
                                       ITSCOPE_VAL_MATH_ADD, 0,
                                       itscopeAttrMathFunction_ReadCallback,
                                       itscopeAttrMathFunction_WriteCallback,
                                       &attrMathFunctionRangeTable));
                                       
        /*- ITSCOPE_ATTR_MATH_SOURCE_1ST -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MATH_SOURCE_1ST,
                                       "ITSCOPE_ATTR_MATH_SOURCE_1ST",
                                       ITSCOPE_VAL_SOUR_CH1,
                                       IVI_VAL_NOT_USER_READABLE, VI_NULL, VI_NULL,
                                       &attrMathSourceRangeTable));
        
        /*- ITSCOPE_ATTR_MATH_SOURCE_2ND -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MATH_SOURCE_2ND,
                                       "ITSCOPE_ATTR_MATH_SOURCE_2ND",
                                       ITSCOPE_VAL_SOUR_CH2,
                                       IVI_VAL_NOT_USER_READABLE, VI_NULL, VI_NULL,
                                       &attrMathSourceRangeTable));
 
        /*- ITSCOPE_ATTR_FFT_VERT_SCALE -*/     
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_FFT_VERT_SCALE,
                                       "ITSCOPE_ATTR_FFT_VERT_SCALE",
                                       ITSCOPE_VAL_MATH_DBV_RMS, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrFFTVertScaleRangeTable));
                                       
        /*- ITSCOPE_ATTR_FFT_WINDOW -*/                                       
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_FFT_WINDOW,
                                       "ITSCOPE_ATTR_FFT_WINDOW",
                                       ITSCOPE_VAL_MATH_HANNING, 0,
                                       itscopeStdViInt32_ReadCallback,
                                       itscopeStdViInt32_WriteCallback,
                                       &attrFFTWindowRangeTable));

        /*- ITSCOPE_ATTR_ADV_DEFINE -*/
   _checkErr (Ivi_AddAttributeViString (vi, ITSCOPE_ATTR_ADV_DEFINE,
                                        "ITSCOPE_ATTR_ADV_DEFINE",
                                        ITSCOPE_VAL_ADV_DEFINE, 0,
                                        itscopeAttrAdvDefine_ReadCallback,
                                        itscopeAttrAdvDefine_WriteCallback));

        /*- ITSCOPE_ATTR_ADV_VAR1 -*/
	_checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_ADV_VAR1,
	                                    "ITSCOPE_ATTR_ADV_VAR1", 0, 0,
	                                    itscopeStdViReal64_ReadCallback,
	                                    itscopeStdViReal64_WriteCallback,
	                                    &attrAdvVar1RangeTable, 0));
	_checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_ADV_VAR1,
	                                         itscopeAttrAdvVar1_RangeTableCallback));

        /*- ITSCOPE_ATTR_ADV_VAR2 -*/                                           
	_checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_ADV_VAR2,
	                                    "ITSCOPE_ATTR_ADV_VAR2", 0, 0,
	                                    itscopeStdViReal64_ReadCallback,
	                                    itscopeStdViReal64_WriteCallback,
	                                    &attrAdvVar2RangeTable, 0));
	_checkErr (Ivi_SetAttrRangeTableCallback (vi, ITSCOPE_ATTR_ADV_VAR2,
	                                         itscopeAttrAdvVar2_RangeTableCallback));
        

    
    
    
        /*- ITSCOPE_ATTR_MODEL -*/
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_MODEL, "ITSCOPE_ATTR_MODEL",
                                       instrModelNo,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, &attrModelRangeTable));
                
        /*- ITSCOPE_ATTR_DATA_WIDTH -*/                
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_DATA_WIDTH,
                                       "ITSCOPE_ATTR_DATA_WIDTH", 1, IVI_VAL_HIDDEN,
                                       itscopeAttrDataWidth_ReadCallback,
                                       itscopeAttrDataWidth_WriteCallback, VI_NULL));
                                       
        /*- ITSCOPE_ATTR_DATA_START_POS -*/                                    
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_DATA_START_POS,
                                       "ITSCOPE_ATTR_DATA_START_POS", 0,
                                       IVI_VAL_HIDDEN,
                                       itscopeAttrDataStartPos_ReadCallback,
                                       itscopeAttrDataStartPos_WriteCallback,
                                       VI_NULL));
                                       
        /*- ITSCOPE_ATTR_DATA_STOP_POS -*/  
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_DATA_STOP_POS,
                                       "ITSCOPE_ATTR_DATA_STOP_POS", 8192,
                                       IVI_VAL_HIDDEN,
                                       itscopeAttrDataStopPos_ReadCallback,
                                       itscopeAttrDataStopPos_WriteCallback, VI_NULL));
                                       
        /*- ITSCOPE_ATTR_FETCH_BUFFER_SIZE -*/                                     
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_FETCH_BUFFER_SIZE,
                                       "ITSCOPE_ATTR_FETCH_BUFFER_SIZE", 16384, 0,
                                       VI_NULL, VI_NULL,
                                       &attrFetchBufferSizeRangeTable));
                                       
        /*- ITSCOPE_ATTR_DATA_YZERO -*/                                    
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_DATA_YZERO,
                                        "ITSCOPE_ATTR_DATA_YZERO", 0,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_HIDDEN,
                                        itscopeAttrDataYZero_ReadCallback, VI_NULL,
                                        VI_NULL, 0));
                                        
        /*- ITSCOPE_ATTR_DATA_YOFF -*/                                     
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_DATA_YOFF,
                                        "ITSCOPE_ATTR_DATA_YOFF", 0,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_HIDDEN,
                                        itscopeAttrDataYoff_ReadCallback, VI_NULL,
                                        VI_NULL, 0));
                                        
        /*- ITSCOPE_ATTR_DATA_YMULT -*/                                    
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_DATA_YMULT,
                                        "ITSCOPE_ATTR_DATA_YMULT", 1.0,
                                        IVI_VAL_MULTI_CHANNEL | IVI_VAL_HIDDEN,
                                        itscopeAttrDataYmult_ReadCallback, VI_NULL,
                                        VI_NULL, 0));
                                        
        /*- ITSCOPE_ATTR_DATA_XINCR -*/                                     
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_DATA_XINCR,
                                        "ITSCOPE_ATTR_DATA_XINCR", 0, IVI_VAL_HIDDEN,
                                        itscopeAttrDataXincr_ReadCallback, VI_NULL,
                                        VI_NULL, 0));
                                        
        /*- ITSCOPE_ATTR_DATA_XZERO -*/                                     
   _checkErr (Ivi_AddAttributeViReal64 (vi, ITSCOPE_ATTR_DATA_XZERO,
                                        "ITSCOPE_ATTR_DATA_XZERO", 0,
                                        IVI_VAL_HIDDEN,
                                        itscopeAttrDataXzero_ReadCallback, VI_NULL,
                                        VI_NULL, 0));
                                        
        /*- ITSCOPE_ATTR_WAVEFORM_SIZE -*/                                      
   _checkErr (Ivi_AddAttributeViInt32 (vi, ITSCOPE_ATTR_WAVEFORM_SIZE,
                                       "ITSCOPE_ATTR_WAVEFORM_SIZE", 0,
                                       IVI_VAL_NOT_USER_WRITABLE, VI_NULL, VI_NULL,
                                       VI_NULL));
   
   
        /*- Setup attribute invalidations -----------------------------------*/
        /*- Acquisition Sub-system ----------------------------------*/
        /*- ITSCOPE_ATTR_INITIATE_CONTINUOUS Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_INITIATE_CONTINUOUS,
                                            ITSCOPE_ATTR_TRIGGER_MODIFIER, VI_TRUE));        
        
        /*- ITSCOPE_ATTR_ACQUISITION_TYPE Invalidations -*/
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_ACQUISITION_TYPE, 
                                            ITSCOPE_ATTR_SAMPLE_MODE, VI_TRUE));
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_ACQUISITION_TYPE,
                                            ITSCOPE_ATTR_DATA_XINCR, VI_TRUE));
                                            
        /*- ITSCOPE_ATTR_HORZ_MIN_NUM_PTS Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS,
                                            ITSCOPE_ATTR_ACQUISITION_TYPE, VI_TRUE));
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS, 
                                            ITSCOPE_ATTR_HORZ_SAMPLE_RATE, VI_TRUE));
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS, 
                                            ITSCOPE_ATTR_HORZ_RECORD_LENGTH, VI_TRUE));
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_MIN_NUM_PTS,
                                            ITSCOPE_ATTR_DATA_XINCR, VI_TRUE));
                                            
        /*- ITSCOPE_ATTR_HORZ_RECORD_LENGTH Invalidations -*/
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_RECORD_LENGTH, 
                                            ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                            VI_TRUE));
    
        /*- ITSCOPE_ATTR_HORZ_TIME_PER_RECORD Invalidations -*/
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                            ITSCOPE_ATTR_HORZ_SAMPLE_RATE, VI_TRUE));
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD, 
                                            ITSCOPE_ATTR_ACQUISITION_START_TIME, VI_TRUE));
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_HORZ_TIME_PER_RECORD,
                                            ITSCOPE_ATTR_DATA_XINCR, VI_TRUE));

        /*- ITSCOPE_ATTR_SAMPLE_MODE Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_SAMPLE_MODE,
                                            ITSCOPE_ATTR_DATA_XINCR, VI_TRUE));                                         
                                            
        /*- Channel Sub-system --------------------------------------*/
        /*- ITSCOPE_ATTR_PROBE_ATTENUATION Invalidations -*/
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_PROBE_ATTENUATION, 
                                            ITSCOPE_ATTR_VERTICAL_RANGE, VI_FALSE));
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_PROBE_ATTENUATION, 
                                            ITSCOPE_ATTR_VERTICAL_OFFSET, VI_FALSE));
                                            
        /*- ITSCOPE_ATTR_VERTICAL_RANGE Invalidations -*/
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_VERTICAL_RANGE, 
                                            ITSCOPE_ATTR_VERTICAL_OFFSET, VI_FALSE));        
                                            
        /*- ITSCOPE_ATTR_TRIGGER_TYPE Invalidations -*/
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_TRIGGER_TYPE, 
                                            ITSCOPE_ATTR_TRIGGER_SOURCE, VI_TRUE));
                                            
        /*- ITSCOPE_ATTR_TRIGGER_SLOPE Invalidations -*/
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_TRIGGER_SLOPE, 
                                            ITSCOPE_ATTR_AC_LINE_TRIGGER_SLOPE, VI_TRUE));

        /*- ITSCOPE_ATTR_TRIGGER_LEVEL Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_TRIGGER_LEVEL,
                                            ITSCOPE_ATTR_TRIGGER_LEVEL_BY_CH, VI_TRUE));    

        /*- ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_TV_TRIGGER_SIGNAL_FORMAT,
                                            ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER, VI_TRUE));                                            
    
        /*- ITSCOPE_ATTR_TV_TRIGGER_EVENT Invalidations -*/                                            
   _checkErr( Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_TV_TRIGGER_EVENT, 
                                            ITSCOPE_ATTR_TV_TRIGGER_LINE_NUMBER, VI_TRUE));

        /*- ITSCOPE_ATTR_RUNT_WIDTH Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_RUNT_WIDTH,
                                            ITSCOPE_ATTR_GLITCH_WIDTH, VI_TRUE));
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_RUNT_WIDTH,
                                            ITSCOPE_ATTR_TRANSITION_DELTA_TIME, VI_TRUE));   
   
        /*- ITSCOPE_ATTR_GLITCH_WIDTH Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_GLITCH_WIDTH,
                                            ITSCOPE_ATTR_RUNT_WIDTH, VI_TRUE));
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_GLITCH_WIDTH,
                                            ITSCOPE_ATTR_TRANSITION_DELTA_TIME, VI_TRUE));
                                            
        /*- ITSCOPE_ATTR_TRANSITION_DELTA_TIME Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_TRANSITION_DELTA_TIME,
                                            ITSCOPE_ATTR_RUNT_WIDTH, VI_TRUE));
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_TRANSITION_DELTA_TIME, 
                                            ITSCOPE_ATTR_GLITCH_WIDTH, VI_TRUE));
        
   
        /*- ITSCOPE_ATTR_MATH_TYPE Invalidations -*/
   _checkErr (Ivi_AddAttributeInvalidation (vi, ITSCOPE_ATTR_MATH_TYPE,
                                            ITSCOPE_ATTR_MATH_FUNCTION, VI_TRUE));
        
        /*- MISCELLANEOUS Invalidations -*/


    /* Restrict for channels */
    {   /* Local scope */
    ViConstString inputChannelList = "CH1,CH2,CH3,CH4";
    ViConstString  inputChListNoRF = "CH1,CH2,CH3,CH4,MATH,REF1,REF2,REF3,REF4";
    
    if( instrModelNo == ITSCOPE_VAL_PE520K1) 
    {   /* 2 channels */
        inputChannelList = "CH1,CH2";
        inputChListNoRF = "CH1,CH2,MATH,REF1,REF2";
    }
    if (instrModelNo == ITSCOPE_VAL_PE510K1)
    {
        inputChannelList = "CH1,CH2,CH3,CH4";    
		inputChListNoRF = "CH1,CH2,CH3,CH4,MATH,REF1,REF2,REF3,REF4";
	}
    
   _checkErr( Ivi_RestrictAttrToChannels (vi, ITSCOPE_ATTR_PROBE_ATTENUATION, inputChannelList));
   _checkErr( Ivi_RestrictAttrToChannels (vi, ITSCOPE_ATTR_PROBE_SENSE_VALUE, inputChannelList));
   _checkErr( Ivi_RestrictAttrToChannels (vi, ITSCOPE_ATTR_VERTICAL_RANGE, inputChannelList));
   _checkErr( Ivi_RestrictAttrToChannels (vi, ITSCOPE_ATTR_VERTICAL_OFFSET, inputChannelList));
   _checkErr( Ivi_RestrictAttrToChannels (vi, ITSCOPE_ATTR_VERTICAL_COUPLING, inputChannelList));
   _checkErr( Ivi_RestrictAttrToChannels (vi, ITSCOPE_ATTR_MAX_INPUT_FREQUENCY, inputChannelList));
   _checkErr( Ivi_RestrictAttrToChannels (vi, ITSCOPE_ATTR_INPUT_IMPEDANCE, inputChannelList));
    
  }
    
Error:
    return error;
}

/*****************************************************************************
 *  Function: itscope_GetDriverSetupOption
 *  Purpose:  Get the value of a DriverSetup option.  The driver setup string
 *            is part of the option string the user passes to the
 *            itscope_InitWithOptions function.  Specifically it is the part
 *            that follows "DriverSetup=".  This function assumes the 
 *            DriverSetup string has the following format:
 *              "OptionTag1 : OptionValue1; OptionTag2 : OptionValue2"
 *            Leading and trailing white space is removed from the option
 *            value and both the option tag and option value may contain
 *            white space.  The option tag is case sensitive.
 *
 *            Example:
 *
 *              "DriverSetup= Model : InstrX ; Special Option:Feature Y "
 *
 *              optionTag "Model"          returns optionValue "InstrX"
 *
 *              optionTag "Special Option" returns optionValue "Feature Y"
 *****************************************************************************/
static ViStatus itscope_GetDriverSetupOption ( ViSession vi, 
											   ViConstString optionTag,
                                               ViInt32 optionValueSize,
                                               ViChar optionValue[])
{                   
    ViStatus error = VI_SUCCESS;
    ViChar   setup[BUFFER_SIZE];
    ViChar*  searchOrigin;
    ViChar*  start;
    ViChar*  end;
    size_t   n;

   _checkErr( Ivi_LockSession (vi, VI_NULL));

    if ((optionValue != VI_NULL) && (optionValueSize > 0))
        optionValue[0] = 0;

    if ((optionTag != VI_NULL) && (optionValue != VI_NULL) && (optionValueSize > 1))
    {
       _checkErr( Ivi_GetAttributeViString (vi, "", IVI_ATTR_DRIVER_SETUP,
                                            0, BUFFER_SIZE, setup));
        searchOrigin = setup;
        while (searchOrigin != VI_NULL)
        {
            searchOrigin = strstr(searchOrigin, optionTag);
            if (searchOrigin)
            {
                searchOrigin += strlen(optionTag);
                start = searchOrigin;
                while (isspace(*start)) start++;
                if (*start == ':')                          
                {
                    start++;
                    while (isspace(*start)) start++;
                    if (*start)
                    {
                        end = strchr (start, ';');
                        if (start != end)
                        {
                            end = (end) ? --end : &setup[strlen(setup)-1];
                            while (isspace(*end)) end--;
                            n = end - start + 1;
                            if ((int)n > optionValueSize - 1)
                                n = optionValueSize - 1;
                            strncpy(optionValue, start, n);
                            optionValue[n] = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
        
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/***************************************************************************** 
 *------------------- End Instrument Driver Source Code ---------------------* 
 *****************************************************************************/
