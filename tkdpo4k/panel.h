
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//
//  "panel.h"
//
//  Declaration and include file for UI build functions
//

//  Date: 12-21-2016
//

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#ifndef _PANEL_INCLUDED
#define _PANEL_INCLUDED

#ifdef __cplusplus
    extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

//----------------------------------------------------------------------------------------------------
//  Include required headers
//----------------------------------------------------------------------------------------------------

#include <cvidef.h> // definition of CVICALLBACK

//----------------------------------------------------------------------------------------------------
//  Prototype UI build functions.
//----------------------------------------------------------------------------------------------------

int BuildPANEL (int hParentPanel);
int BuildCTRLARRAY (int hPanel);

//-------------------------------------------------------------------------------------------------
// Prototype callbacks for Panel: PANEL
// ------------------------------------------------------------------------------------------------

// Control: PANEL_COMMANDBUTTON_2
int CVICALLBACK QuitCallback (int panelHandle, int control, int event, void *callbackData, int eventData1, int eventData2);

// Control: PANEL_TIMER
int CVICALLBACK PlotWaveTime (int panelHandle, int control, int event, void *callbackData, int eventData1, int eventData2);


//-------------------------------------------------------------------------------------------------
// Declare identifiers for Panel:  PANEL
// ------------------------------------------------------------------------------------------------

//extern int PANEL_GRAPH                              ;     // control identifier
//extern int PANEL_COMMANDBUTTON                      ;     // control identifier
//extern int PANEL_COMMANDBUTTON_2                    ;     // control identifier
//extern int PANEL_TIMER                              ;     // control identifier

#ifdef __cplusplus
    }
#endif

#endif // _PANEL_INCLUDED
