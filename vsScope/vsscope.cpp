/********************************************************************************* 
 *  Copyright 2017 Instrumental Systems Corporation.  All Rights Reserved.                   * 
 *********************************************************************************/

/*****************************************************************************
 *  tkdpo4k Instrument Driver Sample Program                              
 *
 *  Purpose:  This sample program initializes the scope, configures it for an
 *            edge triggered acquisition, acquires a waveform, and displays the 
 *            waveform in a message pop-up dialog.
 *				
 *            1) Use LabWindows/CVI runtime:
 *            Enable #define _CVI
 *					for continuous enable #define _CUNTINUOUS
 *					for x64 Windows #define _NI_mswin64_ (for x86 Windows #define _NI_mswin32_)	
 *            2) Use ISVI:
 *				Disable #define _CVI
 *				Enable #define _ISVI
 *				start ISVI data.bin
 *            3) Default - console application
 *				Disable #define _CVI
 *				Disable #define _CUNTINUOUS
 *				Disable #define _ISVI
 *
 *****************************************************************************/

#include	<ctype.h>
#include    "utypes.h"
#include	"gipcy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iviscope.h>

#include "isvi_file.h"
#define _ISVI

#define _CVI			
#define _CUNTINUOUS		
#ifdef _CVI // for LabWindows/CVI
#include <cvirte.h>
#include <userint.h>
#include "read_waveform.h"
//#define _NI_mswin32_
#define _NI_mswin64_	
#endif

#ifndef CHECK_ERROR
static char errFname[2048];
#define CHECK_ERROR(fName, fCall)    if (error = (fCall), (error = (error < 0) ? error : VI_SUCCESS)) \
                                 { 	sprintf(errFname, "#ERR - %s", fName); goto Error; }  else error = error

#endif

extern int 		ChanNums[64];			// 
extern double	Gains[64];				// 
extern double	FreqOffsets[64];		// 


// global variables
/*
#define     DEVICE_NAME	"tkdpo4k"
#define     CH1     "CH1"
#define     CH2     "CH2"
*/
//#define     DEVICE_NAME	"agx2k3k"
//#define     CH1     "CHAN1"
//#define     CH2     "CHAN2"

#define     DEVICE_NAME	"pe510k1"
#define     CH1     "CH1"
#define     CH2     "CH2"

ViSession   vi;
ViStatus    error = VI_SUCCESS;
ViReal64    initialX, incrementX;
ViInt32     actualRecordLength, actualPts;
ViReal64    *waveform = VI_NULL;
ViInt16     *short_waveform = VI_NULL;

ViInt32     lMemsize = 512;// 1024;
ViReal64	rangeY;
ViReal64	offsetY;
ViReal64	rateX;
ViInt32		sampleMode;

#ifdef _CVI
#ifdef _CUNTINUOUS

int CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_LEFT_CLICK:
		QuitUserInterface(0); /* The value passed to QuitUserInterface
							  becomes the value returned
							  by RunUserInterface. */
		break;
	}
	return 0;
}

// 0.1s plot period
int CVICALLBACK PlotWaveTime(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	error = IviScope_ReadWaveform(vi, CH1,
			actualRecordLength, 5000, waveform, &actualPts,
			&initialX, &incrementX);

	DeleteGraphPlot(PANEL, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);

	int wfmHandle = PlotWaveform(PANEL, PANEL_GRAPH, waveform, actualRecordLength, VAL_DOUBLE, 1.0, offsetY, initialX, 1.0, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_YELLOW);

	return 0;
}
#endif
#endif

