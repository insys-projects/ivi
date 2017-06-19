/***************************************************************************** 
 *  Copyright 2014-2016 National Instruments Corporation.  All Rights Reserved.   * 
 *****************************************************************************/

/*****************************************************************************
 *  tkdpo4k Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 2013                                     
 *  Original Release: 03/25/2014                                  
 *
 *  Purpose:  This sample program initializes the scope, configures it to 
 *            output an arbitrary waveform, and displays a message pop-up
 *            dialog.
 *
 *            To run this sample program, do the following:
 *            1) Create a new project in LabWindows/CVI.
 *            2) Add this file to the project.  To add files to the project,
 *               select the Add Files To Project from the Edit menu of the 
 *               Project window.
 *            3) Add the .fp file for the tkdpo4k instrument driver to the 
 *               project.
 *            4) Check that the resource name used in tkdpo4k_InitWithOptions()
 *               is correct. If simulating, change flag to simulate.
 *            5) Run the project.
 * 
 *            VCC or Borland Users:
 *            This example uses CVI's MessagePopup() to display the
 *            measured value.  Replace this MessagePopup() function with a 
 *            printf() or similar display function.  Remove the include 
 *            statement: #include <userint.h> from the source code.  Follow 
 *            the directions for CVI users given above.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <userint.h>
#include "tkdpo4k.h"

#define TKDPO4K_EXAMPLE_ARB_WFM_SIZE 5

/* -------------------------------------------------------------------------- */
/* ============================== Main Entry ================================ */
/* -------------------------------------------------------------------------- */
int main ()
{
    ViSession   vi;
    ViInt32     iArbWfmHandle;
    ViReal64    rData[TKDPO4K_EXAMPLE_ARB_WFM_SIZE] = {0.1,0.2,0.5,0.1,-0.2};
    ViStatus    error = VI_SUCCESS;
    ViString    popUpMsg = "The function generator is outputting the waveform "  
                           "that you configured.\n\nPress <Enter> to continue";

        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 1")
        */
    checkErr( tkdpo4k_InitWithOptions ("tkscope", VI_TRUE, VI_TRUE,
                                       "Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1",
                                       &vi));

    /* Clear arbitrary waveforms in memory */
    checkErr( tkdpo4k_ClearArbWaveform (vi, -1));

    /* Create an arbitrary waveform and upload it to the instrument */
    checkErr( tkdpo4k_CreateArbWaveform (vi,
                                         TKDPO4K_EXAMPLE_ARB_WFM_SIZE,
                                         rData, &iArbWfmHandle));

    /* Configure other output parameters */
    checkErr( tkdpo4k_ConfigureOutputImpedance (vi, TKDPO4K_VAL_FUNC_OUTPUT_IMPEDANCE_HIGHZ));

    /* Configures the arbitrary waveform */
    checkErr( tkdpo4k_ConfigureArbWaveform (vi, iArbWfmHandle, 0.5, 0, 100000));

    /* Enables the output */
    checkErr( tkdpo4k_ConfigureOutputEnabled (vi, VI_TRUE));


    MessagePopup ("Message", popUpMsg);
                                        
Error:
    if (error != VI_SUCCESS)
        {
            ViChar   errStr[2048];

            tkdpo4k_GetError(vi, &error, 2048, errStr);
            MessagePopup ("Error!", errStr);
        }
    
    if (vi)
        tkdpo4k_close (vi);
}
