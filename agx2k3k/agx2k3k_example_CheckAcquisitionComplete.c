/***************************************************************************** 
 *  Copyright 2011-2015 National Instruments Corporation.  All Rights Reserved.   * 
 *****************************************************************************/
 /*****************************************************************************
 *  agx2k3k Instrument Driver Sample Program                              
 *  Developed with LabWindows/CVI 2012                                    
 *  Original Release: 03/30/2013                                  
 *
 *  Purpose:  This sample program initializes the scope, configures it for an
 *            :DIG acquisition, then queries whether the acquisition is finished.
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
 *            This example sets the Ripple measurement first and then got the
 *            ripple measurement resultby setting the ripple channel to CHAN1
 *            Remove the include statement: #include <userint.h> from the source
 *            code.  Follow the directions for CVI users given above.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <userint.h>
#include <windows.h>
#include "agx2k3k.h"

void BuildErrorString (ViSession agx2k3k, ViStatus error, ViString errStr);

main ()

{
    ViSession   agx2k3k;
    ViStatus    error = VI_SUCCESS;
	 
	ViString    strMsg0 = "The acquisition will start."
                        "\n\nPress <Enter> to continue";
    ViString    strMsg1 = "The acquisition is finished."
                        "\n\nPress <Enter> to continue";
        /* 
            If you want to run this sample program and the instrument is not
            present, set the Simulate flag to 1.  (Example:  "Simulate = 0")
        */
    checkErr( agx2k3k_InitWithOptions ("TCPIP0::10.0.42.13::inst0::INSTR", VI_TRUE, VI_FALSE,
                                        "Simulate=0,RangeCheck=1,QueryInstrStatus=1,Cache=1",
                                        &agx2k3k));
	
	ViSession io = Ivi_IOSession(agx2k3k);
	
    checkErr( agx2k3k_AutoSetup (agx2k3k));
	
	MessagePopup("Message", strMsg0);
	
	checkErr( agx2k3k_InitiateAcquisition (agx2k3k));

	checkErr( viPrintf (io, "*OPC"));
	
	ViUInt16 statusByte = 0;
	checkErr( viReadSTB(io, &statusByte));
	
	/*
	Use viReadSTB function to read the Status Byte Register.
	Repeat until bit 5 in the Status Byte Register goes true which indicates that the acquisition is finished.
    */

	while((statusByte & 0x20) != 0x20)
	{
		Sleep(500);
		checkErr( viReadSTB(io, &statusByte));
	}
	
    MessagePopup("Message", strMsg1);


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