//------------------------------------------------------------------------------
// Application entry-point
//------------------------------------------------------------------------------
//int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
int main ()
{
#ifdef _CVI       
	// Initialize the CVI Run-time Engine
	if (InitCVIRTE(0, 0, 0) == 0)
		return -1;
#endif

	/* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 0")
        */
	CHECK_ERROR("InitWithOptions", error = IviScope_InitWithOptions (DEVICE_NAME, VI_TRUE, VI_TRUE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1",
                                        &vi));

	CHECK_ERROR("ConfigureAcquisitionType", error = IviScope_ConfigureAcquisitionType(vi, IVISCOPE_VAL_NORMAL));

	// setup 40mks horizontal 
	CHECK_ERROR("ConfigureAcquisitionRecord", error = IviScope_ConfigureAcquisitionRecord(vi, 0.000005, lMemsize, 0.0));
	//CHECK_ERROR("ConfigureAcquisitionRecord", error = IviScope_ConfigureAcquisitionRecord(vi, 1.0e-3, lMemsize=500, 0.0));

	// setup channel 0: +/-1 V, 50 Om (1 MOhm), DC, no offset
	CHECK_ERROR("ConfigureChannel", error = IviScope_ConfigureChannel (vi, CH1, 2.0, 0.0, IVISCOPE_VAL_DC, 1.0, VI_TRUE));
	//CHECK_ERROR("ConfigureChanCharacteristics", error = IviScope_ConfigureChanCharacteristics (vi, CH1, 1000000, 20000000));
	CHECK_ERROR("ConfigureChanCharacteristics", error = IviScope_ConfigureChanCharacteristics(vi, CH1, 50, 200000000));

	CHECK_ERROR("GetAttributeViReal64 - VERTICAL_RANGE", error = IviScope_GetAttributeViReal64(vi, CH1, IVISCOPE_ATTR_VERTICAL_RANGE, &rangeY));
	CHECK_ERROR("GetAttributeViReal64 - VERTICAL_OFFSET", error = IviScope_GetAttributeViReal64(vi, CH1, IVISCOPE_ATTR_VERTICAL_OFFSET, &offsetY));
		
	CHECK_ERROR("SampleRate", error = IviScope_SampleRate(vi, &rateX));
	CHECK_ERROR("SampleMode", error = IviScope_SampleMode(vi, &sampleMode));
	if (sampleMode == IVISCOPE_VAL_REAL_TIME) {

	} else if (IVISCOPE_VAL_EQUIVALENT_TIME) {

	}

	CHECK_ERROR("ConfigureTrigger", error = IviScope_ConfigureTrigger (vi, IVISCOPE_VAL_EDGE_TRIGGER, 500.0e-9));
	CHECK_ERROR("ConfigureTriggerCoupling", error = IviScope_ConfigureTriggerCoupling (vi, IVISCOPE_VAL_DC));
	CHECK_ERROR("ConfigureEdgeTriggerSource", error = IviScope_ConfigureEdgeTriggerSource (vi, CH1, 0.0, IVISCOPE_VAL_POSITIVE));
    
	// get record length
	CHECK_ERROR("ActualRecordLength", error = IviScope_ActualRecordLength (vi, &actualRecordLength));
	waveform = (ViReal64*)calloc (actualRecordLength, 2*sizeof(ViReal64));
	// first read data
	CHECK_ERROR("ReadWaveform", error = IviScope_ReadWaveform(vi, CH1,
		actualRecordLength, 5000, waveform, &actualPts,
		&initialX, &incrementX));

	
#ifdef _CVI
#ifdef _CUNTINUOUS
	int panelHandle = LoadPanel(0, "read_waveform.uir", PANEL);
	/* display the panel and run the user interface */
	DisplayPanel(panelHandle);
	RunUserInterface();
	if (waveform) free(waveform);
	if (vi) IviScope_close(vi);
	return 0;
#endif
#endif

#ifdef _ISVI

	SCOPE_PARAM scope_param;
	strcpy(scope_param.DevName, DEVICE_NAME);
	scope_param.MaxNumChan = 64;
	scope_param.NumChan = 1;
	scope_param.NumSamples = actualRecordLength;
	scope_param.SampleRate = rateX;
	scope_param.AdcBitRange = 16;
	//scope_param.AdcBitRange = 8;
	scope_param.InpRange = rangeY/2;
	scope_param.SampleSize = 2;				
	scope_param.SamplePerBytes=1;			
	scope_param.isComplexSig=0;			

	// Prepare File Header
	ChanNums[0] = 0;
	Gains[0] = 1.0;
	FreqOffsets[0] = 0.0;

	PrepareFileHeader(&scope_param, &ChanNums[0], &Gains[0], &FreqOffsets[0]);
	WriteFlagSinc(0, 0);

	short_waveform = (ViInt16*)calloc(actualRecordLength, sizeof(ViInt16));

	ViInt16 *pBuf[8];
	for (int loop = 0;; loop++) {
		error = IviScope_ReadWaveform(vi, CH1,
			actualRecordLength, 5000, waveform, &actualPts,
			&initialX, &incrementX);
		DoubleToShort(waveform, short_waveform, &scope_param);

		pBuf[0] = (ViInt16*)short_waveform;
		WriteDataFile(pBuf, 1, scope_param.NumSamples*sizeof(ViInt16));

		if (loop == 0)
			WriteFlagSinc(0xffffffff, 0xffffffff);
		else
			WriteFlagSinc(0xffffffff, 0x0);

		for (int ii = 0;; ii++)
		{
			int rr = ReadFlagSinc();
			if (_kbhit()) if (0x1B == _getch())
			{
				BRDC_printf(_BRDC("\n\n"));
				break;
			}
			if (rr == 0)
				break;
			if (rr == 0xffffffff && ii>0x1000)
				break;
		}
	}

	if (short_waveform)
		free(short_waveform);

	return 0;
#endif


	for (int i = 0; i < 1; i++)
	{
		CHECK_ERROR("ReadWaveform", error = IviScope_ReadWaveform(vi, CH1,
			actualRecordLength, 5000, waveform, &actualPts,
			&initialX, &incrementX));

#ifdef _CVI // for LabWindows/CVI
		YGraphPopup("waveform", waveform, actualPts, VAL_DOUBLE);
#else
		for (size_t i = 0; i < 8; i++)
		{
			//printf("waveform[i] = %f\n", waveform[i]);
			BRDC_printf(_BRDC("waveform[i] = %f\n"), waveform[i]);
		}
#endif
	}
Error:

		if (error != VI_SUCCESS)
        {
            ViChar   errStr[2048];

            IviScope_GetError(vi, &error, 2048, errStr);
#ifdef _CVI // for LabWindows/CVI
			MessagePopup (errFname, errStr);
#else
			//printf("(%s) %s\n", errFname, errStr);
			BRDC_printf(_BRDC("(%s) %s\n"), errFname, errStr);
#endif
		}
    if (waveform)
        free (waveform);
    
    if (vi)
        IviScope_close (vi);
#ifdef _CVI       
	// Close the CVI Run-time Engine
	CloseCVIRTE();
#endif

}



