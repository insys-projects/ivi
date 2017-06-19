/***************************************************************************** 
 *  Copyright 2011-2015 National Instruments Corporation.  All Rights Reserved.   * 
 *****************************************************************************/
 /*****************************************************************************
 *  agx2k3k Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 12.0                                     
 *  Original Release: 08/25/2015                                  
 *
 *  Purpose:  This sample program initializes the scope, configures it for 
 *            SBus SENT signal.
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
	ViChar BusName[] = "SBUS1"; 
	ViInt32 InputSource = AGX2K3K_VAL_SBUS_SENT_SOURCE_CHANNEL1;
	ViChar SignalName[] = "SIGN1"; 
	ViInt32 NumberBase = AGX2K3K_VAL_SBUS_SENT_DISPLAY_BASE_HEX;
	ViInt32 SignalLength = 1; 
	ViReal64 Multiplier = 1.00000000E+000; 
    ViReal64 Offset = 1.0E+000; 
	ViInt32 Order = AGX2K3K_VAL_SBUS_SENT_SIGNAL_ORDER_MSNFIRST; 
	ViInt32 StartBit = 0;
        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 1")
        */
    checkErr( agx2k3k_InitWithOptions ("4ks", VI_TRUE, VI_FALSE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=1,Cache=0",
                                        &agx2k3k));

	checkErr( agx2k3k_ConfigureSENT (agx2k3k, BusName, 300.00E-06, AGX2K3K_VAL_SBUS_SENT_CRC_RECOMMENDED, NumberBase, AGX2K3K_VAL_SBUS_SENT_MESSAGE_FORMAT_NIBBLES, AGX2K3K_VAL_SBUS_SENT_IDLE_STATE_HIGH, 6, VI_FALSE, InputSource, 20));
	checkErr( agx2k3k_ConfigureSENTSignal ( agx2k3k, BusName, SignalName, VI_FALSE, SignalLength, Multiplier, Offset, Order, StartBit));
	

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
