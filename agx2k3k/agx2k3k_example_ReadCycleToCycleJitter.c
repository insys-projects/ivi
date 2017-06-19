/***************************************************************************** 
 *  Copyright 2011-2016 National Instruments Corporation.  All Rights Reserved.   * 
 *****************************************************************************/
 /*****************************************************************************
 *  agx2k3k Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 15.0                                     
 *  Original Release: 02/05/2016                                  
 *
 *  Purpose:  This sample program initializes the scope, configures the jitter
 *  		  analysis feature, and measures a Cycle-to-Cycle jitter, and displays 
 *            it in a pop-up dialog.
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
 *  Note: This example only works on 6000-X Series.  
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
    ViReal64    jitter = 0;
	ViChar      msgStr[256]; 
        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 1")
        */
    checkErr( agx2k3k_InitWithOptions ("MSOX6004A", VI_TRUE, VI_TRUE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=1,Cache=1",
                                        &agx2k3k));
    checkErr( agx2k3k_AutoSetup(agx2k3k));                                     
    
	/* if you get mesurement result of 9.9E+37, consider increasing the
	   timebase scale in agx2k3k_ConfigureAcquisitionRecord() */
    checkErr( agx2k3k_ConfigureAcquisitionRecord (agx2k3k, 1e-4, 500, 0.0));
	checkErr( agx2k3k_ConfigureJitterAnalysisEnabled (agx2k3k, VI_TRUE));
	checkErr( agx2k3k_ReadNCycleToNCycleJitter( agx2k3k, "CHAN1", AGX2K3K_VAL_RISING, 1, &jitter));
	
	sprintf (msgStr, "Cycle-Cycle Jitter = %g", jitter);
    MessagePopup ("Measurement", msgStr);
	
Error:
    if (error != VI_SUCCESS)
        {
            ViChar   errStr[2048];

            BuildErrorString (agx2k3k, error, errStr);
            MessagePopup ("Error!", errStr);
        }
    
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
