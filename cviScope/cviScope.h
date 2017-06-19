/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_WAVE_GRAPH                 2       /* control type: graph, callback function: (none) */
#define  PANEL_SPECTR_GRAPH               3       /* control type: graph, callback function: (none) */
#define  PANEL_CHECK_AUTOSCALE            4       /* control type: radioButton, callback function: WaveAutoScale */
#define  PANEL_TIMER                      5       /* control type: timer, callback function: PlotWaveTimeFreq */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PlotWaveTimeFreq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WaveAutoScale(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
