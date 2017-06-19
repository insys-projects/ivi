/******************************************************************************* 
 *  Copyright 2011 - 2016 National Instruments Corporation.  All Rights Reserved.                 *  
 *******************************************************************************/

/*****************************************************************************
 *  tkdpo4k Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 5.1                                     
 *  Original Release: 09/28/2011                                  
 *
 *  Purpose:  This sample program initializes the scope, configures it for freq-
 *            -uency domain measurement, acquires channel power and ACPR (Adjacent
 *            Channel Power Ratio), then displays the result in a message pop-up 
 *            dialog.
 *                     Note: This example only apply to MDO 4K series models.
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
    ViReal64    channelPower,ACPR;
    ViInt32     powerUnit; 
    
    ViChar      unitStr[256];
    ViChar      resultStr[256];
        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 1")
        */
    checkErr( tkdpo4k_InitWithOptions ("MDO4K", VI_TRUE, VI_TRUE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1",
                                        &vi));
    
    checkErr( tkdpo4k_ConfigureRFMeasurement (vi, TKDPO4K_VAL_RF_MEASURE_TYPE_ACPR, 1000000, 1, 5000000, 99.9));
    
    checkErr( tkdpo4k_ReadFreqDomainMeasurement (vi, &channelPower, &powerUnit, &ACPR, "UA3"));
    
    switch(powerUnit)
        {
            case 0:
                sprintf(unitStr,"%s","DBM");
                break;
            case 1:
                sprintf(unitStr,"%s","DBUW");
                break;
            case 2:
                sprintf(unitStr,"%s","DBMU");
                break;
            case 3:
                sprintf(unitStr,"%s","DBUV");
                break;
            case 4:
                sprintf(unitStr,"%s","DBMA");
                break;
            case 5:
                sprintf(unitStr,"%s","DBUA");
                break;
            default:
                sprintf(unitStr,"%s","Unknown unit");
                break;
        }
    sprintf(resultStr,"Channel Power:%le %s\nACPR Value:%Le",channelPower,unitStr,ACPR);
    MessagePopup("Frequency Domain Measure Result",resultStr);
    
    
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
