//==============================================================================
//
// Title:		cviScope
// Purpose:		A short description of the application.
//
// Created on:	26.05.2017 at 17:59:44 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "cviScope.h"
#include "toolbox.h"

#include <iviscope.h>

//==============================================================================
// Constants
#define     DEVICE_NAME	"pe510k1"
#define     CH1     "CH1"
#define     CH2     "CH2"

//==============================================================================
// Types

//==============================================================================
// Static global variables
static int panelHandle = 0;
ViReal64    initialX, incrementX;
ViInt32     actualRecordLength, actualPts;
ViReal64    *waveform = VI_NULL;
ViInt16     *short_waveform = VI_NULL;

ViInt32     lMemsize = 512;// 1024;
ViReal64	rangeY;
ViReal64	offsetY;
ViReal64	rateX;
ViInt32		sampleMode;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
ViSession   vi;
ViStatus    error = VI_SUCCESS;

//==============================================================================
// Global functions
#ifndef CHECK_ERROR
static char errFname[2048];
#define CHECK_ERROR(fName, fCall)    if (error = (fCall), (error = (error < 0) ? error : VI_SUCCESS)) \
                                 { 	sprintf(errFname, "#ERR - %s", fName); goto Error; }  else error = error
#endif

// 0.1s plot period
int CVICALLBACK PlotWaveTimeFreq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int	i;
	double delta_t,delta_f,frequency,ffreq,frequency_array[actualRecordLength+1];

	error = IviScope_ReadWaveform(vi, CH1,
			actualRecordLength, 5000, waveform, &actualPts,
			&initialX, &incrementX);

	DeleteGraphPlot(PANEL, PANEL_WAVE_GRAPH, -1, VAL_DELAYED_DRAW);

	double xIncrement = 1.0/rateX;
	int wfmHandle = PlotWaveform(PANEL, PANEL_WAVE_GRAPH, waveform, actualRecordLength, VAL_DOUBLE, 1.0, offsetY, initialX, xIncrement, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_YELLOW);


	DeleteGraphPlot(PANEL, PANEL_SPECTR_GRAPH, -1, VAL_DELAYED_DRAW);

	
	HanWin(waveform, actualRecordLength);
	Spectrum(waveform, actualRecordLength);
	frequency = rateX;
    delta_t=1/frequency;
    delta_f=1/(actualRecordLength*delta_t);
    frequency_array[0]=0.0;
    frequency_array[actualRecordLength/2]=1/(2*delta_t);
    for (i=0;i<actualRecordLength/2;i++)
    {
        frequency_array[i]=i*delta_f;
        frequency_array[actualRecordLength-i]=-1*delta_f;
    }

	double maximumValue, minimumValue;
	int maximumIndex, minimumIndex;
	AnalysisLibErrType status; 
	status = MaxMin1D(waveform, actualRecordLength, &maximumValue, &maximumIndex, &minimumValue, &minimumIndex);

    for(int i=0;i<actualPts/2;i++)
    {
 	   waveform[i] = 10.0*log10(Max(waveform[i]/maximumValue,1.0e-15)); 
    }
	
    int spcHandle = PlotXY (PANEL, PANEL_SPECTR_GRAPH, frequency_array, waveform, actualRecordLength/2,
                VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE,
                VAL_SOLID,1, VAL_YELLOW);

	
//	int spcHandle = PlotWaveform(PANEL, PANEL_SPECTR_GRAPH, waveform, actualRecordLength, VAL_DOUBLE, 1.0, offsetY, initialX, 1.0, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_YELLOW);

	return 0;
}

int CVICALLBACK WaveAutoScale(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
   if (event == EVENT_VAL_CHANGED) {
		int radioButtonVal;
		GetCtrlVal (panelHandle, PANEL_CHECK_AUTOSCALE, &radioButtonVal);
		if( radioButtonVal == 0) {
			SetAxisScalingMode (PANEL, PANEL_WAVE_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, actualRecordLength);
		} else {
			SetAxisScalingMode (PANEL, PANEL_WAVE_GRAPH, VAL_BOTTOM_XAXIS, VAL_AUTOSCALE, 0, 0);		
		}
   }	
	return 0;
}

				 
/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{

	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "cviScope.uir", PANEL));

	CHECK_ERROR("InitWithOptions", error = IviScope_InitWithOptions (DEVICE_NAME, VI_TRUE, VI_TRUE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1",
                                        &vi));

	CHECK_ERROR("ConfigureAcquisitionType", error = IviScope_ConfigureAcquisitionType(vi, IVISCOPE_VAL_NORMAL));

	// setup 40mks horizontal 
	CHECK_ERROR("ConfigureAcquisitionRecord", error = IviScope_ConfigureAcquisitionRecord(vi, 0.000005, lMemsize, 0.0));

	// setup channel 0: +/-1 V, 50 Om (1 MOhm), DC, no offset
	CHECK_ERROR("ConfigureChannel", error = IviScope_ConfigureChannel (vi, CH1, 2.0, 0.0, IVISCOPE_VAL_DC, 1.0, VI_TRUE));
	CHECK_ERROR("ConfigureChanCharacteristics", error = IviScope_ConfigureChanCharacteristics(vi, CH1, 50, 200000000));

	CHECK_ERROR("GetAttributeViReal64 - VERTICAL_RANGE", error = IviScope_GetAttributeViReal64(vi, CH1, IVISCOPE_ATTR_VERTICAL_RANGE, &rangeY));
	CHECK_ERROR("GetAttributeViReal64 - VERTICAL_OFFSET", error = IviScope_GetAttributeViReal64(vi, CH1, IVISCOPE_ATTR_VERTICAL_OFFSET, &offsetY));
		
	CHECK_ERROR("SampleRate", error = IviScope_SampleRate(vi, &rateX));
	CHECK_ERROR("SampleMode", error = IviScope_SampleMode(vi, &sampleMode));

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

	/* INITIAL PANELS */
	SetAxisScalingMode (PANEL, PANEL_WAVE_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.0, actualRecordLength * (1.0 / rateX));

	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}
