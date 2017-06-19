/***************************************************************************** 
 *  Copyright 2013 - 2016 National Instruments Corporation.  All Rights Reserved.            * 
 *****************************************************************************/

/*****************************************************************************
 *  tkdpo4k Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 2013                                     
 *  Original Release: 08/26/2013
 *  
 *  Modification History: 
 *  03/25/2014 [zsu] - Correct the title of MessagePopup dialog box.
 *
 *  Purpose:  This sample program initializes the scope, configures it for 
 *            phase measurement, measure the phase between CH1 and CH2, then 
 *            displays the result in a message pop-up dialog.
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

main ()

{
    ViSession   vi;
    ViStatus    error = VI_SUCCESS;
    ViReal64    Phase;
    
    ViChar      resultStr[256];
        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 1")
        */
    checkErr( tkdpo4k_InitWithOptions ("tkscope", VI_TRUE, VI_TRUE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1",
                                        &vi));

    checkErr( tkdpo4k_ConfigureAcquisitionType (vi, TKDPO4K_VAL_NORMAL)); 
        
    checkErr( tkdpo4k_ConfigureChannel (vi, "CH1", 10.0, 0.0, TKDPO4K_VAL_DC, 1.0,
                                         VI_TRUE));
    
    checkErr( tkdpo4k_ConfigureChannel (vi, "CH2", 10.0, 0.0, TKDPO4K_VAL_DC, 1.0,
                                         VI_TRUE));
    
    checkErr( tkdpo4k_FetchPhaseMeasurement (vi, "CH1", "CH2", &Phase));    

    sprintf(resultStr,"Phase between CH1 and CH2: %le",Phase);
    MessagePopup("Phase Measure Result",resultStr);
    
    
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
