/***************************************************************************** 
 *  Copyright 2011-2015 National Instruments Corporation.  All Rights Reserved.   * 
 *****************************************************************************/
 /*****************************************************************************
 *  agx2k3k Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 9.0                                     
 *  Original Release: 06/24/2011                                  
 *
 *  Purpose:  This sample program initializes the scope, configures it for an
 *            edge triggered acquisition, acquires a waveform, and displays the 
 *            waveform in a graph pop-up dialog box.
 *
 *            To run this sample program, do the following:
 *            1) Create a new project in LabWindows/CVI.
 *            2) Add this file to the project.  To add files to the project,
 *               select the Add Files To Project from the Edit menu of the 
 *               Project window.
 *            3) Add one of the following files to the project:
 *               agx2k3k.fp, agx2k3k.c, or agx2k3k.lib.
 *            4) Check that the resource name used in agx2k3k_InitWithOptions()
 *               is correct. If simulating, change flag to simulate.
 *            5) Run the project.
 *
 *            VCC or Borland Users:
 *            This example uses CVI's WaveformGraphPopup() to display the
 *            waveform array.  Replace this WaveformGraphPopup() function with a 
 *            similar display function.  Remove the include statement: 
 *            #include <userint.h> from the source code.  Follow the 
 *            directions for CVI users given above.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <userint.h>
#include "agx2k3k.h"

void BuildErrorString (ViSession agx2k3k, ViStatus error, ViString errStr);

main ()

{
    ViSession   agx2k3k;
    ViStatus    error = VI_SUCCESS;
    ViReal64    initialX, incrementX;
    ViInt32     actualRecordLength, actualPts;
    ViReal64    *waveform = VI_NULL;

        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 1")
        */
		//    checkErr( agx2k3k_InitWithOptions ("TCPIP0::10.0.42.13::inst0::INSTR", VI_TRUE, VI_FALSE,
		//"Simulate=0,RangeCheck=1,QueryInstrStatus=1,Cache=1",
		//                                        &agx2k3k));

	checkErr( agx2k3k_InitWithOptions ("AgScope:0", VI_TRUE, VI_FALSE,
										"Simulate=0,RangeCheck=1,QueryInstrStatus=1,Cache=1",
		                                 &agx2k3k));

    checkErr( agx2k3k_ConfigureAcquisitionType (agx2k3k, AGX2K3K_VAL_NORMAL));
    checkErr( agx2k3k_ConfigureChannel (agx2k3k, "CHAN1", 8.0, 0.0, AGX2K3K_VAL_DC,
			  					 AGX2K3K_VAL_PROBE_SENSE_ON, VI_TRUE));
    checkErr( agx2k3k_ConfigureChanCharacteristics (agx2k3k, "CHAN1", 1.0E+06, 500.0e6));
    checkErr( agx2k3k_ConfigureAcquisitionRecord (agx2k3k, 1.0e-3, 500, 0.0));
    checkErr( agx2k3k_ConfigureTrigger (agx2k3k, AGX2K3K_VAL_EDGE_TRIGGER, 6.0e-8));
    checkErr( agx2k3k_ConfigureEdgeTriggerSource (agx2k3k, "CHAN1", 0.5, AGX2K3K_VAL_POSITIVE));

    checkErr( agx2k3k_ActualRecordLength (agx2k3k, &actualRecordLength));
	waveform = calloc (actualRecordLength, sizeof(ViReal64));
    
    checkErr( agx2k3k_ReadWaveform (agx2k3k, "CHAN1", actualRecordLength, 5000,
									 waveform, &actualPts, &initialX, &incrementX));

#ifndef _MSC_VER
	WaveformGraphPopup ("Acquired Waveform", waveform, actualPts, VAL_DOUBLE, 1.0, 0.0, initialX, incrementX);
#endif


Error:
    if (error != VI_SUCCESS)
        {
            ViChar   errStr[2048];

            BuildErrorString (agx2k3k, error, errStr);
#ifndef _MSC_VER
			MessagePopup ("Error!", errStr);
#else
			printf("ERROR:%s\n", errStr);
#endif
        }
    if (waveform)
        free (waveform);
    
    if (agx2k3k)
        agx2k3k_close (agx2k3k);
}

/*****************************************************************************
 * Function: BuildErrorString   
 * Purpose:  This function builds a string that describes the error that the 
 *           sample program encounters using the error information functions 
 *           that the instrument driver exports.
 ***************************************************************************/
void BuildErrorString (ViSession agx2k3k, ViStatus error, ViString errStr)
{

    ViChar   tempStr[256], errElabStr[256];
    ViChar   *p = errStr;
    ViStatus primaryErr, secondaryErr;
            
    p += sprintf (p, "Driver Status:  (Hex 0x%8X)", error);
                                
        /* Get description of the returned status code */
    agx2k3k_error_message (agx2k3k, error, tempStr);
    p += sprintf (p, " %s\n\n", tempStr);
        
        /* Get any additional error information */
    errElabStr[0] = 0;
    Ivi_GetErrorInfo (agx2k3k, &primaryErr, &secondaryErr, errElabStr);
    
    if (primaryErr == error)
        {
        if (secondaryErr != VI_SUCCESS)
            {
                p += sprintf (p, "Secondary Error:  (Hex 0x%8X)", secondaryErr);
                                
                    /* Get description of the returned status code */
                agx2k3k_error_message (agx2k3k, secondaryErr, tempStr);
                p += sprintf (p, " %s\n\n", tempStr);
            }
        if (errElabStr[0] != 0)
            sprintf (p, "Elaboration:  %s", errElabStr);
        }
}
